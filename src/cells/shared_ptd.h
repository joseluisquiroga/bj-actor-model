

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


// mc_shared_ptd.h

#ifndef MC_SHARED_DATA_PTD_H
#define MC_SHARED_DATA_PTD_H

#if ! defined(MC_IS_PTD_CODE)
	#error Only compile for PTD code (your linux)
#endif

#ifdef __cplusplus
mc_c_decl {
#endif

//======================================================================
// epiphany III macros
	
#define mc_axis_bits	6
#define mc_axis_mask	0x3f

typedef uintptr_t mc_addr_t;
typedef uint16_t mc_workeru_id_t;	// e_coreid_t
typedef uint16_t mc_workeru_co_t;
typedef uint16_t mc_workeru_nn_t;

//define mc_addr_val_in_p16(p16) ((mc_addr_t)(mc_v32_of_p16(p16)))

//======================================================================
// address macros

mc_workeru_id_t
mcm_get_addr_workeru_id_fn(void*);

void*
mcm_addr_with_fn(mc_workeru_id_t id, void* addr);

#define mc_addr_has_id(addr) true
#define mc_addr_get_id(addr) mcm_get_addr_workeru_id_fn((void*)(addr))
#define mc_addr_set_id(id, addr) mcm_addr_with_fn((id), (void*)(addr))
#define mc_addr_has_local_id(addr) (mc_addr_get_id(addr) == MC_WORKERU_INFO->the_workeru_id)
#define mc_addr_is_local(addr) mc_addr_has_local_id(addr)

#define mc_addr_get_disp(addr) ((mc_addr_t)(addr))
#define mc_addr_set_disp(disp, addr) mc_addr_set_id(mc_addr_get_id(addr), (disp))

#define mck_as_glb_pt(pt) ((void*)(pt))
#define mck_as_loc_pt(pt) ((void*)(pt))

#define mc_addr_same_id(addr1, addr2) (mc_addr_get_id(addr1) == mc_addr_get_id(addr2))

#ifdef __cplusplus
}
#endif

#endif // MC_SHARED_DATA_PTD_H


