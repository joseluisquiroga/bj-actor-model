
#include "cell.hh"
#include "sornet.hh"


//================================================================================
//================================================================================
// BEGINING_OF_DBG_SORT_CODE
#ifdef FULL_DEBUG
void
nervenet::sornet_dbg_tests_handler(missive* msv){
	PTD_CK(kernel::get_workeru_nn() == 0);
	//PTD_LOG("sornet_dbg_tests_handler 1\n");

	sornet_transmitter* sn_tmt = (sornet_transmitter*)msv;
	sorkind_t tmt_knd = sn_tmt->d.srt.knd;
	sornet_tok_t tmt_tok = (sornet_tok_t)(sn_tmt->tok);
	num_nod_t tmt_idx = sn_tmt->d.srt.idx;
	void* tmt_inp = sn_tmt->d.srt.inp;
	num_nod_t tmt_min_col = sn_tmt->d.srt.min_col;
	num_nod_t tmt_max_col = sn_tmt->d.srt.max_col;
	//num_nod_t tmt_min_grp = sn_tmt->d.srt.min_grp;
	//num_nod_t tmt_max_grp = sn_tmt->d.srt.max_grp;

	if(tmt_tok == bj_tok_sornet_start){
		PTD_CK(dbg_all_input_sorcells != mc_null);
		bool has_more = sornet_dbg_send_nxt_step(tmt_knd);
		if(! has_more){
			act_left_side.send_sync_to_children(bj_tok_sync_to_children, 
												BJ_SORNET_TIER, tiki_invalid, mc_null);
		}
	}
	PTD_CK(tmt_tok != bj_tok_sornet_cll_out);
	PTD_CK(tmt_tok != bj_tok_sornet_sep_out);
	
	if(tmt_tok == bj_tok_sornet_num_out){
		PTD_CK(tmt_idx < dbg_tot_input_sorcells);
		PTD_CK(dbg_all_output_sorobjs[tmt_idx] == mc_null);
		dbg_all_output_sorobjs[tmt_idx] = tmt_inp;
		PTD_CK(dbg_all_output_sorcols[tmt_idx] == 0);
		dbg_all_output_sorcols[tmt_idx] = tmt_min_col;

		bool ck_end = true;
		MC_MARK_USED(ck_end);
		BJ_DBG_SRT_RNK(ck_end = false);
		
		if(ck_end){
		
			dbg_tot_rcv_output_sorobjs++;
			
			num_nod_t tot_to_rcv = dbg_tot_input_sorcells;
			BJ_DBG_ONE_GRP_ONLY(tot_to_rcv = dbg_tot_grp_items);
			BJ_DBG_ONE_GRP_ONLY(PTD_CK(dbg_tot_grp_items > 0));
			
				
			if(dbg_tot_rcv_output_sorobjs == tot_to_rcv){
				sornet_dbg_end_step(tmt_knd);
			}
		}
	}
	if(tmt_tok == bj_tok_sornet_rank_out){
		PTD_CK(tmt_idx < dbg_tot_input_rnkcells);

		PTD_CK(dbg_all_output_rnk_min_cols[tmt_idx] == BJ_INVALID_SRT_GRP);
		dbg_all_output_rnk_min_cols[tmt_idx] = tmt_min_col;
		PTD_CK(dbg_all_output_rnk_max_cols[tmt_idx] == BJ_INVALID_SRT_GRP);
		dbg_all_output_rnk_max_cols[tmt_idx] = tmt_max_col;
		
		dbg_tot_rcv_output_sorobjs++;

		num_nod_t tot_to_rcv = dbg_tot_input_rnkcells;
		BJ_DBG_ONE_GRP_ONLY(tot_to_rcv = dbg_tot_grp_items);
		BJ_DBG_ONE_GRP_ONLY(PTD_CK(dbg_tot_grp_items > 0));

		if(dbg_tot_rcv_output_sorobjs == tot_to_rcv){
			sornet_dbg_end_step(tmt_knd);
		}
	}
}

bool
nervenet::sornet_dbg_send_nxt_step(sorkind_t knd){
	PTD_CK(kernel::get_workeru_nn() == 0);
	PTD_CK(dbg_all_input_sorcells != mc_null);
	
	dbg_sornet_curr_cntr++;

	if(dbg_sornet_curr_cntr >= dbg_sornet_max_cntr){
		return false;
	}

	BJ_DBG_SORBINS(sornet_dbg_bin_send_step()); 
	BJ_DBG_SORNUMS(sornet_dbg_num_send_step());
	BJ_DBG_RANK_OUTS(sornet_dbg_rnk_send_step());
	BJ_DBG_SRT_RNK(sornet_dbg_num_send_step());

	PTD_LOG("sornet_dbg_send_nxt_step %lu\n", dbg_sornet_curr_cntr);

	return true;
}

bool
nervenet::sornet_dbg_check_order(sorkind_t knd){
	//PTD_LOG("sornet_check_order_beg\n");
	BJ_DBG_ONE_GRP_ONLY(
		num_nod_t mn_idx = dbg_all_input_srt_min_grps[dbg_idx_grp_test];
		num_nod_t mx_idx = dbg_all_input_srt_max_grps[dbg_idx_grp_test];
	);
	
	bj_cmp_obj_func_t fn = sornet_get_cmp_func(knd);

	num_nod_t* all_min_grps = dbg_all_input_srt_min_grps;
	num_nod_t* all_max_grps = dbg_all_input_srt_max_grps;
	
	bool by_grp = false;
	if(knd == sorkind_num){
		PTD_CK(dbg_tot_input_sorcells > 0);
		by_grp = (all_min_grps[0] != BJ_INVALID_SRT_GRP);
	}
	
	num_nod_t aa = 1;
	bool sor_ok = true;
	BJ_DBG_ONE_GRP_ONLY(aa = mn_idx + 1);
	for(; aa < dbg_tot_input_sorcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		void* o1 = dbg_all_output_sorobjs[aa - 1];
		void* o2 = dbg_all_output_sorobjs[aa];
		PTD_CK(o1 != mc_null);
		PTD_CK(o2 != mc_null);
		if(by_grp){
			num_nod_t min_0 = all_min_grps[aa - 1];
			MC_MARK_USED(min_0);
			num_nod_t max_0 = all_max_grps[aa - 1];
			MC_MARK_USED(max_0);
			num_nod_t min_1 = all_min_grps[aa];
			MC_MARK_USED(min_1);
			num_nod_t max_1 = all_max_grps[aa];
			MC_MARK_USED(max_1);
			PTD_CK(min_0 <= max_0);
			PTD_CK(min_1 <= max_1);
			if(min_0 != min_1){
				PTD_CK(min_0 < min_1);
				PTD_CK(max_0 < max_1);
				PTD_CK((max_0 + 1) == min_1);
				
				PTD_CK(min_0 == all_min_grps[min_0]);
				PTD_CK(max_0 == (aa - 1));
				PTD_CK(min_1 == aa);
				PTD_CK_PRT((max_1 == all_max_grps[max_1]), "%ld, (%ld == %ld)", 
						   aa, max_1, all_max_grps[max_1]);
				continue;
			}
			PTD_CK(max_0 == max_1);
		}
		int cv = (*fn)(o1, o2);
		if(cv > 0){
			sor_ok = false;
			break;
		}
	}
	return sor_ok;
}

void
nervenet::sornet_dbg_end_step(sorkind_t knd){
	BJ_DBG_ONE_GRP_ONLY(printf("ENDING_ONE_GRP_ONLY \n"));
	
	PTD_CK(kernel::get_workeru_nn() == 0);
	
	if(knd != sorkind_rnk){
		bool srt_ok = sornet_dbg_check_order(knd);
		if(! srt_ok){
			mck_abort(__LINE__, MC_ABORT_MSG("SORNET_ERROR\n"));
		}
	}
	
	BJ_DBG_SORBINS(sornet_dbg_bin_end_step()); 
	BJ_DBG_SORNUMS(sornet_dbg_num_end_step());
	BJ_DBG_RANK_OUTS(sornet_dbg_rnk_end_step());
	BJ_DBG_SRT_RNK(
		sornet_dbg_num_end_step();
		sornet_dbg_rnk_end_step();
	);

	mck_slog2("sorted_ok_arr_");
	mck_ilog(dbg_sornet_curr_cntr);
	mck_slog2("\n");
	printf("sorted_ok_arr_%ld \n", dbg_sornet_curr_cntr);
	
	mc_init_arr_vals(dbg_tot_input_sorcells, dbg_all_output_sorobjs, mc_null);
	mc_init_arr_vals(dbg_tot_input_sorcells, dbg_all_output_sorcols, 0);
	dbg_tot_rcv_output_sorobjs = 0;

	nervenet* my_net = this;
	bj_send_sornet_tmt(my_net, bj_tok_sornet_start, knd, 0, 0, 0, 0, mc_null, my_net, 0);
}

num_nod_t
bj_dbg_sornet_get_nw_grp(ini_grps_prms& prms){
	num_nod_t nxt_grp = 0;
#if defined(MC_IS_PTD_CODE) && defined(FULL_DEBUG)
	num_nod_t tot_inp_objs = prms.tot_input_objs;
	bool* is_inp_grp = prms.is_input_grp;
	
	PTD_CK(is_inp_grp != mc_null);

	tak_mak& gg = *(prms.num_gen);
	bool ending = false;
	nxt_grp = (num_nod_t)gg.gen_rand_int32_ie(0, tot_inp_objs);
	while(true){
		bool is_grp = is_inp_grp[nxt_grp];
		if(! is_grp){
			break;
		}
		nxt_grp++;
		if(nxt_grp == tot_inp_objs){
			nxt_grp = 0;
			
			if(ending){ break; }
			ending = true;
		}
	}
	PTD_CK(nxt_grp >= 0);
	PTD_CK(nxt_grp < tot_inp_objs);
	is_inp_grp[nxt_grp] = true;
	
#endif
	return nxt_grp;
}

num_nod_t
bj_dbg_rnk_get_grp_max(num_nod_t min_idx, num_nod_t grp_sz, ini_grps_prms& prms){
	num_nod_t curr_idx = BJ_INVALID_SRT_GRP;
#if defined(MC_IS_PTD_CODE) && defined(FULL_DEBUG)
	num_nod_t tot_inp_objs = prms.tot_input_objs;
	num_nod_t* all_rnk_grps = prms.all_rnk_grps;
	PTD_CK(prms.is_input_grp == mc_null);
	
	PTD_CK(grp_sz > 0);
	PTD_CK(min_idx >= 0);
	PTD_CK(min_idx < tot_inp_objs);
	PTD_CK(tot_inp_objs > 0);
	
	curr_idx = min_idx;
	num_nod_t num_grp = 0;
	num_nod_t curr_grp = all_rnk_grps[curr_idx];
	for(; curr_idx < tot_inp_objs; curr_idx++){
		if(curr_grp != all_rnk_grps[curr_idx]){
			curr_grp = all_rnk_grps[curr_idx];
			num_grp++;
			if(num_grp >= grp_sz){
				break;
			}
		}
	}
	
	if(curr_idx >= tot_inp_objs){
		curr_idx = tot_inp_objs - 1;
	} else {
		curr_idx--;
	}
	PTD_CK(curr_idx != BJ_INVALID_SRT_GRP);
#endif
	return curr_idx;
}

void
bj_dbg_sornet_init_grp_arr(ini_grps_prms& prms, bool just_ones){
#ifndef MC_IS_PTD_CODE
	mck_abort(1, mc_cstr("Undefined func: bj_dbg_sornet_init_grp_arr"));
#endif
#if defined(MC_IS_PTD_CODE) && defined(FULL_DEBUG)
	PTD_CK(kernel::get_workeru_nn() == 0);
	
	num_nod_t tot_inp_objs = prms.tot_input_objs;
	num_nod_t* all_min_grps = prms.all_min_grps;
	num_nod_t* all_max_grps = prms.all_max_grps;
	num_nod_t* all_rnk_grps = prms.all_rnk_grps;
	bool* is_inp_grp = prms.is_input_grp;
	
	PTD_CK(all_min_grps != mc_null);
	
	tak_mak& gg = *(prms.num_gen);
	
	//unsigned long max_val = tot_inp_objs - 1;
	unsigned long max_grp_sz = BJ_DBG_MAX_GRP_SZ;

	prms.tot_grps = 0;
	
	num_nod_t grp_ii = 0;
	num_nod_t grp_sz = 0;
	num_nod_t grp_mn_idx = 0;
	num_nod_t grp_mx_idx = 0;
	
	if(is_inp_grp != mc_null){
		mc_init_arr_vals(tot_inp_objs, is_inp_grp, false);
	}
	
	if(just_ones){
		PTD_CK(tot_inp_objs > 0);
		mc_init_arr_vals(tot_inp_objs, all_min_grps, 1);
		all_min_grps[0] = 0;
		return;
	}

	mc_init_arr_vals(tot_inp_objs, all_min_grps, BJ_INVALID_SRT_GRP);
	if(all_max_grps != mc_null){
		mc_init_arr_vals(tot_inp_objs, all_max_grps, BJ_INVALID_SRT_GRP);
	}
	
	for(num_nod_t aa = 0; aa < tot_inp_objs; aa++){
		if(grp_ii == grp_sz){
			prms.tot_grps++;
			grp_ii = 0;
			grp_sz = (num_nod_t)gg.gen_rand_int32_ie(1, max_grp_sz);
			if(all_max_grps == mc_null){
				grp_mn_idx = bj_dbg_sornet_get_nw_grp(prms);
			} else {
				grp_mn_idx = aa;
				if(all_rnk_grps == mc_null){
					grp_mx_idx = (grp_mn_idx + grp_sz - 1);
					if(grp_mx_idx >= tot_inp_objs){
						grp_mx_idx = (tot_inp_objs - 1);
					}
				} else {
					grp_mx_idx = bj_dbg_rnk_get_grp_max(grp_mn_idx, grp_sz, prms);
					grp_sz = (grp_mx_idx - grp_mn_idx + 1);
				}
				PTD_CK(grp_mn_idx <= grp_mx_idx);
				PTD_CK(grp_mn_idx >= 0);
				PTD_CK(grp_mx_idx >= 0);
				PTD_CK(grp_mn_idx < tot_inp_objs);
				PTD_CK_PRT((grp_mx_idx < tot_inp_objs), "%ld (%ld < %ld)", aa, grp_mx_idx, tot_inp_objs);
			}
		}
		grp_ii++;
		PTD_CK(grp_mn_idx != BJ_INVALID_SRT_GRP);
		all_min_grps[aa] = grp_mn_idx;
		
		if(all_max_grps != mc_null){
			PTD_CK((grp_ii < grp_sz) || (aa == grp_mx_idx));
			all_max_grps[aa] = grp_mx_idx;
		}
	}

#endif
}

#endif
// END_OF_DBG_SORT_CODE
//----------------------------------------------------------------------------------


//================================================================================
//================================================================================
// BEGINING_OF_BIN_SORT_CODE

#ifdef BJ_SORNET_BIN_TEST

#define bj_pt_obj_as_bin(pt_oo) (*((binval_t*)(pt_oo)))

int
bj_dbg_cmp_bin_objs(void* obj1, void* obj2){
	binval_t v1 = bj_pt_obj_as_bin(obj1);
	binval_t v2 = bj_pt_obj_as_bin(obj2);
	if(v1 < v2){ return -1; }
	if(v1 > v2){ return 1;}
	return 0;
}

mini_bit_arr_t
nervenet::sornet_dbg_bin_get_mini_sorted_arr(){
	mini_bit_arr_t min_arr = 0;

	PTD_CK(((mini_bit_arr_t)(sizeof(mini_bit_arr_t) * 8)) > ((mini_bit_arr_t)dbg_tot_input_sorcells));

	num_nod_t aa;
	for(aa = 0; aa < dbg_tot_input_sorcells; aa++){
		void* obj = dbg_all_output_sorobjs[aa];
		binval_t vv = bj_pt_obj_as_bin(obj);
		if(vv == binval_top){
			mc_set_bit(&min_arr, aa);
		}
	}
	return min_arr;
}


#if BJ_DBG_TOT_INPUT_SORNODES < 9
#define BJ_SORTED_PATTERN mc_byte_to_binary_pattern
#define BJ_SORTED_DATA mc_byte_to_binary(pt_arr[0])
#else
#define BJ_SORTED_PATTERN mc_byte_to_binary_pattern mc_byte_to_binary_pattern
#define BJ_SORTED_DATA mc_byte_to_binary(pt_arr[1]), mc_byte_to_binary(pt_arr[0])
#endif

void
nervenet::sornet_dbg_bin_end_step(){
	PTD_CK(kernel::get_workeru_nn() == 0);

	mini_bit_arr_t min_arr = sornet_dbg_bin_get_mini_sorted_arr();
	char* pt_arr = (char*)(&min_arr);
	MC_MARK_USED(pt_arr);
	PTD_PRT("SORTED_OK_ARR_%d_" BJ_SORTED_PATTERN "\n", dbg_sornet_curr_cntr, BJ_SORTED_DATA);
	PTD_LOG("SORTED_OK_ARR_%d_" BJ_SORTED_PATTERN "\n", dbg_sornet_curr_cntr, BJ_SORTED_DATA);
}

void
nervenet::sornet_dbg_bin_send_step(){
	mini_bit_arr_t tmp_num = dbg_sornet_curr_cntr;

	cell* src = this;

	PTD_CK(((mini_bit_arr_t)(sizeof(mini_bit_arr_t) * 8)) > ((mini_bit_arr_t)dbg_tot_input_sorcells));
	PTD_LOG("send_cntr counter=%d tot_inp=%d sizeof(mini_bit_arr_t)=%d \n", 
			tmp_num, dbg_tot_input_sorcells, sizeof(mini_bit_arr_t));

	num_nod_t mn_idx = 0;
	num_nod_t mx_idx = dbg_tot_input_sorcells - 1;
	
	num_nod_t aa;
	for(aa = 0; aa < dbg_tot_input_sorcells; aa++){
		bool vv = mc_get_bit(&tmp_num, aa);
		//PTD_LOG("send_cntr vv=%d \n", vv);

		sorcell* srcll = dbg_all_input_sorcells[aa];
		PTD_CK(srcll != mc_null);

		void* obj = mc_null;
		if(vv){
			obj = mck_as_glb_pt(&dbg_net_top);
		} else {
			obj = mck_as_glb_pt(&dbg_net_bottom);
		}
		PTD_CK(obj != mc_null);
		PTD_CK(srcll != mc_null);
		PTD_CK(src != mc_null);

		bj_send_sornet_tmt(src, bj_tok_sornet_bin, sorkind_bin, aa, aa, mn_idx, mx_idx, obj, srcll, aa);
	}
}

#endif
// END_OF_BIN_SORT_CODE
//----------------------------------------------------------------------------------



//================================================================================
//================================================================================
// BEGINING_OF_RANK_OUTS_CODE

#if (defined(BJ_SORNET_RANK_TEST) || defined(BJ_SORNET_SRT_RNK_TEST))

void
nervenet::sornet_dbg_rnk_send_step(){
	//act_left_side.send_sync_to_children(bj_tok_sync_to_children, 
	//									BJ_SORNET_TIER, tiki_invalid, mc_null);
	//PTD_LOG("_RNK_RECODING_THIS_FUNC () \n");
	
	PTD_CK(kernel::get_workeru_nn() == 0);
	cell* src = this;

	PTD_LOG("send_cntr counter=%d tot_outs=%d \n", dbg_sornet_curr_cntr, dbg_tot_input_rnkcells);
	
	PTD_CK(dbg_all_input_rnkcells != mc_null);
	
	mc_init_arr_vals(dbg_tot_input_rnkcells, dbg_all_output_rnk_min_cols, BJ_INVALID_SRT_GRP);
	mc_init_arr_vals(dbg_tot_input_rnkcells, dbg_all_output_rnk_max_cols, BJ_INVALID_SRT_GRP);
	
	ini_grps_prms prms;
	PTD_DBG_CODE(prms.num_gen = &dbg_random_gen);
	prms.tot_input_objs = dbg_tot_input_rnkcells;
	prms.all_min_grps = dbg_all_input_rnkgrps;
	prms.all_max_grps = mc_null;
	prms.is_input_grp = dbg_is_input_rnkgrps;
	
	bj_dbg_sornet_init_grp_arr(prms, false);

	BJ_DBG_GRPS_RANK(
		PTD_DBG_CODE(prms.num_gen = &dbg_random_gen);
		prms.tot_input_objs = dbg_tot_input_rnkcells;
		prms.all_min_grps = dbg_all_input_rnk_min_grps;
		prms.all_max_grps = dbg_all_input_rnk_max_grps;
		prms.all_rnk_grps = dbg_all_input_rnkgrps;
		prms.is_input_grp = mc_null;
		
		bj_dbg_sornet_init_grp_arr(prms, false);
		
		PTD_CK(dbg_tot_input_rnkcells > 0);
		PTD_CK(dbg_all_input_rnk_min_grps[0] != BJ_INVALID_SRT_GRP);
		
		sornet_dbg_rnk_print_input();
	);
	
	num_nod_t mn_idx = 0;
	num_nod_t mx_idx = dbg_tot_input_rnkcells - 1;
	
	num_nod_t aa;
	for(aa = 0; aa < dbg_tot_input_rnkcells; aa++){
		sorcell* srout = dbg_all_input_rnkcells[aa];
		PTD_CK(srout != mc_null);

		//void* obj = (void*)(&(dbg_all_input_rnkgrps[aa]));
		void* obj = bj_num_to_pt(dbg_all_input_rnkgrps[aa]);	// RNK_OBJ_NUM
		PTD_CK(obj != BJ_INVALID_RNK_OBJ);
		
		BJ_DBG_GRPS_RANK(
			mn_idx = all_input_rnk_min_grps[aa];
			mx_idx = all_input_rnk_max_grps[aa];
		);
		
		bj_send_sornet_tmt(src, bj_tok_sornet_rank_start, 
						   sorkind_rnk, aa, aa, mn_idx, mx_idx, obj, srout, aa);
	}
}

void
nervenet::sornet_dbg_rnk_end_step(){
#ifndef MC_IS_PTD_CODE
	mck_abort(1, mc_cstr("Undefined func: nervenet::sornet_dbg_rnk_end_step"));
#endif
#if defined(MC_IS_PTD_CODE) && defined(FULL_DEBUG)
	PTD_CK(kernel::get_workeru_nn() == 0);
	BJ_DBG_ONE_GRP_ONLY(
		num_nod_t mn_idx = dbg_all_input_srt_min_grps[dbg_idx_grp_test];
		num_nod_t mx_idx = dbg_all_input_srt_max_grps[dbg_idx_grp_test];
	);

	PTD_PRINTF("IDXS=[");
	num_nod_t fst = 0;
	BJ_DBG_ONE_GRP_ONLY(fst = mn_idx);
	num_nod_t aa;
	for(aa = 0; aa < dbg_tot_input_rnkcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		PTD_PRINTF("%ld ", aa);
		if(aa == dbg_all_input_srt_max_grps[aa]){ PTD_PRINTF("|"); }
		if(aa > fst){
			BJ_DBG_RANK_OUTS(num_nod_t grp_0 = dbg_all_input_rnkgrps[aa - 1]);
			BJ_DBG_RANK_OUTS(num_nod_t grp_1 = dbg_all_input_rnkgrps[aa]);
			BJ_DBG_SRT_RNK(PTD_CK(dbg_all_srt_endcells[aa - 1] != mc_null));
			BJ_DBG_SRT_RNK(PTD_CK(dbg_all_srt_endcells[aa] != mc_null));
			BJ_DBG_SRT_RNK(num_nod_t grp_0 = dbg_all_srt_endcells[aa - 1]->end_snp.min_grp);
			BJ_DBG_SRT_RNK(num_nod_t grp_1 = dbg_all_srt_endcells[aa]->end_snp.min_grp);
			num_nod_t min_0 = dbg_all_output_rnk_min_cols[aa - 1];
			num_nod_t min_1 = dbg_all_output_rnk_min_cols[aa];
			num_nod_t max_0 = dbg_all_output_rnk_max_cols[aa - 1];
			num_nod_t max_1 = dbg_all_output_rnk_max_cols[aa];

			PTD_CK(min_0 <= max_0);
			PTD_CK(min_1 <= max_1);
			if(grp_0 == grp_1){
				PTD_CK(min_0 == min_1);
				PTD_CK(max_0 == max_1);
			} else {
				PTD_CK(min_0 < min_1);
				PTD_CK(max_0 < max_1);
				PTD_CK((max_0 + 1) == min_1);
				
				PTD_CK(min_0 == dbg_all_output_rnk_min_cols[min_0]);
				PTD_CK(max_0 == (aa - 1));
				PTD_CK(min_1 == aa);
				PTD_CK(max_1 == dbg_all_output_rnk_max_cols[max_1]);
			}
		}
	}
	PTD_PRINTF("]\n");
	BJ_DBG_RANK_OUTS(
		PTD_PRINTF("GRPS=[");
		for(aa = 0; aa < dbg_tot_input_rnkcells; aa++){
			BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
			PTD_PRINTF("%ld ", dbg_all_input_rnkgrps[aa]);
			if(aa == dbg_all_input_srt_max_grps[aa]){ PTD_PRINTF("|"); }
		}
		PTD_PRINTF("]\n");
	);
	
	PTD_PRINTF("MIN_COLS=[");
	for(aa = 0; aa < dbg_tot_input_rnkcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		PTD_PRINTF("%ld ", dbg_all_output_rnk_min_cols[aa]);
		if(aa == dbg_all_input_srt_max_grps[aa]){ PTD_PRINTF("|"); }
	}
	PTD_PRINTF("]\n");
	PTD_PRINTF("MAX_COLS=[");
	for(aa = 0; aa < dbg_tot_input_rnkcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		PTD_PRINTF("%ld ", dbg_all_output_rnk_max_cols[aa]);
		if(aa == dbg_all_input_srt_max_grps[aa]){ PTD_PRINTF("|"); }
	}
	PTD_PRINTF("]\n");
#endif
}

void
nervenet::sornet_dbg_rnk_print_input(){
#ifndef MC_IS_PTD_CODE
	mck_abort(1, mc_cstr("Undefined func: nervenet::sornet_dbg_rnk_print_input"));
#endif
#if defined(MC_IS_PTD_CODE) && defined(FULL_DEBUG)
	num_nod_t aa;
	printf("GRPS=[");
	for(aa = 0; aa < dbg_tot_input_rnkcells; aa++){
		printf("%ld ", dbg_all_input_rnkgrps[aa]);
	}
	printf("]\n");
	printf("MINS=[");
	for(aa = 0; aa < dbg_tot_input_rnkcells; aa++){
		printf("%ld ", dbg_all_input_rnk_min_grps[aa]);
	}
	printf("]\n");
	printf("MAXS=[");
	for(aa = 0; aa < dbg_tot_input_rnkcells; aa++){
		printf("%ld ", dbg_all_input_rnk_max_grps[aa]);
	}
	printf("]\n");
	printf("SIZE=%ld \n", dbg_tot_input_rnkcells);
#endif
}

#endif
// END_OF_RANK_OUTS_CODE
//----------------------------------------------------------------------------------

//================================================================================
//================================================================================
// BEGINING_OF_NUM_SORT_CODE

#if (defined(BJ_SORNET_NUM_TEST) || defined(BJ_SORNET_SRT_RNK_TEST))

int
bj_dbg_cmp_num_objs(void* obj1, void* obj2){
	num_nod_t v1 = bj_pt_obj_as_num(obj1);
	num_nod_t v2 = bj_pt_obj_as_num(obj2);
	if(v1 < v2){ return -1; }
	if(v1 > v2){ return 1;}
	return 0;
}

void
nervenet::sornet_dbg_num_init_val_arr(){
#if defined(MC_IS_PTD_CODE) && defined(FULL_DEBUG)
	//unsigned long init_val = 10;
	//tak_mak gg(init_val);
	tak_mak& gg = dbg_random_gen;
	
	if(dbg_sornet_curr_cntr == 1){
		PTD_CK(dbg_all_input_vals == mc_null);
		dbg_all_input_vals = mc_malloc32(num_nod_t, dbg_tot_input_sorcells);
	}
	
	unsigned long max_val = 2 * dbg_tot_input_sorcells;
	
	PTD_CK(dbg_all_input_vals != mc_null);
	mc_init_arr_vals(dbg_tot_input_sorcells, dbg_all_input_vals, 0);

	for(num_nod_t aa = 0; aa < dbg_tot_input_sorcells; aa++){
		if(dbg_all_input_vals[aa] != 0){
			continue;
		}
		
		num_nod_t val = (num_nod_t)gg.gen_rand_int32_ie(1, max_val);
		dbg_all_input_vals[aa] = val;
		//printf("1.dbg_all_input_vals[ %ld ] = %ld \n", aa, val);
		
		if((aa + 1) == dbg_tot_input_sorcells){ continue; }
		
		num_nod_t num_rep = (num_nod_t)gg.gen_rand_int32_ie(
				BJ_DBG_MIN_NUM_REP_VAL, BJ_DBG_MAX_NUM_REP_VAL);
		
		for(num_nod_t bb = 0; bb < num_rep; bb++){
			long max_rep_idx = dbg_tot_input_sorcells;
			BJ_DBG_GRPS_SORNUMS(
				max_rep_idx = aa + BJ_DBG_MAX_REP_DIST + 1;
				if(max_rep_idx > dbg_tot_input_sorcells){ max_rep_idx = dbg_tot_input_sorcells; }
			);
			PTD_CK(max_rep_idx > (aa + 1));
			PTD_CK(max_rep_idx <= dbg_tot_input_sorcells);
			
			num_nod_t pos = (num_nod_t)gg.gen_rand_int32_ie(aa + 1, max_rep_idx);
			PTD_CK_PRT((pos < max_rep_idx), "%ld  < %ld \n", pos, max_rep_idx);
			// val = (num_nod_t)gg.gen_rand_int32_ie(1, max_val); // DBG_ONLY
			if(dbg_all_input_vals[pos] != 0){
				continue;
			}
			dbg_all_input_vals[pos] = val;
			
			//printf("2.dbg_all_input_vals[ %ld ] = %ld \n", pos, val);
		}
	}
#endif
}

void
nervenet::sornet_dbg_num_send_step(){
	cell* src = this;

	PTD_LOG("TEST_COUNTER=%d tot_inp=%d \n", dbg_sornet_curr_cntr, dbg_tot_input_sorcells);
	
	BJ_DBG_SRT_RNK(PTD_CK(dbg_tot_input_rnkcells == dbg_tot_input_sorcells));
	BJ_DBG_SRT_RNK(mc_init_arr_vals(dbg_tot_input_rnkcells, dbg_all_output_rnk_min_cols,
									BJ_INVALID_SRT_GRP));
	BJ_DBG_SRT_RNK(mc_init_arr_vals(dbg_tot_input_rnkcells, dbg_all_output_rnk_max_cols,
									BJ_INVALID_SRT_GRP));

	sornet_dbg_num_init_val_arr();

	dbg_idx_grp_test = 0;
	dbg_tot_grp_items = 0;
	bool doing_grps = false;
	MC_MARK_USED(doing_grps);
	
	BJ_DBG_GRPS_SORNUMS(
		doing_grps = true;
		ini_grps_prms prms;
		PTD_DBG_CODE(prms.num_gen = &dbg_random_gen);
		prms.tot_input_objs = dbg_tot_input_sorcells;
		prms.all_min_grps = dbg_all_input_srt_min_grps;
		prms.all_max_grps = dbg_all_input_srt_max_grps;
		prms.is_input_grp = mc_null;
		
		bj_dbg_sornet_init_grp_arr(prms, false);
		
		PTD_CK(dbg_tot_input_sorcells > 0);
		PTD_CK(dbg_all_input_srt_min_grps[0] != BJ_INVALID_SRT_GRP);
		PTD_CK(prms.tot_grps > 0);

		dbg_idx_grp_test = (num_nod_t)dbg_random_gen.gen_rand_int32_ie(0, dbg_tot_input_sorcells);
		
		sornet_dbg_num_print_input();
	);

	PTD_CK(dbg_all_input_vals != mc_null);
	
	num_nod_t mn_idx = 0;
	num_nod_t mx_idx = dbg_tot_input_sorcells - 1;
	
	num_nod_t aa;
	for(aa = 0; aa < dbg_tot_input_sorcells; aa++){
		//PTD_LOG("send_cntr vv=%d \n", vv);
		PTD_CK(dbg_all_input_vals[aa] != 0);

		sorcell* srcll = dbg_all_input_sorcells[aa];
		PTD_CK(srcll != mc_null);

		void* obj = (void*)(&(dbg_all_input_vals[aa]));
		PTD_CK(obj != mc_null);
		PTD_CK(srcll != mc_null);
		PTD_CK(src != mc_null);

		BJ_DBG_GRPS_SORNUMS(
			mn_idx = dbg_all_input_srt_min_grps[aa];
			mx_idx = dbg_all_input_srt_max_grps[aa];
		);
		
		BJ_DBG_ONE_GRP_ONLY(
			PTD_CK(doing_grps);
			if(! bj_sornet_idx_inside(dbg_idx_grp_test, mn_idx, mx_idx)){
				continue;
			}
			if(dbg_tot_grp_items == 0){
				dbg_tot_grp_items = (mx_idx - mn_idx + 1);
				PTD_CK(dbg_tot_grp_items > 0);
			}
		)
		
		bj_send_sornet_tmt(src, bj_tok_sornet_num, sorkind_num, aa, aa, mn_idx, mx_idx, obj, srcll, aa);
	}
	BJ_DBG_ONE_GRP_ONLY(
		printf("TOT_ITEMS=%ld GRP_IDX=%ld \n", dbg_tot_grp_items, dbg_idx_grp_test);
	);
}

void
nervenet::sornet_dbg_num_print_input(){
#if defined(MC_IS_PTD_CODE) && defined(FULL_DEBUG)
	
	BJ_DBG_ONE_GRP_ONLY(
		printf("DOING_ONE_GRP_ONLY\n");
		PTD_CK(dbg_idx_grp_test >= 0);
		PTD_CK(dbg_idx_grp_test < dbg_tot_input_sorcells);
		num_nod_t mn_idx = dbg_all_input_srt_min_grps[dbg_idx_grp_test];
		num_nod_t mx_idx = dbg_all_input_srt_max_grps[dbg_idx_grp_test];
	);
	
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n ");
	
	printf("INPUT=[");
	num_nod_t aa;
	for(aa = 0; aa < dbg_tot_input_sorcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		num_nod_t vv = dbg_all_input_vals[aa];
		printf("%ld ", vv);
		if(aa == dbg_all_input_srt_max_grps[aa]){ PTD_PRINTF("|"); }
	}
	printf("]\n");
	printf("MINS=[");
	for(aa = 0; aa < dbg_tot_input_sorcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		printf("%ld ", dbg_all_input_srt_min_grps[aa]);
		if(aa == dbg_all_input_srt_max_grps[aa]){ PTD_PRINTF("|"); }
	}
	printf("]\n");
	printf("MAXS=[");
	for(aa = 0; aa < dbg_tot_input_sorcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		printf("%ld ", dbg_all_input_srt_max_grps[aa]);
		if(aa == dbg_all_input_srt_max_grps[aa]){ PTD_PRINTF("|"); }
	}
	printf("]\n");
	printf("SIZE=%ld \n", dbg_tot_input_sorcells);
#endif
}

void
nervenet::sornet_dbg_num_end_step(){
#if defined(MC_IS_PTD_CODE) && defined(FULL_DEBUG)
	BJ_DBG_ONE_GRP_ONLY(
		num_nod_t mn_idx = dbg_all_input_srt_min_grps[dbg_idx_grp_test];
		num_nod_t mx_idx = dbg_all_input_srt_max_grps[dbg_idx_grp_test];
	);
	
	PTD_CK(kernel::get_workeru_nn() == 0);

	printf("SORTED=[");
	num_nod_t fst = 0;
	BJ_DBG_ONE_GRP_ONLY(fst = mn_idx);
	
	num_nod_t aa;
	for(aa = 0; aa < dbg_tot_input_sorcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		void* obj = dbg_all_output_sorobjs[aa];
		PTD_CK(obj != mc_null);
		num_nod_t vv = bj_pt_obj_as_num(obj);
		
		printf("%ld ", vv);
		if(aa == dbg_all_input_srt_max_grps[aa]){ printf("|"); }
		
		if(aa > fst){
			void* obj_prv = dbg_all_output_sorobjs[aa - 1];
			num_nod_t vv_prv = bj_pt_obj_as_num(obj_prv);
			num_nod_t col_0 = dbg_all_output_sorcols[aa - 1];
			num_nod_t col_1 = dbg_all_output_sorcols[aa];
			num_nod_t grp_0 = dbg_all_input_srt_min_grps[aa - 1];
			num_nod_t grp_1 = dbg_all_input_srt_min_grps[aa];

			PTD_CK_PRT(((aa == fst) || (((vv_prv == vv) && (grp_0 == grp_1)) ? 
										(col_0 == col_1) : (col_0 != col_1))),
				"(%ld == %ld)? %ld %ld \n", vv_prv, vv, col_0, col_1);
		}
	}
	printf("]\n");
	printf("COLORS=[");
	for(aa = 0; aa < dbg_tot_input_sorcells; aa++){
		BJ_DBG_ONE_GRP_ONLY(if(! bj_sornet_idx_inside(aa, mn_idx, mx_idx)){ continue; })
		printf("%ld ", dbg_all_output_sorcols[aa]);
		if(aa == dbg_all_input_srt_max_grps[aa]){ printf("|"); }
	}
	printf("]\n");
#endif
}

#endif
// END_OF_NUM_SORT_CODE
//----------------------------------------------------------------------------------

