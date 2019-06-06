

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

------------------------------------------------------------
dyn_mem.hh

Declaration of dynamic mem funcs.

------------------------------------------------------------*/

#ifndef MC_DYN_MEM_HH
#define MC_DYN_MEM_HH

#include "umm_malloc.h"

#ifdef __cplusplus
mc_c_decl {
#endif

//======================================================================
// aligned umm_malloc memory allocation 
//
// The following code depends on the umm_malloc implementation. 
// So, if you change allocator, you must change the code.

// the umm implementation already is 32 bit aligned

#define a32_malloc(size) umm_malloc(size)
#define a32_realloc(ptr, size) umm_realloc((ptr), (size))
#define a32_free(ptr) umm_free(ptr)

#define ceil_64(vv) (((vv % 8) == 0) ? (vv) : ((vv) + (8 - ((vv) % 8))))

#define umm_aligned_disp 4

mc_opt_sz_fn uint8_t* 
a64_malloc(umm_size_t num_bytes) mc_external_code_ram;

mc_opt_sz_fn uint8_t* 
a64_realloc(uint8_t* ptr, umm_size_t num_bytes) mc_external_code_ram;

mc_opt_sz_fn void 
a64_free(uint8_t* ptr) mc_external_code_ram;

#ifdef MC_IS_EPH_CODE
	#define mc_init_dyn_mem()	umm_init()

	#define mc_malloc32(nam, sz)	(nam *)(a32_malloc(sz * sizeof(nam)))
	#define mc_realloc32(nam, ptr, sz)	(nam *)(a32_realloc((ptr), (sz * sizeof(nam))))
	#define mc_free32(ptr)	a32_free(ptr)

	#define mc_malloc64(nam, sz)	(nam *)(a64_malloc(sz * sizeof(nam)))
	#define mc_realloc64(nam, ptr, sz)	(nam *)(a64_realloc((ptr), (sz * sizeof(nam))))
	#define mc_free64(ptr)	a64_free(ptr)
#endif

#ifdef MC_IS_ZNQ_CODE
	#include "dlmalloc.h"
	extern mspace mch_glb_alloc_mspace;

	#define mc_init_dyn_mem()	

	#define mc_malloc32(nam, sz)	(nam *)(mspace_malloc(mch_glb_alloc_mspace, (sz * sizeof(nam))))
	#define mc_realloc32(nam, ptr, sz)	(nam *)(mspace_realloc(mch_glb_alloc_mspace, ((ptr), (sz * sizeof(nam)))))
	#define mc_free32(ptr)	mspace_free(mch_glb_alloc_mspace, ptr)

	#define mc_malloc64(nam, sz)	((nam *)(mspace_malloc(mch_glb_alloc_mspace, (sz * sizeof(nam)))))
	#define mc_realloc64(nam, ptr, sz)	((nam *)(mspace_realloc(mch_glb_alloc_mspace, ((ptr), (sz * sizeof(nam))))))
	#define mc_free64(ptr)	mspace_free(mch_glb_alloc_mspace, ptr)
#endif

#ifdef MC_IS_PTD_CODE
	uint8_t* 
	mc_malloc_impl(umm_size_t num_bytes);

	uint8_t* 
	mc_realloc_impl(uint8_t* ptr, umm_size_t num_bytes);

	void 
	mc_free_impl(uint8_t* ptr);

	#define mc_init_dyn_mem()

	#define mc_malloc32(nam, sz)	(nam *)(mc_malloc_impl(sz * sizeof(nam)))
	#define mc_realloc32(nam, ptr, sz)	(nam *)(mc_realloc_impl((ptr), (sz * sizeof(nam))))
	#define mc_free32(ptr)	mc_free_impl(ptr)

	#define mc_malloc64(nam, sz)	(nam *)(mc_malloc_impl(sz * sizeof(nam)))
	#define mc_realloc64(nam, ptr, sz)	(nam *)(mc_realloc_impl((ptr), (sz * sizeof(nam))))
	#define mc_free64(ptr)	mc_free_impl(ptr)
#endif

#ifdef __cplusplus
}
#endif

#endif // MC_DYN_MEM_HH


