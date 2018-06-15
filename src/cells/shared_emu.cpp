

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

#include "thread_emu.hh"
#include "booter.h"

mc_sys_sz_st*
mc_get_glb_sys_sz(){
	return &(mck_get_emu_info()->emu_system_sz);
}

mc_core_id_t 
mck_get_core_id(){
	return mck_get_emu_info()->emu_core_id;
}

mc_addr_t
mc_host_addr_to_core_addr(mc_addr_t h_addr){
	return h_addr;
}

mc_addr_t
mc_core_addr_to_host_addr(mc_addr_t c_addr){
	return c_addr;
}

