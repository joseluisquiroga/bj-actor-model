

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


#include <stdio.h>

#include "cell.hh"
#include "booter.h"

char* mch_epiphany_elf_path = (mc_cstr("the_epiphany_executable.elf"));

int mc_manageru_main(int argc, char *argv[])
{
	if(argc > 1){
		mch_epiphany_elf_path = argv[1];
		printf("Using workeru executable: %s \n", mch_epiphany_elf_path);
	}

	kernel::init_manageru_sys();

	printf("STARTING min-manageru \n");

	/*
	kernel* ker = MCK_KERNEL;

	mc_workeru_id_t koid = ker->get_workeru_id(); 
	//mc_addr_t addr_0 = mc_addr_set_id(koid, 0);

	//mc_link_syms_data_st* lk_dat = &(MC_EXTERNAL_RAM_LOAD_DATA);
	//mc_addr_t addr_0 = (mc_addr_t)mch_disp_to_pt(lk_dat->extnl_manageru_alloc_disp);

	cell* act1 = kernel::get_first_cell();

	mc_addr_t eph_act1 = mc_znq_addr_to_eph_addr((mc_addr_t)act1);

	mc_workeru_id_t koid2 = mc_addr_get_id(eph_act1);

	printf("WORKERU_ID=%p Kcell=%p koid2=%p eph_act1=%p \n", 
		(void*)(mc_addr_t)koid, act1, (void*)(mc_addr_t)koid2, (void*)eph_act1);

	//kernel::run_manageru_sys();
	*/

	kernel::finish_manageru_sys();


	return 0;
}


