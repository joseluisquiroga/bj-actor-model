

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

------------------------------------------------------------*/


//  File: read_eph_elf_link_syms_tab_znq.c

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

void mch_read_sections_elf(const void *file, mc_link_syms_data_st* syms);

void
mch_read_eph_link_syms(const char *executable, mc_link_syms_data_st* syms){

	int          fd;
	struct stat  st;
	void        *file;

	if(syms == NULL){
		return;
	}
	memset(syms, 0, sizeof(mc_link_syms_data_st));

	e_set_host_verbosity(H_D0);

	fd = open(executable, O_RDONLY);
	if (fd == -1) {
		warnx("ERROR: Can't open executable file \"%s\".\n", executable);
		return;
	}

	if (fstat(fd, &st) == -1) {
		warnx("ERROR: Can't stat file \"%s\".\n", executable);
		close(fd);
		return;
    }

	file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file == MAP_FAILED) {
		warnx("ERROR: Can't mmap file \"%s\".\n", executable);
		close(fd);
		return;
    }
	
	if (mcl_is_epiphany_exec_elf((Elf32_Ehdr *) file)) {
		mcl_diag(L_D1) { 
			fprintf(MC_STDERR, "mch_read_eph_link_syms(): loading ELF file %s ...\n", executable); 
		}
	} else {
		mcl_diag(L_D1) { 
			fprintf(MC_STDERR, "mch_read_eph_link_syms(): ERROR: unidentified file format\n"); 
		}
		warnx("ERROR: Can't load executable file: unidentified format.\n");
		goto out;
	}

	mch_read_sections_elf(file, syms);
	mc_extnl_ram_load_data_fill(syms);

out:
	munmap(file, st.st_size);
	close(fd);
}

void
mch_read_sections_elf(const void *file, mc_link_syms_data_st* syms)
{
	int ii;
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *phdr;
	Elf32_Shdr *shdr, *sh_strtab;
	const char *strtab;
	int        ihdr;
	uint8_t   *src = (uint8_t *) file;

	MCL_MARK_USED(phdr);
	MCL_MARK_USED(ihdr);
	MCL_MARK_USED(shdr);
	MCL_MARK_USED(sh_strtab);
	MCL_MARK_USED(strtab);

	ehdr = (Elf32_Ehdr *) &src[0];
	phdr = (Elf32_Phdr *) &src[ehdr->e_phoff];
	shdr = (Elf32_Shdr *) &src[ehdr->e_shoff];
	int shnum = ehdr->e_shnum;

	sh_strtab = &shdr[ehdr->e_shstrndx];
	strtab = (char *) &src[sh_strtab->sh_offset];

	for (ii = 0; ii < shnum; ii++) {
		//Elf32_Addr ld_addr = shdr[ii].sh_addr;
		//Elf32_Word ld_sz = shdr[ii].sh_size;
		//Elf32_Word ld_src_sz = shdr[ii].sh_size;
		Elf32_Off  ld_src_off = shdr[ii].sh_offset;
		//Elf32_Word ld_tp = shdr[ii].sh_type;
		//Elf32_Word ld_flags = shdr[ii].sh_flags;

		const char* sect_nm = &strtab[shdr[ii].sh_name];

		if(strcmp(sect_nm, mc_lk_syms_section_nm) != 0){
			mcl_diag(L_D1) { fprintf(MC_STDERR, "mch_read_sections_elf(): section=%s\n", sect_nm); }
			continue;
		}

		uint8_t* pt_src = &src[ld_src_off];
		//size_t blk_sz = ld_src_sz;

		//uint8_t* pt_src_end = pt_src + blk_sz;

		mc_link_syms_data_st* pt_syms = (mc_link_syms_data_st*)pt_src;
		*syms = *pt_syms;

		mcl_diag(L_D1) { fprintf(MC_STDERR, "mch_read_sections_elf(): FOUND section=%s\n", sect_nm); }
		break;
	}
}



