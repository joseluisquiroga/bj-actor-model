

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
/*! \file cell.hh

\brief The one and only include file necessarry to use the Messagging Cells library.

\callgraph
\callergraph
------------------------------------------------------------*/

#ifndef CELL_HH
#define CELL_HH

#include <new>
#include <functional>

#include "dyn_mem.h"
#include "binder.hh"
#include "global.h"
//include "workerus_main.h"

// look for "class mc_aligned "

class kernel;
class agent;
class cell;
class missive;
class agent_ref;
class agent_grp;

typedef agent_grp missive_grp_t;
typedef agent_ref missive_ref_t;
typedef agent_grp cell_grp_t;
typedef agent_ref cell_ref_t;

typedef uint16_t mck_token_t; 
typedef uint16_t mc_alloc_size_t; 


#define BJ_INVALID_TOKEN ((mck_token_t)(~((mck_token_t)0x0)))
#define BJ_INVALID_ALLOC_SZ ((mc_alloc_size_t)(~((mc_alloc_size_t)0x0)))

//-------------------------------------------------------------------------
// flags

typedef uint8_t mc_flags_t;

#define	mc_flag0	((mc_flags_t)0x01)
#define	mc_flag1	((mc_flags_t)0x02)
#define	mc_flag2	((mc_flags_t)0x04)
#define	mc_flag3	((mc_flags_t)0x08)
#define	mc_flag4	((mc_flags_t)0x10)
#define	mc_flag5	((mc_flags_t)0x20)
#define	mc_flag6	((mc_flags_t)0x40)
#define	mc_flag7	((mc_flags_t)0x80)

#define mc_set_mask(flgs, mask) (flgs |= mask)
#define mc_reset_mask(flgs, mask) (flgs &= ~mask)
#define mc_has_mask(flgs, mask) (flgs & mask)

static mc_inline_fn
mc_flags_t	mc_set_flag(mc_flags_t& flgs, mc_flags_t bit_flag){
	flgs = (mc_flags_t)(flgs | bit_flag);
	return flgs;
}

static mc_inline_fn
mc_flags_t 	mc_reset_flag(mc_flags_t& flgs, mc_flags_t bit_flag){
	flgs = (mc_flags_t)(flgs & ~bit_flag);
	return flgs;
}

static mc_inline_fn
bool	mc_get_flag(mc_flags_t flgs, mc_flags_t bit_flag){
	mc_flags_t  resp  = (mc_flags_t)(flgs & bit_flag);
	return (resp != 0);
}

//-------------------------------------------------------------------------
// casts

typedef grip& (* mc_method_1_t)();

typedef void (*mc_kenel_func_t)(void);

//-------------------------------------------------------------------------
// dyn mem

#define mc_pt_invalid_available (&(MCK_KERNEL->invalid_all_available))

typedef void (*mc_dbg_alloc_func_t)(void* obj, mc_alloc_size_t sz);
typedef void* (*mc_alloc_kernel_func_t)(mc_alloc_size_t sz);

#define mck_all_available(nam) MCK_KERNEL->cls_available_##nam

//! Defines 'acquire_alloc' method for class 'nam' with aligment 'align' (32 or 64).
#define MCK_DEFINE_ACQUIRE_ALLOC(nam, align) \
PTD_DBG_CODE(mc_dbg_alloc_func_t nam##_alloc_hook = mc_null); \
nam* \
nam::acquire_alloc(mc_alloc_size_t sz){ \
	MCK_CK(sz != BJ_INVALID_ALLOC_SZ); \
	nam* obj = mc_malloc##align(nam, sz); \
	PTD_DBG_CODE(if(nam##_alloc_hook != mc_null){ (*nam##_alloc_hook)(obj, sz); }); \
	if(obj == mc_null){ \
		mck_slog2(#nam); \
		mck_slog2("_OUT_OF_MEM.acquire_alloc_NULL_OBJECT.\n"); \
		mck_sprt2(#nam); \
		mck_sprt2("_OUT_OF_MEM.acquire_alloc_NULL_OBJECT.\n"); \
		mck_abort(sz, MC_ABORT_MSG("acquire_alloc abort. NULL OBJ.\n")); \
	} \
	MCK_CK(MC_IS_ALIGNED_##align(obj)); \
	for(int bb = 0; bb < sz; bb++){ \
		new (&(obj[bb])) nam(); \
	} \
	return obj; \
} \

// end_macro

#define MCK_DEFINE_NXT_SEPARATE_SZ(nam, curr_sep_sz) \
mc_alloc_size_t \
nam::get_next_separate_sz(){ \
	curr_sep_sz <<= 1; \
	return curr_sep_sz; \
} \

// end_macro

#define MCK_DEFINE_CURR_SEPARATE_SZ(nam, curr_sep_sz) \
mc_alloc_size_t \
nam::get_curr_separate_sz(){ \
	if(curr_sep_sz <= 0){ return 1; } \
	return curr_sep_sz; \
} \

// end_macro

#define MCK_DEFINE_CHECK_CELL_ID(nam) \
bool \
nam::ck_cell_id(mck_handler_idx_t idx){ \
	nam obj; \
	return (obj.get_cell_id() == idx); \
} \

// end_macro

#define MCK_DEFINE_SEPARATE_AVA(nam, all_ava) \
nam* \
nam::separate(mc_alloc_size_t sz){ \
	if(sz == 0){ return mc_null; } \
	if(sz == BJ_INVALID_ALLOC_SZ){ sz = nam::get_curr_separate_sz(); } \
	grip& ava = all_ava; \
	nam* obj = nam::acquire_alloc(sz); \
	for(int bb = 0; bb < sz; bb++){ \
		obj[bb].let_go(); \
		ava.bind_to_my_left(obj[bb]); \
	} \
	return obj; \
} \

// end_macro

//! Declares dynamic memory methods for class 'nam'
#define MCK_DECLARE_MEM_METHODS(nam) \
	static	bool 			ck_cell_id(mck_handler_idx_t idx) mc_external_code_ram; \
	static	mc_alloc_size_t	get_curr_separate_sz() mc_external_code_ram; \
	static	nam*			acquire_alloc(mc_alloc_size_t sz = 1) mc_external_code_ram; \
	static	nam*			separate(mc_alloc_size_t sz) mc_external_code_ram; \

// end_macro

//! Defines dynamic memory methods for class 'nam' with aligment 'align' (32 or 64) and available list 'all_ava'.
#define MCK_DEFINE_MEM_METHODS(nam, align, all_ava, curr_sep_sz) \
	MCK_DEFINE_CHECK_CELL_ID(nam) \
	MCK_DEFINE_CURR_SEPARATE_SZ(nam, curr_sep_sz) \
	MCK_DEFINE_ACQUIRE_ALLOC(nam, align) \
	MCK_DEFINE_SEPARATE_AVA(nam, all_ava) \

// end_macro


//-------------------------------------------------------------------------
// handler ids

//! Type to identify a \ref missive handler.
typedef uint8_t mck_handler_idx_t;

enum mck_workeru_state_t : uint8_t {
	mck_invalid_state = 10,
	mck_inited_state
};

//! Type for functions that handle \ref missive s (it is a pointer to the handler function).
typedef void (*missive_handler_t)(missive* msg);

//-------------------------------------------------------------------------
// cell ids

#define mck_cell_id(cls) MC_CELL_ID_##cls

//	mck_cell_id(kernel) = 0,

enum mck_cell_id_t : mck_handler_idx_t {
	mck_cell_id(agent) = 0,
	mck_cell_id(cell),
	mck_cell_id(missive),
	mck_cell_id(agent_ref),
	mck_cell_id(agent_grp),

	mkc_last_kernel_invalid,
	mck_tot_base_cell_classes
};

//-------------------------------------------------------------------------
// kernel data

enum mck_route_t : uint8_t {
	mck_up_route = 0,
	mck_down_route,
	mck_left_route,
	mck_right_route,
	mck_tot_routes
};

enum mck_signal_t : uint8_t {
	mck_do_pw0_routes_sgnl = 0,
	mck_do_pw2_routes_sgnl,
	mck_do_pw4_routes_sgnl,
	mck_do_pw6_routes_sgnl,
	mck_tot_signals
};

//=============================================================================
/*! \enum mck_ack_t
\brief ack state of a particular routing port.
\ingroup docgrp_inner_working

*/
enum mck_ack_t : uint8_t {
	mck_ready_ack = 30,	//!< It is ready to route (send) again through this route.
	mck_busy_ack, //!< It is has routed (sent) through this route and is waiting to be ready again.
	mck_virgin_ack //!< It is has never used this route yet.
};


//-------------------------------------------------------------------------
/*! \class kernel
\brief The kernel of a workeru that is using the library.
\ingroup docgrp_inner_working

\details 
<p>
Every workeru must have one and only one kernel inited with kernel::init_sys. 

*/

//define mc_virgin mc_null
#define mc_virgin ((missive_grp_t*)(~((mc_addr_t)mc_null)))

#define kernel_signals_arr_sz mck_tot_signals
#define kernel_pw0_routed_arr_sz mc_out_num_workerus
#define kernel_pw2_routed_arr_sz mck_tot_routes
#define kernel_pw4_routed_arr_sz mck_tot_routes
#define kernel_pw6_routed_arr_sz mck_tot_routes

#define kernel_class_names_arr_sz mck_tot_base_cell_classes

//if defined(XXX) && !defined(YYY) 

#ifdef MC_IS_EPH_CODE
	kernel*
	mck_get_first_kernel() mc_external_code_ram;

	//! This workeru's kernel
	extern kernel*	mck_PT_THE_KERNEL;

	#define MCK_FIRST_KERNEL mck_get_first_kernel()

	//! Returns the kernel of this workeru
	#define MCK_KERNEL (mck_PT_THE_KERNEL)
#endif

#ifdef MC_IS_ZNQ_CODE
	kernel*
	mch_get_first_kernel();

	extern kernel*	mch_PT_THE_KERNEL;
	#define MCK_FIRST_KERNEL mch_get_first_kernel()
	#define MCK_KERNEL (mch_PT_THE_KERNEL)
#endif

#ifdef MC_IS_PTD_CODE
	kernel*
	mcm_get_kernel();

	#define MCK_FIRST_KERNEL mcm_get_kernel()
	#define MCK_KERNEL mcm_get_kernel()
#endif

//! Gets the local kernel
#define mck_get_kernel() MCK_KERNEL

kernel*	mck_get_kernel_by_id(mc_workeru_id_t id);

void mc_kernel_handler(missive* msv);

void ptd_dbg_prt_ack_arr(int sz, mck_ack_t* arr);

extern char* err_cell_07 mc_external_data_ram;
extern char* err_cell_08 mc_external_data_ram;

class mc_aligned kernel { 
public:
	// CHANGING THIS STRUCTURE MAY LEAD TO ALIGMENT PROBLEMS IN PARALLELA
	
	uint32_t 	magic_id;
	bool		is_manageru_kernel;

	mc_bool_t signals_arr[kernel_signals_arr_sz];

	uint32_t 	middle_magic_id;
	
	missive_grp_t* pw0_routed_arr[kernel_pw0_routed_arr_sz];
	missive_grp_t* pw2_routed_arr[kernel_pw2_routed_arr_sz];
	missive_grp_t* pw4_routed_arr[kernel_pw4_routed_arr_sz];
	missive_grp_t* pw6_routed_arr[kernel_pw6_routed_arr_sz];

	mck_ack_t pw0_routed_ack_arr[kernel_pw0_routed_arr_sz];
	mck_ack_t pw2_routed_ack_arr[kernel_pw2_routed_arr_sz];
	mck_ack_t pw4_routed_ack_arr[kernel_pw4_routed_arr_sz];
	mck_ack_t pw6_routed_ack_arr[kernel_pw6_routed_arr_sz];

	missive_grp_t*	routed_from_manageru;
	mck_ack_t	routed_ack_from_manageru;

	mc_bool_t has_from_manageru_work;
	mc_bool_t has_to_manageru_work;

	grip to_workerus_work;
	grip to_manageru_work;
	grip from_manageru_work;

	grip in_work;
	grip local_work;
	grip out_work;
	grip sent_work;
	//grip delayed_to_release;

	char* class_names_arr[kernel_class_names_arr_sz];

	grip 	cls_available_cell;
	grip 	cls_available_missive;
	grip 	cls_available_agent_ref;
	grip 	cls_available_agent_grp;

	kernel*	manageru_kernel;

	cell* 	first_cell;

	void*	manageru_load_data = mc_null;

	mc_kenel_func_t	user_func;
	void*	user_data;

	bool		manageru_running;
	uint16_t 	did_work;
	bool 		exit_when_idle;

	mck_token_t 	stop_key;
	mc_kenel_func_t	user_stop_func;
	bool			sent_stop_to_parent;
	mck_token_t 	rcvd_stop_key;
	uint32_t		num_childs_stopping;

	mck_handler_idx_t		tot_cell_subclasses;
	missive_handler_t* 		all_cell_handlers;
	grip** 					all_cell_available; 
	mc_alloc_kernel_func_t*	all_cell_acquire_alloc_funcs; 
	mc_alloc_kernel_func_t*	all_cell_separate_funcs; 
	
	grip* all_kernel_ava[mck_tot_base_cell_classes];
	mc_alloc_kernel_func_t all_kernel_acq[mck_tot_base_cell_classes];
	mc_alloc_kernel_func_t all_kernel_sep[mck_tot_base_cell_classes];

	grip					invalid_all_available;
	
	mck_token_t 	min_module_bridge_token;
	mck_token_t 	max_module_bridge_token;
	
	uint32_t 	end_magic_id;

	kernel() mc_external_code_ram;

	~kernel() mc_external_code_ram;

	void init_kernel() mc_external_code_ram;

	void init_router_ack_arrays() mc_external_code_ram;

	static void
	init_sys(bool is_the_manageru = false) mc_external_code_ram; //!< Static method that inits this workeru kernel.

	static mc_opt_sz_fn void 
	run_sys(bool reset_idle = true); //!< Static method that starts handling \ref missive s. No \ref missive s are handled before.

	static void
	finish_run() mc_external_code_ram;

	static void
	finish_sys() mc_external_code_ram; //!< Static method that finishes this workeru kernel.

	static void
	init_manageru_sys() mc_external_code_ram;

	static void 
	run_manageru_sys() mc_external_code_ram;

	static void
	finish_manageru_sys() mc_external_code_ram;

	//! Returns the kernel
	static mc_inline_fn kernel*
	get_sys(){ return MCK_KERNEL; }

	//! Returns the workeru global info
	static mc_inline_fn mck_glb_sys_st& 
	get_workeru_info(){
		return *MC_WORKERU_INFO;
	}

	static mc_inline_fn mc_off_workeru_st& 
	get_off_shd(){
		return *(MC_WORKERU_INFO->off_workeru_pt);
	}

	//! Returns the system size structure
	static mc_inline_fn mc_sys_sz_st& 
	get_sys_sz(){
		return *MC_SYS_SZ;
		//return MC_WORKERU_INFO->mck_system_sz;
	}

	//! Returns the workeru number
	static mc_inline_fn mc_workeru_nn_t 
	get_workeru_nn(){
		return MC_WORKERU_INFO->the_workeru_nn;
	}

	//! Returns the workeru row
	static mc_inline_fn mc_workeru_co_t 
	get_workeru_ro(){
		return MC_WORKERU_INFO->the_workeru_ro;
	}

	//! Returns the workeru column
	static mc_inline_fn mc_workeru_co_t 
	get_workeru_co(){
		return MC_WORKERU_INFO->the_workeru_co;
	}

	//! Returns the workeru id
	static mc_inline_fn mc_workeru_id_t 
	get_workeru_id(){
		return MC_WORKERU_INFO->the_workeru_id;
	}

	//! Returns the parent workeru id
	static mc_inline_fn mc_workeru_id_t 
	get_parent_workeru_id(){
		return mc_map_get_parent_workeru_id();
	}

	//! Returns this kernel first created \ref cell 
	static mc_inline_fn cell*
	get_first_cell(){
		return MCK_KERNEL->first_cell;
	}

	//! Returns the first_cell (see \ref get_first_cell) of the workeru with id 'dst_id'
	static cell*
	get_first_cell(mc_workeru_id_t dst_id);

	//! Returns the first_cell (see \ref get_first_cell) of the manageru
	static cell*
	get_manageru_cell() mc_external_code_ram;

	//! \brief This methods sets \ref kernel::tot_cell_subclasses ('tot_subcells' MUST be > 'mck_tot_base_cell_classes').*/
	static void
	set_tot_cell_subclasses(mck_handler_idx_t tot_subcells) mc_external_code_ram;

	static void
	init_kernel_cell_handlers(missive_handler_t* hdlrs) mc_external_code_ram;
	
	//! \brief This methods sets \ref kernel::all_cell_handlers to 'hdlrs', it must have size 'tot_cell_subclasses' and the last handler ('tot_cell_subclasses' - 1) must be '&kernel::invalid_handler_func'. The first 'mck_tot_base_cell_classes' are reserved for system use, so the user must not use those positions in the array.
	static void
	set_cell_handlers(missive_handler_t* hdlrs) mc_external_code_ram;

	static void
	init_kernel_cell_mem_funcs(grip** all_ava, mc_alloc_kernel_func_t* all_acq,
							mc_alloc_kernel_func_t* all_sep) mc_external_code_ram;
	
	static void
	set_cell_mem_funcs(grip** all_ava, mc_alloc_kernel_func_t* all_acq,
							mc_alloc_kernel_func_t* all_sep) mc_external_code_ram;
	
	static	void
	invalid_handler_func(missive* msg) mc_external_code_ram;
	
	static	void*
	invalid_alloc_func(mc_alloc_size_t sz) mc_external_code_ram;
	
	static agent*
	do_acquire(mck_handler_idx_t idx, mc_alloc_size_t sz = 1) mc_external_code_ram;

	static kernel*
	get_workeru_kernel(mc_workeru_id_t id) mc_external_code_ram;

	void dbg_set_idle() mc_external_code_ram;

	//! Tells the kernel to exit \ref kernel::run_sys when no more work is pending or done (idle).
	mc_inline_fn void set_idle_exit(){
		//dbg_set_idle();
		exit_when_idle = true;
	}

	mc_opt_sz_fn bool 
	process_signal(binder& in_wrk, int sz, missive_grp_t** arr, mck_ack_t* acks);

	mc_opt_sz_fn void 
	handle_missives();

	mc_opt_sz_fn void 
	add_out_missive(grip* out_wk, missive& msv);

	mc_opt_sz_fn void 
	call_handlers_of_group(missive_grp_t* mgrp, bool has_dbg);

	void
	release_all_missives(grip& all_msv, grip* pending, bool has_dbg) mc_external_code_ram;

	void 
	handle_work_to_manageru() mc_external_code_ram;

	void
	handle_work_from_manageru() mc_external_code_ram;

	void 
	handle_work_to_workerus() mc_external_code_ram;

	void 
	handle_manageru_missives() mc_external_code_ram;

	void 
	call_manageru_handlers_of_group(missive_grp_t* mgrp, bool has_dbg) mc_external_code_ram;

	mc_inline_fn
	void reset_stop_sys(){
		stop_key = 0;
		sent_stop_to_parent = false;
		rcvd_stop_key = 0;
		num_childs_stopping = 0;
	}

	void
	send_stop_to_children() mc_external_code_ram;

	static void 
	stop_sys(mck_token_t key) mc_external_code_ram;

	void 
	handle_stop() mc_external_code_ram;

	void 
	kernel_first_cell_msv_handler(missive* msv) mc_external_code_ram;
};

#define mck_is_valid_handler_idx(idx) ((idx >= 0) && (idx < tot_cell_subclasses))

#define mck_is_valid_handler_index(idx) \
	((all_cell_handlers != mc_null) && mck_is_valid_handler_idx(idx) && \
	(all_cell_handlers[idx] != mc_null))

/*
	PTD_CK_PRT(mck_is_valid_handler_idx(hdlr_idx), "WARNING !. Invalid handler_idx %d with %s dst=%p \n\n", \
		hdlr_idx, (((msv != mc_null) && (msv->dst != mc_null))?(msv->dst->get_class_name()):("UNKNOW CLASS")), \
		(void*)(msv->dst)); \
*/

#define mck_handle_missive_base(msv, hdlr_idx) \
	PTD_DBG_CODE(if(has_dbg){PTD_PRT("BEFORE_HANDLING_MISSIVE_%s \n", msv->get_dbg_string());}); \
	PTD_CK(mck_is_valid_handler_index(hdlr_idx)); \
	if(mck_is_valid_handler_idx(hdlr_idx)){ \
		(*(all_cell_handlers[hdlr_idx]))(msv); \
		PTD_DBG_CODE(mc_set_mask(msv->flg, mc_dbg_msv_received_flag)); \
		PTD_DBG_CODE( \
			if(mc_has_mask(msv->flg, mc_dbg_msv_log_received_flag)){ \
				PTD_LOG("DBG_MSV_received (%p) %d \n", msv, hdlr_idx); \
			} \
		); \
	} \
	PTD_DBG_CODE(if(has_dbg){PTD_PRT("AFTER_HANDLING_MISSIVE_%s \n", msv->get_dbg_string());}); \

// end_macro

#define mck_handle_missive(msv) \
	cell* hdlr_dst = (msv)->dst; \
	PTD_CK(hdlr_dst != mc_null); \
	mck_handle_missive_base(msv, hdlr_dst->handler_idx) \

// end_macro


#define mc_class_name(cls) mc_cstr("{" #cls "}");

#define mck_is_valid_class_name_idx(id) ((id >= 0) && (id < kernel_class_names_arr_sz))

#define mck_set_class_name(cls) class_names_arr[mck_cell_id(cls)] = mc_class_name(cls)

//-------------------------------------------------------------------------
/*! \class agent
\brief The base class for any library object including \ref cell s, \ref missive s,  \ref agent_grp s
and \ref agent_ref s.
\ingroup docgrp_inner_working

*/
class mc_aligned agent: public binder{
public:
	mc_opt_sz_fn 
	agent(){}

	mc_opt_sz_fn 
	~agent(){}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return mck_cell_id(agent);
	}

	virtual mc_opt_sz_fn 
	void	init_me(int caller = 0) mc_external_code_ram;

	PTD_DBG_CODE(
		virtual mc_opt_sz_fn 
		void	dbg_release(int dbg_caller){}
	);

	//! Releases this \ref agent so that it can latter be acquired again.
	mc_opt_sz_fn 
	void	release(int dbg_caller = 1);
		
	mc_inline_fn
	agent*	get_glb_ptr(){
		return (agent*)mck_as_glb_pt(this);
	}

	virtual
	char* 	get_class_name() mc_external_code_ram;
};

//-------------------------------------------------------------------------
/*! \class cell
\brief The base class for any user processing class that wishes send \ref missive s. 
\ingroup docgrp_messaging

\details 
<p>
A cell is the fundamental unit of concurrent computation of the library. 

<p>
The user can think of it as a concurrent object (as in object oriented programming), an object that can receive \ref missive s.

<p>
Every cell has a \ref mck_handler_idx_t called \ref handler_idx .
*/

//======================================================================
// flags

// did_work bits

#define	mc_bit0	((uint16_t)0x0001)
#define	mc_bit1	((uint16_t)0x0002)
#define	mc_bit2	((uint16_t)0x0004)
#define	mc_bit3	((uint16_t)0x0008)
#define	mc_bit4	((uint16_t)0x0010)
#define	mc_bit5	((uint16_t)0x0020)
#define	mc_bit6	((uint16_t)0x0040)
#define	mc_bit7	((uint16_t)0x0080)
#define	mc_bit8	((uint16_t)0x0100)
#define	mc_bit9	((uint16_t)0x0200)
#define	mc_bit10	((uint16_t)0x0400)
#define	mc_bit11	((uint16_t)0x0800)
#define	mc_bit12	((uint16_t)0x1000)
#define	mc_bit13	((uint16_t)0x2000)
#define	mc_bit14	((uint16_t)0x4000)
#define	mc_bit15	((uint16_t)0x8000)


#define	mc_module_bridge_flag mc_flag0

#define	mc_usr_flag0 mc_flag1
#define	mc_usr_flag1 mc_flag2
#define	mc_usr_flag2 mc_flag3
#define	mc_usr_flag3 mc_flag4
#define	mc_usr_flag4 mc_flag5
#define	mc_usr_flag5 mc_flag6
#define	mc_usr_flag6 mc_flag7

#define mc_cell_acquire_arr(num) ((cell*)(kernel::do_acquire(mck_cell_id(cell), num)))
#define mc_cell_acquire() mc_cell_acquire_arr(1)

class mc_aligned cell: public agent {
public:
	MCK_DECLARE_MEM_METHODS(cell);

	mck_handler_idx_t 	handler_idx; //!< Index of my handler func in \ref kernel::all_cell_handlers.
	mc_flags_t 			filaments;

	mc_opt_sz_fn 
	cell(){
		init_me();
	}

	mc_opt_sz_fn 
	~cell(){}

	virtual mc_opt_sz_fn 
	void init_me(int caller = 0){
		handler_idx = mck_cell_id(cell);
		filaments = 0;
	}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return handler_idx;
	}

	mc_opt_sz_fn
	void
	send(cell* des, mck_token_t tok);

	mc_opt_sz_fn
	void
	respond(missive* msv, mck_token_t tok);
};

//-------------------------------------------------------------------------
/*! \class missive
\brief The base class for any user data class that wishes to be sent beetwen \ref cell s.
\ingroup docgrp_messaging

*/

#define mc_missive_acquire_arr(num) ((missive*)(kernel::do_acquire(mck_cell_id(missive), num)))
#define mc_missive_acquire() mc_missive_acquire_arr(1)

#define mc_msv_delayed_flag 			mc_flag0
#define mc_msv_reply_flag 				mc_flag1
#define mc_msv_replying_flag 			mc_flag2
#define mc_dbg_msv_sent_flag 			mc_flag3
#define mc_dbg_msv_received_flag 		mc_flag4
#define mc_dbg_msv_log_received_flag 	mc_flag5

class mc_aligned missive : public agent {
public:
	MCK_DECLARE_MEM_METHODS(missive);

	mc_flags_t			flg;
	cell* 				dst;
	cell*				src;
	mck_token_t 		tok;

	mc_opt_sz_fn 
	missive(){
		init_me();
	}

	mc_opt_sz_fn 
	~missive(){}

	virtual mc_opt_sz_fn 
	void init_me(int dbg_caller = 0){
		PTD_CK_PRT((dbg_caller == 0) || 
			(! mc_has_mask(flg, mc_dbg_msv_sent_flag)) || mc_has_mask(flg, mc_dbg_msv_received_flag), 
			"cll=%d dbg=%p tok=%d src=%p dst=%p\n", 
			dbg_caller, (void*)(uintptr_t)flg, tok, (void*)src, (void*)dst);
		
		//PTD_DBG_CODE(flg = 0);
		flg = 0;
		dst = mc_null;
		src = mc_null;
		tok = 0;
	}

	//! Sends this \ref missive . It calls \ref mck_as_glb_pt with src before sending.
	mc_inline_fn 
	void send(){
		//PTD_CK(flg == 0);
		PTD_CK(	(! mc_has_mask(flg, mc_dbg_msv_sent_flag)) && 
				(! mc_has_mask(flg, mc_dbg_msv_received_flag))	);
		PTD_DBG_CODE(mc_set_mask(flg, mc_dbg_msv_sent_flag));

		PTD_CK(dst != mc_null);
		PTD_CK(mc_addr_in_sys((mc_addr_t)dst));

		src = (cell*)mck_as_glb_pt(src);
		MCK_KERNEL->local_work.bind_to_my_left(*this);
	}
	
	mc_inline_fn 
	void send_to_manageru(){
		PTD_CK(flg == 0);
		PTD_DBG_CODE(mc_set_mask(flg, mc_dbg_msv_sent_flag));

		PTD_CK(dst != mc_null);
		if(! MCK_KERNEL->is_manageru_kernel){
			PTD_CK(mc_addr_get_id((mc_addr_t)dst) != 0);
			PTD_CK(! mc_addr_in_sys((mc_addr_t)dst));
			src = (cell*)mck_as_glb_pt(src);
			MCK_KERNEL->to_manageru_work.bind_to_my_left(*this);
			MCK_KERNEL->has_to_manageru_work = true;
		} else {
			MCK_KERNEL->local_work.bind_to_my_left(*this);
		}
	}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return mck_cell_id(missive);
	}

	mc_inline_fn cell*
	get_source(){
		return src;
	}

	mc_inline_fn void
	set_source(cell* act){
		src = act;
	}

	virtual 
	char*	get_dbg_string(){
		return mc_cstr("UNKNOW_MISSIVE_DESCRIPTION");
	}

	
};

//-------------------------------------------------------------------------
/*! \class agent_grp
\brief A class that is used internally to group \ref missive s, \ref cell s or any
\ref agent derived class. You can use it too.
\ingroup docgrp_inner_working

*/

#define mc_agent_grp_acquire_arr(num) ((agent_grp*)(kernel::do_acquire(mck_cell_id(agent_grp), num)))
#define mc_agent_grp_acquire() mc_agent_grp_acquire_arr(1)

#define mc_dbg_grp_log_steps_flag 		mc_flag0

class mc_aligned agent_grp : public agent {
public:
	MCK_DECLARE_MEM_METHODS(agent_grp);

	grip		all_agts;
	uint8_t 	tot_agts;	// optional use
	mc_bool_t 	handled;

	PTD_DBG_CODE(mc_flags_t	dbg_grp);
	
	mc_opt_sz_fn 
	agent_grp(){
		init_me();
	}

	mc_opt_sz_fn 
	~agent_grp(){}

	virtual mc_opt_sz_fn 
	void init_me(int caller = 0){
		tot_agts = 0;
		handled = mc_false;
		PTD_DBG_CODE(dbg_grp = 0);
	}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return mck_cell_id(agent_grp);
	}

};

//-------------------------------------------------------------------------
/*! \class agent_ref
\brief A class that is used internally to hold a reference 
(local or remote ) to \ref missive s, \ref cell s or any
\ref agent derived class. You can use it too.
\ingroup docgrp_inner_working

*/

#define mc_agent_ref_acquire_arr(num) ((agent_ref*)(kernel::do_acquire(mck_cell_id(agent_ref), num)))
#define mc_agent_ref_acquire() mc_agent_ref_acquire_arr(1)

class mc_aligned agent_ref : public agent {
public:
	MCK_DECLARE_MEM_METHODS(agent_ref);

	agent* 		glb_agent_ptr;

	PTD_DBG_CODE(mc_flags_t	dbg_ref);
	
	mc_opt_sz_fn 
	agent_ref(){
		init_me();
	}

	mc_opt_sz_fn 
	~agent_ref(){}

	virtual mc_opt_sz_fn 
	void init_me(int caller = 0){
		glb_agent_ptr = mc_null;
		PTD_DBG_CODE(dbg_ref = 0);
	}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return mck_cell_id(agent_ref);
	}

};

enum kernel_tok_t : mck_token_t {
	mck_tok_invalid,
	mck_tok_stop_sys_to_parent,
	mck_tok_stop_sys_to_children,
	mck_tok_last
};

void 
mc_kernel_handler(missive* msv) mc_external_code_ram;

#define mc_glb_binder_get_rgt(bdr, id) ((binder*)mc_addr_set_id((id), ((bdr)->bn_right)))
#define mc_glb_binder_get_lft(bdr, id) ((binder*)mc_addr_set_id((id), ((bdr)->bn_left)))

#define mch_glb_binder_get_rgt(bdr, id) \
	(binder*)mc_glb_binder_get_rgt((binder*)mc_workeru_pt_to_manageru_pt(bdr), id)

#define mch_glb_binder_get_lft(bdr, id) \
	(binder*)mc_glb_binder_get_lft((binder*)mc_workeru_pt_to_manageru_pt(bdr), id)

#define MCK_CALL_HANDLER(cls, nam, msv) (((cls*)(mck_as_loc_pt(msv->dst)))->nam(msv))

#define MCK_CALL_REPLY_HANDLER(cls, nam, msv) (((cls*)(mck_as_loc_pt(msv->src)))->nam(msv))

//template<typename T, typename U> constexpr mc_size_t mc_offsetof(U T::*member) mc_external_code_ram;

template<typename T, typename U> constexpr mc_size_t mc_offsetof(U T::*member)
{
    return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

#ifdef __cplusplus
mc_c_decl {
#endif

bool
mck_has_same_module(mc_workeru_id_t dst_id) mc_external_code_ram;

bool
mck_has_module() mc_external_code_ram;

bool
mck_has_same_sub_module(mc_workeru_id_t dst_id) mc_external_code_ram;

bool
mck_is_id_inited(mc_workeru_id_t dst_id) mc_external_code_ram;

bool
mck_ck_type_sizes() mc_external_code_ram;

//mc_opt_sz_fn 
void 
mck_send_irq(mc_workeru_id_t koid, uint16_t num_irq) mc_external_code_ram;

#ifdef __cplusplus
}
#endif


//static  void
//__static_initialization_and_destruction_0(int, int) mc_external_code_ram;
// ignored
// pragma GCC diagnostic warning "-fpermissive"


#endif		// CELL_HH
