

/*------------------------------------------------------------

binder.h

binder class and related.

--------------------------------------------------------------*/

#ifndef BINDER_H
#define BINDER_H

#include "kptr.hh"

//define BINDER_CK(prm) 	DBG_CK(prm)
#define BINDER_CK(prm) 	

//=================================================================
// binder

class binder;

extern const uint8_t* BINDER_BASE_POINTER;

#ifdef MC_IS_EPH_CODE
	//typedef kptr<binder, BINDER_BASE_POINTER> bjk_pt_t;
	typedef uint16_t bjk_pt_t;
	#define bjk_pt_to_binderpt(pt) ((binder*)(mc_addr_t)(pt))
	#define bjk_binderpt_to_pt(bptr) ((intptr_t)(bptr))
#else
	typedef binder* bjk_pt_t;
	#define bjk_pt_to_binderpt(pt) (pt)
	#define bjk_binderpt_to_pt(bptr) (bptr)
#endif

typedef uint8_t bjk_flags_t;

class binder {
public:
	bjk_pt_t	bn_left;
	bjk_pt_t	bn_right;

	binder(){
		init_binder();
	}

	~binder(){
	}

	mc_opt_sz_fn
	void		init_binder(){
		bn_left = bjk_binderpt_to_pt(this);
		bn_right = bjk_binderpt_to_pt(this);
	}

	mc_opt_sz_fn
	bool	is_alone(){
		return ((bn_left == bjk_binderpt_to_pt(this)) && (bn_right == bjk_binderpt_to_pt(this)));
	}

	mc_opt_sz_fn //virtual // mem expensive
	void	let_go(){
		bjk_pt_to_binderpt(bn_left)->bn_right = bn_right;
		bjk_pt_to_binderpt(bn_right)->bn_left = bn_left;
		bn_left = bjk_binderpt_to_pt(this);
		bn_right = bjk_binderpt_to_pt(this);
	}

	mc_opt_sz_fn
	bool	ck_binder(){
		BINDER_CK(bn_right->bn_left == bjk_binderpt_to_pt(this));
		BINDER_CK(bn_left->bn_right == bjk_binderpt_to_pt(this));
		return true;
	}

	mc_opt_sz_fn
	binder&	get_right(){
		return *bjk_pt_to_binderpt(bn_right);
	}

	mc_opt_sz_fn
	binder&	get_left(){
		return *bjk_pt_to_binderpt(bn_left);
	}

	mc_opt_sz_fn
	binder*	get_right_pt(){
		return bjk_pt_to_binderpt(bn_right);
	}

	mc_opt_sz_fn
	binder*	get_left_pt(){
		return bjk_pt_to_binderpt(bn_left);
	}

	mc_opt_sz_fn
	void	bind_to_my_right(binder& the_rgt){
		BINDER_CK(the_rgt.is_alone());
		BINDER_CK(ck_binder());

		the_rgt.bn_right = bn_right;
		the_rgt.bn_left = bjk_binderpt_to_pt(this);
		bjk_pt_to_binderpt(bn_right)->bn_left = bjk_binderpt_to_pt(&the_rgt);
		bn_right = bjk_binderpt_to_pt(&the_rgt);

		BINDER_CK(the_rgt.ck_binder());
		BINDER_CK(ck_binder());
	}

	mc_opt_sz_fn
	void	bind_to_my_left(binder& the_lft){
		BINDER_CK(the_lft.is_alone());
		BINDER_CK(ck_binder());

		the_lft.bn_left = bn_left;
		the_lft.bn_right = bjk_binderpt_to_pt(this);
		bjk_pt_to_binderpt(bn_left)->bn_right = bjk_binderpt_to_pt(&the_lft);
		bn_left = bjk_binderpt_to_pt(&the_lft);

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

		BINDER_CK(ck_binder());

		binder* new_rgt = bjk_pt_to_binderpt(grp.bn_right);
		binder* new_mid = bjk_pt_to_binderpt(grp.bn_left);
		binder* old_rgt = bjk_pt_to_binderpt(bn_right);

		grp.bn_right = bjk_binderpt_to_pt(&grp);
		grp.bn_left = bjk_binderpt_to_pt(&grp);

		bn_right = bjk_binderpt_to_pt(new_rgt);
		bjk_pt_to_binderpt(bn_right)->bn_left = bjk_binderpt_to_pt(this);

		new_mid->bn_right = bjk_binderpt_to_pt(old_rgt);
		bjk_pt_to_binderpt(new_mid->bn_right)->bn_left = bjk_binderpt_to_pt(new_mid);

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

		BINDER_CK(ck_binder());

		binder* new_lft = bjk_pt_to_binderpt(grp.bn_left);
		binder* new_mid = bjk_pt_to_binderpt(grp.bn_right);
		binder* old_lft = bjk_pt_to_binderpt(bn_left);

		grp.bn_right = bjk_binderpt_to_pt(&grp);
		grp.bn_left = bjk_binderpt_to_pt(&grp);

		bn_left = bjk_binderpt_to_pt(new_lft);
		bjk_pt_to_binderpt(bn_left)->bn_right = bjk_binderpt_to_pt(this);

		new_mid->bn_left = bjk_binderpt_to_pt(old_lft);
		bjk_pt_to_binderpt(new_mid->bn_left)->bn_right = bjk_binderpt_to_pt(new_mid);

		BINDER_CK(grp.is_alone());
		BINDER_CK(new_lft->ck_binder());
		BINDER_CK(new_mid->ck_binder());
		BINDER_CK(old_lft->ck_binder());
		BINDER_CK(ck_binder());
	}

	mc_opt_sz_fn
	void 	let_all_go(){
		while(! is_alone()){
			bjk_pt_to_binderpt(bn_right)->let_go();
		}
	}

	static mc_opt_sz_fn binder*
	get_glb_right_pt(binder* bdr);

	static mc_opt_sz_fn binder*
	get_glb_left_pt(binder* bdr);
};

typedef binder grip;



#endif		// BINDER_H


