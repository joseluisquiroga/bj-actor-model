

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

Copyright (C) 2007-2012, 2014-2016. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://github.com/joseluisquiroga/messaging-cells

messaging-cells is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------

preload_sort_net.hh

Declaration of functions to preload the sort net.

--------------------------------------------------------------*/

#ifndef PRELOAD_SORT_NET_H
#define PRELOAD_SORT_NET_H

#include "cell.hh"
#include "sort_net_main.hh"

class pre_sornode;
class pre_sort_net;
class pre_load_snet;

mc_c_decl grip ava_pre_sornode;
mc_c_decl grip ava_pre_sort_net;

enum mgr_srt_hdlr_idx_t : mck_handler_idx_t {
	mgr_idx_invalid = mck_tot_base_cell_classes,
	mgr_idx_pre_sornode,
	mgr_idx_pre_sort_net,
	mgr_idx_last_invalid,
	mgr_idx_total
};

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

	num_nod_t tot_lvs = 0;
	mc_workeru_nn_t* arr_lvs = mc_null;

	num_nod_t curr_nod_id = 0;
};

#define bj_pre_sornode_acquire_arr(num) ((pre_sornode*)(kernel::do_acquire(mgr_idx_pre_sornode, num)))
#define bj_pre_sornode_acquire() bj_pre_sornode_acquire_arr(1)

/*! \class pre_sornode
\brief Class for sornet nodes to load.

*/
class mc_aligned pre_sornode : public agent_grp {
public:
	MCK_DECLARE_MEM_METHODS(pre_sornode)

	num_nod_t	 	nod_id;
	num_nod_t 		level;

	mc_workeru_nn_t	nod_nn;

	num_nod_t 		up_idx;
	connect_kind_t	up_conn;
	pre_sornode* 	out_up;

	num_nod_t 		down_idx;
	connect_kind_t	down_conn;
	pre_sornode* 	out_down;

	void* 			loaded;

	pre_sornode(){
		nod_id = 0;
		level = 0;

		nod_nn = MC_INVALID_WORKERU_NN;

		up_idx = 0;
		up_conn = conn_invalid;
		out_up = mc_null;

		down_idx = 0;
		down_conn = conn_invalid;
		out_down = mc_null;

		loaded = mc_null;
	}

	~pre_sornode(){}

	virtual mc_opt_sz_fn 
	mck_handler_idx_t	get_cell_id(){
		return mgr_idx_pre_sornode;
	}
	
	connect_kind_t	get_conn_kind(num_nod_t conn_idx){
		if(conn_idx == up_idx){ return conn_up; }
		if(conn_idx == down_idx){ return conn_down; }
		PTD_CK(false);
		return conn_invalid;
	}
};

/*! \class pre_sort_net
\brief Class per workeru cnf section to load in workeru. 

*/
class mc_aligned pre_sort_net : public agent_grp {
public:
	MCK_DECLARE_MEM_METHODS(pre_sort_net)

	// sornet info

	num_nod_t tot_pre_sornods;
	grip	all_pre_sornods;

	num_nod_t tmp_nod_idx;

	pre_sort_net(){
		// sornet info
		tot_pre_sornods = 0;

		tmp_nod_idx = 0;
	}

	~pre_sort_net(){}
};

typedef int (*cmp_fn)(const void *, const void *);

/*! \class pre_load_snet
\brief Class full cnf to load in workerus. 

*/
class mc_aligned pre_load_snet {
public:
	long MAGIC;

	num_nod_t tot_pre_sorinput_nod;
	pre_sornode**	all_pre_sorinput_nod;

	long tot_workerus;
	pre_sort_net*		all_cnf;	//!< Array of \ref pre_sort_net s to load in each workeru.

	pre_load_snet(){
		MAGIC = MAGIC_VAL;

		tot_pre_sorinput_nod = 0;
		all_pre_sorinput_nod = mc_null;

		tot_workerus = 0;
		all_cnf = mc_null;
	}

	~pre_load_snet(){}
};

extern pre_load_snet* THE_SORT_NET;

void preload_cnf(long sz, const long* arr);

void manageru_print_nods();

void print_cnf();
void print_pre_sort_nets();

num_nod_t
get_bigger_pow2(num_nod_t nn);

num_nod_t
get_tot_levels(num_nod_t nn);

void create_sornet(num_nod_t num_to_sort);

void sort_net_mgr_init_handlers();

#endif		// PRELOAD_SORT_NET_H


