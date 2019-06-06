

/*************************************************************

This file is part of messaging-cells.

messaging-cells is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

messaging-cells is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with messaging-cells.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2017-2018. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://messaging-cells.github.io/

messaging-cells is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------

File: workeru_loader_znq.c  
Based on: e-loader.c

This file is part of the Epiphany Software Development Kit.

Copyright (C) 2013 Adapteva, Inc.
See AUTHORS for list of contributors.
Support e-mail: <support@adapteva.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
and the GNU Lesser General Public License along with this program,
see the files COPYING and COPYING.LESSER.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <err.h>
#include <elf.h>
#include <inttypes.h>

#include "e-hal.h"

#include "shared.h"
#include "booter.h"

#include "workeru_loader_znq.h"

extern e_platform_t e_platform;

char* mcl_module_names[MCL_MAX_TOT_MODULES];
int mcl_module_names_sz;

e_return_stat_t mcl_load_elf(int row, int col, load_info_t *ld_dat);

//mcl_loader_diag_t mcl_load_verbose = L_D3;
mcl_loader_diag_t mcl_load_verbose = L_D0;

void
mc_ck_memload(uint8_t* dst, uint8_t* src, uint32_t sz){
	bool ok = true;
	for(long aa = 0; aa < sz; aa++){
		if(dst[aa] != src[aa]){
			ok = false;
			break;
		}
	}
	if(! ok){
		mch_write_file("SOURCE_shd_mem_dump.dat", src, sz, false);
		mch_write_file("DEST_shd_mem_dump.dat", dst, sz, false);
		mch_abort_func(9, "mc_ck_memload() FAILED !! CODE_LOADING_FAILED_1 !!\n");
	}
}

uint8_t*
mc_memload(uint8_t* dest, const uint8_t* src, uint32_t sz){
	uint32_t idx = 0;
	for(idx = 0; idx < sz; idx++){
		mc_loop_set_var(dest[idx], src[idx]);
	}
	return dest;
}

#define EM_ADAPTEVA_EPIPHANY   0x1223  /* Adapteva's Epiphany architecture.  */
bool mcl_is_epiphany_exec_elf(Elf32_Ehdr *ehdr)
{
	bool ok = true;
	if(! ehdr){ 
		mcl_diag(L_D1) { fprintf(MC_STDERR, "mcl_is_epiphany_exec_elf(): ERROR_1\n"); }
		return false;
	}
	if(memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0){ 
		mcl_diag(L_D1) { fprintf(MC_STDERR, "mcl_is_epiphany_exec_elf(): NOT ELFMAG\n"); }
		ok = false;
	}
	if(ehdr->e_ident[EI_CLASS] != ELFCLASS32){
		mcl_diag(L_D1) { fprintf(MC_STDERR, "mcl_is_epiphany_exec_elf(): NOT ELFCLASS32\n"); }
		ok = false;
	}
	if(ehdr->e_type != ET_EXEC){
		mcl_diag(L_D1) { fprintf(MC_STDERR, "mcl_is_epiphany_exec_elf(): NOT ET_EXEC\n"); }
		ok = false;
	}
	if(ehdr->e_version != EV_CURRENT){
		mcl_diag(L_D1) { fprintf(MC_STDERR, "mcl_is_epiphany_exec_elf(): NOT EV_CURRENT\n"); }
		ok = false;
	}
	if(ehdr->e_machine != EM_ADAPTEVA_EPIPHANY){
		mcl_diag(L_D1) { fprintf(MC_STDERR, "mcl_is_epiphany_exec_elf(): NOT EM_ADAPTEVA_EPIPHANY\n"); }
		ok = false;
	}

	return ok;
}

static void mcl_clear_sram(e_epiphany_t *dev, unsigned row, unsigned col,unsigned rows, unsigned cols)
{
	unsigned i, j;
	size_t sram_size;
	void *empty;

	/* Assume one chip type */
	sram_size = e_platform.chip[0].sram_size;

	empty = alloca(sram_size);
	memset(empty, 0, sram_size);

	for (i = row; i < row + rows; i++){
		for (j = col; j < col + cols; j++){
			e_write(dev, i, j, 0, empty, sram_size);
		}
	}
}

e_return_stat_t
mc_start_load(load_info_t *ld_dat){
	int          fd;
	struct stat  st;
	void        *file;

	e_set_host_verbosity(H_D0);

	const char *executable = ld_dat->executable;
	e_epiphany_t *dev = ld_dat->dev;
	mc_workeru_co_t row = ld_dat->row;
	mc_workeru_co_t col = ld_dat->col;
	mc_workeru_co_t rows = ld_dat->rows;
	mc_workeru_co_t cols = ld_dat->cols;

	mc_link_syms_data_st* lk_dat = &(MC_EXTERNAL_RAM_LOAD_DATA);

	if (!dev) {
		warnx("ERROR: Can't connect to Epiphany or external memory.\n");
		return E_ERR;
	}

	fd = open(executable, O_RDONLY);
	if (fd == -1) {
		warnx("ERROR: Can't open executable file \"%s\".\n", executable);
		return E_ERR;
	}

	if (fstat(fd, &st) == -1) {
		warnx("ERROR: Can't stat file \"%s\".\n", executable);
		close(fd);
		return E_ERR;
    }

	file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file == MAP_FAILED) {
		warnx("ERROR: Can't mmap file \"%s\".\n", executable);
		close(fd);
		return E_ERR;
    }
	
	uint8_t* pt_mem_start = (uint8_t*)(mch_glb_emem.base);
	uint8_t* pt_mem_end = pt_mem_start + lk_dat->extnl_ram_size;
	uint8_t* pt_file_start = (uint8_t*)(file);
	uint8_t* pt_file_end = pt_mem_start + st.st_size;
	mcl_diag(L_D3) {
		fprintf(MC_STDERR, "load_group(): mem_beg=%p end=%p file_beg=%p end=%p\n",
				pt_mem_start, pt_mem_end, pt_file_start, pt_file_end); 
	}

	if (mcl_is_epiphany_exec_elf((Elf32_Ehdr *) file)) {
		mcl_diag(L_D1) { fprintf(MC_STDERR, "load_group(): loading ELF file %s ...\n", executable); }
	} else {
		mcl_diag(L_D1) { fprintf(MC_STDERR, "load_group(): ERROR: unidentified file format\n"); }
		warnx("ERROR: Can't load executable file: unidentified format.\n");
		munmap(file, st.st_size);
		close(fd);
		return E_ERR;
	}

	mcl_clear_sram(dev, row, col, rows, cols);

	ld_dat->file = file;
	ld_dat->emem = &mch_glb_emem;
	ld_dat->f_sz = st.st_size;
	ld_dat->fd = fd;

	memset(mcl_module_names, 0,  sizeof(mcl_module_names));

	return E_OK;
}

int mc_load_group(load_info_t *ld_dat){

	e_return_stat_t ret_start;
	ret_start = mc_start_load(ld_dat);
	if (ret_start == E_ERR) {
		return E_ERR;
	}

	unsigned row = ld_dat->row;
	unsigned col = ld_dat->col;
	unsigned rows = ld_dat->rows;
	unsigned cols = ld_dat->cols;

	unsigned int irow, icol;

	for (irow=row; irow<(row+rows); irow++) {
		for (icol=col; icol<(col+cols); icol++) {
			e_return_stat_t retval = 
					mcl_load_elf(irow, icol, ld_dat);

			if (retval == E_ERR) {
				warnx("ERROR: Can't load executable file \"%s\".\n", ld_dat->executable);
				munmap(ld_dat->file, ld_dat->f_sz);
				close(ld_dat->fd);
				return E_ERR;
			}
		}
	}

	mcl_diag(L_D1) { fprintf(MC_STDERR, "load_group(): done loading.\n"); }

	munmap(ld_dat->file, ld_dat->f_sz);
	close(ld_dat->fd);

	return E_OK;
}

static bool mcl_is_valid_addr(uint32_t addr)
{
	return (! mc_addr_has_id(addr))
		|| e_is_addr_on_chip((void *) addr)
		|| e_is_addr_in_emem(addr);
}

static bool mcl_is_valid_range(uint32_t from, uint32_t size)
{
	if(!size){
		return true;
	}

	return mcl_is_valid_addr(from) && mcl_is_valid_addr(from + size - 1);
}

mc_addr_t
mc_znq_addr_to_eph_addr(mc_addr_t znq_addr){
	MCH_CK(mch_znq_addr_in_shd_ram(znq_addr));

	mc_addr_t eph_addr = znq_addr - mch_min_shd_znq_addr + mch_min_shd_eph_addr;
	
	return eph_addr;
}

mc_addr_t
mc_eph_addr_to_znq_addr(mc_addr_t eph_addr){
	e_epiphany_t *dev = &mch_glb_dev;

	MCH_CK(mc_addr_has_id(eph_addr));
	
	mc_addr_t znq_addr = mc_null;
	if (mc_addr_in_sys(eph_addr)) {
		mc_workeru_id_t wrku_id = mc_addr_get_id(eph_addr);
		mc_workeru_co_t g_ro = mc_id_to_ro(wrku_id);
		mc_workeru_co_t g_co = mc_id_to_co(wrku_id);

		znq_addr = ((mc_addr_t) dev->core[g_ro][g_co].mems.base) + mc_addr_get_disp(eph_addr);

	} else {
		znq_addr = eph_addr - mch_min_shd_eph_addr + mch_min_shd_znq_addr;
	}
	return znq_addr;
}

mc_addr_t
mc_manageru_eph_loc_addr_to_znq_addr(int row, int col, mc_addr_t ld_addr){
	e_epiphany_t *dev = &mch_glb_dev;

	mc_addr_t dst = mc_null;
	if (! mc_addr_has_id(ld_addr)) {
		dst = ((uintptr_t) dev->core[row][col].mems.base) + ld_addr;
	} else {
		dst = mc_eph_addr_to_znq_addr(ld_addr);
	}
	return dst;
}

char*
mc_addr_case(mc_addr_t ld_addr){
	if(! mc_addr_has_id(ld_addr)){
		return "(local)";
	}
	if(mc_addr_in_sys(ld_addr)){
		return "(onchip)";
	}
	return "(external)";
}

e_return_stat_t
mcl_load_elf(int row, int col, load_info_t *ld_dat)
{
	int ii;
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *phdr;
	Elf32_Shdr *shdr, *sh_strtab;
	const char *strtab;
	int        ihdr;

	e_mem_t *emem = ld_dat->emem;
	uint8_t   *src = (uint8_t *) ld_dat->file;

	e_epiphany_t *dev = ld_dat->dev;
	mc_link_syms_data_st* lk_dat = &(MC_EXTERNAL_RAM_LOAD_DATA);

	uint8_t* pt_ram_base = (uint8_t*)(emem->base);
	uint8_t* pt_end_code = (pt_ram_base + lk_dat->extnl_manageru_data_disp);
	uint8_t* pt_end_mem = (pt_ram_base + lk_dat->extnl_ram_size);
	MCL_MARK_USED(pt_end_code);
	MCL_MARK_USED(pt_end_mem);

	uint8_t* pt_load_dst = (pt_ram_base + lk_dat->extnl_load_disp);
	uint8_t* pt_load_end = (pt_load_dst + lk_dat->extnl_load_size);

	uint8_t* pt_module_dst = pt_load_dst;
	mc_addr_t module_sz = lk_dat->workeru_module_size;

	int module_ii = 0;
	mcl_module_names_sz = 0;

	MCL_MARK_USED(phdr);
	MCL_MARK_USED(ihdr);
	MCL_MARK_USED(shdr);
	MCL_MARK_USED(sh_strtab);
	MCL_MARK_USED(strtab);

	MCH_CK(sizeof(uint8_t*) == sizeof(mc_addr_t));
	MCH_CK(sizeof(Elf32_Addr) == sizeof(mc_addr_t));
	MCH_CK(dev == &mch_glb_dev);
	MCH_CK(emem == &mch_glb_emem);

	mc_workeru_id_t curr_id = mc_ro_co_to_id(row, col);
	MCL_MARK_USED(curr_id);

	ehdr = (Elf32_Ehdr *) &src[0];
	phdr = (Elf32_Phdr *) &src[ehdr->e_phoff];
	shdr = (Elf32_Shdr *) &src[ehdr->e_shoff];
	int shnum = ehdr->e_shnum;

	sh_strtab = &shdr[ehdr->e_shstrndx];
	strtab = (char *) &src[sh_strtab->sh_offset];

	for (ii = 0; ii < shnum; ii++) {
		if (!mcl_is_valid_range(shdr[ii].sh_addr, shdr[ii].sh_size)){
			return E_ERR;
		}
	}

	for (ii = 0; ii < shnum; ii++) {
		Elf32_Addr ld_sh_addr = shdr[ii].sh_addr;
		Elf32_Word ld_sz = shdr[ii].sh_size;
		Elf32_Word ld_src_sz = shdr[ii].sh_size;
		Elf32_Off  ld_src_off = shdr[ii].sh_offset;
		Elf32_Word ld_tp = shdr[ii].sh_type;
		Elf32_Word ld_flags = shdr[ii].sh_flags;

		const char* sect_nm = &strtab[shdr[ii].sh_name];
		mcl_diag(L_D1) { fprintf(MC_STDERR, "%d.LOADING section %s.\n", ii, sect_nm); }

		if(ld_sz == 0){
			continue;
		}

		if(ld_tp != SHT_PROGBITS){
			continue;
		}

		if((ld_flags & SHF_ALLOC) != SHF_ALLOC){
			continue;
		}

		mc_addr_t ld_addr = ld_sh_addr;

		uint8_t* pt_dst = (uint8_t*)mc_manageru_eph_loc_addr_to_znq_addr(row, col, ld_addr);
		uint8_t* pt_src = &src[ld_src_off];
		size_t blk_sz = ld_src_sz;

		if(ld_sh_addr == lk_dat->workeru_module_orig){
			if(blk_sz > module_sz){
				mch_abort_func(11, "Module too big. CODE_LOADING_FAILED_2 !!\n");
				return E_ERR;
			}

			pt_dst = pt_module_dst;
			blk_sz = module_sz;
			pt_module_dst += module_sz;
		
			if((pt_module_dst > pt_load_end) || (module_ii > (MCL_MAX_TOT_MODULES - 1))){
				mch_abort_func(12, "TOO MANY MODULES for MC_VAL_EXTERNAL_LOAD_SIZE. CODE_LOADING_FAILED_3 !!\n");
				return E_ERR;
			}

			mcl_module_names[module_ii] = strdup(sect_nm);
			module_ii++;
		}

		mcl_diag(L_D1) { 
			uint8_t* pt_dst_end = pt_dst + blk_sz;
			uint8_t* pt_src_end = pt_src + blk_sz;
			fprintf(MC_STDERR, 
				"%d.LOADING(%s). row=%d col=%d dst=%p end=%p src=%p end=%p sz=%d p_vaddr=%04x (%p).\n", 
				ii, mc_addr_case(ld_addr), row, col, pt_dst, pt_dst_end, pt_src, pt_src_end, blk_sz, ld_addr, 
				(void*)ld_addr); 
		}

		/*if(MCH_LOAD_WITH_MEMCPY){
			memcpy(pt_dst, pt_src, blk_sz);
		} else {
			mc_memload(pt_dst, pt_src, blk_sz);
		}*/

		memcpy(pt_dst, pt_src, blk_sz);
		MCH_CK(memcmp(pt_dst, pt_src, blk_sz) == 0);

		//mc_ck_memload(pt_dst, pt_src, blk_sz);
	}

	mcl_module_names_sz = module_ii;

	return E_OK;
}

void
mcl_load_module_names(load_info_t *ld_dat){
	mc_link_syms_data_st* lk_dat = &(MC_EXTERNAL_RAM_LOAD_DATA);
	e_mem_t *emem = ld_dat->emem;
	uint8_t* pt_ram_base = (uint8_t*)(emem->base);
	uint8_t* pt_load_dst = (pt_ram_base + lk_dat->extnl_load_disp);
	uint8_t* pt_load_eph_dst = (uint8_t*)(lk_dat->extnl_load_orig);
	mc_addr_t module_sz = lk_dat->workeru_module_size;

	char** pt_load_nams = (char**)(pt_load_dst + (mcl_module_names_sz * module_sz));

	uint8_t* ld_str = (uint8_t*)(&(pt_load_nams[mcl_module_names_sz + 1])); 
	for(int aa = 0; aa < mcl_module_names_sz; aa++){
		mc_addr_t disp_str = ld_str - pt_load_dst;
		pt_load_nams[aa] = (char*)(pt_load_eph_dst + disp_str);

		char* src_str = mcl_module_names[aa];

		int ln_str = strlen(src_str);
		strcpy((char*)ld_str, src_str);
		ld_str += (ln_str + 1);
	}

	pt_load_nams[mcl_module_names_sz] = mc_null;
}

int 
mcl_load_root(load_info_t *ld_dat){

	e_return_stat_t ret_start;
	ret_start = mc_start_load(ld_dat);
	if (ret_start == E_ERR) {
		return E_ERR;
	}

	mc_workeru_co_t root_ro = mc_nn_to_ro(ld_dat->root_nn);
	mc_workeru_co_t root_co = mc_nn_to_co(ld_dat->root_nn);

	e_return_stat_t retval = 
		mcl_load_elf(root_ro, root_co, ld_dat);

	if (retval == E_ERR) {
		warnx("ERROR: Can't load executable file \"%s\".\n", ld_dat->executable);
		munmap(ld_dat->file, ld_dat->f_sz);
		close(ld_dat->fd);
		return E_ERR;
	}

	mcl_load_module_names(ld_dat);

	mcl_diag(L_D1) { fprintf(MC_STDERR, "mc_load_root(): done loading.\n"); }

	munmap(ld_dat->file, ld_dat->f_sz);
	close(ld_dat->fd);

	return E_OK;
}

