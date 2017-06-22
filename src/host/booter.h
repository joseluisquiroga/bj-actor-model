
// attribute.h

#ifndef MC_BOOTER_H
#define MC_BOOTER_H

#include <stdio.h>

#include "debug.h"
#include "stdbool.h"

#include "global.h"

#ifdef __cplusplus
mc_c_decl {
#endif

#define KBY_SZ 1024
#define MBY_SZ 1048576

#define MC_STDERR stderr

#define MC_PURE_LOCAL_ADDR 3
#define MC_CORE_LOCAL_ADDR 4
#define MC_EXTE_LOCAL_ADDR 5


extern mc_link_syms_data_st 	mch_external_ram_load_data;

#define mch_disp_to_extnal_ram_pt(disp) (the_str)

extern bool MCH_LOAD_WITH_MEMCPY;

#define MCM_DLMALLOC_HEAP_SZ (10 * MBY_SZ)
extern uint8_t mcm_dlmalloc_heap[MCM_DLMALLOC_HEAP_SZ];

void 
mch_abort_func(long val, const char* msg);

bool 
mch_call_assert(bool vv_ck, const char* file, int line, const char* ck_str, const char* msg);

#define mch_assert(vv) mch_call_assert(vv, __FILE__, __LINE__, #vv, NULL)

bool
mch_file_append(char* the_pth, char* the_data, long the_sz);

bool mch_ck_sys_data(mc_sys_sz_st* sys1);

void
mch_prt_exception(mck_glb_sys_st* sh_dat);

int 
mch_prt_in_core_shd_dat(mck_glb_sys_st* sh_dat);

bool
mch_rr_ck_zero(mc_rrarray_st* arr);

void
mch_rr_print(mc_rrarray_st* arr);

int
mch_type_sz(mc_type_t tt);

void
mch_reset_log_file(char* f_nm);

void
mch_print_out_buffer(mc_rrarray_st* arr, char* f_nm, mc_core_nn_t num_core);

uint8_t*
mch_read_file(char* the_pth, off_t* size);

int
mch_write_file(char* the_pth, uint8_t* the_data, long the_sz, int write_once);

int 
mch_prt_core_call_stack(const char *elf_nm, int addrs_sz, void** stack_addrs);

void
mch_get_enter(mc_core_co_t row, mc_core_co_t col);

void
mch_ptr_call_stack_trace();

void
mch_read_eph_link_syms(const char *executable, mc_link_syms_data_st* syms);

#define	MCH_DBG_COND_COMM(cond, comm)	\
	MC_DBG( \
		if(cond){ \
			comm; \
			printf("\n"); \
		} \
	) \

//--end_of_def

#define MCH_CK(prm) MC_DBG(mch_assert(prm))

#define MCH_CK_2(prm, comms1) \
	MCH_DBG_COND_COMM((! (prm)), \
		comms1; \
		printf("\n"); \
		mch_assert(prm); \
	) \
	
//--end_of_def


mc_addr_t
mc_znq_addr_to_eph_addr(mc_addr_t znq_addr);

mc_addr_t
mc_eph_addr_to_znq_addr(mc_addr_t eph_addr);

void
mcm_get_call_stack_trace(size_t trace_strs_sz, char** trace_strs);


#ifdef __cplusplus
}
#endif

#endif // MC_BOOTER_H

