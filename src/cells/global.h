

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
/*! \file global.h

\brief Global functions. It is included in \ref cell.hh.

------------------------------------------------------------*/

#ifndef MC_GLOBAL_H
#define MC_GLOBAL_H

#ifdef MC_IS_PTD_CODE
	#include <sched.h>
#endif

#include "debug.h"
#include "shared.h"
#include "loader.h"
#include "trace.h"

#ifdef __cplusplus
mc_c_decl {
#endif

//=====================================================================
// in ekore shared memory

extern unsigned mck_original_ivt_0;

//define MC_MAX_STR_SZ 80
// WHEN TESTING LOGS USE
#define MC_MAX_STR_SZ MC_OUT_BUFF_SZ	

struct mc_aligned mck_glb_sys_def { 
	uint32_t 	magic_id;
	void** 		dbg_stack_trace;
	
	mc_addr_t 	dbg_min_sp;
	mc_addr_t 	dbg_error_code;
	char* 		dbg_error_str;
	uint32_t 	dbg_progress_flag;

	mc_addr_t	znq_shd_mem_base;
	mc_addr_t	eph_shd_mem_base;

	mc_workeru_id_t the_workeru_id;
	mc_workeru_nn_t the_workeru_nn;
	mc_workeru_co_t the_workeru_ro;
	mc_workeru_co_t the_workeru_co;

	mc_workeru_id_t		inited_workeru;
	mck_exception_t 	exception_code;
	
	uint32_t 	magic_end;

	mc_off_workeru_st* off_workeru_pt;
	mc_rrarray_st* 	write_rrarray;
	uint8_t 		dbg_out_str[MC_MAX_STR_SZ];

	mc_addr_t		current_module_addr;
	uint8_t			current_sub_module_id;
	void*			pt_workeru_kernel;

	EPH_CODE(
		void* 		mck_dbg_call_stack_trace[MC_MAX_CALL_STACK_SZ];
		uint16_t 	mck_trace_err;
	)
	PTD_CODE(
		uint8_t 	mck_sync_signal;
		char* 		mck_dbg_call_nams_stack_trace[MC_MAX_CALL_STACK_SZ];
	)
};
typedef struct mck_glb_sys_def mck_glb_sys_st;

//if defined(MC_IS_EPH_CODE) && !defined(IS_PTD_COD) 

#ifdef MC_IS_EPH_CODE
	mck_glb_sys_st*
	mck_get_first_glb_sys() mc_external_code_ram;

	extern mck_glb_sys_st*	mck_glb_pt_sys_data;
	#define MCK_FIRST_GLB_SYS mck_get_first_glb_sys()
	#define MC_WORKERU_INFO (mck_glb_pt_sys_data)
#else
	mck_glb_sys_st*
	mck_get_glb_sys();

	#define MCK_FIRST_GLB_SYS mck_get_glb_sys()
	#define MC_WORKERU_INFO mck_get_glb_sys()
#endif

#ifdef MC_IS_EPH_CODE
	extern mc_off_sys_st mck_external_manageru_data_obj;
	#define MCK_PT_EXTERNAL_MANAGERU_DATA (&mck_external_manageru_data_obj)
#endif

#ifdef MC_IS_ZNQ_CODE
	extern mc_off_sys_st* mcz_pt_external_manageru_data_obj;
	#define MCK_PT_EXTERNAL_MANAGERU_DATA mcz_pt_external_manageru_data_obj
#endif

#ifdef MC_IS_PTD_CODE
	extern mc_off_sys_st mcm_external_manageru_data_obj;
	#define MCK_PT_EXTERNAL_MANAGERU_DATA (&mcm_external_manageru_data_obj)
#endif


//=====================================================================
// global funcs

void 
mck_abort(mc_addr_t err, char* msg) mc_external_code_ram;

void 
mck_set_finished(uint8_t val) mc_external_code_ram;

void 
mck_glb_init(bool is_manageru) mc_external_code_ram;

void 
mck_glb_finish() mc_external_code_ram;

#define MC_B_OPCODE 0x000000e8 // OpCode of the B<*> instruction

void 
mck_set_irq0_handler() mc_external_code_ram;

//======================================================================
// mc_asserts

#define MC_ASSERT_MSG(cond) mc_cstr("ASSERT (" #cond ") failed at " __FILE__ "(" MC_TOSTRING(__LINE__) ")")
//define MC_ASSERT_MSG(cond) mc_cstr(#cond)

#define MCK_INWORKERU_ASSERT(cond) MC_DBG( \
	if(! (cond)){ \
		mck_abort(0xdeadbeaf, MC_ASSERT_MSG(cond)); \
	} )

// end_of_macro

#ifdef MC_IS_EPH_CODE
	#define MCK_CK(cond) MCK_INWORKERU_ASSERT(cond)
#endif

#ifdef MC_IS_PTD_CODE
	#include "booter.h"
	#define MCK_CK(cond) PTD_CK(cond)
#endif

#ifdef MC_IS_ZNQ_CODE
	#include "booter.h"
	#define MCK_CK(cond) MCH_CK(cond)
#endif

#define MCK_MARK_PLACE(nam) MC_DBG(mc_asm(#nam ":")) 

//======================================================================
// naked inside normal func (insted of naked attribute)

#define MCK_START_NAKED_FUNC(nam) \
	mc_asm( \
		".section .text \n\t" \
		".balign 4 \n\t" \
		".global " #nam " \n" \
	#nam ": \n\t" \
	); \

// end_of_macro

#define MCK_END_NAKED_FUNC() \
	mc_asm( \
		"trap 0x3 \n\t" \
		".previous \n\t" \
	); \

// end_of_macro

//define DBG_CODE_SHD_SZ 20
//extern uint16_t DBG_CODE_SHD_1[DBG_CODE_SHD_SZ];

void test_link_shd_code() mc_external_code_ram;

void ck_shd_code();

#ifdef MC_IS_EPH_CODE
	mc_inline_fn mc_addr_t
	mck_get_stack_pointer() {
		uint16_t* sp_val = 0;
		mc_asm("mov %0, sp" : "=r" (sp_val));
		return ((mc_addr_t)sp_val);
	}

	#define MCK_CHECK_SP() MC_DBG( \
		if(mck_get_stack_pointer() < MC_VAL_WORKERU_STACK_ORIG){ \
			mck_abort(__LINE__, MC_ABORT_MSG("Stack_overflow_error")); \
		} \
	) \

	// end_of_macro

#else
	#define MCK_CHECK_SP()
#endif

mc_inline_fn 
uint32_t 
mck_get_tot_modules(){
	return MCK_PT_EXTERNAL_MANAGERU_DATA->tot_modules;
}

mc_addr_t 
mck_get_module_address(uint32_t modl_idx) mc_external_code_ram;

char* 
mck_get_module_name(uint32_t modl_idx) mc_external_code_ram;

void
mck_fill_module_external_addresses(int user_sz, char** user_order, mc_addr_t* user_ext_addr) mc_external_code_ram;

bool
mck_load_module(mc_addr_t ext_addr) mc_external_code_ram;

//! Sets a sub module id to 'id'
#define mck_set_sub_module_id(id) { MC_WORKERU_INFO->current_sub_module_id = (id); }

void mc_manageru_init() mc_external_code_ram;
void mc_manageru_run() mc_external_code_ram;
void mc_manageru_finish() mc_external_code_ram;

extern mc_workeru_nn_t mch_first_load_workeru_nn;
extern char* mch_epiphany_elf_path;
extern void mc_workerus_main() mc_external_code_ram;
extern int mc_manageru_main(int argc, char *argv[]) mc_external_code_ram;

//! True if this workeru is in row 'ro' and column 'co'
#define mck_is_ro_co_workeru(ro, co) \
	((MC_WORKERU_INFO->the_workeru_ro == (ro)) && (MC_WORKERU_INFO->the_workeru_co == (co)))

//! True if this workeru has number 'nn'
#define mck_is_nn_workeru(nn) (MC_WORKERU_INFO->the_workeru_nn == (nn))

//! True if this workeru has id 'id'
#define mck_is_id_workeru(id) (MC_WORKERU_INFO->the_workeru_id == (id))

#define mck_has_off_workeru (MC_WORKERU_INFO->off_workeru_pt != mc_null)

#ifdef __cplusplus
}
#endif

#endif // MC_GLOBAL_H

