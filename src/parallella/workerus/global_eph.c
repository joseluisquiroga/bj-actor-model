

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


#include "interruptions_eph.h"
#include "global.h"
#include "dyn_mem.h"

//======================================================================
// off chip shared memory

mc_off_sys_st mck_external_manageru_data_obj mc_external_manageru_data_ram;

//=====================================================================
// global data

mck_glb_sys_st*	mck_glb_pt_sys_data;

unsigned mck_original_ivt_0;

//=====================================================================
// global funcs

mck_glb_sys_st*
mck_get_first_glb_sys(){
	mck_glb_pt_sys_data = mc_malloc32(mck_glb_sys_st, 1);
	return mck_glb_pt_sys_data;
}

void abort() mc_external_code_ram;

void
abort(){	// Needed when optimizing for size
	MCK_CK(0);
	mck_abort(__LINE__, MC_ABORT_MSG("abort() func.\n"));
	while(1);
}

void 
mck_set_irq0_handler(){
	unsigned * ivt = 0x0;
	mck_original_ivt_0 = *ivt;
	*ivt = ((((unsigned)mck_sync_handler) >> 1) << 8) | MC_B_OPCODE;
}

/*! 
This function aborts execution. It saves the stack trace and the Zynq side will print it after aborting.
*/
void 
mck_abort(mc_addr_t err, char* msg) {
	int16_t sz_trace = -1;
	void** trace = mc_null;

	if(msg != mc_null){
		sz_trace = MC_MAX_CALL_STACK_SZ;
		trace = MC_WORKERU_INFO->mck_dbg_call_stack_trace;
	}
	if((trace != mc_null) && (sz_trace > 0)){
		mck_get_call_stack_trace(sz_trace, trace);
	}
	mck_glb_sys_st* in_shd = MC_WORKERU_INFO;
	in_shd->dbg_error_code = err;
	in_shd->dbg_error_str = msg;

	mc_off_workeru_st* off_workeru_pt = MC_WORKERU_INFO->off_workeru_pt;
	if((off_workeru_pt != mc_null) && (off_workeru_pt->magic_id == MC_MAGIC_ID)){
		mc_loop_set_var(off_workeru_pt->is_finished, MC_FINISHED_VAL);
	}
	
	mc_asm("mov r62, %0" : : "r" (in_shd));
	mc_asm("mov r63, %0" : : "r" (err));
	mc_asm("gid");
	mc_asm("trap 0x3");
	mc_asm("movfs r0, pc");
	mc_asm("jalr r0");
}

void
mc_manageru_init(){
	// a workeru must never call this
	mck_abort(__LINE__, MC_ABORT_MSG("mc_manageru_init abort\n"));
}

void
mc_manageru_run(){
	// a workeru must never call this
	mck_abort(__LINE__, MC_ABORT_MSG("mc_manageru_run abort\n"));
}

void
mc_manageru_finish(){
	// a workeru must never call this
	mck_abort(__LINE__, MC_ABORT_MSG("mc_manageru_finish abort\n"));
}

mc_addr_t 
mck_get_module_address(uint32_t modl_idx){
	uint32_t tot_modls = mck_get_tot_modules();
	if(modl_idx >= tot_modls){
		return mc_null;
	}
	mc_addr_t mod_sz = MC_VAL_WORKERU_MODULE_SIZE;
	uint8_t* pt_ext_mods = (uint8_t*)MC_VAL_EXTERNAL_LOAD_ORIG;
	return ((mc_addr_t)(pt_ext_mods + (mod_sz * modl_idx)));
}

char* 
mck_get_module_name(uint32_t modl_idx){
	uint32_t tot_modls = mck_get_tot_modules();
	if(modl_idx >= tot_modls){
		return mc_null;
	}
	mc_addr_t mod_sz = MC_VAL_WORKERU_MODULE_SIZE;
	uint8_t* pt_ext_mods = (uint8_t*)MC_VAL_EXTERNAL_LOAD_ORIG;
	char** pt_mod_nams = (char**)(pt_ext_mods + (mod_sz * tot_modls));
	return pt_mod_nams[modl_idx];
}

void
mck_fill_module_external_addresses(int user_sz, char** user_order, mc_addr_t* user_ext_addr){
	uint32_t tot_modls = mck_get_tot_modules();
	for(uint32_t aa = 0; aa < user_sz; aa++){
		char* usr_nam = user_order[aa];
		if(usr_nam == mc_null){
			mck_abort(__LINE__, MC_ABORT_MSG("Null string in mck_fill_module_external_addresses (prm 1).\n"));
		}
		for(uint32_t bb = 0; bb < tot_modls; bb++){
			char* link_nam = mck_get_module_name(bb);
			if(link_nam == mc_null){
				mck_abort(__LINE__, MC_ABORT_MSG("Null string in mck_fill_module_external_addresses (prm 2 ).\n"));
			}
			int cc = mc_strcmp(usr_nam, link_nam);
			if(cc == 0){
				user_ext_addr[aa] = mck_get_module_address(bb);
				bb = tot_modls;
			}
		}
	}
}

char dbg_glb_LOADING_module_[] mc_external_data_ram = "LOADING_module_idx__";
char dbg_glb_addr_[] mc_external_data_ram = "__addr__";
char dbg_glb_addr_end[] mc_external_data_ram = "__starting.\n";

bool
mck_load_module(mc_addr_t ext_addr){
	uint8_t* pt_mod = (uint8_t*)ext_addr;
	if(pt_mod == mc_null){
		return false;
	}
	mc_workeru_id_t koid = mck_get_workeru_id();
	mc_workeru_nn_t num_workeru = mc_id_to_nn(koid);

	mck_sprt(dbg_glb_LOADING_module_);
	mck_iprt(num_workeru);
	mck_sprt(dbg_glb_addr_);
	mck_xprt((mc_addr_t)pt_mod);
	mck_sprt(dbg_glb_addr_end);
	
	mck_slog(dbg_glb_LOADING_module_);
	mck_ilog(num_workeru);
	mck_slog(dbg_glb_addr_);
	mck_xlog((mc_addr_t)pt_mod);
	mck_slog(dbg_glb_addr_end);

	mc_addr_t mod_sz = MC_VAL_WORKERU_MODULE_SIZE;
	uint8_t* pt_mem_mod = (uint8_t*)MC_VAL_WORKERU_MODULE_ORIG;
	mc_memcpy(pt_mem_mod, pt_mod, mod_sz);

	MC_WORKERU_INFO->current_module_addr = ext_addr;
	return true;
}

