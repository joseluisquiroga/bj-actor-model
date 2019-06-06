

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


// thread_ptd.h

#ifndef THREAD_PTD_HH
#define THREAD_PTD_HH

#include <pthread.h>

#include "umm_malloc.h"
#include "global.h"
#include "cell.hh"
#include "booter.h"

#define NAMELEN 16

struct ptd_info_st {    // Used as argument to thread_start() 
	pthread_t 		ptd_id;        // id returned by pthread_create() 
	mc_workeru_nn_t	ptd_num;       // workeru consec
	char 			ptd_name[NAMELEN];

	void 		(*ptd_workeru_func)();

	mc_workeru_id_t 	ptd_workeru_id;	// workeru id as in epiphany arch
	mc_sys_sz_st 	ptd_system_sz;
	mck_glb_sys_st	ptd_glb_sys_data;
	mc_workeru_id_t	ptd_map_parent_workeru_id;
	mc_workeru_nn_t	ptd_map_tot_children;
	mc_load_map_st*	ptd_map_loaded;
	kernel 			ptd_THE_KERNEL;
};

typedef struct ptd_info_st ptd_info_t;

struct thread_info_st {    // Used as argument to thread_start() 
	ptd_info_t		thd_ptd;
	char*			thd_log_fnam;
	umm_block 		thd_umm_heap[UMM_HEAP_NUM_BLOCKS];
};

typedef struct thread_info_st thread_info_t;

extern ptd_info_t*	mcm_MANAGERU_PTD_INFO;

extern thread_info_t* ALL_THREADS_INFO;
extern int TOT_THREADS;
extern pthread_t MANAGERU_THREAD_ID;

uint16_t
mck_get_thread_idx();

ptd_info_t*
mck_get_ptd_info();

mc_inline_fn bool
mcm_addr_in_manageru(void* addr){
	uint8_t* pt = (uint8_t*)addr;
	uint8_t* hh = (uint8_t*)mcm_dlmalloc_heap;
	uint8_t* ll = hh + sizeof(mcm_dlmalloc_heap);
	if((pt >= hh) && (pt < ll)){ return true; }
	return false;
}

mc_inline_fn bool
mck_addr_in_workerus(void* addr){
	uint8_t* pt = (uint8_t*)addr;
	uint8_t* hh = (uint8_t*)ALL_THREADS_INFO;
	uint8_t* ll = hh + (sizeof(thread_info_t) * TOT_THREADS);
	if((pt >= hh) && (pt < ll)){ return true; }
	return false;
}

mc_inline_fn mc_workeru_nn_t
mck_get_addr_idx(void* addr){
	PTD_CK(mck_addr_in_workerus(addr));
	mc_workeru_nn_t idx = (mc_workeru_nn_t)(((uintptr_t)addr - (uintptr_t)ALL_THREADS_INFO) / sizeof(thread_info_t));
	return idx;
}

mc_inline_fn uintptr_t
mck_get_addr_offset(void* addr){
	PTD_CK(mck_addr_in_workerus(addr));
	uintptr_t ofs = (uintptr_t)(((uintptr_t)addr - (uintptr_t)ALL_THREADS_INFO) % sizeof(thread_info_t));
	return ofs;
}

mc_workeru_id_t
mcm_get_addr_workeru_id_fn(void* addr);

void*
mcm_addr_with_fn(mc_workeru_id_t id, void* addr);

mc_inline_fn bool
mc_is_manageru_thread(){
	return (pthread_self() == MANAGERU_THREAD_ID);
}

void
mc_uint16_to_hex_bytes(uint16_t ival, uint8_t* hex_str);

void *
thread_start(void *arg);

void 
mcm_thread_abort();

void
mch_load_map();

#ifdef __cplusplus
mc_c_decl {
#endif


#ifdef __cplusplus
}
#endif

#endif // THREAD_PTD_HH

