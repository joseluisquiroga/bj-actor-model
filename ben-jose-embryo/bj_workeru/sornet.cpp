
#include "cell.hh"
#include "sornet.hh"

void 
sorcell_sornet_handler(missive* msv){
	MCK_CALL_HANDLER(sorcell, sornet_handler, msv);
}

void 
endcell_sornet_handler(missive* msv){
	MCK_CALL_HANDLER(endcell, sornet_handler, msv);
}

void 
nervenet_sornet_handler(missive* msv){
	MCK_CALL_HANDLER(nervenet, sornet_handler, msv);
}


void bj_sornet_init_handlers(){
	missive_handler_t* hndlrs = bj_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_sorcell] = sorcell_sornet_handler;
	hndlrs[idx_endcell] = endcell_sornet_handler;
	hndlrs[idx_nervenet] = nervenet_sornet_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
}


bj_cmp_obj_func_t
sornet_get_cmp_func(sorkind_t knd){
	BJ_DBG_SORBINS(return &bj_dbg_cmp_bin_objs);
	BJ_DBG_SORNUMS(return &bj_dbg_cmp_num_objs);
	BJ_DBG_RANK_OUTS(return &bj_cmp_rnk_objs);
	
	bj_cmp_obj_func_t fn = mc_null;
	switch(knd){
		case sorkind_bin:
			mck_abort(1, mc_cstr("Invalid sornet_tok_t 1 (sornet_get_cmp_func)"));
		break;
		case sorkind_sep:
			fn = &bj_cmp_sep_objs;
		break;
		case sorkind_cll:
			fn = &bj_cmp_cell_objs;
		break;
		case sorkind_num:
			BJ_DBG_SRT_RNK(fn = &bj_dbg_cmp_num_objs);
		break;
		case sorkind_rnk:
			fn = &bj_cmp_rnk_objs;
		break;
		default:
			mck_abort(1, mc_cstr("Invalid sornet_tok_t 2 (sornet_get_cmp_func)"));
		break;
	}
	PTD_CK(fn != mc_null);
	return fn;
}

void
sorcell::sornet_handler(missive* msv){
	sornet_transmitter* sn_tmt = (sornet_transmitter*)msv;
	sorkind_t tmt_knd = sn_tmt->d.srt.knd;
	switch(tmt_knd){
		case sorkind_bin:
		case sorkind_sep:
		case sorkind_cll:
		case sorkind_num:
			sornet_srt_handler(sn_tmt);
			break;
		case sorkind_rnk:
			sornet_rnk_handler(sn_tmt);
			break;
		default:
			mck_abort(1, mc_cstr("Invalid sorkind (sorcell::sornet_handler)"));
		break;
	}
}

void
sorcell::calc_color(){
	num_nod_t min_col = mc_min(up_snp.min_col, down_snp.min_col);
	up_snp.min_col = min_col;
	down_snp.min_col = min_col;

	num_nod_t max_col = mc_max(up_snp.max_col, down_snp.max_col);
	up_snp.max_col = max_col;
	down_snp.max_col = max_col;
}

void
sornapse::reset(void* invalid_val){
	min_col = BJ_INVALID_SRT_GRP;
	max_col = BJ_INVALID_SRT_GRP;
	min_grp = BJ_INVALID_SRT_GRP;
	max_grp = BJ_INVALID_SRT_GRP;
	inp = invalid_val;
}
	
void
sorcell::sornet_reset(void* invalid_val){
	up_snp.reset(invalid_val);
	down_snp.reset(invalid_val);
}

void
sornapse::set_snp_fields(sornet_transmitter* sn_tmt, bool set_inp){
	PTD_CK(min_col == BJ_INVALID_SRT_GRP);
	PTD_CK(max_col == BJ_INVALID_SRT_GRP);
	PTD_CK(min_grp == BJ_INVALID_SRT_GRP);
	PTD_CK(max_grp == BJ_INVALID_SRT_GRP);
	
	if(set_inp){ inp = sn_tmt->d.srt.inp; }
	min_col = sn_tmt->d.srt.min_col;
	max_col = sn_tmt->d.srt.max_col;
	min_grp = sn_tmt->d.srt.min_grp;
	max_grp = sn_tmt->d.srt.max_grp;

	PTD_CK(min_col != BJ_INVALID_SRT_GRP);
	PTD_CK(max_col != BJ_INVALID_SRT_GRP);
	PTD_CK(min_grp != BJ_INVALID_SRT_GRP);
	PTD_CK(max_grp != BJ_INVALID_SRT_GRP);
}

void
sorcell::sornet_set_fields(sornet_transmitter* sn_tmt, void* invalid_val){
	num_nod_t tmt_idx = sn_tmt->d.srt.idx;
	PTD_CK(sn_tmt->d.srt.inp != invalid_val);
	
	sornapse& the_snp = get_snp(tmt_idx);
	PTD_CK(the_snp.inp == invalid_val);
	the_snp.set_snp_fields(sn_tmt);
	PTD_CK(bj_sornet_idx_inside(the_snp.idx, the_snp.min_grp, the_snp.max_grp));
	PTD_CK(the_snp.inp != invalid_val);
}

void
bj_send_sornet_tmt(cell* src, sornet_tok_t tok, sorkind_t knd, num_nod_t min_col, num_nod_t max_col, 
				   num_nod_t min_grp, num_nod_t max_grp,
				   void* obj, cell* dst, num_nod_t idx, endcell* ecll, bool dbg_msv)
{
	PTD_CK(src != mc_null);
	PTD_CK_PRT(((knd == sorkind_rnk) || (obj != mc_null) || (tok == bj_tok_sornet_start)), 
			   "KIND=%d \n", knd);
	PTD_CK(knd != sorkind_invalid);

	sornet_transmitter* sn_tmt = bj_sornet_transmitter_acquire();
	sn_tmt->init_sornet_transmitter();
	//sn_tmt->wrk_side = side_left;

	sn_tmt->src = src;
	sn_tmt->dst = dst;
	sn_tmt->tok = tok;
	
	sn_tmt->d.srt.knd = knd;
	sn_tmt->d.srt.idx = idx;
	sn_tmt->d.srt.inp = obj;
	sn_tmt->d.srt.min_col = min_col;
	sn_tmt->d.srt.max_col = max_col;
	sn_tmt->d.srt.min_grp = min_grp;
	sn_tmt->d.srt.max_grp = max_grp;

	if(sn_tmt->dst == mc_null){
		switch(knd){
			case sorkind_sep:
				tok = bj_tok_sornet_sep_out;
				break;
			case sorkind_cll:
				tok = bj_tok_sornet_cll_out;
				break;
			case sorkind_bin:
			case sorkind_num:
				tok = bj_tok_sornet_num_out;
				break;
			case sorkind_rnk:
				tok = bj_tok_sornet_rank_out;
				//PTD_CK(obj == mc_null);
				break;
			default:
				mck_abort(1, mc_cstr("Invalid sorkind (bj_send_sornet_tmt)"));
				break;
		}
		//nervenet* root_net = bj_nervenet->get_nervenet(mc_nn_to_id(0));
		//sn_tmt->dst = root_net;
		
		PTD_CK(ecll != mc_null);
		sn_tmt->dst = ecll;
		sn_tmt->tok = tok;

		bool send_root_dbg_out = false;
		MC_MARK_USED(send_root_dbg_out);
		BJ_DBG_SORBINS(send_root_dbg_out = true);
		BJ_DBG_SORNUMS(send_root_dbg_out = true);
		BJ_DBG_RANK_OUTS(send_root_dbg_out = true);
		BJ_DBG_SRT_RNK(send_root_dbg_out = true);
		//BJ_DBG_SRT_RNK(if(knd == sorkind_rnk){ send_root_dbg_out = true; });
		
		MC_DBG(
			if(send_root_dbg_out){
				nervenet* root_net = bj_nervenet->get_nervenet(mc_nn_to_id(0));
				sornet_transmitter* dbg_sn_tmt = bj_sornet_transmitter_acquire();
				dbg_sn_tmt->init_sornet_transmitter();
				
				dbg_sn_tmt->copy_sornet_transmitter(*sn_tmt);
				dbg_sn_tmt->dst = root_net;
				
				/*PTD_PRT(
					"sending_to_root. knd=%d (%ld,%ld,%ld,%ld,%ld) \n", 
					knd, dbg_sn_tmt->d.srt.idx, dbg_sn_tmt->d.srt.min_col, dbg_sn_tmt->d.srt.max_col, 
					dbg_sn_tmt->d.srt.min_grp, dbg_sn_tmt->d.srt.max_grp
				);*/
				
				dbg_sn_tmt->send();
			}
		);
		PTD_LOG(
			"SORNET_NULL. knd=%d %s (%ld,%ld,%ld,%ld,%ld) \n", 
			knd, bj_tok_to_str(tok),
			sn_tmt->d.srt.idx, sn_tmt->d.srt.min_col, sn_tmt->d.srt.max_col, 
			sn_tmt->d.srt.min_grp, sn_tmt->d.srt.max_grp
		);
	}
	PTD_DBG_CODE(if(dbg_msv){ sn_tmt->flg |= mc_dbg_msv_log_received_flag; });

	sn_tmt->send();
}

void
sornet_transmitter::copy_sornet_transmitter(sornet_transmitter& orig){
	// this func is necessary so that code fits in CODE area for parallella
	//wrk_side = orig.wrk_side;

	src = orig.src;
	dst = orig.dst;
	tok = orig.tok;
	
	d.srt.knd = orig.d.srt.knd;
	d.srt.idx = orig.d.srt.idx;
	d.srt.inp = orig.d.srt.inp;
	d.srt.min_col = orig.d.srt.min_col;
	d.srt.max_col = orig.d.srt.max_col;
	d.srt.min_grp = orig.d.srt.min_grp;
	d.srt.max_grp = orig.d.srt.max_grp;
}

void bj_sornet_kernel_func(){
	nervenet* my_net = bj_nervenet;
	if(my_net->act_left_side.sync_is_ending){
		kernel::stop_sys(bj_tok_sornet_end);
	}
}

void
nervenet::sornet_handler(missive* msv){
	mck_token_t tmt_tok = msv->tok;
	if(bj_tok_sync_to_children == tmt_tok){
		act_left_side.send_sync_to_children(bj_tok_sync_to_children, BJ_SORNET_TIER, tiki_invalid, mc_null);
		return;
	}

	PTD_DBG_CODE(sornet_dbg_tests_handler(msv));
}

void bj_sornet_main() {
	mc_workeru_nn_t nn = kernel::get_workeru_nn();
	
	MCK_CK(sizeof(void*) >= sizeof(num_nod_t));

	bj_sornet_init_handlers();

	PTD_LOG("SORNET___ %d \n", nn);

	kernel* ker = mck_get_kernel();
	ker->user_func = bj_sornet_kernel_func;

	nervenet* my_net = bj_nervenet;
	my_net->init_sync_cycle();

	sornet_transmitter::separate(my_net->tot_wu_sorcells * 2);

	mck_slog2("__dbg2.sornet\n");

	//bj_print_active_cnf(side_left, tiki_invalid, mc_cstr("snt_before"), 3, 0, 
	//	bj_dbg_prt_nd_neu_flag | bj_dbg_prt_nd_pol_flag);

	if(nn == 0){
		mini_bit_arr_t max_cntr = 0;
		BJ_DBG_SORBINS(max_cntr = ((((mini_bit_arr_t)1) << BJ_DBG_TOT_INPUT_SORNODES) + 1));
		BJ_DBG_SORNUMS(max_cntr = BJ_DBG_TOT_NUM_TESTS);
		BJ_DBG_RANK_OUTS(max_cntr = BJ_DBG_TOT_RNK_TESTS);
		BJ_DBG_SRT_RNK(max_cntr = BJ_DBG_TOT_SRT_RNK_TESTS);
		
		my_net->dbg_sornet_max_cntr = max_cntr;
		sorkind_t knd = sorkind_invalid;
		BJ_DBG_SORBINS(knd = sorkind_bin);
		BJ_DBG_SORNUMS(knd = sorkind_num);
		BJ_DBG_RANK_OUTS(knd = sorkind_rnk);
		BJ_DBG_SRT_RNK(knd = sorkind_num);
		bj_send_sornet_tmt(my_net, bj_tok_sornet_start, knd, 0, 0, 0, 0, mc_null, my_net, 0);

		PTD_PRT("dbg_sornet_max_cntr=%d\n", max_cntr);
		PTD_LOG("dbg_sornet_max_cntr=%d\n", max_cntr);
	}
	kernel::run_sys();

	//bj_print_active_cnf(side_left, tiki_invalid, mc_cstr("snt_after"), 3, 0, 
	//	bj_dbg_prt_nd_neu_flag | bj_dbg_prt_nd_pol_flag);

	PTD_PRT("...............................END_SORNET\n");
	mck_slog2("END_SORNET___");
	mck_ilog(nn);
	mck_slog2("_________________________\n");
	mck_sprt2("dbg2.sornet.end\n");

}

int
bj_cmp_rnk_objs(void* obj1, void* obj2){
	num_nod_t v1 = bj_pt_to_num(obj1);
	num_nod_t v2 = bj_pt_to_num(obj2);
	if(v1 < v2){ return -1; }
	if(v1 > v2){ return 1;}
	return 0;
}

bool
sorcell::is_up_direct(void* invalid_val){
	if((up_snp.inp != invalid_val) && 
		! bj_sornet_idx_inside(down_snp.idx, up_snp.min_grp, up_snp.max_grp))
	{
		PTD_CK(down_snp.inp == invalid_val);
		return true;
	}
	return false;
}

bool
sorcell::is_down_direct(void* invalid_val){
	if((down_snp.inp != invalid_val) && 
		! bj_sornet_idx_inside(up_snp.idx, down_snp.min_grp, down_snp.max_grp))
	{
		PTD_CK(up_snp.inp == invalid_val);
		return true;
	}
	return false;
}

void
sorcell::sornet_handle_direct(sornet_transmitter* sn_tmt, void* invalid_val){
	sorkind_t tmt_knd = sn_tmt->d.srt.knd;
	sornet_tok_t tmt_tok = (sornet_tok_t)(sn_tmt->tok);

	sorcell* src = this;
	sorcell* up_dst = up_snp.out;
	sorcell* down_dst = down_snp.out;
	
	bool with_grps = false;
	MC_MARK_USED(with_grps);
	BJ_DBG_GRPS_SORNUMS(with_grps = true);
	
	if(is_up_direct(invalid_val)){
		PTD_CK(with_grps); // debug
		bool is_end = mc_get_flag(edge_flags, bj_rnk_up_end_flag);
		if(up_snp.jump_to_end(tmt_knd, srt_sz, is_end)){
			up_dst = mc_null; 
		}
		
		bj_send_sornet_tmt(src, tmt_tok, tmt_knd, up_snp.min_col, up_snp.max_col, 
						   up_snp.min_grp, up_snp.max_grp, 
							up_snp.inp, up_dst, up_snp.idx, up_snp.axon); // can_be_end
		sornet_reset(invalid_val);
		return;
	}

	if(is_down_direct(invalid_val)){
		PTD_CK(with_grps); // debug
		bool is_end = mc_get_flag(edge_flags, bj_rnk_down_end_flag);
		if(down_snp.jump_to_end(tmt_knd, srt_sz, is_end)){
			down_dst = mc_null; 
		}
		
		bj_send_sornet_tmt(src, tmt_tok, tmt_knd, down_snp.min_col, down_snp.max_col, 
						   down_snp.min_grp, down_snp.max_grp, 
							down_snp.inp, down_dst, down_snp.idx, down_snp.axon); // can_be_end
		sornet_reset(invalid_val);
		return;
	}	
}

void
sorcell::sornet_srt_handler(sornet_transmitter* sn_tmt){
	sorkind_t tmt_knd = sn_tmt->d.srt.knd;
	sornet_tok_t tmt_tok = (sornet_tok_t)(sn_tmt->tok);
	void* invalid_val = BJ_INVALID_SRT_OBJ;
	
	sornet_set_fields(sn_tmt, invalid_val);
	sornet_handle_direct(sn_tmt, invalid_val);
	
	PTD_DBG_CODE(
		bool is_input = mc_get_flag(edge_flags, bj_srt_input_flag);
		if(is_input){
			nervenode* nd = (nervenode*)(sn_tmt->src);
			node_kind_t nd_ki = nd->ki;
			long nd_id = nd->id;
			num_tier_t nd_ti = nd->stabi_num_tier;
			sornapse& the_snp = get_snp(sn_tmt->d.srt.idx);
			PTD_LOG("INPUT_SORCELL_%sRCV_t%ld_%s_%ld (%p) (%ld,%ld,%ld,%ld,%ld) \n", 
				((tmt_knd == sorkind_sep)?("SEP_"):("")), nd_ti,
				node_kind_to_str(nd_ki), nd_id, (void*)nd, 
				the_snp.idx, the_snp.min_col, the_snp.max_col, the_snp.min_grp, the_snp.max_grp
			);
		}
	);

	sorcell* src = this;
	sorcell* up_dst = up_snp.out;
	sorcell* down_dst = down_snp.out;
	
	bj_cmp_obj_func_t fn = sornet_get_cmp_func(tmt_knd);
	PTD_CK(fn != mc_null);

	bool with_grps = false;
	MC_MARK_USED(with_grps);
	BJ_DBG_GRPS_SORNUMS(with_grps = true);
	
	if((up_snp.inp != invalid_val) && (down_snp.inp != invalid_val)){
		PTD_CK(up_snp.min_grp == down_snp.min_grp);
		PTD_CK(up_snp.max_grp == down_snp.max_grp);
		
		if(up_snp.jump_to_srt_end(srt_sz)){
			PTD_CK(with_grps || ((up_dst == mc_null) && (down_dst == mc_null)));
			up_dst = mc_null;
			down_dst = mc_null;
		}
		
		int cv = (*fn)(up_snp.inp, down_snp.inp);
		if(cv < 0){
			bj_send_sornet_tmt(src, tmt_tok, tmt_knd, up_snp.min_col, up_snp.max_col, 
							   up_snp.min_grp, up_snp.max_grp, 
							   up_snp.inp, up_dst, up_snp.idx, up_snp.axon); // can_be_end
			bj_send_sornet_tmt(src, tmt_tok, tmt_knd, down_snp.min_col, down_snp.max_col, 
							   down_snp.min_grp, down_snp.max_grp, 
							   down_snp.inp, down_dst, down_snp.idx, down_snp.axon); // can_be_end
		} else {
			if(cv == 0){
				calc_color();
			}
			bj_send_sornet_tmt(src, tmt_tok, tmt_knd, up_snp.min_col, up_snp.max_col, 
							   up_snp.min_grp, up_snp.max_grp, 
							   up_snp.inp, down_dst, down_snp.idx, down_snp.axon); // can_be_end
			bj_send_sornet_tmt(src, tmt_tok, tmt_knd, down_snp.min_col, down_snp.max_col, 
							   down_snp.min_grp, down_snp.max_grp, 
							   down_snp.inp, up_dst, up_snp.idx, up_snp.axon); // can_be_end
		}

		PTD_LOG(
			"SORCELL_FIRED. knd=%d (%ld,%ld,%ld,%ld,%ld) (%ld,%ld,%ld,%ld,%ld) \n", tmt_knd, 
			up_snp.idx, up_snp.min_col, up_snp.max_col, up_snp.min_grp, up_snp.max_grp,
			down_snp.idx, down_snp.min_col, down_snp.max_col, down_snp.min_grp, down_snp.max_grp
		);

		sornet_reset(invalid_val);
	}
}

void
sorcell::sornet_rnk_handler(sornet_transmitter* sn_tmt){
	sorkind_t tmt_knd = sn_tmt->d.srt.knd;
	sornet_tok_t tmt_tok = (sornet_tok_t)(sn_tmt->tok);
	void* invalid_val = BJ_INVALID_RNK_OBJ;

	sornet_set_fields(sn_tmt, invalid_val);
	sornet_handle_direct(sn_tmt, invalid_val);
	
	sorcell* src = this;
	sorcell* up_dst = up_snp.out;
	sorcell* down_dst = down_snp.out;
	
	if((up_snp.inp != invalid_val) && (down_snp.inp != invalid_val)){
		PTD_CK(up_snp.min_grp == down_snp.min_grp);
		PTD_CK(up_snp.max_grp == down_snp.max_grp);
		
		bool is_up_end = mc_get_flag(edge_flags, bj_rnk_up_end_flag);
		if(up_snp.jump_to_rnk_end(srt_sz, is_up_end)){
			up_dst = mc_null;
		}
		bool is_dw_end = mc_get_flag(edge_flags, bj_rnk_down_end_flag);
		if(down_snp.jump_to_rnk_end(srt_sz, is_dw_end)){
			down_dst = mc_null;
		}
		
		int cv = bj_cmp_rnk_objs(up_snp.inp, down_snp.inp);
		if(cv == 0){
			calc_color();
		}
		
		bj_send_sornet_tmt(src, tmt_tok, tmt_knd, up_snp.min_col, up_snp.max_col, 
						   up_snp.min_grp, up_snp.max_grp, 
						   up_snp.inp, up_dst, up_snp.idx, up_snp.axon); // can_be_end
		bj_send_sornet_tmt(src, tmt_tok, tmt_knd, down_snp.min_col, down_snp.max_col, 
						   down_snp.min_grp, down_snp.max_grp, 
						   down_snp.inp, down_dst, down_snp.idx, down_snp.axon); // can_be_end

		sornet_reset(invalid_val);
		//PTD_LOG("SCELL (%d %d) fired\n", up_snp.idx, down_snp.idx);
	}
}

bool
sornapse::jump_to_end(sorkind_t knd, num_nod_t srt_sz, bool is_end){
	bool rsp = false;
	switch(knd){
		case sorkind_bin:
		case sorkind_sep:
		case sorkind_cll:
		case sorkind_num:
			rsp = jump_to_srt_end(srt_sz);
		break;
		case sorkind_rnk:
			rsp = jump_to_rnk_end(srt_sz, is_end);
		break;
		default:
			mck_abort(1, mc_cstr("Invalid sornet_tok_t (sornapse::jump_to_end)"));
		break;
	}
	return rsp;
}

bool
sornapse::jump_to_srt_end(num_nod_t srt_sz){
	PTD_CK(min_grp >= 0);
	PTD_CK(max_grp >= 0);
	u_num_nod_t mn = min_grp;
	u_num_nod_t mx = max_grp;
	u_num_nod_t bi_xor = mn ^ mx;
	u_num_nod_t num_bits = sizeof(u_num_nod_t) * 8;
	bool got_zero = false;
	num_nod_t aa = 0;
	for(aa = num_bits - 1; aa >= 0; aa--){
		bool is_zero = (bi_xor >> aa);
		if(is_zero){
			PTD_CK(mc_get_bit(&bi_xor, aa));
			got_zero = true;
			break;
		}
		PTD_CK(! mc_get_bit(&bi_xor, aa));
	}
	u_num_nod_t out_sz = 2;
	if(got_zero && (aa >= 0) && (aa < ((num_nod_t)(num_bits - 1)) )){
		out_sz = 0;
		out_sz = mc_set_bit(&out_sz, aa + 1);
	} 
	bool rsp = (((num_nod_t)out_sz) == srt_sz);
	return rsp;
}

bool
sornapse::jump_to_rnk_end(num_nod_t srt_sz, bool is_end){
	if(is_end){
		num_nod_t nxt_jmp_sz = srt_sz * 2;
		PTD_CK(idx >= 0);
		PTD_CK(nxt_jmp_sz > 0);
		PTD_CK(min_grp >= 0);
		PTD_CK(max_grp >= 0);
		PTD_CK(min_grp <= max_grp);
		
		bool up_in = bj_sornet_idx_inside(idx + nxt_jmp_sz, min_grp, max_grp);
		bool down_in = bj_sornet_idx_inside(idx - nxt_jmp_sz, min_grp, max_grp);
		if(! up_in && ! down_in){
			return true;
		}
	}
	return false;
}

void
endcell::sornet_handler(missive* msv){
	BJ_DBG_SORBINS(return);
	BJ_DBG_SORNUMS(return);
	BJ_DBG_RANK_OUTS(return);
	
	sornet_transmitter* sn_tmt = (sornet_transmitter*)msv;
	sorkind_t tmt_knd = sn_tmt->d.srt.knd;
	switch(tmt_knd){
		case sorkind_bin:
		case sorkind_sep:
		case sorkind_cll:
		case sorkind_num:
			sornet_srt_handler(sn_tmt);
			break;
		case sorkind_rnk:
			sornet_rnk_handler(sn_tmt);
			break;
		default:
			mck_abort(1, mc_cstr("Invalid sorkind (endcell::sornet_handler)"));
		break;
	}
	//PTD_LOG("ECLL_%d RCV\n", end_snp.idx);
}

void
endcell::sornet_srt_handler(sornet_transmitter* sn_tmt){
	sornet_tok_t tok = (sornet_tok_t)(sn_tmt->tok);
	
	PTD_CK((tok == bj_tok_sornet_num_out) || (tok == bj_tok_sornet_cll_out) || 
		(tok == bj_tok_sornet_sep_out));
	PTD_CK(sn_tmt->d.srt.idx == end_snp.idx);

	PTD_CK(end_snp.min_col == BJ_INVALID_SRT_GRP);
	PTD_CK(end_snp.max_col == BJ_INVALID_SRT_GRP);
	//PTD_CK(end_snp.min_grp == BJ_INVALID_SRT_GRP);
	PTD_CK(end_snp.max_grp == BJ_INVALID_SRT_GRP);
	
	cell* src = this;
	sornet_tok_t etok = bj_tok_sornet_rank_obj;
	if(tok == bj_tok_sornet_cll_out){
		etok = bj_tok_sornet_rank_cll;
	} else 
	if(tok == bj_tok_sornet_sep_out){
		etok = bj_tok_sornet_rank_sep;
	}

	PTD_CK(end_snp.axon != mc_null);
	bj_send_sornet_tmt(src, etok, sorkind_rnk, BJ_INVALID_SRT_GRP, BJ_INVALID_SRT_GRP, 
					BJ_INVALID_SRT_GRP, BJ_INVALID_SRT_GRP, 
					sn_tmt->d.srt.inp, end_snp.axon, end_snp.idx);
	
	num_nod_t obj_val = sn_tmt->d.srt.min_col;
	
	end_snp.min_grp = obj_val;

	PTD_CK(sizeof(void*) >= sizeof(num_nod_t));
	PTD_CK(obj_val != BJ_INVALID_SRT_GRP);
	void* obj = bj_num_to_pt(obj_val);
	PTD_CK(obj != BJ_INVALID_RNK_OBJ);
	
	num_nod_t aa = end_snp.idx;
	
	PTD_CK(end_snp.out != mc_null);
	bj_send_sornet_tmt(src, bj_tok_sornet_rank_start, 
					sorkind_rnk, aa, aa, sn_tmt->d.srt.min_grp, sn_tmt->d.srt.max_grp, 
					obj, end_snp.out, aa); 
	
}

void
endcell::sornet_rnk_set_inp(void* tmt_inp){
	PTD_CK(tmt_inp != mc_null);
	PTD_CK(end_snp.inp == mc_null);
	end_snp.inp = tmt_inp;
	
	nervenode* nd = (nervenode*)tmt_inp;
	PTD_CK(end_ki == nd_invalid);
	end_ki = nd->ki;
	if(end_ki == nd_neu){
		nxt_neu = (neuron*)nd;
	} else {
		PTD_CK(bj_is_pol(end_ki));
		nxt_pol = (polaron*)nd;
	}
	
	PTD_DBG_CODE(
		long neu_id = (nxt_neu != mc_null)?(nxt_neu->id):(0);
		long pol_id = (nxt_pol != mc_null)?(nxt_pol->id):(0);
	);
	PTD_LOG(
		"ENDCELL_SET_NXT_t%ld_%s_%ld (%p) n%ld (%p) p%ld (%p) %s (%ld,%ld,%ld,%ld,%ld) \n", 
		nd->stabi_num_tier, node_kind_to_str(nd->ki), nd->id, (void*)this, 
		neu_id, (void*)nxt_neu, pol_id, (void*)nxt_pol, bj_tok_to_str(out_tok), 
		end_snp.idx, end_snp.min_col, end_snp.max_col, end_snp.min_grp, end_snp.max_grp
	);
}

void
endcell::sornet_rnk_handler(sornet_transmitter* sn_tmt){
	BJ_DBG_SRT_RNK(return);

	endcell* ecll = this;
	MC_MARK_USED(ecll);
	
	sornet_tok_t tmt_tok = (sornet_tok_t)(sn_tmt->tok);
	
	PTD_CK(sn_tmt->d.srt.idx == end_snp.idx);
	void* tmt_inp = sn_tmt->d.srt.inp;
	
	switch(tmt_tok){
		case bj_tok_sornet_rank_out:
			end_snp.set_snp_fields(sn_tmt, false);
		break;
		case bj_tok_sornet_rank_sep:
			out_tok = bj_tok_sornet_rank_sep;
			sornet_rnk_set_inp(tmt_inp);
		break;
		case bj_tok_sornet_rank_cll:
			out_tok = bj_tok_sornet_rank_cll;
			sornet_rnk_set_inp(tmt_inp);
		break;
		case bj_tok_sornet_rank_obj:
		{
			PTD_CK(tmt_inp != mc_null);
			PTD_CK(end_snp.inp == mc_null);
			end_snp.inp = tmt_inp;
		}
		break;
		default:
			mck_abort(1, mc_cstr("Invalid sorkind (endcell::sornet_rnk_handler)"));
		break;
	}
	
	if((end_snp.inp != mc_null) && (end_snp.min_col != BJ_INVALID_SRT_GRP)){
		nervenode* nd = mc_null;
		PTD_DBG_CODE(
			node_kind_t nd_ki = nd_invalid;
			long nd_id = 0;
			num_tier_t nd_ti = BJ_INVALID_NUM_TIER;
		);
		MC_MARK_USED(nd);
		cell* src = this;
		cell* dst = (cell*)(end_snp.inp);
		PTD_CK(dst != mc_null);
		
		PTD_CK(end_ki != nd_invalid);
		bool is_rnk_sep = (out_tok == bj_tok_sornet_rank_sep);
		if((out_tok == bj_tok_sornet_rank_cll) || is_rnk_sep){
			PTD_DBG_CODE(
				nd = (nervenode*)dst;
				nd_ki = nd->ki;
				nd_id = nd->id;
				nd_ti = nd->stabi_num_tier;
			);
			PTD_CK(end_ki != nd_invalid);
			nervenode* dst2 = mc_null;
			if(end_ki == nd_neu){
				dst2 = nxt_pol;
			} else {
				dst2 = nxt_neu;
			}
			
			sornet_tok_t rdy_tok = bj_tok_sornet_invalid;
			if(out_tok == bj_tok_sornet_rank_cll){
				rdy_tok = bj_tok_sornet_rank_cll_rdy;
			} else {
				PTD_CK(out_tok == bj_tok_sornet_rank_sep);
				rdy_tok = bj_tok_sornet_rank_sep_rdy;
			}
			
			if(dst2 != mc_null){
				PTD_LOG("ENDCELL_%sRDY_FOR_t%ld_%ld (%p) -> %s_%ld  (%p) tok=%s (%ld,%ld,%ld,%ld,%ld) \n", 
					(is_rnk_sep)?("SEP_"):(""),
					dst2->stabi_num_tier, end_snp.idx, (void*)ecll, 
					node_kind_to_str(dst2->ki), dst2->id, (void*)dst2, bj_tok_to_str(rdy_tok),
					end_snp.idx, end_snp.min_col, end_snp.max_col, end_snp.min_grp, end_snp.max_grp
				);
				bj_send_sornet_tmt(src, rdy_tok, sorkind_rnk, 
								BJ_INVALID_SRT_GRP, BJ_INVALID_SRT_GRP, 
								BJ_INVALID_SRT_GRP, BJ_INVALID_SRT_GRP, 
								mc_null, dst2, 0);
			}
		}
		
		bool dbg_log_msv = false;
		//PTD_DBG_CODE(bool dbg_log_msv = ((nd_ki == nd_neg) && (nd_id == -1)););
		MC_MARK_USED(dbg_log_msv);

		bj_send_sornet_tmt(src, out_tok, sorkind_rnk, end_snp.min_col, end_snp.max_col, 
						end_snp.min_grp, end_snp.max_grp, 
						end_snp.out, dst, end_snp.idx, mc_null, dbg_log_msv);
		
		PTD_LOG(
			"ENDCELL_%sFIRED_t%ld_%ld (%p) -> %s_%ld (%p) tok=%s (%ld,%ld,%ld,%ld,%ld) \n", 
			(is_rnk_sep)?("SEP_"):(""), nd_ti, end_snp.idx, (void*)ecll, 
			node_kind_to_str(nd_ki), nd_id, (void*)nd, bj_tok_to_str(out_tok), 
			end_snp.idx, end_snp.min_col, end_snp.max_col, end_snp.min_grp, end_snp.max_grp
		);
		
		end_snp.reset(mc_null);
		end_ki = nd_invalid;
		out_tok = bj_tok_sornet_invalid;
	}
}

