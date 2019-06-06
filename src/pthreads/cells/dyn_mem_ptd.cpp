

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

#include "thread_ptd.hh"
#include "booter.h"
#include "dyn_mem.h"

#include "dlmalloc.h"

extern mspace mcm_glb_mspace;

uint8_t* 
mc_malloc_impl(umm_size_t num_bytes){
	if(mc_is_manageru_thread()){
		return (uint8_t*)mspace_malloc(mcm_glb_mspace, num_bytes);
	}
	return a64_malloc(num_bytes);
}

uint8_t* 
mc_realloc_impl(uint8_t* ptr, umm_size_t num_bytes){
	if(mc_is_manageru_thread()){
		return (uint8_t*)mspace_realloc(mcm_glb_mspace, ptr, num_bytes);
	}
	return a64_realloc(ptr, num_bytes);
}

void 
mc_free_impl(uint8_t* ptr){
	if(mc_is_manageru_thread()){
		mspace_free(mcm_glb_mspace, ptr);
		return;
	}
	a64_free(ptr);
}

