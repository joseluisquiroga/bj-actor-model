

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
#include "global.h"

//include "link_syms_vals.h"

mc_sys_sz_st 	mck_system_sz;


mc_addr_t
mc_manageru_addr_to_workeru_addr(mc_addr_t h_addr){
	return (MC_VAL_EXTERNAL_RAM_ORIG + (h_addr - MC_WORKERU_INFO->znq_shd_mem_base));
}

mc_addr_t
mc_workeru_addr_to_manageru_addr(mc_addr_t c_addr){
	return (MC_WORKERU_INFO->znq_shd_mem_base + (c_addr - MC_VAL_EXTERNAL_RAM_ORIG));
}

