

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

sornet.hh

Declaration of functions to use the sornet.

--------------------------------------------------------------*/

#ifndef SORNET_H
#define SORNET_H

#include "nervenet.hh"

#define BJ_SORNET_TIER 0

#define bj_pt_obj_as_num(pt_oo) (*((num_nod_t*)(pt_oo)))

#define bj_pt_to_num(pt) ((num_nod_t)pt)
#define bj_num_to_pt(num) ((void*)num)

//define BJ_SORNET_BIN_TEST
//define BJ_SORNET_NUM_TEST
//define BJ_SORNET_RANK_TEST
//define BJ_SORNET_SRT_RNK_TEST

#ifdef BJ_SORNET_BIN_TEST
#define BJ_DBG_SORBINS(prm) prm
#else
#define BJ_DBG_SORBINS(prm) 
#endif

#ifdef BJ_SORNET_NUM_TEST
#define BJ_DBG_SORNUMS(prm) prm
#else
#define BJ_DBG_SORNUMS(prm) 
#endif

#define BJ_DBG_GRPS_SORNUMS(prm) prm

#ifdef BJ_SORNET_RANK_TEST
#define BJ_DBG_RANK_OUTS(prm) prm
#else
#define BJ_DBG_RANK_OUTS(prm) 
#endif

#define BJ_DBG_GRPS_RANK(prm) 

#ifdef BJ_SORNET_SRT_RNK_TEST
#define BJ_DBG_SRT_RNK(prm) prm
#else
#define BJ_DBG_SRT_RNK(prm) 
#endif

#define BJ_DBG_ONE_GRP_ONLY(prm) 


void sorcell_sornet_handler(missive* msv) bj_sornet_cod;
void endcell_sornet_handler(missive* msv) bj_sornet_cod;
void nervenet_sornet_handler(missive* msv) bj_sornet_cod;

int bj_dbg_cmp_bin_objs(void* obj1, void* obj2) mc_external_code_ram;
int bj_dbg_cmp_num_objs(void* obj1, void* obj2) mc_external_code_ram;
int bj_cmp_rnk_objs(void* obj1, void* obj2) bj_sornet_cod;

void bj_sornet_kernel_func() bj_sornet_cod;
void bj_sornet_init_handlers() mc_external_code_ram;
void bj_sornet_main() mc_external_code_ram;

#endif		// SORNET_H


