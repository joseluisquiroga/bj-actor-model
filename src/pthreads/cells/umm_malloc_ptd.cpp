// ----------------------------------------------------------------------------
// umm_mallocptd_.c - 
//
// ----------------------------------------------------------------------------

#include "umm_malloc.h"

#include "thread_ptd.hh"
#include "booter.h"

umm_block*
umm_get_heap(){
	uint16_t thd_idx = mck_get_thread_idx();
	return ALL_THREADS_INFO[thd_idx].thd_umm_heap;
}

