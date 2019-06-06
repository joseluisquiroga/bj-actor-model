

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

// loader_eph.c 

#include "global.h"
#include "all_regs.h"
#include "umm_malloc.h"

mc_workeru_id_t 		mcz_parent_broadcast_workeru_id;
mc_load_map_st* 	mcz_broadcast_map;
mc_workeru_nn_t 		mcz_tot_children;

mc_load_map_st* mck_first_load_map;  // in shared mem

mc_link_syms_data_st mck_external_ram_load_data mc_lk_syms_dat = {
	.extnl_ram_size = MC_VAL_EXTERNAL_RAM_SIZE,
	.extnl_code_size = MC_VAL_EXTERNAL_CODE_SIZE,
	.extnl_load_size = MC_VAL_EXTERNAL_LOAD_SIZE,
	.extnl_manageru_data_size = MC_VAL_EXTERNAL_MANAGERU_DATA_SIZE,
	.extnl_manageru_alloc_size = MC_VAL_EXTERNAL_MANAGERU_ALLOC_SIZE,
	.extnl_ram_orig = MC_VAL_EXTERNAL_RAM_ORIG,
	.extnl_code_orig = MC_VAL_EXTERNAL_CODE_ORIG,
	.extnl_load_orig = MC_VAL_EXTERNAL_LOAD_ORIG,
	.extnl_manageru_data_orig = MC_VAL_EXTERNAL_MANAGERU_DATA_ORIG,
	.extnl_manageru_alloc_orig = MC_VAL_EXTERNAL_MANAGERU_ALLOC_ORIG,

	.workeru_module_orig = MC_VAL_WORKERU_MODULE_ORIG,
	.workeru_module_size = MC_VAL_WORKERU_MODULE_SIZE,

	.extnl_code_disp = 0,
	.extnl_load_disp = 0,
	.extnl_manageru_data_disp = 0,
	.extnl_manageru_alloc_disp = 0
};

void*
mc_add_lk_syms(){
	mc_extnl_ram_load_data_fill(&mck_external_ram_load_data);
	return (void*)&mck_external_ram_load_data;
};

char* err_loader_eph_01 mc_external_data_ram = "mc_load_map. Null mcz_broadcast_map.";
char* err_loader_eph_02 mc_external_data_ram = "mc_load_map. Bad num workeru.";

void
mc_load_map(){
	mcz_tot_children = 0;

	mc_load_map_st* mp = mcz_broadcast_map;
	mc_workeru_id_t koid = mck_get_workeru_id();

	/*
	mck_sprt("K=");
	mck_xprt(koid);
	mck_sprt("_P=");
	mck_xprt(mcz_parent_broadcast_workeru_id);
	mck_sprt("_\n");

	mck_sprt("STARTING_LOAD_MAP workeru___");
	mck_xprt(koid);
	EPH_CODE(
		mck_sprt("___umm_heap___");
		mck_xprt((mc_addr_t)umm_heap);
	)
	//mck_sprt("___MC_WORKERU_INFO___");
	//mck_xprt((mc_addr_t)MC_WORKERU_INFO);
	mck_sprt("___\n");
	*/

	if(mp == mc_null){
		if(MCK_PT_EXTERNAL_MANAGERU_DATA->first_load_workeru_id != koid){
			mck_abort((mc_addr_t)err_loader_eph_01, err_loader_eph_01);
		}
		if(mck_first_load_map != mc_null){
			mcz_broadcast_map = mck_first_load_map;
		} else {
			mcz_broadcast_map = mc_get_first_load_map();
		}
		mp = mcz_broadcast_map;
	}
	if(mp->num_workeru != mc_id_to_nn(koid)){
		//mck_sprt("ABORTING_LOAD_MAP");
		mck_abort((mc_addr_t)err_loader_eph_02, err_loader_eph_02);
		return;
	}
	//mck_slog("NUM_WORKERU_OK2=");
	//mck_ilog(mcz_broadcast_map->num_workeru);
	//mck_slog("___\n");

	mc_workeru_id_t* pt_parent = &mcz_parent_broadcast_workeru_id;
	mc_load_map_st** pt_ld_map = &mcz_broadcast_map;

	if(mp->childs == mc_null){ return; }

	//mck_sprt("pt_ld_map___");
	//mck_xprt((mc_addr_t)pt_ld_map);
	//mck_sprt("___\n");

	//uint32_t SYNC = (1 << E_SYNC);
	//uint32_t SYNC = (1 << 0);
	uint32_t SYNC = 1;
	int aa = 0;
	mc_load_map_st* ch_map = (mp->childs)[aa];
	while(ch_map != mc_null){
		mc_workeru_id_t ch_id = mc_nn_to_id(ch_map->num_workeru);

		uint8_t* dst = (uint8_t*)mc_addr_set_id(ch_id, 0x0);

		mc_memcpy(dst, (uint8_t*)0x0, mc_workeru_tot_mem);

		unsigned *ivt = (unsigned*)mc_addr_set_id(ch_id, 0x0);
		mc_loop_set_var(*ivt, mck_original_ivt_0);

		mc_load_map_st** ch_ld_map = (mc_load_map_st**)mc_addr_set_id(ch_id, pt_ld_map);
		mc_loop_set_var(*ch_ld_map, ch_map);

		mc_workeru_id_t* ch_ld_parent = (mc_workeru_id_t*)mc_addr_set_id(ch_id, pt_parent);
		mc_loop_set_var(*ch_ld_parent, koid);

		uint32_t* rem_reg = (uint32_t*)mc_addr_set_id(ch_id, MC_REG_ILATST);

		//mck_sprt("rem_reg___");
		//mck_xprt((mc_addr_t)rem_reg);
		//mck_sprt("___\n");

		*rem_reg = SYNC;

		aa++;
		ch_map = (mp->childs)[aa];
	}

	mcz_tot_children = aa;
}

/*
void
mc_prt_margin(int lv){
	while(lv-- >= 0){ mck_sprt("   "); }
}

void
mc_prt_load_map(mc_load_map_st* mp, int lv){
	mc_prt_margin(lv);
	mck_sprt("[");
	if(mp == mc_null){
		mck_sprt("NULL_MAP");
		mck_sprt("]\n");
		return;
	}
	mck_sprt("_");
	mck_iprt(mp->num_workeru);
	if(mp->childs == mc_null){
		//mc_prt_margin(lv);
		//mck_sprt("NULL_CHILDS \n");
		mck_sprt("_]\n");
		return;
	}
	mck_sprt("_\n");

	int aa = 0;
	mc_load_map_st* ch_map = (mp->childs)[aa];
	while(ch_map != mc_null){
		mc_prt_load_map(ch_map, lv + 1);
		aa++;
		ch_map = (mp->childs)[aa];
	}
	
	mc_prt_margin(lv);
	mck_sprt("]\n");
}

*/

