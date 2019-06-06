

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

// loader_ptd.cpp

#include "global.h"
#include "umm_malloc.h"

#include "thread_ptd.hh"

mc_load_map_st* mck_first_load_map;

mc_workeru_id_t
mc_map_get_parent_workeru_id(){
	return mck_get_ptd_info()->ptd_map_parent_workeru_id;
}

mc_load_map_st*
mc_map_get_loaded(){
	return mck_get_ptd_info()->ptd_map_loaded;
}

mc_workeru_nn_t
mc_map_get_tot_children(){
	return mck_get_ptd_info()->ptd_map_tot_children;
}

