

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


#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "trace.h"

#include "booter.h"
#include "thread_ptd.hh"

//=====================================================================

uint16_t
mck_get_call_stack_trace(int16_t, void**) {
	mck_glb_sys_st* glb = MC_WORKERU_INFO;

	mcm_get_call_stack_trace(MC_MAX_CALL_STACK_SZ, glb->mck_dbg_call_nams_stack_trace);
	return 0;
}

void 
mck_wait_sync(uint32_t info, int16_t sz_trace, void** trace){
	mc_off_workeru_st* off_workeru_pt = MC_WORKERU_INFO->off_workeru_pt;
	if(off_workeru_pt == mc_null){
		return;
	}
	if((sz_trace > 0) && (trace != mc_null)){
		mck_get_call_stack_trace(sz_trace, trace);
	}
	if(info == MC_NOT_WAITING){
		info = MC_WAITING_ENTER;
	}
	mc_loop_set_var(off_workeru_pt->is_waiting, info);

	uint8_t& sync_sg = MC_WORKERU_INFO->mck_sync_signal;
	while(sync_sg == 0){
		sched_yield();
	}

	mc_loop_set_var(off_workeru_pt->is_waiting, MC_NOT_WAITING);
	sync_sg = 0;
}

