

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
/*! \file mods_test.cpp
\ingroup docgrp_modules

\brief Main file for the modules example.

\details 

\include mods_test.cpp

------------------------------------------------------------*/


/*! \defgroup docgrp_modules modules example
\brief Modules example

\details 
The modules example has three modules.

*/

#include "cell.hh"
#include "pru_1.h"
#include "pru_2.h"
#include "pru_3.h"

mc_c_decl int main();

char* m1_nam mc_external_data_ram = mc_cstr("module1");
char* m2_nam mc_external_data_ram = mc_cstr("module2");
char* m3_nam mc_external_data_ram = mc_cstr("module3");

#define TOT_MODS 3

#define MOD1_IDX 1
#define MOD2_IDX 2
#define MOD3_IDX 0

char* all_mod_nams[TOT_MODS] mc_external_data_ram;
mc_addr_t* all_mod_addr;

void
init_module_nams() mc_external_code_ram;

void
init_module_nams(){
	all_mod_nams[MOD1_IDX] = m1_nam;
	all_mod_nams[MOD2_IDX] = m2_nam;
	all_mod_nams[MOD3_IDX] = m3_nam;
}


void
print_module_nams() mc_external_code_ram;

void
print_module_nams(){
	uint32_t to_m = mck_get_tot_modules();
	uint32_t aa = 0;
	mck_sprt2("ALL_MODULE_NAMES=\n");
	for(aa = 0; aa < to_m; aa++){
		char* nam0 = mck_get_module_name(aa);
		mck_sprt2(nam0);
		mck_sprt2("\n");
	}
}

void
print_module_addrs() mc_external_code_ram;

void
print_module_addrs(){
	uint32_t aa = 0;
	mck_sprt2("ALL_MODULE_ADDRS=\n");
	for(aa = 0; aa < TOT_MODS; aa++){
		char* nam0 = all_mod_nams[aa];
		mck_sprt2(nam0);
		mck_sprt2(" addr____");
		mck_xprt(all_mod_addr[aa]);
		mck_sprt2("____\n");
	}
}

void
link_all_modules_aux_fn() mc_external_code_ram;

void
link_all_modules_aux_fn(){
	m1_main();
	m2_main();
	m3_main();
}

int main() {
	mck_glb_init(false);
	mck_slog2("Modules main \n");

	init_module_nams();
	PTD_CK(mck_get_tot_modules() >= TOT_MODS);

	all_mod_addr = mc_malloc32(mc_addr_t, TOT_MODS);
	mck_fill_module_external_addresses(TOT_MODS, all_mod_nams, all_mod_addr);

	if(MC_WORKERU_INFO->the_workeru_nn == 0){	
		//print_module_nams();
		//print_module_addrs();
		bool ok = mck_load_module(all_mod_addr[MOD2_IDX]);
		if(ok){
			m2_main();
		}
	}
	if(MC_WORKERU_INFO->the_workeru_nn == 1){	
		//print_module_nams();
		//print_module_addrs();
		bool ok = mck_load_module(all_mod_addr[MOD1_IDX]);
		if(ok){
			m1_main();
		}
	}
	if(MC_WORKERU_INFO->the_workeru_nn == 3){	
		//print_module_nams();
		//print_module_addrs();
		bool ok = mck_load_module(all_mod_addr[MOD3_IDX]);
		if(ok){
			m3_main();
		}
	}
	mck_slog2("______________________\n");

	mck_glb_finish();
	return 0;
}

