

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

//include "booter.h"
#include "cell.hh"

char* mch_epiphany_elf_path = (mc_cstr("the_epiphany_executable.elf"));

missive_handler_t nil_mgr_handlers[mck_tot_base_cell_classes];

void sort_net_mgr_init_handlers(){
	missive_handler_t* hndlrs = nil_mgr_handlers;
	mc_init_arr_vals(mck_tot_base_cell_classes, hndlrs, mc_null);
	hndlrs[mck_tot_base_cell_classes - 1] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(mck_tot_base_cell_classes);
	kernel::set_cell_handlers(hndlrs);
}

int mc_manageru_main(int argc, char *argv[])
{
	if(argc > 1){
		mch_epiphany_elf_path = argv[1];
		printf("Using workeru executable: %s \n", mch_epiphany_elf_path);
	}

	kernel::init_manageru_sys();

	sort_net_mgr_init_handlers();

	kernel::run_manageru_sys();
	kernel::finish_manageru_sys();

	printf("ALL FINISHED ==================================== \n");

	return 0;
}


