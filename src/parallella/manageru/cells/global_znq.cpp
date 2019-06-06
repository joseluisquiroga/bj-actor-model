

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
#include "global.h"
#include "booter.h"

//======================================================================
// off chip shared memory

mc_off_sys_st* mcz_pt_external_manageru_data_obj = mc_null;

//=====================================================================
// global funcs

mck_glb_sys_st	mch_glb_sys_data;

void 
mck_abort(mc_addr_t err, char* msg) {
	char full_msg[200];
	snprintf(full_msg, 200, "ABORTED_ZNQ_MANAGERU_MC_SYS. %s\n", msg);
	mch_abort_func(err, full_msg);
}

mck_glb_sys_st*
mck_get_glb_sys(){
	return &mch_glb_sys_data;
}

void 
mck_set_irq0_handler(){
}

