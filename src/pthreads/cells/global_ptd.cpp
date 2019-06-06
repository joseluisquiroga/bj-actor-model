

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


#include "global.h"

#include "thread_ptd.hh"
#include "booter.h"

//======================================================================
// off chip shared memory

mc_off_sys_st mcm_external_manageru_data_obj;

//=====================================================================
// global funcs

void 
mck_abort(mc_addr_t err, char* orig_msg) {
	char msg[300];
	ptd_info_t* inf = mck_get_ptd_info();
	snprintf(msg, 300, "ABORTED THREAD=%ld \t WORKERU_ID=%x MSG=%s\n", inf->ptd_id, inf->ptd_workeru_id, orig_msg);
	mch_abort_func(err, msg);
}

mck_glb_sys_st*
mck_get_glb_sys(){
	return &(mck_get_ptd_info()->ptd_glb_sys_data);
}

void 
mck_set_irq0_handler(){
}

mc_addr_t 
mck_get_module_address(uint32_t modl_idx){
	return mc_null;
}

char* 
mck_get_module_name(uint32_t modl_idx){
	return mc_cstr("NO_MODULE_NAME");
}

void
mck_fill_module_external_addresses(int user_sz, char** user_order, mc_addr_t* user_ext_addr){
	for(int aa = 0; aa < user_sz; aa++){
		char* usr_nam = user_order[aa];
		if(usr_nam == mc_null){
			mck_abort(__LINE__, MC_ABORT_MSG("Null string in mck_fill_module_external_addresses (prm 1).\n"));
		}
		user_ext_addr[aa] = (mc_addr_t)usr_nam;
	}
}

bool
mck_load_module(mc_addr_t ext_addr){
	PTD_LOG("LOADING MODULE %p %s \n", (void*)ext_addr, (char*)ext_addr);
	MC_WORKERU_INFO->current_module_addr = ext_addr;
	return true;
}

