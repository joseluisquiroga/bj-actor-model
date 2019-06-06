
#include "cell.hh"
#include "stabi.hh"

void 
nervenet_mirrow_handler(missive* msv){
	MCK_CALL_HANDLER(nervenet, mirrow_handler, msv);
}

void bj_mirrow_init_handlers(){
	missive_handler_t* hndlrs = bj_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_nervenet] = nervenet_mirrow_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
}

void
nervenet::mirrow_handler(missive* msv){
	PTD_CK(((mirrow_tok_t)msv->tok) == bj_tok_mirrow_start);

	mirrow_nervenet();
	kernel::stop_sys(bj_tok_mirrow_end);
}

void
bj_stabi_reset_all_tiers(grip& dst_grp, grip& src_grp){
	while(! src_grp.is_alone()){
		tierset* tis = (tierset*)(binder*)(src_grp.bn_right);
		dst_grp.move_all_to_my_right(*tis);
		tis->release(111);
	}
}

void
tierset::mirrow_tiset(tierset& src_tis, net_side_t src_sd){
	binder * fst, * lst, * wrk;

	binder* snps = &(src_tis.ti_all);

	fst = (binder*)(snps->bn_right);
	lst = (binder*)mck_as_loc_pt(snps);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		synapse* snp = get_synapse_from_binder(src_sd, wrk);
		MC_MARK_USED(snp);

		if(src_sd == side_left){
			snp->right_handle.let_go();
			ti_all.bind_to_my_left(snp->right_handle);
		} else {
			PTD_CK(src_sd == side_right);
			snp->let_go();
			ti_all.bind_to_my_left(*snp);
		}
	}

	ti_id = src_tis.ti_id;
}

void
nervenet::mirrow_start_all_nods(grip& all_nod, net_side_t sd, sornet_range& mates_rng){
	binder * fst, * lst, * wrk;

	binder* pt_all_nod = &(all_nod);
	fst = (binder*)(pt_all_nod->bn_right);
	lst = (binder*)mck_as_loc_pt(pt_all_nod);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		nervenode* my_nod = (nervenode*)wrk;
		my_nod->mirrow_sides(sd, mates_rng);
	}
}

void 
nervenet::mirrow_nervenet(){
	PTD_LOG("mirrow_nervenet_start \n");

	net_side_t sd = side_left;

	netstate& lst_nst = get_active_netstate(side_left);
	netstate& rgt_nst = get_active_netstate(side_right);
	
	tierdata& lft = get_last_tier(lst_nst.all_propag_tiers);
	tierdata& rgt = get_last_tier(rgt_nst.all_propag_tiers);

	if(rgt.tdt_id > lft.tdt_id){
		sd = side_right;
	}

	//PTD_LOG("mirrow_nervenet_nods %s \n", net_side_to_str(sd));
	PTD_LOG("mirrow_nervenet_nods \n");
	
	stabi_wu_flags = 0;
	
	PTD_CK(active_neus_col.has_value());
	PTD_CK(active_pols_col.has_value());
	
	PTD_CK(! stabi_nxt_active_neus_rng.has_value());
	PTD_CK(! stabi_nxt_inactive_neus_rng.has_value());
	PTD_CK(! stabi_nxt_active_pols_rng.has_value());
	PTD_CK(! stabi_nxt_inactive_pols_rng.has_value());

	stabi_min_pol = mc_null;
	stabi_max_pol = mc_null;
	
	mirrow_start_all_nods(all_wu_active_neu, sd, active_pols_col);
	mirrow_start_all_nods(all_wu_active_pos, sd, active_neus_col);
	mirrow_start_all_nods(all_wu_active_neg, sd, active_neus_col);

	//mck_slog2("end_mirrow_nervenet \n");
}

void bj_mirrow_main() {
	mck_set_sub_module_id(BJ_STABI_SUB_MODULE_MIRROW);

	mc_workeru_nn_t nn = kernel::get_workeru_nn();
	MC_MARK_USED(nn);

	bj_mirrow_init_handlers();

	PTD_LOG("MIRROW___ %d \n", nn);

	kernel* ker = mck_get_kernel();
	ker->user_func = mc_null;

	nervenet* my_net = bj_nervenet;
	my_net->init_sync_cycle();

	//mck_slog2("__dbg1.mirrow\n");

	my_net->send(my_net, bj_tok_mirrow_start);
	kernel::run_sys();

	my_net->act_left_side.init_stabi_tiers();
	
	my_net->inc_layers();

	bj_print_active_cnf(side_left, tiki_propag, mc_cstr("MIRROWED_"), 5, 0);
	bj_print_active_cnf(side_right, tiki_propag, mc_cstr("MIRROWED_"), 5, 0);

	PTD_PRT("...............................END_MIRROW\n");
	PTD_LOG("END_MIRROW___%d___\n", nn);
	mck_sprt2("dbg1.mirrow.end\n");

}

void
netstate::init_stabi_tiers(){
	tierdata* ti_dat = bj_tierdata_acquire();
	tierdata& lti_prop = get_last_tier(all_propag_tiers);

	PTD_CK(lti_prop.got_all_neus());
	PTD_CK(lti_prop.all_delayed.is_alone());

	ti_dat->tdt_id = 0;

	PTD_CK(ti_dat->tdt_flags == 0);
	PTD_CK(ti_dat->all_delayed.is_alone());
	PTD_CK(ti_dat->num_inert_chdn == 0);

	ti_dat->inp_neus = lti_prop.inp_neus - lti_prop.off_neus;
	PTD_CK(ti_dat->inp_neus != BJ_INVALID_NUM_NODE);

	PTD_CK(all_stabi_tiers.is_alone());
	all_stabi_tiers.bind_to_my_left(*ti_dat);
}

void
nervenet::inc_layers(){
	layerdata* ly_dat = bj_layerdata_acquire();
	all_layers.bind_to_my_left(*ly_dat);
}

void
nervenode::mirrow_sides(net_side_t src_sd, sornet_range& mates_rng){
	PTD_LOG("mirrow_nod_start \n");

	nervenode* nd = this;
	MC_MARK_USED(nd);
	EPH_CODE(MCK_CK(! mc_addr_has_id(nd)));

	binder * fst, * lst, * wrk, * src;

	net_side_t dst_sd = side_left;
	if(dst_sd == src_sd){ dst_sd = side_right; }

	side_state& src_st = get_side_state(src_sd);
	side_state& dst_st = get_side_state(dst_sd);

	// mirrow active_sets

	synset& src_set = src_st.step_active_set;
	synset& dst_set = dst_st.step_active_set;

	PTD_CK(src_set.all_grp.is_alone());
	PTD_CK(dst_set.all_grp.is_alone());

	src = &(src_set.all_syn);

	fst = (binder*)(src->bn_right);
	lst = (binder*)mck_as_loc_pt(src);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		synapse* snp = get_synapse_from_binder(src_sd, wrk);

		if(src_sd == side_left){
			snp->right_handle.let_go();
			dst_set.add_right_synapse(snp);
		} else {
			PTD_CK(src_sd == side_right);
			snp->let_go();
			dst_set.add_left_synapse(snp);
		}
	}

	//PTD_LOG("mirrow_nod_act \n");

	// mirrow ti and src

	dst_st.propag_num_tier = src_st.propag_num_tier;
	dst_st.propag_source = src_st.propag_source;

	// mirrow tiers

	grip tmp_ti;

	bj_stabi_reset_all_tiers(tmp_ti, dst_st.propag_tiers);

	src = &(src_st.propag_tiers);

	fst = (binder*)(src->bn_right);
	lst = (binder*)mck_as_loc_pt(src);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		tierset* src_tis = (tierset*)wrk;
		tierset* dst_tis = bj_tierset_acquire();
		
		dst_tis->ti_id = src_tis->ti_id;
		dst_st.propag_tiers.bind_to_my_left(*dst_tis);
		
		dst_tis->mirrow_tiset(*src_tis, src_sd);
	}

	PTD_CK(tmp_ti.is_alone());

	//PTD_LOG("mirrow_nod_tis \n");

	// mirrow others

	dst_st.step_prev_tot_active = src_st.step_prev_tot_active;

	dst_st.step_flags = src_st.step_flags;
	dst_st.step_num_complete = src_st.step_num_complete;
	dst_st.step_num_ping = src_st.step_num_ping;
	
	// init stabi
	
	stabi_arr_sz = 0;
	stabi_num_tier = 0;
	stabi_flags = 0;

	PTD_CK(stabi_col.has_value());
	
	if(stabi_idx > mates_rng.max_idx){
		//mc_set_flag(stabi_flags, bj_stabi_srt_always_sep_rdy_flag);
		mc_set_flag(stabi_flags, bj_stabi_srt_sep_rdy_flag);
	}

	PTD_LOG("MIRROW_ID_ARR_%s_%d_%s %s \n", get_ki_str(), id,
		bj_dbg_stabi_col_arr_to_str(stabi_arr_sz, stabi_arr_dat, BJ_DBG_STR_CAP, bj_nervenet->dbg_str1),
		((stabi_idx > mates_rng.max_idx)?("PRE_RDY"):(""))
	);

	//PTD_LOG("mirrow_nod_end \n");
}

