

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

nervenet.hh

Declaration of nervenet class.

--------------------------------------------------------------*/

#ifndef NERVENET_H
#define NERVENET_H


#ifdef MC_IS_PTD_CODE
	#include <stdio.h>
	#include <string.h>

	#include <sstream>
	typedef std::ostringstream bj_dbg_str_stream;

	#ifdef FULL_DEBUG
		#include "tak_mak.hh"
	#endif
#endif


#include "cell.hh"
#include "solver.hh"

class pre_sornode;
class pre_item_pgroup;
class pre_pgroup;
class pre_cnf_node;
class pre_cnf_net;
class pre_load_cnf;

class base_transmitter;
class synapse;
class nervenode;
class side_state;
class neuron;
class polaron;
class sorcell;
class endcell;
class pgrp_item;
class pgroup;
class netstate;
class nervenet;

//define SYNC_LOG(...) PTD_LOG(__VA_ARGS__)
#define SYNC_LOG(...) 

//define SYNC_CODE(...) PTD_CODE(__VA_ARGS__)
#define SYNC_CODE(...) 

enum binval_t : uint8_t {
	binval_invalid = 0,
	binval_bottom = 11,
	binval_top = 22
};

enum sorkind_t : uint8_t {
	sorkind_invalid,
	sorkind_bin,
	sorkind_num,
	sorkind_rnk,
	sorkind_cll,
	sorkind_sep
};

enum rangekind_t : uint8_t {
	rangekind_invalid,
	rangekind_neus,
	rangekind_pols
};

enum net_side_t : uint8_t {
	side_invalid,
	side_left,
	side_right
};

enum tier_kind_t : uint8_t {
	tiki_invalid,
	tiki_propag,
	tiki_stabi
};

enum sync_tok_t : mck_token_t {
	bj_tok_sync_invalid = mck_tok_last + 1,
	bj_tok_sync_add_tier,
	bj_tok_sync_inert_child,
	bj_tok_sync_confl_up_neu,
	bj_tok_sync_confl_up_pol,
	bj_tok_sync_confl_down_neu,
	bj_tok_sync_confl_down_pol,
	bj_tok_sync_to_children,
	bj_tok_sync_end
};

enum load_tok_t : mck_token_t {
	bj_tok_load_invalid = bj_tok_sync_end + 1,
	bj_tok_load_nw_syn,
	bj_tok_load_no_lits,
	bj_tok_load_end
};

enum propag_tok_t : mck_token_t {
	bj_tok_propag_invalid = bj_tok_load_end + 1,
	bj_tok_propag_start,
	bj_tok_propag_ping,
	bj_tok_propag_charge_all,
	bj_tok_propag_charge_src,
	bj_tok_propag_tier_done,
	bj_tok_propag_end
};

enum mirrow_tok_t : mck_token_t {
	bj_tok_mirrow_invalid = bj_tok_propag_end + 1,
	bj_tok_mirrow_start,
	bj_tok_mirrow_end
};

enum stabi_tok_t : mck_token_t {
	bj_tok_stabi_invalid = bj_tok_mirrow_end + 1,
	bj_tok_stabi_start,
	bj_tok_stabi_sep_bcast,
	bj_tok_stabi_range_bcast,
	bj_tok_stabi_sep_neu,
	bj_tok_stabi_sep_pol,
	bj_tok_stabi_ping,
	bj_tok_stabi_color,
	bj_tok_stabi_tier_done,
	bj_tok_stabi_end
};

enum sornet_tok_t : mck_token_t {
	bj_tok_sornet_invalid = bj_tok_stabi_end + 1,
	bj_tok_sornet_start,
	bj_tok_sornet_sep_out,
	bj_tok_sornet_cll_out,
	bj_tok_sornet_num_out,
	bj_tok_sornet_bin,
	bj_tok_sornet_num,
	bj_tok_sornet_rank_start,
	bj_tok_sornet_rank_jump,
	bj_tok_sornet_rank_step,
	bj_tok_sornet_rank_cll,
	bj_tok_sornet_rank_cll_rdy,
	bj_tok_sornet_rank_sep,
	bj_tok_sornet_rank_sep_rdy,
	bj_tok_sornet_rank_obj,
	bj_tok_sornet_rank_out,
	bj_tok_sornet_end
};

enum pgroup_tok_t : mck_token_t {
	bj_tok_pgroup_invalid = bj_tok_sornet_end + 1,
	bj_tok_pgroup_start,
	bj_tok_pgroup_add_item,
	bj_tok_pgroup_remove_item,
	bj_tok_pgroup_broadcast,
	bj_tok_pgroup_item_read_ref,
	bj_tok_pgroup_item_write_ref,
	bj_tok_pgroup_end
};

enum bj_hdlr_idx_t : uint8_t {
	idx_invalid = bj_mgr_idx_total,
	idx_base_transmitter,
	idx_synset,
	idx_tierset,
	idx_synapse,
	idx_neuron,
	idx_polaron,
	idx_sorcell,
	idx_endcell,
	idx_pgrp_item,
	idx_pgroup,
	idx_tierdata,
	idx_layerdata,
	idx_nervenet,
	idx_last_invalid,
	idx_total
};

struct mc_aligned callee_prms {
public:
	synapse* snp = mc_null;
	net_side_t sd = side_invalid;
	bool rec = false;
	void* aux = mc_null;
};

typedef void (nervenode::*bj_callee_t)(callee_prms& pms);
typedef void (nervenode::*bj_callee_prm_t)(void* pms);
typedef int (*bj_cmp_obj_func_t)(void* obj1, void* obj2);

void ptd_prt_tok_codes() mc_external_code_ram;

char* net_side_to_str(net_side_t sd) mc_external_code_ram;
char* node_kind_to_str(node_kind_t ki) mc_external_code_ram;

mc_inline_fn bool is_sync_tok(mck_token_t tok){
	return ((bj_tok_sync_invalid <= tok) && (tok <= bj_tok_sync_end));
}

mc_inline_fn bool is_load_tok(mck_token_t tok){
	return ((bj_tok_load_invalid <= tok) && (tok <= bj_tok_load_end));
}

mc_inline_fn bool is_propag_tok(mck_token_t tok){
	return ((bj_tok_propag_invalid <= tok) && (tok <= bj_tok_propag_end));
}

mc_inline_fn bool is_stabi_tok(mck_token_t tok){
	return ((bj_tok_stabi_invalid <= tok) && (tok <= bj_tok_stabi_end));
}


char* bj_tok_to_str(mck_token_t tok);
char* sync_tok_to_str(sync_tok_t tok) mc_external_code_ram;
char* load_tok_to_str(load_tok_t tok) mc_external_code_ram;
char* propag_tok_to_str(propag_tok_t tok) mc_external_code_ram;
char* stabi_tok_to_str(stabi_tok_t tok) mc_external_code_ram;
char* sornet_tok_to_str(sornet_tok_t tok) mc_external_code_ram;

void dbg_call_all_synapses(binder* nn_all_snp, bj_callee_t mth, net_side_t sd, bool from_rec) mc_external_code_ram;

void with_all_synapses(binder* nn_all_snp, bj_callee_t mth, net_side_t sd, 
		bool from_rec = false, bool dbg_mates = false, void* aux = mc_null);

void with_all_nervenodes(binder* nn_all_nod, bj_callee_prm_t mth, void* prm);

//net_side_t opp_side_of(net_side_t sd) bj_propag_cod;

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

//	return (((obj) != mc_null)?((obj)->get_class_name() == cnam##_cls_nam):(false)); 
//	if(pt_obj != mc_null){ return (pt_obj->get_class_name() == cnam##_cls_nam); } 
//	return false; 


struct mc_aligned signal_data {
public:
	missive* msv = mc_null;
	synapse* snp = mc_null;
	mck_token_t tok = mck_tok_invalid;
	net_side_t sd = side_invalid;
	num_tier_t ti = BJ_INVALID_NUM_TIER;
	//num_syn_t	id_arr_sz = 0;
	//num_syn_t*  id_arr_dat = mc_null;
	num_nod_t 	col_idx;
};


BJ_DECLARE_CLS_NAM(synset)
BJ_DECLARE_CLS_NAM(tierset)
BJ_DECLARE_CLS_NAM(base_transmitter)
BJ_DECLARE_CLS_NAM(synapse)
BJ_DECLARE_CLS_NAM(nervenode)
BJ_DECLARE_CLS_NAM(neuron)
BJ_DECLARE_CLS_NAM(polaron)
BJ_DECLARE_CLS_NAM(sorcell)
BJ_DECLARE_CLS_NAM(endcell)
BJ_DECLARE_CLS_NAM(tierdata)
BJ_DECLARE_CLS_NAM(layerdata)
BJ_DECLARE_CLS_NAM(nervenet)

#define	bj_ss_ranked_snps_flag mc_flag1

int
bj_cmp_num_syn(num_syn_t const & n1, num_syn_t const & n2) mc_external_code_ram;

#define bj_synset_acquire_arr(num) ((synset*)(kernel::do_acquire(idx_synset, num)))
#define bj_synset_acquire() bj_synset_acquire_arr(1)

class mc_aligned synset : public agent {
public:
	MCK_DECLARE_MEM_METHODS(synset)

	PTD_DBG_CODE(mc_flags_t		ss_flags);

	num_syn_t 	num_ss_recv;
	num_syn_t 	num_ss_ping;

	num_syn_t 	tot_syn;
	grip		all_syn;
	grip		all_grp;

	synset() mc_external_code_ram;
	~synset() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_synset;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);

	void add_left_synapse(synapse* snp);
	void add_right_synapse(synapse* snp);

	void propag_handler(missive* msv) bj_propag_cod;

	void dbg_rec_call_all(bj_callee_t mth, net_side_t sd) mc_external_code_ram;

	void transmitter_send_all_rec(bj_callee_t mth, net_side_t sd);

	void reset_rec_tree();

	bool is_synset_empty();

	synapse* get_first_snp(net_side_t sd);

	virtual
	char* 	get_class_name() mc_external_code_ram;
};

#define bj_tierset_acquire_arr(num) ((tierset*)(kernel::do_acquire(idx_tierset, num)))
#define bj_tierset_acquire() bj_tierset_acquire_arr(1)

class mc_aligned tierset : public agent {
public:
	MCK_DECLARE_MEM_METHODS(tierset)

	num_tier_t 	ti_id;
	grip		ti_all;	//!< \ref synapse s in this tier

	tierset() mc_external_code_ram;
	~tierset() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_tierset;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);

	void mirrow_tiset(tierset& tis, net_side_t src_sd) mc_external_code_ram;

	virtual
	char* 	get_class_name() mc_external_code_ram;
};

#define BJ_INVALID_SRT_GRP BJ_INVALID_IDX

#define BJ_INVALID_SRT_OBJ mc_null
#define BJ_INVALID_RNK_OBJ ((void*)BJ_INVALID_IDX)

class mc_aligned sornet_range {
public:
	num_nod_t 	min_idx;
	num_nod_t 	max_idx;

	sornet_range(){
		init_me();
	}
	
	~sornet_range(){
	}

	void init_me(){
		min_idx = BJ_INVALID_SRT_GRP;
		max_idx = BJ_INVALID_SRT_GRP;
	}
	
	bool has_value(){
		bool d1 = (min_idx != BJ_INVALID_SRT_GRP);
		bool d2 = (max_idx != BJ_INVALID_SRT_GRP);
		MC_MARK_USED(d2);
		PTD_CK(d1 == d2);
		PTD_CK(! d1 || (min_idx >= 0));
		PTD_CK(! d1 || (max_idx >= min_idx));
		return d1;
	}

	bool equal_to(sornet_range& rr){
		bool e1 = (rr.min_idx == min_idx);
		bool e2 = (rr.max_idx == max_idx);
		return (e1 && e2);
	}
	
};

struct mc_aligned propag_transmitter_data {
public:
	net_side_t wrk_side;
	num_tier_t wrk_tier;
};

struct mc_aligned sornet_transmitter_data {
public:
	sorkind_t	knd;
	num_nod_t	idx;
	void*		inp;
	num_nod_t 	min_col;
	num_nod_t 	max_col;
	num_nod_t 	min_grp;
	num_nod_t 	max_grp;
};

struct mc_aligned stabi_transmitter_data {
public:
	num_tier_t wrk_tier;
	num_nod_t 	col_idx;
};

struct mc_aligned sync_transmitter_data {
public:
	net_side_t wrk_side;
	num_tier_t wrk_tier;
	nervenode* 	 cfl_src;
	bool 	has_zero_act_neus;
};

struct mc_aligned pgroup_transmitter_data {
public:
	pgrp_item* 			itm;
	base_transmitter*	nxt;
};

union mc_aligned all_transmitter_data {
public:
	propag_transmitter_data prp;
	sync_transmitter_data syc;
	stabi_transmitter_data stb;
	sornet_transmitter_data srt;
};

typedef base_transmitter dbg_transmitter;
typedef base_transmitter load_transmitter;
typedef base_transmitter propag_transmitter;
typedef base_transmitter sync_transmitter;
typedef base_transmitter stabi_transmitter;
typedef base_transmitter sornet_transmitter;
typedef base_transmitter pgroup_transmitter;

#define bj_base_transmitter_acquire_arr(num) \
	((base_transmitter*)(kernel::do_acquire(idx_base_transmitter, num)))

#define bj_base_transmitter_acquire() bj_base_transmitter_acquire_arr(1)
	
#define bj_dbg_transmitter_acquire() bj_base_transmitter_acquire()
#define bj_load_transmitter_acquire() bj_base_transmitter_acquire()
#define bj_propag_transmitter_acquire() bj_base_transmitter_acquire()
#define bj_sync_transmitter_acquire() bj_base_transmitter_acquire()
#define bj_stabi_transmitter_acquire() bj_base_transmitter_acquire()
#define bj_sornet_transmitter_acquire() bj_base_transmitter_acquire()
#define bj_pgroup_transmitter_acquire() bj_base_transmitter_acquire()

class mc_aligned base_transmitter : public missive {
public:
	MCK_DECLARE_MEM_METHODS(base_transmitter)

	all_transmitter_data d;

	base_transmitter() mc_external_code_ram;
	~base_transmitter() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_base_transmitter;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);
	
	void init_sornet_transmitter() bj_sornet_cod;
	void copy_sornet_transmitter(sornet_transmitter& orig) mc_external_code_ram;
	
	base_transmitter* clone_transmitter();
	
	virtual 
	char*	get_dbg_string(){
		return bj_tok_to_str(tok);
	}

	virtual
	char* 	get_class_name() mc_external_code_ram;

};


struct mc_aligned sornapse {
public:
	num_nod_t 	min_col = BJ_INVALID_SRT_GRP;
	num_nod_t 	max_col = BJ_INVALID_SRT_GRP;
	num_nod_t 	min_grp = BJ_INVALID_SRT_GRP;
	num_nod_t 	max_grp = BJ_INVALID_SRT_GRP;
	num_nod_t 	idx = 0;
	void*		inp = mc_null;
	sorcell*	out = mc_null;
	endcell*	axon = mc_null;

	void reset(void* invalid_val) bj_sornet_cod;
	
	void set_snp_fields(sornet_transmitter* sn_tmt, bool set_inp = true) bj_sornet_cod;
	
	bool jump_to_end(sorkind_t tmt_knd, num_nod_t srt_sz, bool is_end) bj_sornet_cod;
	bool jump_to_srt_end(num_nod_t srt_sz) bj_sornet_cod;
	bool jump_to_rnk_end(num_nod_t srt_sz, bool is_end) bj_sornet_cod;
};

#define bj_sorcell_acquire_arr(num) ((sorcell*)(kernel::do_acquire(idx_sorcell, num)))
#define bj_sorcell_acquire() bj_sorcell_acquire_arr(1)

class mc_aligned sorcell : public cell {
public:
	MCK_DECLARE_MEM_METHODS(sorcell)
	
	MC_DBG(
		num_nod_t	 	nod_id;
		num_nod_t 		level;
	);

	num_nod_t 	srt_sz;
	mc_flags_t  edge_flags;
	
	sornapse	up_snp;
	sornapse	down_snp;
	
	sorcell() mc_external_code_ram;
	~sorcell() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_sorcell;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) mc_external_code_ram;

	void calc_color() bj_sornet_cod;
	void sornet_reset(void* invalid_val) bj_sornet_cod;
	void sornet_set_fields(sornet_transmitter* sn_tmt, void* invalid_val) bj_sornet_cod;
	bool is_up_direct(void* invalid_val) bj_sornet_cod;
	bool is_down_direct(void* invalid_val) bj_sornet_cod;
	
	sornapse& get_snp(num_nod_t snp_idx){
		if(snp_idx == up_snp.idx){
			return up_snp;
		} 
		PTD_CK(snp_idx == down_snp.idx);
		return down_snp;
	}

	void sornet_handler(missive* msv) bj_sornet_cod;

	void sornet_handle_direct(sornet_transmitter* sn_tmt, void* invalid_val) bj_sornet_cod;
	
	void sornet_srt_handler(sornet_transmitter* sn_tmt) bj_sornet_cod;
	void sornet_rnk_handler(sornet_transmitter* sn_tmt) bj_sornet_cod;

	void sornet_dbg_prt() mc_external_code_ram;

	void load_from(pre_sornode* nod) bj_load_cod;
	
	virtual
	char* 	get_class_name() mc_external_code_ram;
};

#define bj_sornet_idx_inside(idx, min, max) ((min <= idx) && (idx <= max))

#define bj_endcell_acquire_arr(num) ((endcell*)(kernel::do_acquire(idx_endcell, num)))
#define bj_endcell_acquire() bj_endcell_acquire_arr(1)

class mc_aligned endcell : public cell {
public:
	MCK_DECLARE_MEM_METHODS(endcell)

	sornapse	end_snp;
	
	node_kind_t	end_ki;
	sornet_tok_t out_tok;

	neuron*		nxt_neu;
	polaron*	nxt_pol;
	
	endcell() mc_external_code_ram;
	~endcell() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_endcell;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) mc_external_code_ram;

	void sornet_handler(missive* msv) bj_sornet_cod;

	void sornet_srt_handler(sornet_transmitter* sn_tmt) bj_sornet_cod;
	void sornet_rnk_handler(sornet_transmitter* sn_tmt) bj_sornet_cod;

	void sornet_rnk_set_inp(void* inp);
	
	virtual
	char* 	get_class_name() mc_external_code_ram;
};

bj_cmp_obj_func_t sornet_get_cmp_func(sorkind_t knd) bj_sornet_cod;
void bj_send_sornet_tmt(cell* src, sornet_tok_t tok, sorkind_t knd, num_nod_t min_col, num_nod_t max_col, 
						num_nod_t min_grp, num_nod_t max_grp,
						void* obj, cell* dst, num_nod_t idx, 
						endcell* ecll = mc_null, bool dbg_msv = false) bj_sornet_cod;

num_nod_t bj_sornet_calc_grp_sz(num_nod_t min_grp, num_nod_t max_grp) bj_sornet_cod;


#define bj_pgrp_item_acquire_arr(num) ((pgrp_item*)(kernel::do_acquire(idx_pgrp_item, num)))
#define bj_pgrp_item_acquire() bj_pgrp_item_acquire_arr(1)

class mc_aligned pgrp_item : public cell {
public:
	MCK_DECLARE_MEM_METHODS(pgrp_item)
	
	void*		pnt;
	pgrp_item*	lft;
	pgrp_item*	rgt;

	void*		dat;
	
	pgrp_item() mc_external_code_ram;
	~pgrp_item() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_pgrp_item;
	}
	
	mc_inline_fn 
	pgrp_item* create_get_pnt(){ 
		PTD_CK(pnt != mc_null);
		return (pgrp_item*)pnt; 
	}

	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) mc_external_code_ram;

	pgrp_item*	create_get_pnt_of_nxt_add(pgroup* grp) mc_external_code_ram;

	void pgrp_op_handler(missive* msv) bj_pgroup_cod;
	
	void dbg_prt_nodes(long dd) mc_external_code_ram;
	
	virtual
	char* 	get_class_name() mc_external_code_ram;
};

#define bj_pgroup_acquire_arr(num) ((pgroup*)(kernel::do_acquire(idx_pgroup, num)))
#define bj_pgroup_acquire() bj_pgroup_acquire_arr(1)

class mc_aligned pgroup : public cell {
public:
	MCK_DECLARE_MEM_METHODS(pgroup)
	
	pgrp_item*	up;
	pgrp_item*	fst;
	pgrp_item*	lst;

	pgroup() mc_external_code_ram;
	~pgroup() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_pgroup;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) mc_external_code_ram;
	
	void create_add_item(pgrp_item* itm) mc_external_code_ram;

	void pgrp_op_handler(missive* msv) bj_pgroup_cod;
	
	void dbg_prt_nodes() mc_external_code_ram;
	
	virtual
	char* 	get_class_name() mc_external_code_ram;
};


#define bj_id_arr_copy(dst, sz, src) for(num_syn_t idx = 0; idx < sz; idx++){ dst[idx] = src[idx]; }

#define bj_synapse_acquire_arr(num) ((synapse*)(kernel::do_acquire(idx_synapse, num)))
#define bj_synapse_acquire() bj_synapse_acquire_arr(1)

class mc_aligned synapse : public cell {
public:
	MCK_DECLARE_MEM_METHODS(synapse)

	binder		right_handle;

	nervenode*	owner;
	synapse*	mate;

	synapse() mc_external_code_ram;
	~synapse() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_synapse;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);

	void load_handler(missive* msv) bj_load_cod;
	void propag_handler(missive* msv) bj_propag_cod;
	void stabi_handler(missive* msv) bj_stabi_cod;

	void propag_send_transmitter(propag_tok_t tok, net_side_t sd, bool dbg_is_forced = false) bj_propag_cod;
	void stabi_send_transmitter(stabi_tok_t tok, nervenode* src_nd = mc_null, 
				bool dbg_is_forced = false) mc_external_code_ram;

	mc_inline_fn binder& get_side_binder(net_side_t sd) bj_propag_cod;

	virtual
	char* 	get_class_name() mc_external_code_ram;
};

synapse* get_synapse_from_binder(net_side_t sd, binder* bdr);

#define bj_get_syn_of_rgt_handle(bdr) ((synapse*)(((uint8_t*)bdr) - mc_offsetof(&synapse::right_handle)))

#define	bj_stt_charge_all_flag mc_flag1

class mc_aligned side_state : public binder {
public:
	mc_flags_t		step_flags;
	synset			step_active_set;
	num_syn_t 		step_prev_tot_active;
	num_syn_t		step_num_complete;
	num_syn_t		step_num_ping;

	num_tier_t		propag_num_tier;
	synapse*		propag_source;
	grip			propag_tiers;

	PTD_DBG_CODE(bool was_all_pg);

	side_state() mc_external_code_ram;
	~side_state() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);

	bool charge_all_active(signal_data* dat, node_kind_t ki) bj_propag_cod;
	void step_reset_complete();

	tierset*	dbg_get_tiset(grip& all_ti, num_tier_t nti) mc_external_code_ram;

	tierset*	get_tiset(num_tier_t nti = BJ_INVALID_NUM_TIER) bj_propag_cod;
	tierset&	add_tiset(num_tier_t nti) bj_propag_cod;

	mc_inline_fn void update_prv_tot_active() bj_load_cod;

	bool is_mono(num_tier_t nti) bj_propag_cod;

	mc_inline_fn bool neu_all_ping(tier_kind_t tiki){
		num_syn_t lim = 0;
		if(tiki == tiki_propag){ lim = 1; }
		bool all_pg = ((step_prev_tot_active > lim) && (step_num_ping == step_prev_tot_active));
		PTD_CK(! all_pg || (step_active_set.tot_syn > 0));
		return all_pg;
	}

	void send_all_propag(nervenode* nd, signal_data* dat) bj_propag_cod;

	mc_inline_fn bool is_full(){
		return (step_num_complete == step_prev_tot_active);
	}

	void propag_send_all_ti_done(nervenode* nd, net_side_t sd, num_tier_t dat_ti) bj_propag_cod;
	
};

bool bj_can_delay_tier(num_tier_t the_ti, grip& all_ti);

void bj_stabi_reset_all_tiers(grip& dst_grp, grip& src_grp) mc_external_code_ram;
int bj_cmp_sep_objs(void* obj1, void* obj2) mc_external_code_ram;
int bj_cmp_cell_objs(void* obj1, void* obj2) mc_external_code_ram;
int bj_cmp_stabi_color_arrs(num_syn_t sz1, num_nod_t* arr1, num_syn_t sz2, num_nod_t* arr2) bj_stabi_cod;

typedef int (*bj_cmp_func_t)(binder* obj1, binder* obj2);

bool bj_is_sorted(grip& grp, bj_cmp_func_t fn, int ord) mc_external_code_ram;

int bj_dbg_cmp_synset_by_arr_id(binder* obj1, binder* obj2) mc_external_code_ram;
int bj_dbg_cmp_synset_by_tot_syn(binder* obj1, binder* obj2) mc_external_code_ram;

char* bj_dbg_stabi_col_arr_to_str(num_syn_t sz1, num_nod_t* arr1, 
								 int sz_str, char* str) mc_external_code_ram;

#define	bj_stabi_send_pings_flag 			mc_flag0
#define	bj_stabi_srt_start_sep_flag 		mc_flag1
#define	bj_stabi_srt_sep_rdy_flag 			mc_flag2
#define	bj_stabi_srt_cll_rdy_flag 			mc_flag3
#define	bj_stabi_ti_done_flag 				mc_flag4

class mc_aligned nervenode : public cell {
public:
	node_kind_t 	ki;
	long			id;
	num_syn_t		sz;
	num_tier_t		creat_tier;

	side_state		left_side;
	side_state		right_side;

	num_tier_t		stabi_num_tier;
	
	mc_flags_t		stabi_flags;
	num_tier_t		stabi_tmp_tier;
	num_syn_t		stabi_arr_sz;
	num_nod_t*  	stabi_arr_dat;
	num_syn_t		stabi_prv_arr_sz;
	num_nod_t*  	stabi_prv_arr_dat;

	sornet_range	stabi_col;
	num_nod_t 		stabi_idx;
	sorcell*		stabi_out;
	
	nervenode() mc_external_code_ram;
	~nervenode() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);

	void init_nervenode_with(pre_cnf_node* nod) bj_load_cod;

	side_state& get_side_state(net_side_t sd);	// Used in loading. It has to common code.

	char* get_ki_str() mc_external_code_ram;
	
	// PROPAG

	void propag_recv_transmitter(signal_data* dat) bj_propag_cod;
	void propag_recv_charge_all(signal_data* dat) bj_propag_cod;
	void propag_recv_charge_src(signal_data* dat) bj_propag_cod;
	void propag_recv_ping(signal_data* dat) bj_propag_cod;
	void propag_recv_tier_done(signal_data* dat) bj_propag_cod;

	virtual 
	void propag_send_snp_propag(callee_prms& pms) bj_propag_cod;

	void propag_send_snp_charge_src(callee_prms& pms) bj_propag_cod;
	void propag_send_snp_tier_done(callee_prms& pms) bj_propag_cod;

	void send_confl_tok(signal_data* dat, sync_tok_t the_tok) mc_external_code_ram;

	virtual 
	bool is_tier_complete(signal_data* dat);

	bool stabi_recalc_snd_pings_flg(signal_data* dat) mc_external_code_ram;

	virtual 
	bool stabi_recalc_send_pings(signal_data* dat) mc_external_code_ram;

	virtual 
	void propag_start_nxt_tier(signal_data* dat) bj_propag_cod;

	void dbg_prt_syn(callee_prms& pms) mc_external_code_ram;

	void dbg_prt_nod(net_side_t sd, tier_kind_t tiki, char* prefix, num_pulse_t num_pul, 
			num_tier_t num_ti) mc_external_code_ram;

	// STABI

	void mirrow_sides(net_side_t sd, sornet_range& mates_rng) mc_external_code_ram;
	void stabi_recv_transmitter(signal_data* dat) mc_external_code_ram;
	void stabi_recv_color(signal_data* dat) mc_external_code_ram;
	void stabi_recv_ping(signal_data* dat) mc_external_code_ram;
	void stabi_recv_tier_done(signal_data* dat) mc_external_code_ram;

	void stabi_send_snp_color(callee_prms& pms) mc_external_code_ram;
	void stabi_send_snp_tier_done(callee_prms& pms) mc_external_code_ram;

	virtual 
	void dbg_prt_tier_done(signal_data* dat) mc_external_code_ram;
	
	void stabi_send_sep(void* pm) mc_external_code_ram;
	void stabi_set_start_sep(void*) mc_external_code_ram;
	void stabi_send_start_pol_sep(void* pm) mc_external_code_ram;
	void stabi_send_start_neu_sep(void* pm) mc_external_code_ram;
	bool stabi_is_rdy_to_srt() mc_external_code_ram;
	void stabi_send_start_srt(int caller = 0) mc_external_code_ram;

	void stabi_set_tier_done(num_tier_t num_ti) mc_external_code_ram;
	
	//virtual 
	//void stabi_send_nxt_tier_color() bj_stabi_cod;
	
	virtual 
	void stabi_start_nxt_tier(signal_data* dat) mc_external_code_ram;

	void stabi_reset_complete() mc_external_code_ram;
	void stabi_insert_color(num_nod_t col_idx) mc_external_code_ram;
	
	bool stabi_is_active(){
		return (! left_side.step_active_set.is_synset_empty());
	}
	
	void stabi_set_color(sornet_transmitter* sn_tmt) mc_external_code_ram;
	void stabi_set_inactive(grip& all_inac) mc_external_code_ram;
	void stabi_update_min_max() mc_external_code_ram;

	void stabi_recv_rnk_cll(missive* msv) mc_external_code_ram;
	void stabi_fix_pre_rdy(sornet_range& mates_rng) mc_external_code_ram;
	
	void dbg_prt_active_synset(net_side_t sd, tier_kind_t tiki, char* prefix, 
				num_tier_t num_ti) mc_external_code_ram;

	PTD_DBG_CODE(void dbg_prt_snp_id(callee_prms& pms) mc_external_code_ram);

	virtual
	char* 	get_class_name() mc_external_code_ram;

	// stabi
	
	void stabi_send_all_ti_done(nervenode* nd, num_tier_t dbg_ti) mc_external_code_ram;
	
};

neuron* bj_get_neu(binder* dlyd, tier_kind_t tiki, net_side_t sd);

#define bj_get_nod_of_pt_lft_st(bdr) ((nervenode*)(((uint8_t*)bdr) - mc_offsetof(&nervenode::left_side)))
#define bj_get_nod_of_pt_rgt_st(bdr) ((nervenode*)(((uint8_t*)bdr) - mc_offsetof(&nervenode::right_side)))

#define bj_neuron_acquire_arr(num) ((neuron*)(kernel::do_acquire(idx_neuron, num)))
#define bj_neuron_acquire() bj_neuron_acquire_arr(1)

class mc_aligned neuron : public nervenode {
public:
	MCK_DECLARE_MEM_METHODS(neuron)

	//PTD_CODE(int pru_attr);
	
	neuron() mc_external_code_ram;
	~neuron() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_neuron;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);

	void get_delayed_data(tier_kind_t tiki, net_side_t sd, num_tier_t& the_ti, mck_token_t& tok);
	bool can_delay(tier_kind_t tiki, net_side_t sd);
	bool is_to_delay(tier_kind_t tiki, net_side_t sd, num_tier_t the_ti, grip& all_ti);
	
	void propag_handler(missive* msv) bj_propag_cod;
	void stabi_handler(missive* msv) bj_stabi_cod;

	void propag_neuron_start() bj_propag_cod;
	void stabi_neuron_start() bj_stabi_cod;

	virtual 
	bool stabi_recalc_send_pings(signal_data* dat) bj_stabi_cod;

	virtual 
	void propag_start_nxt_tier(signal_data* dat) bj_propag_cod;

	virtual 
	void propag_send_snp_propag(callee_prms& pms) bj_propag_cod;

	void pru_callee(callee_prms& pms) mc_external_code_ram;

	//virtual 
	//void stabi_send_nxt_tier_color() bj_stabi_cod;
	
	virtual 
	void stabi_start_nxt_tier(signal_data* dat) bj_stabi_cod;
	
	void stabi_recv_rnk_sep(missive* msv);
	
	virtual
	char* 	get_class_name() mc_external_code_ram;
};

#define bj_polaron_acquire_arr(num) ((polaron*)(kernel::do_acquire(idx_polaron, num)))
#define bj_polaron_acquire() bj_polaron_acquire_arr(1)

class mc_aligned polaron : public nervenode {
public:
	MCK_DECLARE_MEM_METHODS(polaron)
	
	polaron*		opp;

	polaron() mc_external_code_ram;
	~polaron() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_polaron;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) mc_external_code_ram;

	void load_handler(missive* msv) bj_load_cod;
	void stabi_handler(missive* msv) bj_stabi_cod;

	void propag_send_snp_charge_all(callee_prms& pms) bj_propag_cod;

	void charge_all_confl_and_start_nxt_ti(signal_data* dat) mc_external_code_ram;
	void charge_all_and_start_nxt_ti(signal_data* dat) bj_propag_cod;

	virtual 
	bool is_tier_complete(signal_data* dat);

	virtual 
	bool stabi_recalc_send_pings(signal_data* dat) bj_stabi_cod;
	
	virtual 
	void propag_start_nxt_tier(signal_data* dat) bj_propag_cod;

	virtual 
	void dbg_prt_tier_done(signal_data* dat) mc_external_code_ram;

	//virtual 
	//void stabi_send_nxt_tier_color() bj_stabi_cod;

	virtual 
	void stabi_start_nxt_tier(signal_data* dat) bj_stabi_cod;
	
	void stabi_recv_rnk_sep(missive* msv) mc_external_code_ram;
	
	virtual
	char* 	get_class_name() mc_external_code_ram;
};

#define	bj_sent_inert_flag mc_flag0

#define bj_tierdata_acquire_arr(num) ((tierdata*)(kernel::do_acquire(idx_tierdata, num)))
#define bj_tierdata_acquire() bj_tierdata_acquire_arr(1)

class mc_aligned tierdata : public agent {
public:
	MCK_DECLARE_MEM_METHODS(tierdata)

	num_tier_t	tdt_id;
	mc_flags_t	tdt_flags;

	binder	all_delayed;

	mc_workeru_nn_t num_inert_chdn;
	mc_workeru_nn_t num_zero_act_neus_chdn;

	num_nod_t inp_neus;
	num_nod_t off_neus;
	num_nod_t rcv_neus;
	num_nod_t stl_neus;
	num_nod_t dly_neus;

	tierdata() mc_external_code_ram;
	~tierdata() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_tierdata;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);

	void add_all_inp_from(grip& grp, net_side_t sd) mc_external_code_ram;

	mc_inline_fn void inc_rcv(){ rcv_neus++; }
	mc_inline_fn void inc_off(){ off_neus++; }

	void update_tidat();

	mc_inline_fn bool is_tidat_empty(){
		return ((inp_neus != BJ_INVALID_NUM_NODE) && (inp_neus == 0));
	}

	mc_inline_fn bool got_all_neus(){
		return ((inp_neus != BJ_INVALID_NUM_NODE) && (inp_neus == rcv_neus));
	}

	mc_inline_fn bool is_inert(){
		return ((inp_neus != BJ_INVALID_NUM_NODE) && ((inp_neus - off_neus) == dly_neus));
	}
	
	mc_inline_fn num_nod_t num_active_neus(){
		PTD_CK(inp_neus != BJ_INVALID_NUM_NODE);
		return (inp_neus - off_neus);
	}

	mc_inline_fn tierdata& prv_tier(){
		return *((tierdata*)bn_left);
	}

	mc_inline_fn bool is_first_tier(grip& all_tiers){
		return (bn_left == &all_tiers);
	}

	void delay_binder(binder& bdr, int dbg_caller);
	
	void proc_delayed(tier_kind_t tiki, grip& all_ti, net_side_t sd, 
					  bool star_nxt_ti) mc_external_code_ram;

	virtual
	char* 	get_class_name() mc_external_code_ram;
};


#define bj_layerdata_acquire_arr(num) ((layerdata*)(kernel::do_acquire(idx_layerdata, num)))
#define bj_layerdata_acquire() bj_layerdata_acquire_arr(1)

class mc_aligned layerdata : public agent {
public:
	MCK_DECLARE_MEM_METHODS(layerdata)

	num_layer_t		lyr_id;
	grip 			all_inactive_pol;
	grip 			all_inactive_neu;

	sornet_range	inactive_neus_col;
	sornet_range	inactive_pols_col;

	layerdata() mc_external_code_ram;
	~layerdata() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return idx_layerdata;
	}
	
	virtual mc_opt_sz_fn 
	void init_me(int caller = 0);

	virtual
	char* 	get_class_name() mc_external_code_ram;
};

class mc_aligned netstate {
public:
	net_side_t my_side;

	bool		sync_all_zero_act_neus;
	bool		sync_is_inactive;
	num_tier_t	sync_wait_tier;
	bool 		sync_is_ending;

	grip 		all_propag_tiers;
	grip 		all_stabi_tiers;
	
	sync_tok_t  tok_confl;
	nervenode*	nod_confl;
	num_tier_t	ti_confl;

	netstate() mc_external_code_ram;
	~netstate() mc_external_code_ram;

	void broadcast_tier(tier_kind_t tiki, tierdata& lti, int dbg_caller) mc_external_code_ram;

	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) mc_external_code_ram;

	void init_sync() mc_external_code_ram;

	void init_propag_tiers(nervenet& nnt) mc_external_code_ram;
	void init_stabi_tiers() mc_external_code_ram;

	void init_sync_transmitter(sync_transmitter& tmt, num_tier_t the_ti, nervenode* cfl_src, bool has_zan);

	void send_sync_transmitter(tier_kind_t tiki, nervenet* the_dst, sync_tok_t the_tok, num_tier_t the_ti,
			nervenode* cfl_src = mc_null, bool has_zan = false) mc_external_code_ram;

	void send_sync_to_children(sync_tok_t the_tok, num_tier_t the_ti, tier_kind_t tiki, 
			nervenode* cfl_src, bool has_zan = false) mc_external_code_ram; 

	void send_up_confl_tok(sync_tok_t the_tok, num_tier_t the_ti, nervenode* the_cfl) mc_external_code_ram;

	void propag_handle_my_sync() bj_propag_cod;
	void stabi_handle_my_sync() bj_stabi_cod;

	void update_sync_inert(tier_kind_t tiki, bool remove_full = false);

	grip& get_all_tiers(tier_kind_t tiki) mc_external_code_ram;

	tierdata& get_tier(tier_kind_t tiki, grip& all_ti, num_tier_t nti, int dbg_caller) mc_external_code_ram;
	
	void dbg_prt_all_tiers(tier_kind_t tiki, char* prefix, num_tier_t num_ti) mc_external_code_ram;

	void inc_tier_rcv(nervenode* nd, tier_kind_t tiki, num_tier_t the_ti, 
					  grip& all_ti) mc_external_code_ram;
	
};

void inc_tier(tier_kind_t tiki, grip& all_ti, net_side_t sd, int dbg_caller);

mc_inline_fn num_tier_t prv_num_tier(num_tier_t the_ti){
	PTD_CK(the_ti != BJ_INVALID_NUM_TIER);
	return (the_ti - 1);
}

mc_inline_fn tierdata& get_last_tier(grip& all_ti){
	PTD_CK(! all_ti.is_alone());
	return *((tierdata*)(all_ti.bn_left));
}

mc_inline_fn bool is_last_tier(grip& all_ti, tierdata& tdt){
	PTD_CK(! all_ti.is_alone());
	return (((tierdata*)(all_ti.bn_left)) == (&tdt));
}

class mc_aligned dbg_stats {
public:
	mc_alloc_size_t dbg_tot_new_synset;
	mc_alloc_size_t dbg_tot_new_tierset;
	mc_alloc_size_t dbg_tot_new_base_transmitter;
	mc_alloc_size_t dbg_tot_new_synapse;
	mc_alloc_size_t dbg_tot_new_side_state;
	mc_alloc_size_t dbg_tot_new_nervenode;
	mc_alloc_size_t dbg_tot_new_neuron;
	mc_alloc_size_t dbg_tot_new_polaron;
	mc_alloc_size_t dbg_tot_new_sorcell;
	mc_alloc_size_t dbg_tot_new_endcell;
	mc_alloc_size_t dbg_tot_new_pgrp_item;
	mc_alloc_size_t dbg_tot_new_pgroup;
	mc_alloc_size_t dbg_tot_new_tierdata;
	mc_alloc_size_t dbg_tot_new_layerdata;

	dbg_stats() mc_external_code_ram;
	~dbg_stats() mc_external_code_ram;

	virtual mc_opt_sz_fn 
	void init_me(int caller = 0) mc_external_code_ram;

	void dbg_prt_all() mc_external_code_ram;
};


struct mc_aligned ini_grps_prms {
public:
	PTD_DBG_CODE(tak_mak* num_gen = mc_null);
	num_nod_t tot_input_objs = 0;
	void**	all_output_objs = mc_null;
	num_nod_t* all_min_grps = mc_null;
	num_nod_t* all_max_grps = mc_null;
	num_nod_t* all_rnk_grps = mc_null;
	bool* is_input_grp = mc_null;
	num_nod_t tot_grps = 0;
};


#define BJ_MAX_ID_ARR_SZ 260

#define BJ_DBG_STR_CAP 2048

typedef unsigned long mini_bit_arr_t;

#define	bj_stabi_started_pol_sep_flag 			mc_flag0
#define	bj_stabi_sent_active_neus_rng_flag 		mc_flag1
#define	bj_stabi_sent_inactive_neus_rng_flag 	mc_flag2
#define	bj_stabi_sent_active_pols_rng_flag 		mc_flag3
#define	bj_stabi_sent_inactive_pols_rng_flag 	mc_flag4

class mc_aligned nervenet : public cell {
public:
	MCK_DECLARE_MEM_METHODS(nervenet)

	long MAGIC;

	//num_syn_t tmp_id_arr[BJ_MAX_ID_ARR_SZ];

	num_nod_t tot_wu_neus;
	num_nod_t tot_wu_vars;
	num_nod_t tot_wu_lits;
	num_nod_t tot_wu_rels;

	mc_alloc_size_t	num_sep_tiersets;
	mc_alloc_size_t	num_sep_tierdatas;
	mc_alloc_size_t	num_sep_layerdatas;

	grip		ava_base_transmitters;
	grip		ava_synsets;
	grip		ava_tiersets;
	grip		ava_synapses;
	grip		ava_neurons;
	grip		ava_polarons;
	grip		ava_sorcells;
	grip		ava_endcells;
	grip		ava_pgrp_items;
	grip		ava_pgroups;
	grip		ava_tierdatas;
	grip		ava_layerdatas;

	grip* all_ava[idx_total];
	mc_alloc_kernel_func_t all_acq[idx_total];
	mc_alloc_kernel_func_t all_sep[idx_total];
	
	missive_handler_t all_net_handlers[idx_total];

	pre_load_cnf*	shd_full_cnf;
	pre_cnf_net*	shd_cnf;

	num_nod_t tot_loading;
	num_nod_t tot_loaded;

	grip	all_wu_active_neu;
	grip	all_wu_active_pos;
	grip	all_wu_active_neg;

	mc_workeru_nn_t sync_tot_children;
	mc_workeru_id_t sync_parent_id;
	mc_load_map_st* sync_map;

	netstate	act_left_side;
	netstate	act_right_side;

	sornet_range	active_neus_col;
	sornet_range	active_pols_col;

	sornet_range	stabi_nxt_active_neus_rng;
	sornet_range	stabi_nxt_inactive_neus_rng;
	sornet_range	stabi_nxt_active_pols_rng;
	sornet_range	stabi_nxt_inactive_pols_rng;
	
	polaron*		stabi_min_pol;
	polaron*		stabi_max_pol;
	
	mc_flags_t		stabi_wu_flags;
	
	num_nod_t 	stabi_wu_tot_nod_sep;
	num_nod_t 	stabi_wu_num_rcv_sep;
	
	grip 		all_layers;
	
	PTD_DBG_CODE(
		dbg_stats all_dbg_dat;
		char 	dbg_str1[BJ_DBG_STR_CAP];
		char 	dbg_str2[BJ_DBG_STR_CAP];
	);

	num_nod_t tot_wu_sorcells;
	grip	all_wu_sorcells;

	num_nod_t tot_wu_rnkcells;
	grip	all_wu_rnkcells;
	
	num_nod_t tot_wu_srt_endcells;
	grip	all_wu_srt_endcells;

	num_nod_t tot_wu_rnk_endcells;
	grip	all_wu_rnk_endcells;

	mini_bit_arr_t 	dbg_sornet_curr_cntr;
	mini_bit_arr_t 	dbg_sornet_max_cntr;
	
	PTD_DBG_CODE(tak_mak dbg_random_gen);
	num_nod_t*	dbg_all_input_vals;

	binval_t	dbg_net_top;
	binval_t	dbg_net_bottom;

	num_nod_t 	dbg_tot_input_sorcells;
	sorcell**	dbg_all_input_sorcells;
	num_nod_t*	dbg_all_input_srt_min_grps;
	num_nod_t*	dbg_all_input_srt_max_grps;
	
	num_nod_t 	dbg_tot_grp_items;
	num_nod_t 	dbg_idx_grp_test;
	
	num_nod_t 	dbg_tot_rcv_output_sorobjs;

	endcell**	dbg_all_srt_endcells;
	
	void**		dbg_all_output_sorobjs;
	num_nod_t*	dbg_all_output_sorcols;

	num_nod_t 	dbg_tot_input_rnkcells;
	sorcell**	dbg_all_input_rnkcells;
	
	num_nod_t*	dbg_all_input_rnkgrps;
	num_nod_t*	dbg_all_input_rnk_min_grps;
	num_nod_t*	dbg_all_input_rnk_max_grps;
	num_nod_t*	dbg_all_output_rnk_min_cols;
	num_nod_t*	dbg_all_output_rnk_max_cols;
	bool*		dbg_is_input_rnkgrps;
	
	nervenet() mc_external_code_ram;
	~nervenet() mc_external_code_ram;

	void init_nervenet_with(pre_cnf_net* pre_net) mc_external_code_ram;
	void init_mem_funcs() mc_external_code_ram;

	void sync_handler(tier_kind_t tiki, missive* msv);

	void dbg_only_handler(missive* msv) bj_dbg_only_cod;
	void load_handler(missive* msv) bj_load_cod;
	void propag_handler(missive* msv) bj_propag_cod;
	void sornet_handler(missive* msv) bj_sornet_cod;

	void init_sync_cycle() mc_external_code_ram;
	void propag_nervenet_start() bj_propag_cod;

	void propag_handle_sync() bj_propag_cod;
	void send_parent_tok_empty_child(net_side_t sd) bj_propag_cod;

	bool propag_has_all_charged() bj_propag_cod;
	
	void mirrow_handler(missive* msv) bj_stabi_cod;
	void mirrow_nervenet() bj_stabi_cod;
	void mirrow_start_all_nods(grip& all_nod, net_side_t sd, sornet_range& mates_rng) bj_stabi_cod;

	void stabi_handler(missive* msv) bj_stabi_cod;
	void stabi_nervenet_start() bj_stabi_cod;
	
	void stabi_inc_rcv_sep(node_kind_t nod_ki) mc_external_code_ram;

	void send_all_neus(mck_token_t tok) mc_external_code_ram;

	void send_all_children(base_transmitter& tmt, char* dbg_str = mc_null) mc_external_code_ram; 
	
	void stabi_broadcast_range(rangekind_t knd, sornet_range active, sornet_range inactive);
	void stabi_broadcast_sep() mc_external_code_ram;
	void stabi_broadcast_all_ranges() mc_external_code_ram;
	void stabi_start_pol_sep() mc_external_code_ram;
	bool stabi_has_all_ranges() mc_external_code_ram;
	void stabi_set_ranges(missive* msv) mc_external_code_ram;
	void stabi_begin_subgrouping() mc_external_code_ram;
	//bool stabi_has_pre_rdy_quas();
	bool stabi_has_pre_rdy_neus() mc_external_code_ram;
	void stabi_mark_all_pre_rdy(sornet_range& mates_rng, grip& all_act) mc_external_code_ram;
	
	bool sornet_dbg_check_order(sorkind_t knd) mc_external_code_ram;

	void sornet_dbg_tests_handler(missive* msv) mc_external_code_ram;
	bool sornet_dbg_send_nxt_step(sorkind_t knd) mc_external_code_ram;
	void sornet_dbg_end_step(sorkind_t knd) mc_external_code_ram;

	void sornet_dbg_bin_send_step() mc_external_code_ram;
	void sornet_dbg_bin_end_step() mc_external_code_ram;
	mini_bit_arr_t sornet_dbg_bin_get_mini_sorted_arr() mc_external_code_ram;

	void sornet_dbg_num_init_val_arr() mc_external_code_ram;
	void sornet_dbg_num_send_step() mc_external_code_ram;
	void sornet_dbg_num_end_step() mc_external_code_ram;
	void sornet_dbg_num_print_input() mc_external_code_ram;

	void sornet_dbg_rnk_send_step() mc_external_code_ram;
	void sornet_dbg_rnk_end_step() mc_external_code_ram;
	void sornet_dbg_rnk_print_input() mc_external_code_ram;

	nervenet* get_nervenet(mc_workeru_id_t workeru_id) mc_external_code_ram;

	netstate& get_active_netstate(net_side_t sd) mc_external_code_ram;

	void inc_layers() mc_external_code_ram;
	
	layerdata& get_last_layer(){
		PTD_CK(! all_layers.is_alone());
		return *((layerdata*)(all_layers.bn_left));
	}
	
	virtual
	char* 	get_class_name() mc_external_code_ram;
};

num_nod_t bj_dbg_sornet_get_nw_grp(ini_grps_prms& prms) mc_external_code_ram;
num_nod_t bj_dbg_rnk_get_grp_max(num_nod_t min_idx, num_nod_t grp_sz, 
								 ini_grps_prms& prms) mc_external_code_ram;
void bj_dbg_sornet_init_grp_arr(ini_grps_prms& prms, bool just_ones) mc_external_code_ram;

#define bj_num_sep_tiersets (bj_nervenet->num_sep_tiersets)
#define bj_num_sep_tierdatas (bj_nervenet->num_sep_tierdatas)
#define bj_num_sep_layerdatas (bj_nervenet->num_sep_layerdatas)

#define bj_nervenet ((nervenet*)(kernel::get_sys()->user_data))
#define bj_ava_base_transmitters (bj_nervenet->ava_base_transmitters)
#define bj_ava_synsets (bj_nervenet->ava_synsets)
#define bj_ava_tiersets (bj_nervenet->ava_tiersets)
#define bj_ava_synapses (bj_nervenet->ava_synapses)
#define bj_ava_neurons (bj_nervenet->ava_neurons)
#define bj_ava_polarons (bj_nervenet->ava_polarons)
#define bj_ava_sorcells (bj_nervenet->ava_sorcells)
#define bj_ava_endcells (bj_nervenet->ava_endcells)
#define bj_ava_pgrp_items (bj_nervenet->ava_pgrp_items)
#define bj_ava_pgroups (bj_nervenet->ava_pgroups)
#define bj_ava_tierdatas (bj_nervenet->ava_tierdatas)
#define bj_ava_layerdatas (bj_nervenet->ava_layerdatas)

#define bj_handlers (bj_nervenet->all_net_handlers)

extern missive_handler_t bj_nil_handlers[];

//void bj_propag_kernel_func();
//void bj_stabi_kernel_func();

#define	bj_dbg_prt_nd_neu_flag mc_flag1
#define	bj_dbg_prt_nd_pol_flag mc_flag2
#define	bj_dbg_prt_id_pol_flag mc_flag3

void bj_print_loaded_poles(grip& all_pol, node_kind_t ki) mc_external_code_ram;
void bj_print_loaded_cnf() mc_external_code_ram;

void bj_print_active_cnf(net_side_t sd, tier_kind_t tiki, char* prefix, num_pulse_t num_pul, 
			num_tier_t num_ti, mc_flags_t prt_flgs = bj_dbg_prt_nd_neu_flag) mc_external_code_ram;

void bj_print_class_szs() mc_external_code_ram;

char* bj_dbg_tier_kind_to_str(tier_kind_t tiki) mc_external_code_ram;

void bj_init_nervenet() mc_external_code_ram;

mc_inline_fn num_tier_t get_last_stb_ti(){
	return get_last_tier(bj_nervenet->act_left_side.all_stabi_tiers).tdt_id;
}

#endif		// NERVENET_H


