
#include "cell.hh"

#include "load_cnf.hh"
#include "preload.hh"

void 
nervenode::init_nervenode_with(pre_cnf_node* nod) { 
	pre_load_cnf* pre_cnf = bj_nervenet->shd_full_cnf;
	nervenode* nd = this;
	MC_MARK_USED(nd);
	
	ki = nod->ki; 
	id = nod->id; 
	sz = nod->pre_sz; 
	
	// stabi init
	
	stabi_num_tier = 0;
	stabi_arr_sz = 0;
	stabi_arr_dat = mc_malloc32(num_nod_t, sz);
	
	if(ki == nd_neu){
		stabi_prv_arr_sz = 0;
		stabi_prv_arr_dat = mc_malloc32(num_nod_t, sz);
	}
	
	stabi_col.min_idx = 0;
	if(ki == nd_neu){
		stabi_col.max_idx = pre_cnf->tot_ccls - 1;
	} else {
		long num_vars = pre_cnf->tot_vars;
		stabi_col.max_idx = ((num_vars * 2) - 1);
	}
	
	stabi_idx = nod->srt_nd.idx;

	PTD_CODE(
		pre_sornode* pre_out = (pre_sornode*)mc_manageru_pt_to_workeru_pt(nod->srt_nd.out);
		mc_spin_cond(pre_out->loaded == mc_null);
		stabi_out = (sorcell*)(pre_out->loaded);
		
		pre_endnode* e_nd = pre_cnf->all_pre_rank_end_nod[stabi_idx];
		mc_spin_cond(e_nd->loaded == mc_null);
		
		endcell* the_end = (endcell*)(e_nd->loaded);
		PTD_CK(the_end->end_snp.idx == stabi_idx);
		if(ki == nd_neu){
			the_end->nxt_neu = (neuron*)nd;
		} else {
			PTD_CK(bj_is_pol(ki));
			the_end->nxt_pol = (polaron*)nd;
		}
	);
} 

void 
nervenet_load_handler(missive* msv){
	MCK_CALL_HANDLER(nervenet, load_handler, msv);
}

void 
polaron_load_handler(missive* msv){
	MCK_CALL_HANDLER(polaron, load_handler, msv);
}

void 
synapse_load_handler(missive* msv){
	MCK_CALL_HANDLER(synapse, load_handler, msv);
}

void
nervenet::init_nervenet_with(pre_cnf_net* pre_net){
	mck_slog2("init_nervenet_with_1 \n");
	tot_wu_neus = pre_net->tot_pre_neus;
	tot_wu_vars = pre_net->tot_pre_vars;
	tot_wu_lits = pre_net->tot_pre_lits;
	tot_wu_rels = pre_net->tot_pre_rels;
	
	pre_load_cnf* pre_cnf = bj_nervenet->shd_full_cnf;
	long num_vars = pre_cnf->tot_vars;
	
	active_neus_col.min_idx = 0;
	active_neus_col.max_idx = pre_cnf->tot_ccls - 1;
	
	active_pols_col.min_idx = 0;
	active_pols_col.max_idx = ((num_vars * 2) - 1);
	
	mck_slog2("init_nervenet_with_2 \n");
}

void
side_state::update_prv_tot_active(){
	step_prev_tot_active = step_active_set.tot_syn;
}

void bj_load_init_cnf(){
	nervenet* my_net = bj_nervenet;
	pre_cnf_net* nn_cnf = bj_nervenet->shd_cnf;

	PTD_LOG("bj_load_init_cnf_1 \n");
	MCK_CK(nn_cnf != mc_null);
	
	my_net->init_nervenet_with(nn_cnf);

	PTD_LOG("%ld local tot_vars\n", my_net->tot_wu_vars);

	long num_neus = my_net->tot_wu_neus;
	long num_vars = my_net->tot_wu_vars;
	long num_rels = my_net->tot_wu_rels;

	//PTD_PRT("tot_lits=%ld tot_vars=%ld tot_neus=%ld TOT_RELS=%ld \n", 
	//		tots.tot_lits, tots.tot_vars, tots.tot_neus, tots.tot_rels);

	long sep_msvs = 3 * num_rels;	// almost (lft + rgt)
	long sep_ssts = 2 * num_rels;	// lft + rgt
	long sep_tsts = (num_vars >> 3);
	long sep_snps = 2 * num_rels;	// lft + rgt
	long sep_pols = 2 * num_vars;
	long sep_neus = num_neus;

	if(sep_tsts > my_net->num_sep_tiersets){
		my_net->num_sep_tiersets = sep_tsts;
	}
	
	PTD_LOG("bj_load_init_cnf_3 \n");

	load_transmitter::separate(sep_msvs);
	synset::separate(sep_ssts);
	tierset::separate(-1);
	synapse::separate(sep_snps);
	polaron::separate(sep_pols);
	neuron::separate(sep_neus);

	mck_slog2("bj_load_init_cnf_finished \n");	

}

void bj_load_shd_cnf(){
	nervenet* my_net = bj_nervenet;
	pre_cnf_net* nn_cnf = bj_nervenet->shd_cnf;
	MCK_CK(nn_cnf != mc_null);
	
	binder * fst, * lst, * wrk;

	binder* nn_all_pos = &(nn_cnf->all_pre_pos); // nn_cnf is already workeru_pt so nn_all_pos is workeru_pt
	fst = (binder*)mc_manageru_pt_to_workeru_pt(nn_all_pos->bn_right);
	lst = nn_all_pos;
	for(wrk = fst; wrk != lst; wrk = (binder*)mc_manageru_pt_to_workeru_pt(wrk->bn_right)){
		pre_cnf_node* nod = (pre_cnf_node*)wrk;

		PTD_CK(nod->ki == nd_pos);
		PTD_CK(nod->opp_nod != mc_null);

		pre_cnf_node* opp = (pre_cnf_node*)mc_manageru_pt_to_workeru_pt(nod->opp_nod);
		PTD_CK(opp->ki == nd_neg);
		PTD_CK(nod->id == -(opp->id));
		//pre_cnf_node* opp = nod->opp_nod;

		polaron* pos_nod = bj_polaron_acquire();
		polaron* neg_nod = bj_polaron_acquire();

		pos_nod->init_nervenode_with(nod);
		neg_nod->init_nervenode_with(opp);

		pos_nod->opp = neg_nod;
		neg_nod->opp = pos_nod;

		my_net->all_wu_active_pos.bind_to_my_left(*pos_nod);
		my_net->all_wu_active_neg.bind_to_my_left(*neg_nod);

		//PTD_PRT("[k%d id%ld sz%ld] \n", nod->ki, nod->id, nod->sz);
		//PTD_PRT("[k%d id%ld sz%ld] \n", opp->ki, opp->id, opp->sz);
		//PTD_PRT("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

		PTD_CK(nod->loaded == mc_null);
		PTD_CK(opp->loaded == mc_null);

		nod->loaded = mck_as_glb_pt(pos_nod);
		opp->loaded = mck_as_glb_pt(neg_nod);
	}

	mck_slog2("LOADED_polarons\n");
	
	binder* nn_all_neu = &(nn_cnf->all_pre_neu); // nn_cnf is already workeru_pt so nn_all_neu is workeru_pt
	fst = (binder*)mc_manageru_pt_to_workeru_pt(nn_all_neu->bn_right);
	lst = nn_all_neu;
	for(wrk = fst; wrk != lst; wrk = (binder*)mc_manageru_pt_to_workeru_pt(wrk->bn_right)){
		pre_cnf_node* sh_neu = (pre_cnf_node*)wrk;
		PTD_CK(sh_neu->ki == nd_neu);

		neuron* my_neu = bj_neuron_acquire();
		my_net->all_wu_active_neu.bind_to_my_left(*my_neu);

		neuron* my_glb_neu = (neuron*)mck_as_glb_pt(my_neu);

		my_neu->init_nervenode_with(sh_neu);
		sh_neu->loaded = my_glb_neu;

		mck_slog2("[");

		binder* nn_all_snp = &(sh_neu->all_agts);

		binder * fst2, * lst2, * wrk2;
		fst2 = (binder*)mc_manageru_pt_to_workeru_pt(nn_all_snp->bn_right);
		lst2 = nn_all_snp;
		for(wrk2 = fst2; wrk2 != lst2; wrk2 = (binder*)mc_manageru_pt_to_workeru_pt(wrk2->bn_right)){
			agent_ref* sh_snp = (agent_ref*)wrk2;
			pre_cnf_node* pol = (pre_cnf_node*)mc_manageru_pt_to_workeru_pt(sh_snp->glb_agent_ptr);

			mc_spin_cond(pol->loaded == mc_null);
			polaron* my_pol = (polaron*)(pol->loaded);
			
			MCK_CK(my_pol->id == pol->id);

			synapse* my_snp = bj_synapse_acquire();

			my_snp->owner = my_glb_neu;

			//MCK_CK(my_neu->left_side.step_active_set.parent == mc_null);
			my_neu->left_side.step_active_set.add_left_synapse(my_snp);
			my_neu->left_side.update_prv_tot_active();

			load_transmitter* msv = bj_load_transmitter_acquire();
			PTD_CK(msv->d.prp.wrk_side == side_invalid);
			msv->src = my_snp;
			msv->dst = my_pol;
			msv->tok = bj_tok_load_nw_syn;
			msv->send();

			/*
			long& tot_ld = bj_nervenet->tot_loading;
			tot_ld++;
			if(tot_ld == tots.tot_lits){
				PTD_PRT("SENT LAST------------------------ \n");
			}
			PTD_PRT("SND to pole %d msv from neu %d \n", my_pol->id, my_neu->id);
			*/

			mck_ilog(my_pol->id);
			mck_slog2(" ");
		}
		
		mck_slog2("]__loaded__\n");
	}

	mck_slog2("LOADED_neurons\n");
	
	if(my_net->tot_wu_lits == 0){
		load_transmitter* msv = bj_load_transmitter_acquire();
		PTD_CK(msv->d.prp.wrk_side == side_invalid);
		msv->src = my_net;
		msv->dst = my_net;
		msv->tok = bj_tok_load_no_lits;
		msv->send();
	}
}

void
nervenet::load_handler(missive* msv){
	load_tok_t tok = (load_tok_t)msv->tok;
	MC_MARK_USED(tok);
	MCK_CK(tok == bj_tok_load_no_lits);

	PTD_CODE(mc_workeru_nn_t nn = mck_get_kernel()->get_workeru_nn());
	PTD_LOG("ENDING_CNF_LOAD_nervenet %d --------------- PARENT=%x \n", nn, mc_map_get_parent_workeru_id());
	kernel::stop_sys(bj_tok_load_end);
	PTD_LOG("ENDING_CNF_LOAD_nervenet_2 %d --------------- PARENT=%x \n", nn, mc_map_get_parent_workeru_id());
}

void
polaron::load_handler(missive* msv){
	cell* syn_src = msv->src;
	load_tok_t tok = (load_tok_t)msv->tok;
	MC_MARK_USED(syn_src);
	MC_MARK_USED(tok);
	MCK_CK(tok == bj_tok_load_nw_syn);

	synapse* mt_snp = (synapse*)syn_src;
	synapse* my_snp = bj_synapse_acquire();

	//PTD_PRT("RCV msv pole %d from neu %d \n", id, mt_snp->owner->id);
	//PTD_PRT("polaron::load_handler got snp=%p %s \n", (void*)mt_snp, mt_snp->get_class_name());
	//PTD_CK(syn_src->get_class_name() == synapse_cls_nam);
	PTD_CK(bj_ck_is_synapse(syn_src));

	polaron* my_glb_pol = (polaron*)mck_as_glb_pt(this);

	my_snp->owner = my_glb_pol;
	my_snp->mate = mt_snp;
	MCK_CK(my_snp->mate != mc_null);

	//MCK_CK(left_side.step_active_set.parent == mc_null);
	left_side.step_active_set.add_left_synapse(my_snp);
	left_side.update_prv_tot_active();

	load_transmitter* msv2 = bj_load_transmitter_acquire();
	PTD_CK(msv2->d.prp.wrk_side == side_invalid);
	msv2->src = my_snp;
	msv2->dst = mt_snp;
	msv2->tok = bj_tok_load_nw_syn;
	msv2->send();
}

void
print_childs(){
	mc_load_map_st* mp = mc_map_get_loaded();

	mck_slog2("NUM_WORKERU=");
	mck_ilog(mp->num_workeru);
	mck_slog2("___\n");

	//PTD_PRT("NUM_WORKERU=%d \n", mp->num_workeru);

	if(mp->childs == mc_null){ 
		mck_slog2("NULL_CHILDS\n");
		return; 
	}

	int aa = 0;
	mc_load_map_st* ch_map = (mp->childs)[aa];
	while(ch_map != mc_null){
		//PTD_PRT("CHILD=%d \n", ch_map->num_workeru);
		mck_slog2("CHILD=");
		mck_ilog(ch_map->num_workeru);
		mck_slog2("___\n");

		aa++;
		ch_map = (mp->childs)[aa];
	}
	mck_slog2("END_PRT_CHILDS\n");
}

void
synapse::load_handler(missive* msv){
	cell* syn_src = msv->src;
	load_tok_t tok = (load_tok_t)msv->tok;
	MC_MARK_USED(syn_src);
	MC_MARK_USED(tok);
	PTD_CK_PRT(tok == bj_tok_load_nw_syn, "BAD_TOK= %s (%d) (%s)\n\n", load_tok_to_str(tok), tok, 
			propag_tok_to_str((propag_tok_t)(msv->tok)));

	synapse* mt_snp = (synapse*)syn_src;

	//PTD_PRT("synapse::load_handler got snp=%p %s \n", (void*)mt_snp, mt_snp->get_class_name());
	//PTD_CK(syn_src->get_class_name() == synapse_cls_nam);
	PTD_CK(bj_ck_is_synapse(syn_src));

	mate = mt_snp;
	PTD_CK(mate != mc_null);

	nervenet* my_net = bj_nervenet;
	long& tot_ld = my_net->tot_loaded;
	tot_ld++;
	if(tot_ld == my_net->tot_wu_lits){
		PTD_CODE(mc_workeru_nn_t nn = mck_get_kernel()->get_workeru_nn());
		//print_childs();
		//mck_get_kernel()->set_idle_exit();
		PTD_LOG("ENDING_CNF_LOAD_synapse %d --------------- PARENT=%x \n", nn, mc_map_get_parent_workeru_id());
		kernel::stop_sys(bj_tok_load_end);
		PTD_LOG("ENDING_CNF_LOAD_synapse_2 %d --------------- PARENT=%x \n", nn, mc_map_get_parent_workeru_id());
	}
	//PTD_PRT("RCV5 msv neu %d from pole %d LOADED=(%ld/%ld) \n", 
	//	owner->id, mt_snp->owner->id, tot_ld, my_net->tot_lits);
}

void
tierdata::add_all_inp_from(grip& grp, net_side_t sd){
	binder * fst, * lst, * wrk;

	binder* pt_grp = &(grp);
	fst = (binder*)(pt_grp->bn_right);
	lst = pt_grp;

	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		nervenode* my_nod = (nervenode*)wrk;
		side_state& stt = my_nod->get_side_state(sd);
		num_syn_t tsn = stt.step_active_set.tot_syn;
		//PTD_LOG("ADD_NOD %s %ld tot_syn=%ld\n", node_kind_to_str(my_nod->ki), my_nod->id, tsn);
		if(tsn > 0){
			if(my_nod->ki == nd_neu){
				inp_neus++;
			} 
			//else {
			//	PTD_CK(bj_is_pol(my_nod->ki));
			//	inp_pols++;
			//}
		}
	}
}

void
netstate::init_propag_tiers(nervenet& my_net){
	tierdata* ti_dat = bj_tierdata_acquire();

	ti_dat->tdt_id = 0;
	ti_dat->inp_neus = 0;
	//ti_dat->inp_pols = 0;		// OLD

	ti_dat->add_all_inp_from(my_net.all_wu_active_neu, my_side);
	ti_dat->add_all_inp_from(my_net.all_wu_active_pos, my_side);
	ti_dat->add_all_inp_from(my_net.all_wu_active_neg, my_side);

	//PTD_CK((my_side != side_right) || (ti_dat->inp_neus == 0));	// OLD

	all_propag_tiers.bind_to_my_left(*ti_dat);
}

void bj_test_func_1() mc_external_code_ram;
void bj_test_func_2(binder* pt_1) mc_external_code_ram;

void bj_init_nervenet(){
	kernel* ker = mck_get_kernel();
	mc_workeru_nn_t nn = kernel::get_workeru_nn();

	if(ker->magic_id != MC_MAGIC_ID){
		mck_slog2("BAD_MAGIC\n");
	}

	nervenet* my_net = nervenet::acquire_alloc();
	if(my_net == mc_null){
		mck_abort(1, mc_cstr("CAN NOT INIT GLB WORKERU DATA"));
	}
	ker->user_data = my_net;

	pre_load_cnf* pre_cnf = (pre_load_cnf*)(ker->manageru_load_data);
	MCK_CK(pre_cnf->MAGIC == MAGIC_VAL);
	bj_nervenet->shd_full_cnf = pre_cnf;

	pre_cnf_net* nn_cnf = (pre_cnf_net*)mc_manageru_addr_to_workeru_addr((mc_addr_t)(pre_cnf->all_cnf + nn));
	bj_nervenet->shd_cnf = nn_cnf;	
}

void
bj_load_init_handlers(){
	missive_handler_t* hndlrs = bj_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_nervenet] = nervenet_load_handler;
	hndlrs[idx_polaron] = polaron_load_handler;
	hndlrs[idx_synapse] = synapse_load_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
	
	bj_nervenet->init_mem_funcs();
}

void bj_load_main() {
	bj_init_nervenet();
	bj_load_init_handlers();

	bj_load_init_cnf();
	
	PTD_CODE(
	bj_load_shd_ranknet();
	bj_load_shd_sornet();
	bj_init_ends_srt_sornet();
	bj_init_ends_rnk_sornet();
	mck_slog2("end_of_init_all_sort_cells\n");
	);

	bj_load_shd_cnf();
	mck_slog2("end_of_bj_load_shd_cnf\n");
	
	//MC_DBG(if(kernel::get_workeru_nn() == 0){ bj_test_func_1(); });

	kernel::run_sys();

	nervenet* my_net = bj_nervenet;
	my_net->act_left_side.init_propag_tiers(*my_net);
	my_net->act_right_side.init_propag_tiers(*my_net);

	PTD_CODE(tierdata& dat = get_last_tier(my_net->act_left_side.all_propag_tiers));
	PTD_LOG("\n=========================================================================\n");
	PTD_LOG("ENDED_LOADING. inp_neu=%ld \n", dat.inp_neus);
}

void bj_test_func_1(){
	neuron* neu2 = bj_neuron_acquire();
	binder* sy1_n2 = (&(neu2->left_side.step_active_set.all_syn));
	MCK_CK(! mc_addr_has_id(sy1_n2));
	bj_test_func_2(sy1_n2);

	neuron* gl_neu2 = (neuron*)mck_as_glb_pt(neu2);
	binder* sy2_n2 = (&(gl_neu2->left_side.step_active_set.all_syn));
	//MCK_CK(! mc_addr_has_id(sy2_n2));
	bj_test_func_2(sy2_n2);
}

void bj_test_func_2(binder* pt_1){
	binder *lst = pt_1;
	MC_MARK_USED(lst);

	MCK_CK(mc_addr_is_local(lst));
	MCK_CK(! mc_addr_has_id(lst));
	mck_slog2("TEST_OK\n");
	// mck_as_loc_pt(pt)
}


