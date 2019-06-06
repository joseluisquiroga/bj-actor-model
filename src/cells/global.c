

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
#include "dyn_mem.h"

//include "link_syms_vals.h"

//======================================================================
// global funcs

//EPH_CODE(extern mc_addr_t LD_EXTERNAL_RAM_ORIG);

void 
mck_glb_init(bool is_manageru) {
	ZNQ_CODE(
		if(MC_EXTERNAL_RAM_LOAD_DATA.extnl_ram_orig == 0){
			mck_abort(__LINE__, MC_ABORT_MSG("mck_glb_init. Use mc_manageru_init before.\n"));
		}
	);
	mc_init_dyn_mem();

	// basic init
	mck_set_irq0_handler();
	EPH_CODE(mc_add_lk_syms());

	mck_glb_sys_st* glb_dat = MCK_FIRST_GLB_SYS;
	mc_memset((uint8_t*)glb_dat, 0, sizeof(mck_glb_sys_st));

	glb_dat->off_workeru_pt = mc_null;
	glb_dat->write_rrarray = mc_null;

	mc_sys_sz_st* sys_sz = MC_SYS_SZ;
	mc_init_glb_sys_sz(sys_sz);
	
	if(MCK_PT_EXTERNAL_MANAGERU_DATA->magic_id != MC_MAGIC_ID){
		mck_abort(__LINE__, MC_ABORT_MSG("mck_glb_init_case_1.\n"));
	}

	MCK_PT_EXTERNAL_MANAGERU_DATA->pt_this_from_eph = MCK_PT_EXTERNAL_MANAGERU_DATA;	// should be same for all workerus
	
	// glb_sys_sz init
	mc_workeru_id_t koid = mck_get_workeru_id();
	mc_memset((uint8_t*)sys_sz, 0, sizeof(mc_sys_sz_st));
	*sys_sz = MCK_PT_EXTERNAL_MANAGERU_DATA->wrk_sys;

	// num_workeru init
	mc_workeru_nn_t num_workeru = mc_id_to_nn(koid);

	glb_dat->magic_id = MC_MAGIC_ID;
	glb_dat->dbg_stack_trace = mc_null;
	glb_dat->magic_end = MC_MAGIC_END;	
	glb_dat->znq_shd_mem_base = MCK_PT_EXTERNAL_MANAGERU_DATA->znq_shared_mem_base;
	glb_dat->eph_shd_mem_base = MCK_PT_EXTERNAL_MANAGERU_DATA->eph_shared_mem_base;
	glb_dat->the_workeru_id = koid;
	glb_dat->the_workeru_ro = mc_id_to_ro(koid);
	glb_dat->the_workeru_co = mc_id_to_co(koid);
	glb_dat->the_workeru_nn = num_workeru;
	glb_dat->current_module_addr = (mc_addr_t)mc_null;
	glb_dat->current_sub_module_id = 0;
	glb_dat->pt_workeru_kernel = mc_null;

	if(num_workeru < mc_out_num_workerus){
		glb_dat->off_workeru_pt = &((MCK_PT_EXTERNAL_MANAGERU_DATA->sys_workerus)[num_workeru]);
		if((MCK_PT_EXTERNAL_MANAGERU_DATA->sys_out_buffs)[num_workeru].magic_id != MC_MAGIC_ID){
			mck_abort(__LINE__, MC_ABORT_MSG("mck_glb_init_case_2.\n"));
		}

		mc_workeru_out_st* out_st = &((MCK_PT_EXTERNAL_MANAGERU_DATA->sys_out_buffs)[num_workeru]);

		glb_dat->write_rrarray = &(out_st->wr_arr);
		mc_rr_init(glb_dat->write_rrarray, MC_OUT_BUFF_SZ, out_st->buff, 0);

		if(glb_dat->off_workeru_pt->magic_id != MC_MAGIC_ID){
			mck_abort(__LINE__, MC_ABORT_MSG("mck_glb_init_case_3.\n"));
		}

		// glb_dat->off_workeru_pt init	
		//mc_loop_set_var(glb_dat->off_workeru_pt->magic_id, MC_MAGIC_ID);
		void* glb_dat_wid = (void*)mc_addr_set_id(koid, glb_dat);
		MC_MARK_USED(glb_dat_wid);

		mc_loop_set_var(glb_dat->off_workeru_pt->ck_workeru_id, koid);
		mc_loop_set_var(glb_dat->off_workeru_pt->workeru_data, glb_dat_wid);
		
		mck_set_finished(MC_NOT_FINISHED_VAL);
		mc_loop_set_var(glb_dat->off_workeru_pt->is_waiting, MC_NOT_WAITING);
	}

	EPH_CODE(
		MCK_CK(glb_dat->eph_shd_mem_base == MC_VAL_EXTERNAL_RAM_ORIG)
		MCK_CK(glb_dat->znq_shd_mem_base != 0);
	)

	#if defined(MC_IS_EPH_CODE) && defined(MC_PLL_LOADING)
		mc_load_map();
	#endif

}

void 
mck_glb_finish(){
	mck_set_finished(MC_FINISHED_VAL);
}

void
mc_extnl_ram_load_data_fill(mc_link_syms_data_st* syms){
	mc_addr_t ex_orig = syms->extnl_ram_orig;
	syms->extnl_code_disp = syms->extnl_code_orig - ex_orig;
	syms->extnl_load_disp = syms->extnl_load_orig - ex_orig;
	syms->extnl_manageru_data_disp = syms->extnl_manageru_data_orig - ex_orig;
	syms->extnl_manageru_alloc_disp = syms->extnl_manageru_alloc_orig - ex_orig;
}

void 
mck_set_finished(uint8_t val) {
	mc_off_workeru_st* off_workeru_pt = MC_WORKERU_INFO->off_workeru_pt; 
	if(off_workeru_pt != mc_null){
		mc_loop_set_var(off_workeru_pt->is_finished, val);
	}
}


