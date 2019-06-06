// ----------------------------------------------------------------------------
// umm_malloc_eph.c - 
//
// ----------------------------------------------------------------------------

#include "umm_malloc.h"

//umm_block umm_heap[UMM_HEAP_NUM_BLOCKS] mc_alloc_ram;
umm_block *umm_heap = (umm_block*)MCK_LINK_SCRIPT_SYM_VAL(LD_WORKERU_ALLOC_ORIG);

