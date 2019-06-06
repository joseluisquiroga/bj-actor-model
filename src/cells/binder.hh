

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
/*! \file binder.h

\brief The base class for objects that can form duble linked lists and structures.

\callgraph
\callergraph
------------------------------------------------------------*/

#ifndef BINDER_H
#define BINDER_H

#include "shared.h"

//define BINDER_CK(prm) 	DBG_CK(prm)
#define BINDER_CK(prm) PTD_CK(prm)

//=================================================================
// binder

class binder;

typedef binder* mck_sptr_t;
//define mck_pt_to_binderpt(pt) (pt)
//define mck_binderpt_to_pt(bptr) (bptr)

class mc_aligned binder {
public:
	mck_sptr_t	bn_left;
	mck_sptr_t	bn_right;

	binder(){
		init_binder();
	}

	~binder(){
	}

	mc_opt_sz_fn
	void		init_binder(){
		binder* loc_ths = (binder*)mck_as_loc_pt(this);
		bn_left = loc_ths;
		bn_right = loc_ths;
	}

	mc_opt_sz_fn
	bool	is_alone(){
		binder* loc_ths = (binder*)mck_as_loc_pt(this);
		return ((bn_left == loc_ths) && (bn_right == loc_ths));
	}

	mc_opt_sz_fn //virtual // mem expensive
	void	let_go(){
		binder* loc_ths = (binder*)mck_as_loc_pt(this);
		bn_left->bn_right = bn_right;
		bn_right->bn_left = bn_left;
		bn_left = loc_ths;
		bn_right = loc_ths;
	}

	mc_opt_sz_fn
	bool	ck_binder(){
		PTD_CODE(binder* loc_ths = (binder*)mck_as_loc_pt(this));
		BINDER_CK(bn_right->bn_left == loc_ths);
		BINDER_CK(bn_left->bn_right == loc_ths);
		return true;
	}

	mc_opt_sz_fn
	binder&	get_right(){
		return *bn_right;
	}

	mc_opt_sz_fn
	binder&	get_left(){
		return *bn_left;
	}

	mc_opt_sz_fn
	binder*	get_right_pt(){
		return bn_right;
	}

	mc_opt_sz_fn
	binder*	get_left_pt(){
		return bn_left;
	}

	mc_opt_sz_fn
	void	bind_to_my_right(binder& the_rgt){
		binder* loc_ths = (binder*)mck_as_loc_pt(this);
		BINDER_CK(the_rgt.is_alone());
		BINDER_CK(ck_binder());

		the_rgt.bn_right = bn_right;
		the_rgt.bn_left = loc_ths;
		bn_right->bn_left = &the_rgt;
		bn_right = &the_rgt;

		BINDER_CK(the_rgt.ck_binder());
		BINDER_CK(ck_binder());
	}

	mc_opt_sz_fn
	void	bind_to_my_left(binder& the_lft){
		binder* loc_ths = (binder*)mck_as_loc_pt(this);
		BINDER_CK(the_lft.is_alone());
		BINDER_CK(ck_binder());

		the_lft.bn_left = bn_left;
		the_lft.bn_right = loc_ths;
		bn_left->bn_right = &the_lft;
		bn_left = &the_lft;

		BINDER_CK(the_lft.ck_binder());
		BINDER_CK(ck_binder());
	}

	mc_opt_sz_fn
	bool	is_single(){
		return (! is_alone() && (bn_left == bn_right));
	}

	mc_opt_sz_fn
	bool	is_multiple(){
		return (! is_alone() && ! is_single());
	}

	mc_opt_sz_fn
	void	move_all_to_my_right(binder& grp){
		if(grp.is_alone()){
			return;
		}
		binder* loc_ths = (binder*)mck_as_loc_pt(this);

		BINDER_CK(ck_binder());

		binder* new_rgt = grp.bn_right;
		binder* new_mid = grp.bn_left;
		binder* old_rgt = bn_right;

		grp.bn_right = &grp;
		grp.bn_left = &grp;

		bn_right = new_rgt;
		bn_right->bn_left = loc_ths;

		new_mid->bn_right = old_rgt;
		new_mid->bn_right->bn_left = new_mid;

		BINDER_CK(grp.is_alone());
		BINDER_CK(new_rgt->ck_binder());
		BINDER_CK(new_mid->ck_binder());
		BINDER_CK(old_rgt->ck_binder());
		BINDER_CK(ck_binder());
	}

	mc_opt_sz_fn
	void	move_all_to_my_left(binder& grp){
		if(grp.is_alone()){
			return;
		}
		binder* loc_ths = (binder*)mck_as_loc_pt(this);

		BINDER_CK(ck_binder());

		binder* new_lft = grp.bn_left;
		binder* new_mid = grp.bn_right;
		binder* old_lft = bn_left;

		grp.bn_right = &grp;
		grp.bn_left = &grp;

		bn_left = new_lft;
		bn_left->bn_right = loc_ths;

		new_mid->bn_left = old_lft;
		new_mid->bn_left->bn_right = new_mid;

		BINDER_CK(grp.is_alone());
		BINDER_CK(new_lft->ck_binder());
		BINDER_CK(new_mid->ck_binder());
		BINDER_CK(old_lft->ck_binder());
		BINDER_CK(ck_binder());
	}

	mc_opt_sz_fn
	void 	let_all_go(){
		while(! is_alone()){
			bn_right->let_go();
		}
	}

	mc_size_t	calc_size() mc_external_code_ram;
};

typedef binder grip;

#ifdef __cplusplus
mc_c_decl {
#endif

#ifdef __cplusplus
}
#endif


#endif		// BINDER_H


/*
	manageru_binder() mc_external_code_ram;
	~manageru_binder() mc_external_code_ram;
	void	init_manageru_binder() mc_external_code_ram;
	bool	is_alone() mc_external_code_ram;
	void	let_go() mc_external_code_ram;
	bool	ck_binder() mc_external_code_ram;
	binder&	get_right() mc_external_code_ram;
	binder&	get_left() mc_external_code_ram;
	binder*	get_right_pt() mc_external_code_ram;
	binder*	get_left_pt() mc_external_code_ram;
	void	bind_to_my_right(binder& the_rgt) mc_external_code_ram;
	void	bind_to_my_left(binder& the_lft) mc_external_code_ram;
	bool	is_single() mc_external_code_ram;
	bool	is_multiple() mc_external_code_ram;
	void	move_all_to_my_right(binder& grp) mc_external_code_ram;
	void	move_all_to_my_left(binder& grp) mc_external_code_ram;
	void 	let_all_go() mc_external_code_ram;
*/
