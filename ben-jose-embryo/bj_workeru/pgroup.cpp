
#include <setjmp.h>
#include "cell.hh"

#include "nervenet.hh"
#include "pgroup.hh"

void pru_lambda2()
{
	std::function<int(int aa, int bb)> ff = [&] (int aa, int bb) -> int { return aa + bb; }; 
}

void 
pgrp_item_pgrp_op_handler(missive* msv){
	//PTD_CK(! msv->is_replying());
	MCK_CALL_HANDLER(pgrp_item, pgrp_op_handler, msv);
}

void 
pgroup_pgrp_op_handler(missive* msv){
	/*if(msv->is_replying()){
		MCK_CALL_REPLY_HANDLER(pgroup, pgrp_op_handler, msv);
		msv->release();
		return;
	}*/
	MCK_CALL_HANDLER(pgroup, pgrp_op_handler, msv);
}

void
bj_pgroup_init_handlers(){
	missive_handler_t* hndlrs = bj_handlers;

	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_pgrp_item] = pgrp_item_pgrp_op_handler;
	hndlrs[idx_pgroup] = pgroup_pgrp_op_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
}

void
pgrp_item::pgrp_op_handler(missive* msv){
	propag_tok_t tok = (propag_tok_t)msv->tok;

	switch(tok){
		default:
			mck_abort(1, mc_cstr("BAD_PGROUP_TOK"));
		break;
	}
}

void
pgroup::pgrp_op_handler(missive* msv){
	propag_tok_t tok = (propag_tok_t)msv->tok;

	switch(tok){
		default:
			mck_abort(1, mc_cstr("BAD_PGROUP_TOK"));
		break;
	}
}


pgrp_item*
pgrp_item::create_get_pnt_of_nxt_add(pgroup* grp){
	pgrp_item* curr = this;
	
	while((curr->pnt != (void*)grp) && (curr->create_get_pnt()->rgt == curr)){
		curr = curr->create_get_pnt();
	}

	if(curr->pnt != (void*)grp){
		curr = curr->create_get_pnt();
		if(curr->rgt == mc_null){
			PTD_CK(curr != mc_null);
			return curr;
		}
		curr = curr->rgt;
	} 
	
	while(curr->lft != mc_null){
		curr = curr->lft;
	}

	PTD_CK(curr != mc_null);
	return curr;
}

void
pgroup::create_add_item(pgrp_item* itm){
	pgroup* grp = this;
	
	if(fst == mc_null){
		fst = itm;
		lst = itm;
		itm->pnt = grp;
		return;
	}
	
	PTD_CK(lst != mc_null);
	
	pgrp_item* pnt_nxt = lst->create_get_pnt_of_nxt_add(grp);
	PTD_CK(pnt_nxt != mc_null);
	
	if(pnt_nxt->lft == mc_null){
		pnt_nxt->lft = itm;
		itm->pnt = pnt_nxt;
		lst = itm;
		return;
	} 
	
	PTD_CK(pnt_nxt->rgt == mc_null);

	pnt_nxt->rgt = itm;
	itm->pnt = pnt_nxt;
	lst = itm;
}

void
bj_create_pgroup(pgroup& grp, num_nod_t num_items){
	
	PTD_CK(grp.fst == mc_null);
	PTD_CK(grp.lst == mc_null);
	
	pgrp_item::separate(num_items);
	
	for(num_nod_t aa = 0; aa < num_items; aa++){
		pgrp_item* itm = bj_pgrp_item_acquire();
		
		grp.create_add_item(itm);
	}
}

void
pgrp_item::dbg_prt_nodes(long dd){
	mck_ilog(dd);
	mck_slog2(" : ");
	
	if(lft != mc_null){
		lft->dbg_prt_nodes(dd + 1);
	}
	if(rgt != mc_null){
		rgt->dbg_prt_nodes(dd + 1);
	}
}
	
void
pgroup::dbg_prt_nodes(){
	//function<void(int aa, int bb)> ff = [&] (int aa, int bb) -> void { aa + bb; }; 

	
	if(fst == mc_null){
		mck_slog2("EMPTY\n");
		return;
	}
	
	mck_lock_log();
	fst->dbg_prt_nodes(1);
	mck_slog2("\n");
	mck_unlock_log();
}

/*
jmp_buf env;
void LabelTest();

void test_ljmp()
{
	if(setjmp(env)){
		LabelTest();
		mck_slog2("This MUST NOT be printed.\n");
	}
	mck_slog2("This MUST be printed.\n");
}

void LabelTest(){
	longjmp(env,1);
}
*/

