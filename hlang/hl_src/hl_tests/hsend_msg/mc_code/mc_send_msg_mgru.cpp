

#include <stdio.h>

#include "cell.hh"

char* mch_epiphany_elf_path = (mc_cstr("the_epiphany_executable.elf"));

missive_handler_t nil_mgr_handlers[mck_tot_base_cell_classes];

void tests_init_handlers(){
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

	tests_init_handlers();

	kernel::run_manageru_sys();
	kernel::finish_manageru_sys();

	printf("ALL_HL_TESTS_FINISHED ==================================== \n");

	return 0;
}


