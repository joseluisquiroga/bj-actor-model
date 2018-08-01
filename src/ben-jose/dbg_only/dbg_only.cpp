
#include "cell.hh"

#include "dbg_only.hh"
#include "preload.hh"

void 
nervenet_dbg_only_handler(missive* msv){
	MCK_CALL_HANDLER(nervenet, dbg_only_handler, msv);
}

void
nervenet::dbg_only_handler(missive* msv){
	load_tok_t tok = (load_tok_t)msv->tok;
	MC_MARK_USED(tok);
	MCK_CK(tok == bj_tok_load_no_lits);

	PTD_CODE(mc_workeru_nn_t nn = mck_get_kernel()->get_workeru_nn());
	PTD_LOG("ENDING_DBG_ONLY %d --------------- PARENT=%x \n", nn, mc_map_get_parent_workeru_id());
	kernel::stop_sys(bj_tok_load_end);
}

void
bj_dbg_only_init_handlers(){
	missive_handler_t* hndlrs = bj_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_nervenet] = nervenet_dbg_only_handler;

	kernel::set_handlers(idx_total, hndlrs);
}

void bj_dbg_separate(){
	nervenet* my_net = bj_nervenet;
	pre_cnf_net* nn_cnf = bj_nervenet->shd_cnf;

	my_net->init_nervenet_with(nn_cnf);
	nervenet& tots = *my_net;

	long num_neus = tots.tot_neus;
	long num_vars = tots.tot_vars;
	long num_rels = tots.tot_rels;

	long sep_msvs = 3 * num_rels;	// almost (lft + rgt)
	long sep_ssts = 2 * num_rels;	// lft + rgt
	long sep_tsts = (num_vars >> 3);
	long sep_snps = 2 * num_rels;	// lft + rgt
	long sep_pols = 2 * num_vars;
	long sep_neus = num_neus;

	if(sep_tsts > my_net->num_sep_tiersets){
		my_net->num_sep_tiersets = sep_tsts;
	}

	transmitter::separate(sep_msvs);
	synset::separate(sep_ssts);
	tierset::separate(-1);
	synapse::separate(sep_snps);
	polaron::separate(sep_pols);
	neuron::separate(sep_neus);

	PTD_LOG("Separated transmitters %ld\n", sep_msvs);

	transmitter* msv = transmitter::acquire();
	PTD_CK(msv->wrk_side == side_invalid);
	msv->src = my_net;
	msv->dst = my_net;
	msv->tok = bj_tok_load_no_lits;
	msv->send();
}

void bj_dbg_only_main() {
	kernel* ker = mck_get_kernel();
	mc_workeru_nn_t nn = kernel::get_workeru_nn();

	if(ker->magic_id != MC_MAGIC_ID){
		mck_slog2("BAD_MAGIC\n");
	}
	
	nervenet* my_net = nervenet::acquire_alloc();
	if(my_net == mc_null){
		mck_abort(1, mc_cstr("CAN NOT INIT GLB CORE DATA"));
	}
	ker->user_data = my_net;

	pre_load_cnf* pre_cnf = (pre_load_cnf*)(ker->manageru_load_data);

	pre_cnf_net* nn_cnf = (pre_cnf_net*)mc_manageru_addr_to_workeru_addr((mc_addr_t)(pre_cnf->all_cnf + nn));
	bj_nervenet->shd_cnf = nn_cnf;
	
	bj_dbg_only_init_handlers();

	bj_dbg_separate();

	kernel::run_sys();

	PTD_LOG("\n=========================================================================\n");
}

