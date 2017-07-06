

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

preload_cnf.hh

Declaration of functions to preload cnfs in dimacs files.

--------------------------------------------------------------*/

#ifndef PRELOAD_CNF_H
#define PRELOAD_CNF_H

#include "cell.hh"

class pre_cnf_node;
class pre_cnf;

extern grip ava_pre_cnf_node;

enum node_kind_t : uint8_t {
	nd_invalid = 0,
	nd_pos,
	nd_neg,
	nd_ccl
};

class mc_aligned pre_cnf_node : public agent_grp {
public:
	MCK_DECLARE_MEM_METHODS(pre_cnf_node)
	
	node_kind_t 	ki;
	long			id;
	long			sz;
	pre_cnf_node* 	opp;

	pre_cnf_node(){
		ki = nd_invalid;
		id = 0;
		sz = 0;
		opp = mc_null;
	}

	~pre_cnf_node(){}

};

class mc_aligned core_cnf {
public:
	long tot_ccls;
	long tot_vars;
	long tot_lits;

	grip	all_ccl;
	grip	all_pos;
	grip	all_neg;

	core_cnf(){
		tot_ccls = 0;
		tot_vars = 0;
		tot_lits = 0;
	}

	~core_cnf(){}
};

typedef int (*cmp_fn)(const void *, const void *);

class mc_aligned pre_cnf {
public:
	long tot_ccls;
	long tot_vars;
	long tot_lits;

	pre_cnf_node**	all_ccl;
	pre_cnf_node**	all_pos;
	pre_cnf_node**	all_neg;

	long tot_nods;
	pre_cnf_node**	all_nod;

	long tot_cores;
	core_cnf*		all_cnf;

	pre_cnf(){
		tot_ccls = 0;
		tot_vars = 0;
		tot_lits = 0;

		all_ccl = mc_null;
		all_pos = mc_null;
		all_neg = mc_null;

		tot_nods = 0;
		all_nod = mc_null;

		tot_cores = 0;
		all_cnf = mc_null;
	}

	~pre_cnf(){}
};

extern pre_cnf* THE_CNF;

void preload_cnf(long sz, const long* arr);
void print_cnf();
void print_nods();


#endif		// PRELOAD_CNF_H


