

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


#include "shared.h"
#include "booter.h"

#include "workeru_loader_znq.h"

mc_sys_sz_st mch_system_sz;

mc_sys_sz_st*
mc_get_glb_sys_sz(){
	return &mch_system_sz;
}


mc_workeru_id_t 
mck_get_workeru_id(){
	mc_link_syms_data_st* lk_dat = &(MC_EXTERNAL_RAM_LOAD_DATA);
	return mc_addr_get_id(lk_dat->extnl_manageru_alloc_orig);
}

mc_addr_t
mc_manageru_addr_to_workeru_addr(mc_addr_t h_addr){
	return mc_znq_addr_to_eph_addr(h_addr);
}

mc_addr_t
mc_workeru_addr_to_manageru_addr(mc_addr_t c_addr){
	return mc_eph_addr_to_znq_addr(c_addr);
}

