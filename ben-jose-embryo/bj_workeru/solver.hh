

/*************************************************************

This file is part of ben-jose.

ben-jose is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

ben-jose is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ben-jose.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2007-2012, 2014-2016. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://github.com/joseluisquiroga/ben-jose

ben-jose is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------

solver.hh

Declaration of main bj solver function.

--------------------------------------------------------------*/

#ifndef SOLVER_H
#define SOLVER_H

#include "cell.hh"

//-----------------------------------------

//define BJ_DBG_ONLY 
//define BJ_DBG_SORNET 
#define BJ_NORMAL 

#ifdef BJ_DBG_SORNET
#define BJ_DBG_SORNET_CODE(prm) prm
#else
#define BJ_DBG_SORNET_CODE(prm) 
#endif

//-----------------------------------------

#define BJ_DBG_TOT_NUM_TESTS 20
#define BJ_DBG_TOT_RNK_TESTS 20
#define BJ_DBG_TOT_SRT_RNK_TESTS 20

#define BJ_DBG_TOT_INPUT_SORNODES 50
#define BJ_DBG_TOT_INPUT_RNKNODES BJ_DBG_TOT_INPUT_SORNODES
#define BJ_DBG_MAX_GRP_SZ 10
#define BJ_DBG_MIN_NUM_REP_VAL 0
#define BJ_DBG_MAX_NUM_REP_VAL 3
#define BJ_DBG_MAX_REP_DIST 3

#define BJ_INVALID_IDX -1

#define bj_dbg_only_cod mc_mod1_cod
#define bj_dbg_only_dat mc_mod1_dat

#define bj_load_cod mc_mod2_cod
#define bj_load_dat mc_mod2_dat

#define bj_propag_cod mc_mod3_cod
#define bj_propag_dat mc_mod3_dat

#define bj_stabi_cod mc_mod4_cod
#define bj_stabi_dat mc_mod4_dat

#define bj_sornet_cod mc_mod5_cod
#define bj_sornet_dat mc_mod5_dat

#define bj_pgroup_cod mc_mod6_cod
#define bj_pgroup_dat mc_mod6_dat

#define bj_nervenet_cod 
#define bj_nervenet_dat 
#define bj_nervenet_mem mc_mod0_cod

#define	bj_srt_input_flag 		mc_flag0
#define	bj_rnk_input_flag 		mc_flag1
#define	bj_rnk_up_out_flag 		mc_flag2
#define	bj_rnk_down_out_flag 	mc_flag3
#define	bj_rnk_up_end_flag 		mc_flag4
#define	bj_rnk_down_end_flag 	mc_flag5

typedef unsigned long u_num_nod_t;
typedef long num_nod_t;
typedef uint8_t num_syn_t;
typedef uint32_t num_tier_t;
typedef uint32_t num_pulse_t;
typedef uint32_t dbg_consec_t;
typedef uint32_t num_layer_t;

#define BJ_INVALID_NUM_NODE ((num_nod_t)(-1))
#define BJ_INVALID_NUM_SYNAPSE ((num_syn_t)(~((num_syn_t)0x0)))
#define BJ_INVALID_NUM_TIER ((num_tier_t)(~((num_tier_t)0x0)))


#define BJ_LAST_TIER (BJ_INVALID_NUM_TIER - 1)

#define BJ_MAX_NODE_SZ mc_maxof(num_syn_t)

#define MAGIC_VAL 987654

enum node_kind_t : uint8_t {
	nd_invalid = 0,
	nd_pos,
	nd_neg,
	nd_neu
};

#define bj_is_pol(ki) ((ki == nd_pos) || (ki == nd_neg))

#define bj_cell_id(cls) BJ_CELL_ID_##cls

enum bj_cell_id_t : mck_handler_idx_t {
	bj_cell_id(pre_sornode) = mck_tot_base_cell_classes,
	bj_cell_id(pre_endnode),
	bj_cell_id(pre_item_pgroup),
	bj_cell_id(pre_pgroup),
	bj_cell_id(pre_cnf_node),

	bj_cell_id(pru_hcell),
	
	bj_mgr_last_invalid,
	bj_mgr_idx_total
};

#endif		// SOLVER_H


