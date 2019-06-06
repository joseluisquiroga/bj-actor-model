

/*************************************************************

This file is part of ben-jose.

ben-jose is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

ben-jose is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ben-jose.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2007-2012, 2014-2016. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://github.com/joseluisquiroga/ben-jose

ben-jose is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------

workerus_sort_net.hh

Declaration of functions to use the sorting_net.

--------------------------------------------------------------*/

#ifndef WORKERUS_SORT_NET_H
#define WORKERUS_SORT_NET_H

#ifdef MC_IS_PTD_CODE
#include <stdio.h>
#include <string.h>

#include <sstream>
typedef std::ostringstream bj_dbg_str_stream;
#endif

#include "cell.hh"
#include "sort_net_main.hh"

class pre_sort_net;

class sornet_signal;
class sorcell;
class sorting_net;

//define SYNC_LOG(...) PTD_LOG(__VA_ARGS__)
#define SYNC_LOG(...) 

//define SYNC_CODE(...) PTD_CODE(__VA_ARGS__)
#define SYNC_CODE(...) 

enum binval_t : uint8_t {
	binval_invalid = 0,
	binval_bottom = 11,
	binval_top = 22
};

enum net_side_t : uint8_t {
	side_invalid,
	side_left,
	side_right
};

enum sornet_tok_t : mck_token_t {
	bj_tok_sornet_invalid = mck_tok_last + 1,
	bj_tok_sornet_start,
	bj_tok_sornet_out,
	bj_tok_sync_to_children,
	bj_tok_sornet_bin,
	bj_tok_sornet_end
};

enum bj_hdlr_idx_t : mck_handler_idx_t {
	idx_invalid = mck_tot_base_cell_classes,
	idx_sorcell,
	idx_sorting_net,
	idx_sornet_signal,
	idx_last_invalid,
	idx_total
};

typedef int (*bj_cmp_obj_func_t)(void* obj1, void* obj2);

void ptd_prt_tok_codes() mc_external_code_ram;


#define BJ_IS_CLS(obj, cnam) (((obj) != mc_null)?((obj)->get_class_name() == cnam##_cls_nam):(false))

#define BJ_DECLARE_CLS_NAM(cnam) \
extern char cnam##_cls_nam[] mc_external_data_ram; \
bool bj_ck_is_##cnam(agent* pt_obj) mc_external_code_ram; \

// end_of_macro

#define BJ_DEFINE_GET_CLS_NAM(cnam) \
char cnam##_cls_nam[] = "{" #cnam "}"; \
char* cnam::get_class_name(){ return cnam##_cls_nam; } \
bool bj_ck_is_##cnam(agent* obj){ \
	return BJ_IS_CLS(obj, cnam); \
} \

// end_of_macro


BJ_DECLARE_CLS_NAM(sornet_signal)
BJ_DECLARE_CLS_NAM(sorcell)
BJ_DECLARE_CLS_NAM(sorting_net)

#define BJ_DEFINE_GET_CLS_NAM(cnam) \
char cnam##_cls_nam[] = "{" #cnam "}"; \
char* cnam::get_class_name(){ return cnam##_cls_nam; } \
bool bj_ck_is_##cnam(agent* obj){ \
	return BJ_IS_CLS(obj, cnam); \
} \

// end_of_macro


#define	bj_ss_ranked_snps_flag mc_flag1

#define bj_sornet_signal_acquire_arr(num) ((sornet_signal*)(kernel::do_acquire(idx_sornet_signal, num)))
#define bj_sornet_signal_acquire() bj_sornet_signal_acquire_arr(1)

class mc_aligned sornet_signal : public missive {
public:
	MCK_DECLARE_MEM_METHODS(sornet_signal)

	num_nod_t	idx;
	void*		obj;

	sornet_signal() mc_external_code_ram;
	~sornet_signal() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_sornet_signal;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) bj_sornet_cod;

	virtual
	char* 	get_class_name() mc_external_code_ram;
};


#define	bj_stt_charge_all_flag mc_flag1
#define	bj_stt_stabi_intact_id_flag mc_flag2


typedef int (*bj_cmp_func_t)(binder* obj1, binder* obj2);

bool bj_is_sorted(grip& grp, bj_cmp_func_t fn, int ord) mc_external_code_ram;


#define bj_sorcell_acquire_arr(num) ((sorcell*)(kernel::do_acquire(idx_sorcell, num)))
#define bj_sorcell_acquire() bj_sorcell_acquire_arr(1)

class mc_aligned sorcell : public cell {
public:
	MCK_DECLARE_MEM_METHODS(sorcell)

	num_nod_t 	color;

	num_nod_t 	dbg_level;

	num_nod_t 	up_idx;
	void*		up_inp;
	sorcell*	up_out;

	num_nod_t 	down_idx;
	void*		down_inp;
	sorcell*	down_out;

	sorcell() mc_external_code_ram;
	~sorcell() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_sorcell;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) mc_external_code_ram;

	void sornet_handler(missive* msv) bj_sornet_cod;
	bj_cmp_obj_func_t sornet_get_cmp_func(sornet_tok_t tmt_tok) bj_sornet_cod;

	virtual
	char* 	get_class_name() mc_external_code_ram;
};

void bj_send_sornet_tmt(cell* src, sornet_tok_t tok, void* obj, sorcell* dst, num_nod_t idx) bj_sornet_cod;


#define BJ_MAX_ID_ARR_SZ 260

#define BJ_DBG_STR_CAP 2048

typedef unsigned long mini_bit_arr_t;

class mc_aligned sorting_net : public cell {
public:
	MCK_DECLARE_MEM_METHODS(sorting_net)

	long MAGIC;

	bool		sync_is_ending;

	grip		ava_sornet_signals;
	grip		ava_sorcells;

	grip* all_ava[idx_total];
	mc_alloc_kernel_func_t all_acq[idx_total];
	mc_alloc_kernel_func_t all_sep[idx_total];
	
	missive_handler_t srt_handlers[idx_total];

	pre_sort_net*	shd_cnf;

	num_nod_t tot_loading;
	num_nod_t tot_loaded;

	mc_workeru_nn_t sync_tot_children;
	mc_workeru_id_t sync_parent_id;
	mc_load_map_st* sync_map;

	num_nod_t tot_sorcells;
	grip	all_sorcells;

	mini_bit_arr_t 	dbg_sornet_curr_cntr;
	mini_bit_arr_t 	dbg_sornet_max_cntr;

	binval_t	net_top;
	binval_t	net_bottom;

	num_nod_t 	tot_input_sorcells;
	sorcell**	all_input_sorcells;
	num_nod_t 	tot_rcv_output_sorobjs;
	void**		all_output_sorobjs;

	sorting_net() mc_external_code_ram;
	~sorting_net() mc_external_code_ram;

	void init_sorting_net_with(pre_sort_net* pre_net) mc_external_code_ram;
	void init_mem_funcs() mc_external_code_ram;

	void sornet_handler(missive* msv) bj_sornet_cod;

	void init_sync_cycle() mc_external_code_ram;

	void send_all_neus(mck_token_t tok);

	bool sornet_check_order(bj_cmp_obj_func_t fn) bj_sornet_cod;

	mini_bit_arr_t sornet_dbg_bin_get_mini_sorted_arr() bj_sornet_cod;
	bool sornet_dbg_send_cntr() bj_sornet_cod;
	void sornet_dbg_end_step() bj_sornet_cod;
	void sornet_dbg_bin_handler(missive* msv) bj_sornet_cod;

	void send_sync_to_children(sornet_tok_t the_tok);
	void send_sync_transmitter(sorting_net* the_dst, sornet_tok_t the_tok);

	sorting_net* get_sorting_net(mc_workeru_id_t workeru_id);

	virtual
	char* 	get_class_name() mc_external_code_ram;
};

char* tok_to_str(sornet_tok_t the_tok);

#define bj_sorting_net ((sorting_net*)(kernel::get_sys()->user_data))
#define bj_ava_sornet_signals (bj_sorting_net->ava_sornet_signals)
#define bj_ava_sorcells (bj_sorting_net->ava_sorcells)

#define bj_handlers (bj_sorting_net->srt_handlers)

//extern missive_handler_t bj_nil_handlers[];

#define	bj_dbg_prt_nd_neu_flag mc_flag1
#define	bj_dbg_prt_nd_pol_flag mc_flag2
#define	bj_dbg_prt_id_pol_flag mc_flag3

void bj_print_loaded_cnf() mc_external_code_ram;

void bj_print_class_szs() mc_external_code_ram;

void bj_init_sorting_net() mc_external_code_ram;

void sorcell_sornet_handler(missive* msv) bj_sornet_cod;
void nervenet_sornet_handler(missive* msv) bj_sornet_cod;

int bj_cmp_bin_objs(void* obj1, void* obj2) bj_sornet_cod;
void bj_send_sornet_tmt(cell* src, sornet_tok_t tok, void* obj, sorcell* dst, num_nod_t idx) bj_sornet_cod;

void bj_load_shd_sornet();

void bj_sornet_kernel_func() bj_sornet_cod;
//void bj_sornet_init_handlers() mc_external_code_ram;
void bj_sornet_main() bj_sornet_cod;

#endif		// WORKERUS_SORT_NET_H


