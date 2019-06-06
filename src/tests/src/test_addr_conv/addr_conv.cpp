

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


#include <math.h>
#include <sched.h>
#include <string.h>

#include <e-hal.h>
#include <epiphany-hal-api-local.h>

#include "booter.h"
#include "tak_mak.hh"

#include "workeru_loader_znq.h"

//define NUM_OPERS 100000
#define NUM_OPERS 10000000

char* mch_epiphany_elf_path = (mc_cstr("the_epiphany_executable.elf"));

#define MCT_WORKERUID(_addr) ((_addr) >> 20)
static inline bool mct_is_local(uint32_t addr)
{
	return MCT_WORKERUID(addr) == 0;
}

#define addr_in_shd_mem(addr) (! (\
	((addr) >= MCH_EXTERNAL_RAM_BASE_PT) && \
	((addr) < (MCH_EXTERNAL_RAM_BASE_PT + lk_dat->extnl_ram_size)) \
	)) \


int 
mc_manageru_main(int argc, char *argv[])
{
	e_mem_t emem;
	e_epiphany_t dev;
	e_platform_t platform;
	mc_link_syms_data_st syms;
	char* elf_path = (mc_cstr("the_epiphany_executable.elf"));

	if(argc > 1){
		elf_path = argv[1];
		printf("Using workeru executable: %s \n", elf_path);
	}

	mc_link_syms_data_st* lk_dat = &(syms);
	mch_read_eph_link_syms(elf_path, lk_dat);

	if(lk_dat->extnl_ram_orig == 0) {
		printf("ERROR: Can't read external memory location from '%s'\n", elf_path);
		printf("Make sure linker script for '%s' defines LD_EXTERNAL_* symbols\n\n", elf_path);
		mch_abort_func(301, "301. ERROR: Bad ELF\n");
	}

	// sys init

	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	if (e_alloc(&emem, 0, lk_dat->extnl_ram_size)) {
		mch_abort_func(302, "302. ERROR: Can't allocate external memory buffer!\n\n");
	}

	MCH_EXTERNAL_RAM_BASE_PT = ((uint8_t*)emem.base);
	MC_MARK_USED(MCH_EXTERNAL_RAM_BASE_PT);

	// dev init
	
	e_open(&dev, 0, 0, platform.rows, platform.cols);

	mc_sys_sz_st* g_sys_sz = MC_SYS_SZ;
	mch_init_glb_sys_sz_with_dev(g_sys_sz, &dev);

	mc_off_sys_st* pt_shd_data = (mc_off_sys_st*)mch_disp_to_pt(lk_dat->extnl_manageru_data_disp);
	MCH_CK(sizeof(*pt_shd_data) == sizeof(mc_off_sys_st));

	/*
	printf("MCH_EXTERNAL_RAM_BASE_PT=%p \n", MCH_EXTERNAL_RAM_BASE_PT);
	printf("pt_shd_data=%p \n", pt_shd_data);

	printf("extnl_ram_size = %p \n", (void*)syms.extnl_ram_size);
	printf("extnl_code_size = %p \n", (void*)syms.extnl_code_size);
	printf("extnl_load_size = %p \n", (void*)syms.extnl_load_size);
	printf("extnl_manageru_data_size = %p \n", (void*)syms.extnl_manageru_data_size);
	printf("extnl_manageru_alloc_size = %p \n", (void*)syms.extnl_manageru_alloc_size);
	printf("extnl_ram_orig = %p \n", (void*)syms.extnl_ram_orig);
	printf("extnl_code_orig = %p \n", (void*)syms.extnl_code_orig);
	printf("extnl_load_orig = %p \n", (void*)syms.extnl_load_orig);
	printf("extnl_manageru_data_orig = %p \n", (void*)syms.extnl_manageru_data_orig);
	printf("extnl_manageru_alloc_orig = %p \n", (void*)syms.extnl_manageru_alloc_orig);
	printf("extnl_code_disp = %p \n", (void*)syms.extnl_code_disp);
	printf("extnl_load_disp = %p \n", (void*)syms.extnl_load_disp);
	printf("extnl_manageru_data_disp = %p \n", (void*)syms.extnl_manageru_data_disp);
	printf("extnl_manageru_alloc_disp = %p \n", (void*)syms.extnl_manageru_alloc_disp);
	*/

	memset(pt_shd_data, 0, sizeof(*pt_shd_data));

	//============================================================

	mc_workeru_co_t row, col, max_row, max_col;

	void* min_shd = (void*)MCH_EXTERNAL_RAM_BASE_PT;
	void* max_shd = (void*)(MCH_EXTERNAL_RAM_BASE_PT + lk_dat->extnl_ram_size);

	max_row = 1;
	max_col = 2;
	max_row = dev.rows;
	max_col = dev.cols;
	for (row=0; row < max_row; row++){
		for (col=0; col < max_col; col++){
			void* dst = ((void*) dev.workeru[row][col].mems.base);
			printf("min=%p dst=%p max=%p\n", min_shd, dst, max_shd);
			MCH_CK(addr_in_shd_mem(dst));
		}
	}

	
	unsigned long init_val = 10;
	tak_mak gg(init_val);

	printf("STARTING ADDR CONV \n");

	for(long kk=0; kk < NUM_OPERS; kk++) {
		printf("%ld\r", kk);
		//long ii = gg.gen_rand_int32_ie(0, TEST_NUM_PTS);
		//long osz = gg.gen_rand_int32_ie(MIN_OMC_SZ, MAX_OMC_SZ);
		uint32_t ld_addr = gg.gen_rand_int32();
		//uint32_t ld_addr = gg.gen_rand_int32_ie(0, 1000);

		bool islocal = mct_is_local(ld_addr);
		bool isonchip = islocal ? true : e_is_addr_on_chip((void *) ((uintptr_t) ld_addr));
		bool isexternal = ((! islocal) && (! isonchip));

		unsigned workeru_id = ld_addr >> 20;
		mc_addr_t workeru_id_2 = mc_addr_get_id(ld_addr);

		MCH_CK(workeru_id == workeru_id_2);

		bool islocal_2 = (workeru_id_2 == 0);
		bool isonchip_2 = mc_addr_in_sys(ld_addr);
		bool isexternal_2 = ((! islocal) && (! isonchip));

		//printf("%" PRIu32 " == %p  workeru_id=%u \n", ld_addr, (void*)ld_addr, workeru_id);
		bool islocal_3 = ! mc_addr_has_id(ld_addr);
		
		MCH_CK(islocal == islocal_2);
		MCH_CK(islocal == islocal_3);
		MCH_CK(isonchip == isonchip_2);
		MCH_CK(isexternal == isexternal_2);

		if(!islocal && isonchip){
			unsigned  globrow, globcol;
			ee_get_coords_from_id(&dev, workeru_id, &globrow, &globcol);
			mc_workeru_co_t g_ro = mc_id_to_ro(workeru_id);
			mc_workeru_co_t g_co = mc_id_to_co(workeru_id);

			//printf("workeru_id=%p \n", (void*)workeru_id);
			//printf("g_ro=%d globrow=%d\n", g_ro, globrow);

			MCH_CK(g_ro == globrow);
			MCH_CK(g_co == globcol);
		}

	}

	printf("\nFINISHED\n");
	

	//============================================================

	e_reset_group(&dev); // FAILS. Why?
	e_reset_system();
	
	e_close(&dev);
	
	e_free(&emem);
	e_finalize();

	return 0;
}


