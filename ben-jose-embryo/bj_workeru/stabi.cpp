
#include "cell.hh"
#include "stabi.hh"

#define BJ_DBG_STABI_MAX_TIER 10

void 
sorcell_stabi_handler(missive* msv){
	MCK_CALL_HANDLER(sorcell, sornet_handler, msv);
}

void 
endcell_stabi_handler(missive* msv){
	MCK_CALL_HANDLER(endcell, sornet_handler, msv);
}

void 
neuron_stabi_handler(missive* msv){
	MCK_CALL_HANDLER(neuron, stabi_handler, msv);
}

void 
polaron_stabi_handler(missive* msv){
	MCK_CALL_HANDLER(polaron, stabi_handler, msv);
}

void 
synapse_stabi_handler(missive* msv){
	MCK_CALL_HANDLER(synapse, stabi_handler, msv);
}

void 
nervenet_stabi_handler(missive* msv){
	MCK_CALL_HANDLER(nervenet, stabi_handler, msv);
}


void bj_stabi_init_handlers(){
	missive_handler_t* hndlrs = bj_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_sorcell] = sorcell_stabi_handler;
	hndlrs[idx_endcell] = endcell_stabi_handler;
	hndlrs[idx_neuron] = neuron_stabi_handler;
	hndlrs[idx_polaron] = polaron_stabi_handler;
	hndlrs[idx_synapse] = synapse_stabi_handler;
	hndlrs[idx_nervenet] = nervenet_stabi_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
}

void
synapse::stabi_handler(missive* msv){
	stabi_transmitter* sb_tmt = (stabi_transmitter*)msv;

	signal_data dat;
	dat.msv = msv;
	dat.snp = this;
	dat.tok = (stabi_tok_t)(sb_tmt->tok);
	dat.sd = side_left;
	
	dat.ti = sb_tmt->d.stb.wrk_tier;
	dat.col_idx = sb_tmt->d.stb.col_idx;

	PTD_CK(dat.sd == side_left);
	PTD_CK(dat.ti != BJ_INVALID_NUM_TIER);

	//if(bj_nervenet->act_left_side.sync_is_ending){ return; }

	nervenode* owr = (nervenode*)(mck_as_loc_pt(owner));
	owr->stabi_recv_transmitter(&dat);
}

int bj_cmp_stabi_color_arrs(num_syn_t sz1, num_nod_t* arr1, num_syn_t sz2, num_nod_t* arr2){
	num_syn_t msz = mc_min(sz1, sz2);
	for(num_syn_t aa = 0; aa < msz; aa++){
		num_nod_t v1 = arr1[aa];
		num_nod_t v2 = arr2[aa];
		if(v1 < v2){ return -1; }
		if(v1 > v2){ return 1; }
	}
	if(sz1 < sz2){ return -1; }
	if(sz1 > sz2){ return 1; }
	return 0;
}

void
nervenode::stabi_recv_transmitter(signal_data* dat){
	//PTD_CK(dat->sd == side_left);
	PTD_CODE(
		//side_state& stt = left_side;
		PTD_CK(stabi_num_tier != BJ_INVALID_NUM_TIER);

		bool ok_3 =  false;
		if(ki == nd_neu){
			ok_3 = ((dat->ti + 1) == stabi_num_tier);
		} else {
			ok_3 = (dat->ti == stabi_num_tier);
		}
		
		PTD_CK_PRT(ok_3, "%s %s %ld %s (%d > %d)", stabi_tok_to_str((stabi_tok_t)dat->tok),
			node_kind_to_str(ki), id, net_side_to_str(side_left), (dat->ti + 1), stabi_num_tier);

		//PTD_CK(dat->ti < BJ_DBG_STABI_MAX_TIER); // debug_purposes_only
		PTD_CK(	(bj_nervenet->sync_parent_id != 0) ||
				bj_nervenet->act_left_side.sync_is_inactive ||
				(dat->ti < BJ_DBG_STABI_MAX_TIER)
		); // debug_purposes_only

		PTD_CK(dat->snp != mc_null);
		nervenode* rem_nd = dat->snp->mate->owner;
		PTD_LOG(" ::RECV_stabi_transmitter_%s_t%d_%s [%s %ld <<- %s %ld] #ti%d \n", 
			net_side_to_str(side_left), dat->ti, stabi_tok_to_str((stabi_tok_t)dat->tok), 
			node_kind_to_str(ki), id, node_kind_to_str(rem_nd->ki), rem_nd->id, 
			stabi_num_tier
		);
	);

	switch(dat->tok){
		case bj_tok_stabi_color:
			stabi_recv_color(dat);
		break;
		case bj_tok_stabi_ping:
			stabi_recv_ping(dat);
		break;
		case bj_tok_stabi_tier_done:
			stabi_recv_tier_done(dat);
		break;
		default:
			mck_abort(1, mc_cstr("nervenode::stabi_recv_transmitter. BAD_STABI_TOK"));
		break;
	}
}

void 
nervenode::stabi_send_snp_tier_done(callee_prms& pms){
	PTD_CK(pms.snp->owner == this);
	pms.snp->stabi_send_transmitter(bj_tok_stabi_tier_done);
}

void
nervenode::stabi_send_all_ti_done(nervenode* nd, num_tier_t dbg_ti){
	side_state& sst = left_side;
	net_side_t sd = side_left;
	
	PTD_CK(stabi_num_tier != BJ_INVALID_NUM_TIER);
	//PTD_CK(&(nd->left_side) == this);

	sst.step_active_set.transmitter_send_all_rec(&nervenode::stabi_send_snp_tier_done, sd);

	PTD_CK((dbg_ti == BJ_INVALID_NUM_TIER) || (nd->ki != nd_neu) || ((dbg_ti + 1) == stabi_num_tier));
	PTD_CK((dbg_ti == BJ_INVALID_NUM_TIER) || (nd->ki == nd_neu) || (dbg_ti == stabi_num_tier));

	PTD_CK(stabi_num_tier != BJ_INVALID_NUM_TIER);
	stabi_num_tier++;

	PTD_LOG("stb_INC_TIER_t%d_%s_%ld\n", stabi_num_tier, node_kind_to_str(nd->ki), nd->id);
}

void
nervenode::stabi_recv_ping(signal_data* dat){
	stabi_insert_color(dat->col_idx);
	
	side_state& stt = left_side;
	stt.step_num_ping++;

	PTD_LOG("stb_INC_PINGS_t%d_%s_%ld #pings=%d of %d \n", dat->ti, node_kind_to_str(ki), id,
			stt.step_num_ping, stt.step_prev_tot_active);
}

void
synapse::stabi_send_transmitter(stabi_tok_t tok, nervenode* src_nd, bool dbg_is_forced){
	//net_side_t sd = side_left;

	if(bj_nervenet->act_left_side.sync_is_ending){ return; }

	num_tier_t ti = owner->stabi_num_tier;

	MC_DBG(node_kind_t the_ki = owner->ki);
	MCK_CK((the_ki == nd_pos) || (the_ki == nd_neg) || (the_ki == nd_neu));
	PTD_CODE(nervenode* rem_nd = mate->owner);
	PTD_LOG("::stabi_send_transmitter_t%d_%s [%s_%ld ->> %s_%s_%ld k%d] \n", 
			ti, stabi_tok_to_str(tok), node_kind_to_str(owner->ki), owner->id, 
		((dbg_is_forced)?("FORCED"):("")), node_kind_to_str(rem_nd->ki), rem_nd->id, 
		mc_id_to_nn(mc_addr_get_id(mate)));

	stabi_transmitter* trm = bj_stabi_transmitter_acquire();
	trm->d.stb.wrk_tier = BJ_INVALID_NUM_TIER;
	trm->d.stb.col_idx = BJ_INVALID_IDX;
	
	trm->src = this;
	trm->dst = mate;
	trm->tok = tok;
	
	trm->d.stb.wrk_tier = ti;
	if(src_nd != mc_null){
		trm->d.stb.col_idx = src_nd->stabi_col.min_idx;
	}
	trm->send();
}

void
nervenode::stabi_recv_tier_done(signal_data* dat){
	net_side_t sd = side_left;
	nervenode* nd = this;
	side_state& stt = left_side;

	stt.step_num_complete++;

	PTD_LOG("STABI_ADD_TIER_END_t%d_%s_%ld_compl(%d_of_%d) \n", dat->ti,
			node_kind_to_str(ki), id, 
			stt.step_num_complete, stt.step_prev_tot_active);

	if(is_tier_complete(dat)){
		bool inc_ti = stabi_recalc_send_pings(dat);

		MC_DBG(dbg_prt_tier_done(dat));

		bool to_dly = false;
		netstate& nstt = bj_nervenet->get_active_netstate(sd);
		grip& all_ti = nstt.all_stabi_tiers;
		num_tier_t the_ti = stabi_num_tier;

		if(inc_ti){
			nstt.get_tier(tiki_stabi, nstt.all_stabi_tiers, dat->ti, ((ki == nd_neu)?(8):(9))); // nw tier
		}
		
		if((ki == nd_neu)){
			PTD_CK(dat->ti == (the_ti - 1));
			to_dly = ((neuron*)nd)->is_to_delay(tiki_propag, dat->sd, the_ti, all_ti);
			if(! to_dly){
				nstt.inc_tier_rcv(nd, tiki_stabi, the_ti, all_ti);
			} else {
				tierdata& lti = get_last_tier(all_ti);
				lti.delay_binder(stt, 3);
			}
		}

		char* ts = bj_dbg_tier_kind_to_str(tiki_stabi);
		MC_MARK_USED(ts);
		PTD_LOG("STABI_TIER_COMPLETE_t%d_%s_%ld %s_%s ((%d > 0) && (%d == %d) && (%d < %d)) %s \n", dat->ti, 
					node_kind_to_str(ki), id, ts, ((to_dly)?("TO_DELAY"):("")), 
			stt.step_prev_tot_active, stt.step_num_ping, stt.step_prev_tot_active, 
			get_last_tier(all_ti).tdt_id, (the_ti - 1),
			((mc_get_flag(stabi_flags, bj_stabi_send_pings_flag))?("SEND_PINGS"):(""))
		);
		
		PTD_CK(get_last_stb_ti() < 15); // DEBUG_PURPOSES_ONLY

		if(! to_dly){
			stabi_start_nxt_tier(dat);
		}
	}
}

bool
nervenode::stabi_is_rdy_to_srt(){
	bool f2 = mc_get_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag);
	//bool f3 = mc_get_flag(stabi_flags, bj_stabi_srt_always_cll_rdy_flag);
	//bool rdy = (f2 || f3);
	return f2;
}

void
nervenode::stabi_send_start_srt(int dbg_caller){
	bool f1 = mc_get_flag(stabi_flags, bj_stabi_ti_done_flag);
	bool f2 = stabi_is_rdy_to_srt();
	
	if(f1 && f2){
		nervenode* src = this;
		cell* srcll = stabi_out;
	
		PTD_COND_LOG(((ki == nd_pos) && (id == 2)), "DBG_pos_2_RESETting_ti_done \n");
		
		mc_reset_flag(stabi_flags, bj_stabi_ti_done_flag);
		mc_reset_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag);

		bj_send_sornet_tmt(src, bj_tok_sornet_num, sorkind_cll, 
				stabi_idx, stabi_idx, stabi_col.min_idx, stabi_col.max_idx, src, srcll, stabi_idx);

		PTD_LOG("stb_SENT_SRT_t%d_%s_%ld (%ld,%ld,%ld,%ld,%ld) %d \n", 
				stabi_tmp_tier, node_kind_to_str(ki), id, 
				stabi_idx, stabi_idx, stabi_idx, stabi_col.min_idx, stabi_col.max_idx, 
				dbg_caller
		);
		
	}
}

void
nervenode::stabi_reset_complete(){
	PTD_CK(! mc_get_flag(stabi_flags, bj_stabi_srt_start_sep_flag));
	PTD_CK(! mc_get_flag(stabi_flags, bj_stabi_srt_sep_rdy_flag));
	PTD_CK(! mc_get_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag));
	PTD_CK(! mc_get_flag(stabi_flags, bj_stabi_ti_done_flag));
	//PTD_CK(stabi_flags == 0);
	
	stabi_tmp_tier = BJ_INVALID_NUM_TIER;
	
	if(bj_is_pol(ki)){
		PTD_CK(stabi_prv_arr_dat == mc_null);
		stabi_arr_sz = 0;
		return;
	}
	PTD_CK(stabi_prv_arr_dat != mc_null);
	
	num_nod_t*  tmp_dat = stabi_prv_arr_dat;
	stabi_prv_arr_dat = stabi_arr_dat;
	stabi_prv_arr_sz = stabi_arr_sz;
	
	stabi_arr_sz = 0;
	stabi_arr_dat = tmp_dat;
}

void
nervenode::dbg_prt_tier_done(signal_data* dat){
	net_side_t sd = side_left;
	MC_DBG(dbg_prt_nod(sd, tiki_stabi, mc_cstr("stb_TIER"), 8, dat->ti));
	PTD_DBG_CODE(dbg_prt_active_synset(sd, tiki_stabi, mc_cstr("ACTi"), dat->ti));
}

void
polaron::dbg_prt_tier_done(signal_data* dat){
	net_side_t sd = side_left;
	polaron* pol = this;
	MC_DBG(pol->dbg_prt_nod(sd, tiki_stabi, mc_cstr("stb_TIER"), 8, dat->ti));
	MC_DBG(opp->dbg_prt_nod(sd, tiki_stabi, mc_cstr("stb_TIER"), 8, dat->ti));
	PTD_DBG_CODE(pol->dbg_prt_active_synset(sd, tiki_stabi, mc_cstr("ACTi"), dat->ti));
	PTD_DBG_CODE(opp->dbg_prt_active_synset(sd, tiki_stabi, mc_cstr("ACTi"), dat->ti));
}

void 
nervenode::stabi_send_snp_color(callee_prms& pms){
	PTD_CK(pms.snp != mc_null);
	PTD_CK(pms.snp->owner == this);
	nervenode* nd = this;

	bool snd_png = mc_get_flag(stabi_flags, bj_stabi_send_pings_flag);
	if(snd_png){
		//PTD_CK(! bj_is_pol(ki));
		pms.snp->stabi_send_transmitter(bj_tok_stabi_ping, nd);
	} else {
		pms.snp->stabi_send_transmitter(bj_tok_stabi_color, nd);
	}
}

void bj_stabi_kernel_func(){
	nervenet* my_net = bj_nervenet;
	my_net->act_left_side.stabi_handle_my_sync();
}

void 
netstate::stabi_handle_my_sync(){
	update_sync_inert(tiki_stabi, true);
	
	if(sync_is_ending){
		SYNC_LOG(" SYNCR_STOPPING_WORKERU_%s \n", net_side_to_str(my_side));
		//mck_get_kernel()->set_idle_exit();
		//kernel::stop_sys(bj_tok_propag_end);
		kernel::stop_sys(bj_tok_stabi_end);
	}
}

void
nervenet::stabi_handler(missive* msv){
	mck_token_t msv_tok = msv->tok;
	
	PTD_CK(msv_tok != bj_tok_sornet_sep_out);
	PTD_CK(msv_tok != bj_tok_sornet_cll_out);
	PTD_CK(msv_tok != bj_tok_sornet_num_out);
	PTD_CK(msv_tok != bj_tok_sornet_rank_out);
	
	if(msv_tok == bj_tok_stabi_start){
		stabi_nervenet_start();
		//kernel::stop_sys(bj_tok_stabi_end); // DEBUG purposes only
		return;
	}
	if(msv_tok == bj_tok_stabi_range_bcast){
		stabi_set_ranges(msv);
		return;
	}
	if(msv_tok == bj_tok_stabi_sep_bcast){
		stabi_start_pol_sep();
		return;
	}
	
	/*if(bj_tok_sync_to_children == tmt_tok){
		act_left_side.send_sync_to_children(bj_tok_sync_to_children, BJ_SORNET_TIER, tiki_invalid, mc_null);
		return;
	}*/ // sornet_handler

	sync_handler(tiki_stabi, msv);
}

void bj_stabi_main() {
	mck_set_sub_module_id(BJ_STABI_SUB_MODULE_STABI);

	mc_workeru_nn_t nn = kernel::get_workeru_nn();
	MC_MARK_USED(nn);

	bj_stabi_init_handlers();

	PTD_LOG("STABI___ %d \n", nn);

	kernel* ker = mck_get_kernel();
	ker->user_func = bj_stabi_kernel_func;

	nervenet* my_net = bj_nervenet;
	my_net->init_sync_cycle();

	mck_slog2("__dbg2.stabi\n");

	bj_print_active_cnf(side_left, tiki_stabi, mc_cstr("stb_before"), 3, 0, 
		bj_dbg_prt_nd_neu_flag | bj_dbg_prt_nd_pol_flag);

	my_net->send(my_net, bj_tok_stabi_start);
	kernel::run_sys();

	bj_print_active_cnf(side_left, tiki_stabi, mc_cstr("stb_after"), 3, 0, 
		bj_dbg_prt_nd_neu_flag | bj_dbg_prt_nd_pol_flag);

	PTD_PRT("...............................END_STABI\n");
	PTD_LOG("END_STABI___%d \n", nn);
}

void
nervenode::stabi_insert_color(num_nod_t col_idx){
	num_nod_t* arr = stabi_arr_dat;
	num_nod_t ii = ((num_nod_t)stabi_arr_sz) - 1;
	PTD_CK((ii + 1) < sz);
	while((ii >= 0) && (arr[ii] > col_idx)){
		arr[ii + 1] = arr[ii];
		ii--;
	}
	PTD_CK((ii + 1) >= 0);
	arr[ii + 1] = col_idx;
	
	stabi_arr_sz++;
	PTD_CK(stabi_arr_sz <= sz);
}

void
nervenode::stabi_recv_color(signal_data* dat){
	stabi_insert_color(dat->col_idx);
}

int bj_cmp_cell_objs(void* obj1, void* obj2){
	nervenode* n1 = (nervenode*)obj1;
	nervenode* n2 = (nervenode*)obj2;
	int c_val = bj_cmp_stabi_color_arrs(n1->stabi_arr_sz, n1->stabi_arr_dat, 
										n2->stabi_arr_sz, n2->stabi_arr_dat);
	return c_val;
}

void
neuron::stabi_handler(missive* msv){
	mck_token_t tok = msv->tok;

	switch(tok){
		case bj_tok_stabi_start:
			stabi_neuron_start();
		break;
		case bj_tok_sornet_rank_sep_rdy:
		break;
		case bj_tok_sornet_rank_cll_rdy:
		{
			PTD_CK_PRT((! mc_get_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag)),
					   "t%ld_%s_%ld", stabi_num_tier, node_kind_to_str(ki), id);
			if(stabi_is_active()){
				PTD_LOG("stb_set_cll_rdy_t%d_%s_%ld (%ld,%ld,%ld) \n", 
						stabi_tmp_tier, node_kind_to_str(ki), id,
						stabi_idx, stabi_col.min_idx, stabi_col.max_idx
				);
				mc_set_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag);
				stabi_send_start_srt(1);
			}
		}
		break;
		case bj_tok_sornet_rank_sep:
			PTD_LOG("RCV_RNK_SEP_ (%p) %s_%ld (%ld,%ld,%ld) \n", (void*)this, node_kind_to_str(ki), id,
				stabi_idx, stabi_col.min_idx, stabi_col.max_idx
			);
			stabi_recv_rnk_sep(msv);
		break;
		case bj_tok_sornet_rank_cll:
			stabi_recv_rnk_cll(msv);
			stabi_fix_pre_rdy(bj_nervenet->active_pols_col);
		break;
		default:
			mck_abort(1, mc_cstr("BAD_STABI_TOK"));
		break;
	}
}

void
polaron::stabi_handler(missive* msv){
	mck_token_t tok = msv->tok;
	polaron* pol = this;

	switch(tok){
		case bj_tok_sornet_rank_sep_rdy:
			PTD_CK(! mc_get_flag(stabi_flags, bj_stabi_srt_sep_rdy_flag));
			mc_set_flag(stabi_flags, bj_stabi_srt_sep_rdy_flag);
			
			stabi_send_start_pol_sep(mc_null);
		break;
		case bj_tok_sornet_rank_cll_rdy:
		{
			PTD_CK(! mc_get_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag));
			if(pol->stabi_is_active() || opp->stabi_is_active()){
				PTD_LOG("stb_set_cll_rdy_t%d_%s_%ld (%ld,%ld,%ld) \n", 
						stabi_tmp_tier, node_kind_to_str(ki), id,
						stabi_idx, stabi_col.min_idx, stabi_col.max_idx
				);
				mc_set_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag);
				
				bool p_rdy = pol->stabi_is_rdy_to_srt();
				bool o_rdy = opp->stabi_is_rdy_to_srt();
				if(p_rdy && o_rdy){
					pol->stabi_send_start_srt(2);
					opp->stabi_send_start_srt(3);
				}
			}
		}
		break;
		case bj_tok_sornet_rank_sep:
			PTD_LOG("RCV_RNK_SEP_ (%p) %s_%ld (%ld,%ld,%ld) \n", (void*)this, node_kind_to_str(ki), id,
				stabi_idx, stabi_col.min_idx, stabi_col.max_idx
			);
			stabi_recv_rnk_sep(msv);
		break;
		case bj_tok_sornet_rank_cll:
			stabi_recv_rnk_cll(msv);
			stabi_fix_pre_rdy(bj_nervenet->active_neus_col);
		break;
		default:
			mck_abort(1, mc_cstr("BAD_STABI_TOK"));
		break;
	}
}

int bj_cmp_sep_objs(void* obj1, void* obj2){
	nervenode* n1 = (nervenode*)obj1;
	nervenode* n2 = (nervenode*)obj2;
	
	bool a1 = n1->stabi_is_active();
	bool a2 = n2->stabi_is_active();
	
	if(a1 && ! a2){ return -1; }
	if(! a1 && a2){ return 1; }
	
	return 0;
}

void
nervenet::stabi_nervenet_start(){
	PTD_LOG("stabi_nervenet_start \n");
	stabi_wu_num_rcv_sep = 0;
	stabi_wu_tot_nod_sep = 0;
	with_all_nervenodes(&all_wu_active_neu, &nervenode::stabi_send_sep, mc_null);
	PTD_LOG("end_stabi_nervenet_start tot_nod_sep=%ld \n", stabi_wu_tot_nod_sep);
}

void
nervenode::stabi_send_sep(void* pms){
	nervenode* src = this;
	cell* srcll = stabi_out;

	bj_send_sornet_tmt(src, bj_tok_sornet_num, sorkind_sep, 
			stabi_idx, stabi_idx, stabi_col.min_idx, stabi_col.max_idx, src, srcll, stabi_idx);
	
	PTD_CK(stabi_col.has_value());
	PTD_CK(stabi_idx >= 0);
	
	PTD_LOG("STABI_SENT_SEP_%s_%ld (%ld,%ld,%ld,%ld,%ld) \n", 
			node_kind_to_str(ki), id, 
			stabi_idx, stabi_idx, stabi_idx, stabi_col.min_idx, stabi_col.max_idx
	);
		
	bj_nervenet->stabi_wu_tot_nod_sep++;
}

void
nervenode::stabi_set_color(sornet_transmitter* sn_tmt){
	num_nod_t mn_idx = sn_tmt->d.srt.min_col;
	num_nod_t mx_idx = sn_tmt->d.srt.max_col;
	
	PTD_CK(bj_sornet_idx_inside(mn_idx, stabi_col.min_idx, stabi_col.max_idx));
	PTD_CK(bj_sornet_idx_inside(mx_idx, stabi_col.min_idx, stabi_col.max_idx));
	PTD_CK(bj_sornet_idx_inside(stabi_idx, stabi_col.min_idx, stabi_col.max_idx));
	
	stabi_col.min_idx = mn_idx;
	stabi_col.max_idx = mx_idx;
	stabi_idx = sn_tmt->d.srt.idx;
	stabi_out = (sorcell*)(sn_tmt->d.srt.inp);
	
	PTD_CK(stabi_col.has_value());
	PTD_CK(bj_sornet_idx_inside(stabi_idx, mn_idx, mx_idx));
	
	PTD_CK(stabi_out != mc_null);
	PTD_CK((stabi_out->up_snp.idx == stabi_idx) || (stabi_out->down_snp.idx == stabi_idx));
	
	PTD_LOG("STABI_NEW_COL_t%d_%s_%ld idx=%d col=[%d:%d] \n", 
			stabi_tmp_tier, node_kind_to_str(ki), id, stabi_idx, stabi_col.min_idx, stabi_col.max_idx
	);
	
}

void
nervenode::stabi_set_inactive(grip& all_inac){
	nervenode* nod = this;
	let_go();
	all_inac.bind_to_my_left(*nod);	
}

void
neuron::stabi_recv_rnk_sep(missive* msv){
	sornet_transmitter* sn_tmt = (sornet_transmitter*)msv;
	stabi_set_color(sn_tmt);

	bool is_act = stabi_is_active();
	if(! is_act){
		layerdata& ly_dat = bj_nervenet->get_last_layer();
		stabi_set_inactive(ly_dat.all_inactive_neu);
	}
	PTD_CK_PRT(stabi_col.has_value(), "%s id=%ld col=(%ld, %ld)", node_kind_to_str(ki), id, 
		stabi_col.min_idx, stabi_col.max_idx
	);
	if(is_act){
		if(! bj_nervenet->stabi_nxt_active_neus_rng.has_value()){
			bj_nervenet->stabi_nxt_active_neus_rng = stabi_col;
		}
	} else {
		if(! bj_nervenet->stabi_nxt_inactive_neus_rng.has_value()){
			bj_nervenet->stabi_nxt_inactive_neus_rng = stabi_col;
		}
	}
	PTD_LOG("STABI_RCV_SEP_%s_%ld (%ld,%ld,%ld) \n", node_kind_to_str(ki), id,
		stabi_idx, stabi_col.min_idx, stabi_col.max_idx
	);
	bj_nervenet->stabi_inc_rcv_sep(ki);
}

void
polaron::stabi_recv_rnk_sep(missive* msv){
	sornet_transmitter* sn_tmt = (sornet_transmitter*)msv;
	stabi_set_color(sn_tmt);

	PTD_CK(! mc_get_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag));
	mc_set_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag);
	
	bool is_act = stabi_is_active();
	if(! is_act){
		layerdata& ly_dat = bj_nervenet->get_last_layer();
		stabi_set_inactive(ly_dat.all_inactive_pol);
	}
	PTD_CK_PRT(stabi_col.has_value(), "%s id=%ld col=(%ld, %ld)", node_kind_to_str(ki), id, 
		stabi_col.min_idx, stabi_col.max_idx
	);
	if(is_act){
		if(! bj_nervenet->stabi_nxt_active_pols_rng.has_value()){
			bj_nervenet->stabi_nxt_active_pols_rng = stabi_col;
		}
	} else {
		if(! bj_nervenet->stabi_nxt_inactive_pols_rng.has_value()){
			bj_nervenet->stabi_nxt_inactive_pols_rng = stabi_col;
		}
	}
	PTD_LOG("STABI_RCV_SEP_%s_%ld (%ld,%ld,%ld) \n", node_kind_to_str(ki), id,
		stabi_idx, stabi_col.min_idx, stabi_col.max_idx
	);
	bj_nervenet->stabi_inc_rcv_sep(ki);
}

void
nervenet::stabi_inc_rcv_sep(node_kind_t nod_ki){
	stabi_wu_num_rcv_sep++;
	if(stabi_wu_num_rcv_sep == stabi_wu_tot_nod_sep){
		PTD_LOG("STABI_GOT_ALL_SEP_%s_ %ld \n", node_kind_to_str(nod_ki), stabi_wu_num_rcv_sep);
			
		if(nod_ki == nd_neu){
			stabi_wu_num_rcv_sep = 0;
			stabi_wu_tot_nod_sep = 0;
			
			stabi_start_pol_sep();
		} else {
			PTD_CK(bj_is_pol(nod_ki));
			
			stabi_wu_num_rcv_sep = 0;
			stabi_wu_tot_nod_sep = 0;
			
			stabi_begin_subgrouping();
		}
	}
}

void
nervenet::stabi_broadcast_range(rangekind_t knd, sornet_range active, sornet_range inactive){
	sornet_transmitter sn_tmt;
	sn_tmt.tok = bj_tok_stabi_range_bcast;
	sn_tmt.d.srt.knd = (sorkind_t)knd;
	sn_tmt.d.srt.min_col = active.min_idx;
	sn_tmt.d.srt.max_col = active.max_idx;
	sn_tmt.d.srt.min_grp = inactive.min_idx;
	sn_tmt.d.srt.max_grp = inactive.max_idx;
	
	mc_workeru_id_t pnt_id = bj_nervenet->sync_parent_id;
	if(pnt_id != 0){
		nervenet* src = this;
		nervenet* pnt_net = bj_nervenet->get_nervenet(pnt_id);
		
		base_transmitter* tmt = sn_tmt.clone_transmitter();
		tmt->src = src;
		tmt->dst = pnt_net;
		
		tmt->send();
	} 
	send_all_children(sn_tmt);
}

void
nervenet::stabi_broadcast_all_ranges(){
	bool r1 = stabi_nxt_active_neus_rng.has_value();
	bool r2 = stabi_nxt_inactive_neus_rng.has_value();
	bool s1 = mc_get_flag(stabi_wu_flags, bj_stabi_sent_active_neus_rng_flag);
	bool s2 = mc_get_flag(stabi_wu_flags, bj_stabi_sent_inactive_neus_rng_flag);
	
	if((r1 && ! s1) || (r2 && ! s2)){
		if(r1 && ! s1){
			mc_set_flag(stabi_wu_flags, bj_stabi_sent_active_neus_rng_flag);
		}
		if(r2 && ! s2){
			mc_set_flag(stabi_wu_flags, bj_stabi_sent_inactive_neus_rng_flag);
		}
		stabi_broadcast_range(rangekind_neus, stabi_nxt_active_neus_rng, stabi_nxt_inactive_neus_rng);
	}
	
	bool r3 = stabi_nxt_active_pols_rng.has_value();
	bool r4 = stabi_nxt_inactive_pols_rng.has_value();
	bool s3 = mc_get_flag(stabi_wu_flags, bj_stabi_sent_active_pols_rng_flag);
	bool s4 = mc_get_flag(stabi_wu_flags, bj_stabi_sent_inactive_pols_rng_flag);
	if((r3 && ! s3) || (r4 && ! s4)){
		if(r3 && ! s3){
			mc_set_flag(stabi_wu_flags, bj_stabi_sent_active_pols_rng_flag);
		}
		if(r4 && ! s4){
			mc_set_flag(stabi_wu_flags, bj_stabi_sent_inactive_pols_rng_flag);
		}
		stabi_broadcast_range(rangekind_pols, stabi_nxt_active_pols_rng, stabi_nxt_inactive_pols_rng);
	}
}

void
nervenet::stabi_set_ranges(missive* msv){
	sornet_transmitter* sn_tmt = (sornet_transmitter*)msv;
	rangekind_t knd = (rangekind_t)(sn_tmt->d.srt.knd);

	sornet_range active;
	sornet_range inactive;
	active.min_idx = sn_tmt->d.srt.min_col;
	active.max_idx = sn_tmt->d.srt.max_col;
	inactive.min_idx = sn_tmt->d.srt.min_grp;
	inactive.max_idx = sn_tmt->d.srt.max_grp;
	
	bool a_ok = active.has_value();
	bool i_ok = inactive.has_value();
	bool added_some = false;
	
	if(knd == rangekind_neus){
		bool r1 = stabi_nxt_active_neus_rng.has_value();
		if(! r1 && a_ok){
			stabi_nxt_active_neus_rng = active;
			added_some = true;
		}
		bool r2 = stabi_nxt_inactive_neus_rng.has_value();
		if(! r2 && i_ok){
			stabi_nxt_inactive_neus_rng = inactive;
			added_some = true;
		}
	} else {
		PTD_CK(knd == rangekind_pols);
		bool r3 = stabi_nxt_active_pols_rng.has_value();
		if(! r3 && a_ok){
			stabi_nxt_active_pols_rng = active;
			added_some = true;
		}
		bool r4 = stabi_nxt_inactive_pols_rng.has_value();
		if(! r4 && i_ok){
			stabi_nxt_inactive_pols_rng = inactive;
			added_some = true;
		}
	}
	
	if(added_some){
		stabi_begin_subgrouping();
	}
}

void
nervenode::stabi_set_start_sep(void* pm){
	mc_set_flag(stabi_flags, bj_stabi_srt_start_sep_flag);
}

void
nervenode::stabi_send_start_pol_sep(void* pm){
	PTD_CK(bj_is_pol(ki));
	bool f1 = mc_get_flag(stabi_flags, bj_stabi_srt_start_sep_flag);
	bool f2 = mc_get_flag(stabi_flags, bj_stabi_srt_sep_rdy_flag);
	//bool f3 = mc_get_flag(stabi_flags, bj_stabi_srt_always_sep_rdy_flag);
	
	//if(f1 && (f2 || f3)){
	if(f1 && f2){
		mc_reset_flag(stabi_flags, bj_stabi_srt_start_sep_flag);
		mc_reset_flag(stabi_flags, bj_stabi_srt_sep_rdy_flag);
		stabi_send_sep(mc_null);
	}
}

void
neuron::stabi_neuron_start(){
	PTD_CK(left_side.step_active_set.all_grp.is_alone());
	PTD_CK(right_side.step_active_set.all_grp.is_alone());

	left_side.step_active_set.transmitter_send_all_rec(&nervenode::stabi_send_snp_color, side_left);

	stabi_send_all_ti_done(this, BJ_INVALID_NUM_TIER);
	left_side.step_reset_complete();
	stabi_reset_complete();
}

void
nervenet::stabi_broadcast_sep(){
	stabi_transmitter sp_tmt;
	sp_tmt.tok = bj_tok_stabi_sep_bcast;
	
	mc_workeru_id_t pnt_id = bj_nervenet->sync_parent_id;
	if(pnt_id != 0){
		nervenet* src = this;
		nervenet* pnt_net = bj_nervenet->get_nervenet(pnt_id);
		
		base_transmitter* tmt = sp_tmt.clone_transmitter();
		tmt->src = src;
		tmt->dst = pnt_net;
		
		tmt->send();
	} 
	send_all_children(sp_tmt);
}

void
nervenet::stabi_start_pol_sep(){
	bool alrdy_started = mc_get_flag(stabi_wu_flags, bj_stabi_started_pol_sep_flag);
	if(! alrdy_started){
		mc_set_flag(stabi_wu_flags, bj_stabi_started_pol_sep_flag);
		
		stabi_broadcast_sep();
	
		with_all_nervenodes(&all_wu_active_pos, &nervenode::stabi_set_start_sep, mc_null);
		with_all_nervenodes(&all_wu_active_pos, &nervenode::stabi_send_start_pol_sep, mc_null);
		
		with_all_nervenodes(&all_wu_active_neg, &nervenode::stabi_set_start_sep, mc_null);
		with_all_nervenodes(&all_wu_active_neg, &nervenode::stabi_send_start_pol_sep, mc_null);
	}
}

void
nervenet::stabi_begin_subgrouping(){
	stabi_broadcast_all_ranges();
	if(stabi_has_all_ranges()){
		layerdata& ly_dat = get_last_layer();
		
		active_neus_col = stabi_nxt_active_neus_rng;
		ly_dat.inactive_neus_col = stabi_nxt_inactive_neus_rng;
		active_pols_col = stabi_nxt_active_pols_rng;
		ly_dat.inactive_pols_col = stabi_nxt_inactive_pols_rng;
		
		/*if(stabi_has_pre_rdy_quas()){
			stabi_mark_all_pre_rdy(active_neus_col, all_wu_active_pos);
			stabi_mark_all_pre_rdy(active_neus_col, all_wu_active_neg);
		}*/
		if(stabi_has_pre_rdy_neus()){
			stabi_mark_all_pre_rdy(active_pols_col, all_wu_active_neu);
		}
		
		send_all_neus(bj_tok_stabi_start);
		
		PTD_LOG("STABI_ALL_RNG an(%ld,%ld) in(%ld,%ld) ap(%ld,%ld) ip(%ld,%ld) \n", 
			stabi_nxt_active_neus_rng.min_idx, stabi_nxt_active_neus_rng.max_idx, 
			stabi_nxt_inactive_neus_rng.min_idx, stabi_nxt_inactive_neus_rng.max_idx,
			stabi_nxt_active_pols_rng.min_idx, stabi_nxt_active_pols_rng.max_idx,
			stabi_nxt_inactive_pols_rng.min_idx, stabi_nxt_inactive_pols_rng.max_idx
		);
	}
}

void 
nervenode::stabi_set_tier_done(num_tier_t num_ti){
	PTD_COND_LOG(((ki == nd_pos) && (id == 2)), "DBG_pos_2_settting_ti_done \n");
	PTD_CK_PRT((! mc_get_flag(stabi_flags, bj_stabi_ti_done_flag)), "%s_%ld %s \n", 
		node_kind_to_str(ki), id, ((stabi_is_active())?("ACTIVE"):(""))
	);
	PTD_CK(stabi_tmp_tier == BJ_INVALID_NUM_TIER);
	
	mc_set_flag(stabi_flags, bj_stabi_ti_done_flag);
	stabi_tmp_tier = num_ti;
	
	PTD_CK(stabi_tmp_tier != BJ_INVALID_NUM_TIER);
	
	PTD_LOG("stb_set_tier_done_t%d_%s_%ld (%ld,%ld,%ld) \n", 
			stabi_tmp_tier, node_kind_to_str(ki), id,
			stabi_idx, stabi_col.min_idx, stabi_col.max_idx
	);
}

void
nervenode::stabi_start_nxt_tier(signal_data* dat){
	mck_abort(1, mc_cstr("nervenode::stabi_start_nxt_tier"));
}

void
neuron::stabi_start_nxt_tier(signal_data* dat){
	num_tier_t num_ti = dat->ti;
	stabi_set_tier_done(num_ti);
	stabi_send_start_srt(4);
}

void
polaron::stabi_start_nxt_tier(signal_data* dat){
	num_tier_t num_ti = dat->ti;
	polaron* pol = this;
	pol->stabi_set_tier_done(num_ti);
	opp->stabi_set_tier_done(num_ti);

	bool p_rdy = pol->stabi_is_rdy_to_srt();
	bool o_rdy = opp->stabi_is_rdy_to_srt();
	if(p_rdy && o_rdy){
		pol->stabi_send_start_srt(5);
		opp->stabi_send_start_srt(6);
	}
}

void
nervenode::stabi_update_min_max(){
	if(ki == nd_neu){
		return;
	}
	PTD_CK(bj_is_pol(ki));
	polaron* pol = (polaron*)this;
	nervenet* nnt = bj_nervenet;
	sornet_range& rng = nnt->active_pols_col;
	
	PTD_CK(bj_sornet_idx_inside(stabi_idx, rng.min_idx, rng.max_idx));
	
	PTD_CK(rng.min_idx == 0);
	if(stabi_idx == 0){
		nnt->stabi_min_pol = pol;
	}
	PTD_CK(rng.max_idx != BJ_INVALID_SRT_GRP);
	if(stabi_idx == rng.max_idx){
		nnt->stabi_max_pol = pol;
	}
}

void
nervenode::stabi_recv_rnk_cll(missive* msv){
	sornet_transmitter* sn_tmt = (sornet_transmitter*)msv;
	nervenode* nd = this;
	//net_side_t sd = side_left;
	side_state& stt = left_side;
	netstate& nst = bj_nervenet->act_left_side;
	
	stabi_set_color(sn_tmt);
	stabi_update_min_max();
	
	PTD_LOG("stb_TIDONE_t%d_%s_%ld (%ld,%ld,%ld) \n", stabi_tmp_tier, node_kind_to_str(ki), id,
		stabi_idx, stabi_col.min_idx, stabi_col.max_idx
	);
		
	PTD_CK(stt.is_full());

	stt.step_active_set.transmitter_send_all_rec(&nervenode::stabi_send_snp_color, side_left);

	PTD_CK(stabi_tmp_tier != BJ_INVALID_NUM_TIER);
	
	stabi_send_all_ti_done(nd, stabi_tmp_tier);
	stt.step_reset_complete();
	stabi_reset_complete();

	if(ki == nd_neu){
		nst.update_sync_inert(tiki_stabi, true);
	}
}

bool
nervenet::stabi_has_all_ranges(){
	PTD_CK(! stabi_nxt_inactive_neus_rng.equal_to(active_neus_col));
	PTD_CK(! stabi_nxt_inactive_pols_rng.equal_to(active_pols_col));
	
	bool an = stabi_nxt_active_neus_rng.has_value();
	bool in = stabi_nxt_inactive_neus_rng.has_value();
	bool ap = stabi_nxt_active_pols_rng.has_value();
	bool ip = stabi_nxt_inactive_pols_rng.has_value();
	
	bool aan = stabi_nxt_active_neus_rng.equal_to(active_neus_col);
	bool aap = stabi_nxt_active_pols_rng.equal_to(active_pols_col);
	
	bool all_ok = (an && (in || aan) && ap && (ip || aap));
	return all_ok;
}

/*bool
nervenet::stabi_has_pre_rdy_quas(){
	PTD_CK_PRT((active_neus_col.min_idx == 0), "min_idx=%ld", active_neus_col.min_idx);
	PTD_CK(active_pols_col.min_idx == 0);
	bool has_prdy = (active_pols_col.max_idx > active_neus_col.max_idx);
	return has_prdy;
}*/

bool
nervenet::stabi_has_pre_rdy_neus(){
	PTD_CK(active_neus_col.min_idx == 0);
	PTD_CK(active_pols_col.min_idx == 0);
	bool has_prdy = (active_neus_col.max_idx > active_pols_col.max_idx);
	return has_prdy;
}

void
nervenet::stabi_mark_all_pre_rdy(sornet_range& mates_rng, grip& all_act){
	binder * fst, * lst, * wrk;

	binder* pt_all_nod = &(all_act);
	fst = (binder*)(pt_all_nod->bn_right);
	lst = (binder*)mck_as_loc_pt(pt_all_nod);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		nervenode* my_nod = (nervenode*)wrk;
		my_nod->stabi_fix_pre_rdy(mates_rng);
	}
}

void
nervenode::stabi_fix_pre_rdy(sornet_range& mates_rng){
	if(stabi_idx > mates_rng.max_idx){
		PTD_LOG("PRE_RDY_%s_%ld \n", node_kind_to_str(ki), id);
		mc_set_flag(stabi_flags, bj_stabi_srt_cll_rdy_flag);
	}
}

bool
nervenode::stabi_recalc_send_pings(signal_data* dat){
	mck_abort(1, mc_cstr("nervenode::stabi_recalc_send_pings"));
	return false;
}

bool
neuron::stabi_recalc_send_pings(signal_data* dat){
	bool inc1 = ! stabi_recalc_snd_pings_flg(dat);
	return inc1;
}

bool
polaron::stabi_recalc_send_pings(signal_data* dat){
	polaron* pol = this;
	bool inc1 = ! pol->stabi_recalc_snd_pings_flg(dat);
	bool inc2 = ! opp->stabi_recalc_snd_pings_flg(dat);
	bool inc3 = (inc1 || inc2);
	return inc3;
}

bool 
nervenode::stabi_recalc_snd_pings_flg(signal_data* dat){
	mc_reset_flag(stabi_flags, bj_stabi_send_pings_flag);
	bool all_png = left_side.neu_all_ping(tiki_stabi);
	bool snd_pngs = false;
	if(bj_is_pol(ki)){
		PTD_LOG("stb_RECALC_t%d_%s_%ld #pings=%d of %d (%d) \n", dat->ti, node_kind_to_str(ki), id, 
				left_side.step_num_ping, left_side.step_prev_tot_active, all_png
		);
		if(all_png){
			mc_set_flag(stabi_flags, bj_stabi_send_pings_flag);
			snd_pngs = true;
		}
		return snd_pngs;
	}
	
	PTD_CK(stabi_prv_arr_dat != mc_null);	
	int rr = bj_cmp_stabi_color_arrs(stabi_arr_sz, stabi_arr_dat, stabi_prv_arr_sz, stabi_prv_arr_dat);
	bool is_intact = (rr == 0);
	PTD_CK_PRT((! all_png || is_intact), "t%d_%s_%ld prv=%s arr=%s \n", stabi_tmp_tier, 
		node_kind_to_str(ki), id,
		bj_dbg_stabi_col_arr_to_str(stabi_prv_arr_sz, stabi_prv_arr_dat, BJ_DBG_STR_CAP, 
									bj_nervenet->dbg_str1),
		bj_dbg_stabi_col_arr_to_str(stabi_arr_sz, stabi_arr_dat, BJ_DBG_STR_CAP, 
									bj_nervenet->dbg_str2)
	);
	
	if(is_intact){
		mc_set_flag(stabi_flags, bj_stabi_send_pings_flag);
		snd_pngs = true;
	}
	return snd_pngs;
}

