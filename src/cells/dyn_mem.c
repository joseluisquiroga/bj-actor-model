

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


#include "dyn_mem.h"
#include "global.h"

uint8_t* 
a64_malloc(umm_size_t num_bytes){
	umm_size_t mem_sz = ceil_64(num_bytes);
	uint8_t* tmp = (uint8_t*)umm_malloc(mem_sz);
	if(tmp != umm_null){
		tmp += umm_aligned_disp;
		MCK_CK(MC_IS_ALIGNED_64(tmp));
	}
	return tmp; 
}

uint8_t* 
a64_realloc(uint8_t* ptr, umm_size_t num_bytes){
	umm_size_t mem_sz = ceil_64(num_bytes);
	void* umm_ptr = umm_null;
	if(ptr != umm_null){
		umm_ptr = (void*)(((uint8_t*)ptr) - umm_aligned_disp);
	}
	uint8_t* tmp = (uint8_t*)umm_realloc((void*)umm_ptr, mem_sz);
	if(tmp != umm_null){
		tmp += umm_aligned_disp;
		MCK_CK(MC_IS_ALIGNED_64(tmp));
	}
	return tmp; 
}

void 
a64_free(uint8_t* ptr){
	if(ptr != umm_null){ 
		void* umm_ptr = (void*)(((uint8_t*)ptr) - umm_aligned_disp);
		umm_free(umm_ptr); 
		//ptr = umm_null;
	}
}

