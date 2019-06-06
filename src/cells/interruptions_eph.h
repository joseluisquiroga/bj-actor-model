

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


#ifndef MC_INTERRUPTIONS_H
#define MC_INTERRUPTIONS_H

#include <inttypes.h>
#include "attribute.h"

#ifdef __cplusplus
mc_c_decl {
#endif

void mc_opt_sz_fn mc_isr_fn 
mck_sync_handler(void); // IVT_RESET

void mc_opt_sz_fn mc_isr_fn 
mck_software_exception_handler(void); // ivt_entry_software_exception

void mc_opt_sz_fn mc_isr_fn 
mck_page_miss_handler(void); // ivt_entry_page_miss

void mc_opt_sz_fn mc_isr_fn 
mck_timer0_handler(void); // ivt_entry_timer0

#ifdef MC_IS_EPH_CODE

	void mc_inline_fn
	mck_enable_all_irq() {
		mc_asm("mov r0, #0x0"); 
		mc_asm("movts imask, r0");
	}

	void mc_inline_fn
	mck_disable_all_irq() {
		mc_asm("mov r0, #0x3ff"); 
		mc_asm("movts imask, r0");
	}

	void mc_inline_fn
	mck_global_irq_disable() {
		mc_asm("gid"); 
	}

	void mc_inline_fn
	mck_global_irq_enable() {
		mc_asm("gie"); 
	}

	#define mck_simple_abort(func, excode) \
		mck_glb_sys_st* in_workeru_pt = MC_WORKERU_INFO; \
		in_workeru_pt->exception_code = excode; \
		in_workeru_pt->dbg_error_code = (mc_addr_t)(func); \
		mc_off_workeru_st* off_workeru_pt = MC_WORKERU_INFO->off_workeru_pt; \
		if((off_workeru_pt != mc_null) && (off_workeru_pt->magic_id == MC_MAGIC_ID)){ \
			off_workeru_pt->is_finished = MC_FINISHED_VAL; \
		} \
		mc_asm("trap 0x3"); \

	// end_macro

#else 

	void mc_inline_fn
	mck_enable_all_irq() {
	}

	void mc_inline_fn
	mck_disable_all_irq() {
	}

	void mc_inline_fn
	mck_global_irq_disable() {
	}

	void mc_inline_fn
	mck_global_irq_enable() {
	}

#endif	// MC_IS_EPH_CODE

#ifdef __cplusplus
}
#endif

#endif // MC_INTERRUPTIONS_H
