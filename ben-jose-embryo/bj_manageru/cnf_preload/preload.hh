

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

preload.hh

Declaration of functions to preload cnfs in dimacs files.

--------------------------------------------------------------*/

#ifndef PRELOAD_CNF_H
#define PRELOAD_CNF_H

#include "cell.hh"
#include "solver.hh"

class pre_sornode;
class pre_endnode;
class pre_item_pgroup;
class pre_pgroup;
class pre_cnf_node;
class pre_cnf_net;
class pre_load_cnf;

extern grip ava_pre_sornode;
extern grip ava_pre_endnode;
extern grip ava_pre_item_pgroup;
extern grip ava_pre_pgroup;
extern grip ava_pre_cnf_node;
extern grip ava_pre_cnf_net;

typedef unsigned long pre_node_sz_t;

enum sornod_kind_t : uint8_t {
	snod_invalid,
	snod_half,
	snod_alte
};

enum connect_kind_t : uint8_t {
	conn_invalid,
	conn_up,
	conn_down
};

struct mc_aligned sornet_prms {
public:
	num_nod_t tot_nods = 0;
	pre_sornode** arr_nods = mc_null;
	pre_endnode** arr_endnods = mc_null;

	num_nod_t tot_lvs = 0;
	mc_workeru_nn_t* arr_lvs = mc_null;

	num_nod_t curr_nod_id = 0;

	num_nod_t curr_merge_sz = 0;
	num_nod_t first_lv_for_merge_sz = 0;
};

struct mc_aligned pre_sornapse {
public:
	num_nod_t 		idx = BJ_INVALID_IDX;
	pre_sornode* 	out = mc_null;
	pre_endnode* 	axon = mc_null;
};

mc_inline_fn void
bj_init_pre_sornapse(pre_sornapse& psnp){
	psnp.idx = BJ_INVALID_IDX;
	psnp.out = mc_null;
	psnp.axon = mc_null;
}

/*! \class pre_sornode
\brief Class for sornet nodes to load.

*/

#define bj_pre_sornode_acquire_arr(num) ((pre_sornode*)(kernel::do_acquire(bj_cell_id(pre_sornode), num)))
#define bj_pre_sornode_acquire() bj_pre_sornode_acquire_arr(1)

class mc_aligned pre_sornode : public agent {
public:
	MCK_DECLARE_MEM_METHODS(pre_sornode)

	num_nod_t	 	nod_id;
	num_nod_t 		level;

	num_nod_t 		srt_sz;
	mc_flags_t		edge_flags;
	
	pre_sornapse	up_pns;
	pre_sornapse	down_pns;

	void* 			loaded;

	pre_sornode(){
		nod_id = 0;
		level = 0;

		srt_sz = 0;
		edge_flags = 0;
		
		bj_init_pre_sornapse(up_pns);	// for parallella to work
		bj_init_pre_sornapse(down_pns);	// for parallella to work

		loaded = mc_null;
	}

	~pre_sornode(){}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return bj_cell_id(pre_sornode);
	}
	
	void reset_up_end();
	void set_up_end();
	void reset_down_end();
	void set_down_end();

	void dbg_log_nod();	
};

/*! \class pre_endnode
\brief Class for output sornet nodes to load.

*/

#define bj_pre_endnode_acquire_arr(num) ((pre_endnode*)(kernel::do_acquire(bj_cell_id(pre_endnode), num)))
#define bj_pre_endnode_acquire() bj_pre_endnode_acquire_arr(1)

class mc_aligned pre_endnode : public agent {
public:
	MCK_DECLARE_MEM_METHODS(pre_endnode)

	pre_sornapse	nxt;
	void* 			loaded;

	pre_endnode(){
		bj_init_pre_sornapse(nxt);	// for parallella to work
		
		loaded = mc_null;
	}

	~pre_endnode(){}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return bj_cell_id(pre_endnode);
	}
};

/*
struct mc_aligned pgrp_prms {
public:
	num_nod_t tot_items = 0;
	pre_item_pgroup** arr_items = mc_null;
};
*/

/*! \class pre_item_pgroup
\brief Class for elementes of parallel groups.

*/

#define bj_pre_item_pgroup_acquire_arr(num) \
	((pre_item_pgroup*)(kernel::do_acquire(bj_cell_id(pre_item_pgroup), num)))
	
#define bj_pre_item_pgroup_acquire() bj_pre_item_pgroup_acquire_arr(1)

class mc_aligned pre_item_pgroup : public agent {
public:
	MCK_DECLARE_MEM_METHODS(pre_item_pgroup)

	void*				pnt;
	pre_item_pgroup*	lft;
	pre_item_pgroup*	rgt;
	
	void* 			loaded;

	pre_item_pgroup(){
		pnt = mc_null;
		lft = mc_null;
		rgt = mc_null;
		
		loaded = mc_null;
	}

	~pre_item_pgroup(){}
	
	pre_item_pgroup* get_pnt(){ 
		PTD_CK(pnt != mc_null);
		return (pre_item_pgroup*)pnt; 
	}

	pre_item_pgroup* get_pnt_of_nxt_add(pre_pgroup* grp);

	void prt_nodes(long dd);
	
	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return bj_cell_id(pre_item_pgroup);
	}
};

/*! \class pre_pgroup
\brief Class for parallel groups.

*/

#define bj_pre_pgroup_acquire_arr(num) \
	((pre_pgroup*)(kernel::do_acquire(bj_cell_id(pre_pgroup), num)))
	
#define bj_pre_pgroup_acquire() bj_pre_pgroup_acquire_arr(1)

class mc_aligned pre_pgroup : public agent {
public:
	MCK_DECLARE_MEM_METHODS(pre_pgroup)

	pre_item_pgroup*	up;
	pre_item_pgroup*	fst;
	pre_item_pgroup*	lst;
	
	void* 			loaded;

	pre_pgroup(){
		up = mc_null;
		fst = mc_null;
		lst = mc_null;
		
		loaded = mc_null;
	}

	~pre_pgroup(){}

	void add_item(pre_item_pgroup* itm);
	
	void prt_nodes();
	
	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return bj_cell_id(pre_pgroup);
	}
};

/*! \class pre_cnf_node
\brief Class for neurons and polarons to load. All refs are in all_agts (inherited from agent_grp).

*/

#define bj_pre_cnf_node_acquire_arr(num) ((pre_cnf_node*)(kernel::do_acquire(bj_cell_id(pre_cnf_node), num)))
#define bj_pre_cnf_node_acquire() bj_pre_cnf_node_acquire_arr(1)

class mc_aligned pre_cnf_node : public agent_grp {
public:
	MCK_DECLARE_MEM_METHODS(pre_cnf_node)
	
	node_kind_t 	ki;
	long			id;
	pre_node_sz_t	pre_sz;
	pre_cnf_node* 	opp_nod;
	
	pre_sornapse	srt_nd;

	void* 			loaded;

	pre_cnf_node(){
		ki = nd_invalid;
		id = 0;
		pre_sz = 0;
		opp_nod = mc_null;

		bj_init_pre_sornapse(srt_nd);	// for parallella to work

		loaded = mc_null;
	}

	~pre_cnf_node(){}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return bj_cell_id(pre_cnf_node);
	}
};

/*! \class pre_cnf_net
\brief Class per workeru cnf section to load in workeru. 

*/
class mc_aligned pre_cnf_net : public agent_grp {
public:
	MCK_DECLARE_MEM_METHODS(pre_cnf_net)

	num_nod_t tot_pre_neus;
	num_nod_t tot_pre_vars;
	num_nod_t tot_pre_lits;
	num_nod_t tot_pre_rels;

	grip	all_pre_neu;
	grip	all_pre_pos;
	grip	all_pre_neg;
	
	pre_pgroup	test_grp_1;
	pre_pgroup	test_grp_2;

	// sornet info

	num_nod_t tot_pre_sornods;
	grip	all_pre_sornods;

	num_nod_t tot_pre_rnknods;
	grip	all_pre_rnknods;
	
	num_nod_t tot_pre_srt_endnods;
	grip	all_pre_srt_endnods;
	
	num_nod_t tot_pre_rnk_endnods;
	grip	all_pre_rnk_endnods;
	
	num_nod_t tmp_nod_idx;

	pre_cnf_net(){
		tot_pre_neus = 0;
		tot_pre_vars = 0;
		tot_pre_lits = 0;
		tot_pre_rels = 0;

		// sornet info
		tot_pre_sornods = 0;
		tot_pre_rnknods = 0;
		tot_pre_srt_endnods = 0;
		tot_pre_rnk_endnods = 0;
		
		tmp_nod_idx = 0;
	}

	~pre_cnf_net(){}
};

typedef int (*cmp_fn)(const void *, const void *);

/*! \class pre_load_cnf
\brief Class full cnf to load in workerus. 

*/
class mc_aligned pre_load_cnf {
public:
	long MAGIC;

	unsigned long max_nod_sz;

	num_nod_t tot_ccls;
	num_nod_t tot_vars;
	num_nod_t tot_lits;

	pre_cnf_node**	all_ccl;
	pre_cnf_node**	all_pos;
	pre_cnf_node**	all_neg;

	num_nod_t tot_tmp_pre_load_nods;
	pre_cnf_node**	all_tmp_pre_load_nods;

	num_nod_t tot_pre_sorinput_nod;
	pre_sornode**	all_pre_sorinput_nod;

	num_nod_t tot_pre_srt_end_nod;
	pre_endnode**	all_pre_srt_end_nod;
	
	num_nod_t tot_pre_rank_in_nod;
	pre_sornode**	all_pre_rank_in_nod;

	num_nod_t tot_pre_rank_end_nod;
	pre_endnode**	all_pre_rank_end_nod;
	
	long tot_workerus;
	pre_cnf_net*		all_cnf;	//!< Array of \ref pre_cnf_net s to load in each workeru.

	pre_load_cnf(){
		MAGIC = MAGIC_VAL;

		max_nod_sz = 0;

		tot_ccls = 0;
		tot_vars = 0;
		tot_lits = 0;

		all_ccl = mc_null;
		all_pos = mc_null;
		all_neg = mc_null;

		tot_tmp_pre_load_nods = 0;
		all_tmp_pre_load_nods = mc_null;

		tot_pre_sorinput_nod = 0;
		all_pre_sorinput_nod = mc_null;
		
		tot_pre_rank_in_nod = 0;
		all_pre_rank_in_nod = mc_null;
		
		tot_workerus = 0;
		all_cnf = mc_null;
	}

	~pre_load_cnf(){}
};

extern pre_load_cnf* THE_CNF;

extern grip* bj_mgr_all_ava[];
extern mc_alloc_kernel_func_t bj_mgr_all_acq[];
extern mc_alloc_kernel_func_t bj_mgr_all_sep[];

void preload_cnf_init();
void preload_sornet();
void preload_cnf(long sz, const long* arr);

void manageru_print_nods();

void print_cnf();
void print_pre_cnf_nets();

num_nod_t get_bigger_pow2(num_nod_t nn);
num_nod_t get_tot_levels(num_nod_t nn);
void create_sornet(num_nod_t num_to_sort);
void create_ranknet(num_nod_t num_to_sort);
void create_pgroup(pre_pgroup& grp, num_nod_t num_items);
void bj_mgr_init_mem_funcs();

#endif		// PRELOAD_CNF_H


