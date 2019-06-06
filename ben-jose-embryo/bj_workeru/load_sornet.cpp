
#include "cell.hh"

#include "load_cnf.hh"
#include "preload.hh"

#define bj_get_loaded_of_pt(pt_cls, pt) (((pt_cls*)(pt))->loaded)

#define bj_wait_set_pt(nod_cls, nod, out_cls, out) \
	if(nod != mc_null){ \
		mc_spin_cond(bj_get_loaded_of_pt(nod_cls, nod) == mc_null) \
		out = (out_cls*)bj_get_loaded_of_pt(nod_cls, nod); \
	} \

// end_macro

void
sorcell::sornet_dbg_prt(){
	bool up_end = mc_get_flag(edge_flags, bj_rnk_up_end_flag);
	MC_MARK_USED(up_end);
	bool down_end = mc_get_flag(edge_flags, bj_rnk_down_end_flag);
	MC_MARK_USED(down_end);
	PTD_PRINTF("lding_scll %ld[%ld %ld] f(%d,%d) \n", 
			   srt_sz, up_snp.idx, down_snp.idx, up_end, down_end);
}

void
sorcell::load_from(pre_sornode* nod){
	sorcell* scll = this;

	MC_DBG(
		nod_id = nod->nod_id;
		level = nod->level;
	);

	srt_sz = nod->srt_sz;
	
	edge_flags = nod->edge_flags;
	
	PTD_CK(up_snp.inp == mc_null);
	up_snp.idx = nod->up_pns.idx;
	if(nod->up_pns.out != mc_null){
		up_snp.inp = (void*)mc_manageru_pt_to_workeru_pt(nod->up_pns.out); // up_inp used as tmp
	}

	PTD_CK(down_snp.inp == mc_null);
	down_snp.idx = nod->down_pns.idx;
	if(nod->down_pns.out != mc_null){
		down_snp.inp = (void*)mc_manageru_pt_to_workeru_pt(nod->down_pns.out); // down_inp used as tmp
	}
	
	void* pt_up_axon = (void*)mc_manageru_pt_to_workeru_pt(nod->up_pns.axon);
	bj_wait_set_pt(pre_endnode, pt_up_axon, endcell, up_snp.axon);
	void* pt_down_axon = (void*)mc_manageru_pt_to_workeru_pt(nod->down_pns.axon);
	bj_wait_set_pt(pre_endnode, pt_down_axon, endcell, down_snp.axon);

	MCK_CK(nod->loaded == mc_null);
	MCK_CK(scll != mc_null);
	//nod->loaded = mck_as_glb_pt(scll);
	mc_loop_set_var(nod->loaded, mck_as_glb_pt(scll));
}

void bj_init_end_cells(binder* nn_all_endnods)
{
	binder * fst, * lst, * wrk;
	
	fst = (binder*)mc_manageru_pt_to_workeru_pt(nn_all_endnods->bn_right);
	lst = nn_all_endnods;
	for(wrk = fst; wrk != lst; wrk = (binder*)mc_manageru_pt_to_workeru_pt(wrk->bn_right)){
		pre_endnode* end_nd = (pre_endnode*)wrk;
		MCK_CK(end_nd->loaded != mc_null);
		endcell* ecll = (endcell*)(end_nd->loaded);
		MCK_CK(ecll != mc_null);
		
		mck_slog2("ecll_");
		mck_ilog(ecll->end_snp.idx);
		
		MCK_CK(end_nd->nxt.idx == ecll->end_snp.idx);
		if(end_nd->nxt.out != mc_null){
			MCK_CK(ecll->end_snp.out == mc_null);
			pre_sornode* the_out = (pre_sornode*)mc_manageru_pt_to_workeru_pt(end_nd->nxt.out);
			
			mck_slog2("__out=");
			mck_ilog(the_out->up_pns.idx);
			mck_slog2("_");
			mck_ilog(the_out->down_pns.idx);
			
			bj_wait_set_pt(pre_sornode, the_out, sorcell, ecll->end_snp.out);
			MCK_CK(ecll->end_snp.out != mc_null);
		}
		mck_slog2(",");
		if(end_nd->nxt.axon != mc_null){
			MCK_CK(ecll->end_snp.axon == mc_null);
			pre_endnode* the_axon = (pre_endnode*)mc_manageru_pt_to_workeru_pt(end_nd->nxt.axon);
			
			mck_slog2("__axon=");
			mck_ilog(the_axon->nxt.idx);
			
			bj_wait_set_pt(pre_endnode, the_axon, endcell, ecll->end_snp.axon);
			MCK_CK(ecll->end_snp.axon != mc_null);
		}
		
		mck_slog2(".\n");
	}
}

void bj_load_shd_nods(num_nod_t tot_wu_endnods, binder* nn_all_endnods, binder* nn_all_ecells, 
					  num_nod_t tot_wu_nods, binder* nn_all_nods, binder* nn_all_sclls, 
					  void* invalid_val)
{
	sorcell::separate(tot_wu_nods);
	endcell::separate(tot_wu_endnods);

	PTD_LOG("Separated tot_wu_nods=%ld tot_wu_endnods=%ld \n", tot_wu_nods, tot_wu_endnods);
	
	num_nod_t ck_wu_endnods = 0;
	MC_MARK_USED(ck_wu_endnods);

	binder * fst, * lst, * wrk;
	
	fst = (binder*)mc_manageru_pt_to_workeru_pt(nn_all_endnods->bn_right);
	lst = nn_all_endnods;
	for(wrk = fst; wrk != lst; wrk = (binder*)mc_manageru_pt_to_workeru_pt(wrk->bn_right)){
		pre_endnode* end_nd = (pre_endnode*)wrk;
		endcell* ecll = bj_endcell_acquire();
		ck_wu_endnods++;
		
		MCK_CK(ecll != mc_null);
		MCK_CK(end_nd->loaded == mc_null);
		ecll->end_snp.idx = end_nd->nxt.idx;

		PTD_LOG("AQUIRED_ENDCELL %p idx=%ld \n", ecll, ecll->end_snp.idx);
		
		//end_nd->loaded = mck_as_glb_pt(ecll);
		mc_loop_set_var(end_nd->loaded, mck_as_glb_pt(ecll));
		
		nn_all_ecells->bind_to_my_left(*ecll);
	}
	
	PTD_CK(ck_wu_endnods == tot_wu_endnods);

	num_nod_t ck_wu_nods = 0;
	MC_MARK_USED(ck_wu_nods);
	
	fst = (binder*)mc_manageru_pt_to_workeru_pt(nn_all_nods->bn_right);
	lst = nn_all_nods;
	for(wrk = fst; wrk != lst; wrk = (binder*)mc_manageru_pt_to_workeru_pt(wrk->bn_right)){
		pre_sornode* nod = (pre_sornode*)wrk;
		sorcell* scll = bj_sorcell_acquire();
		ck_wu_nods++;
		
		scll->load_from(nod);
		nn_all_sclls->bind_to_my_left(*scll);
	}

	PTD_CK(ck_wu_nods == tot_wu_nods);
	
	fst = (binder*)(nn_all_sclls->bn_right);
	lst = nn_all_sclls;
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		sorcell* scll = (sorcell*)wrk;

		bj_wait_set_pt(pre_sornode, scll->up_snp.inp, sorcell, scll->up_snp.out);
		bj_wait_set_pt(pre_sornode, scll->down_snp.inp, sorcell, scll->down_snp.out);
		
		scll->up_snp.inp = invalid_val;
		scll->down_snp.inp = invalid_val;

		PTD_CK(scll->up_snp.inp == invalid_val);
		PTD_CK(scll->down_snp.inp == invalid_val);
	};
}

void bj_init_ends_srt_sornet(){
	mck_slog2("bj_init_ends_srt_sornet_started\n");
	
	pre_cnf_net* nn_cnf = bj_nervenet->shd_cnf;
	binder* nn_all_endnods = &(nn_cnf->all_pre_srt_endnods); // nn_cnf is already workeru_pt 
	bj_init_end_cells(nn_all_endnods);
	
	mck_slog2("end_of_ENDS_SRT\n");
}

void bj_init_ends_rnk_sornet(){
	mck_slog2("bj_init_ends_rnk_sornet_started\n");
	
	pre_cnf_net* nn_cnf = bj_nervenet->shd_cnf;
	binder* nn_all_endnods = &(nn_cnf->all_pre_rnk_endnods); // nn_cnf is already workeru_pt 
	bj_init_end_cells(nn_all_endnods);

	mck_slog2("end_of_ENDS_RNK\n");
}

void bj_load_shd_sornet(){
	mck_slog2("bj_load_shd_sornet_started\n");
	
	nervenet* my_net = bj_nervenet;
	pre_cnf_net* nn_cnf = bj_nervenet->shd_cnf;
	void* invalid_val = BJ_INVALID_SRT_OBJ;

	binder* nn_all_endnods = &(nn_cnf->all_pre_srt_endnods); // nn_cnf is already workeru_pt 
	binder* nn_all_eclls = &(my_net->all_wu_srt_endcells); // nn_cnf is already workeru_pt 
	binder* nn_all_nods = &(nn_cnf->all_pre_sornods); // nn_cnf is already workeru_pt 
	binder* nn_all_sclls = &(my_net->all_wu_sorcells); // nn_cnf is already workeru_pt 
	bj_load_shd_nods(nn_cnf->tot_pre_srt_endnods, nn_all_endnods, nn_all_eclls, 
					 nn_cnf->tot_pre_sornods, nn_all_nods, nn_all_sclls, invalid_val);
	
	my_net->tot_wu_srt_endcells = nn_cnf->tot_pre_srt_endnods;
	my_net->tot_wu_sorcells = nn_cnf->tot_pre_sornods;
	
	mck_slog2("end_of_SRT_bj_load_shd_nods \n");
	
	kernel* ker = mck_get_kernel();
	mc_workeru_nn_t nn = kernel::get_workeru_nn();

	if(nn == 0){
		pre_load_cnf* pre_cnf = (pre_load_cnf*)(ker->manageru_load_data);
		pre_sornode** all_input = 
			(pre_sornode**)mc_manageru_addr_to_workeru_addr((mc_addr_t)(pre_cnf->all_pre_sorinput_nod));
		pre_endnode** all_ends = 
			(pre_endnode**)mc_manageru_addr_to_workeru_addr((mc_addr_t)(pre_cnf->all_pre_srt_end_nod));

		num_nod_t tot_sorinp = pre_cnf->tot_pre_sorinput_nod;
		mck_slog2("TOT_INPUT_SORCELLS=");
		mck_ilog(tot_sorinp);	
		mck_slog2("\n");

		my_net->dbg_tot_input_sorcells = tot_sorinp;
		my_net->dbg_all_input_sorcells = mc_malloc32(sorcell*, tot_sorinp);
		mc_init_arr_vals(tot_sorinp, my_net->dbg_all_input_sorcells, mc_null);
		my_net->dbg_all_input_srt_min_grps = mc_malloc32(num_nod_t, tot_sorinp);
		mc_init_arr_vals(tot_sorinp, my_net->dbg_all_input_srt_min_grps, BJ_INVALID_SRT_GRP);
		my_net->dbg_all_input_srt_max_grps = mc_malloc32(num_nod_t, tot_sorinp);
		mc_init_arr_vals(tot_sorinp, my_net->dbg_all_input_srt_max_grps, BJ_INVALID_SRT_GRP);

		my_net->dbg_all_srt_endcells = mc_malloc32(endcell*, tot_sorinp);
		mc_init_arr_vals(tot_sorinp, my_net->dbg_all_srt_endcells, mc_null);
		
		my_net->dbg_all_output_sorobjs = mc_malloc32(void*, tot_sorinp);
		mc_init_arr_vals(tot_sorinp, my_net->dbg_all_output_sorobjs, mc_null);
		my_net->dbg_all_output_sorcols = mc_malloc32(num_nod_t, tot_sorinp);
		mc_init_arr_vals(tot_sorinp, my_net->dbg_all_output_sorcols, 0);

		sorcell** all_sorcell = my_net->dbg_all_input_sorcells;
		endcell** all_endcell = my_net->dbg_all_srt_endcells;
		
		num_nod_t aa;
		for(aa = 0; aa < tot_sorinp; aa++){
			sorcell* inp_aa = (sorcell*)mc_manageru_addr_to_workeru_addr((mc_addr_t)(all_input[aa]));
			bj_wait_set_pt(pre_sornode, inp_aa, sorcell, all_sorcell[aa]);

			endcell* end_aa = (endcell*)mc_manageru_addr_to_workeru_addr((mc_addr_t)(all_ends[aa]));
			bj_wait_set_pt(pre_endnode, end_aa, endcell, all_endcell[aa]);
		}
	}

	mck_slog2("end_of_bj_load_shd_sornet \n");	
}

void bj_load_shd_ranknet(){
	mck_slog2("bj_load_shd_ranknet_started\n");
	
	nervenet* my_net = bj_nervenet;
	pre_cnf_net* nn_cnf = bj_nervenet->shd_cnf;
	void* invalid_val = BJ_INVALID_RNK_OBJ;

	binder* nn_all_endnods = &(nn_cnf->all_pre_rnk_endnods); // nn_cnf is already workeru_pt 
	binder* nn_all_eclls = &(my_net->all_wu_rnk_endcells); // nn_cnf is already workeru_pt 
	binder* nn_all_nods = &(nn_cnf->all_pre_rnknods); // nn_cnf is already workeru_pt
	binder* nn_all_sclls = &(my_net->all_wu_rnkcells); // nn_cnf is already workeru_pt
	bj_load_shd_nods(nn_cnf->tot_pre_rnk_endnods, nn_all_endnods, nn_all_eclls, 
					 nn_cnf->tot_pre_rnknods, nn_all_nods, nn_all_sclls, invalid_val);
	
	my_net->tot_wu_rnk_endcells = nn_cnf->tot_pre_rnk_endnods;
	my_net->tot_wu_rnkcells = nn_cnf->tot_pre_rnknods;
	
	mck_slog2("end_of_RNK_bj_load_shd_nods \n");

	kernel* ker = mck_get_kernel();
	mc_workeru_nn_t nn = kernel::get_workeru_nn();

	if(nn == 0){
		pre_load_cnf* pre_cnf = (pre_load_cnf*)(ker->manageru_load_data);
		pre_sornode** all_input = 
			(pre_sornode**)mc_manageru_addr_to_workeru_addr((mc_addr_t)(pre_cnf->all_pre_rank_in_nod));

		num_nod_t tot_rnkinp = pre_cnf->tot_pre_rank_in_nod;
		mck_slog2("TOT_INPUT_RNKCELLS=");
		mck_ilog(tot_rnkinp);	
		mck_slog2("\n");

		my_net->dbg_tot_input_rnkcells = tot_rnkinp;
		my_net->dbg_all_input_rnkcells = mc_malloc32(sorcell*, tot_rnkinp);
		mc_init_arr_vals(tot_rnkinp, my_net->dbg_all_input_rnkcells, mc_null);

		my_net->dbg_all_input_rnkgrps = mc_malloc32(num_nod_t, tot_rnkinp);
		mc_init_arr_vals(tot_rnkinp, my_net->dbg_all_input_rnkgrps, BJ_INVALID_SRT_GRP);
		my_net->dbg_all_input_rnk_min_grps = mc_malloc32(num_nod_t, tot_rnkinp);
		mc_init_arr_vals(tot_rnkinp, my_net->dbg_all_input_rnk_min_grps, BJ_INVALID_SRT_GRP);
		my_net->dbg_all_input_rnk_max_grps = mc_malloc32(num_nod_t, tot_rnkinp);
		mc_init_arr_vals(tot_rnkinp, my_net->dbg_all_input_rnk_max_grps, BJ_INVALID_SRT_GRP);
		
		my_net->dbg_all_output_rnk_min_cols = mc_malloc32(num_nod_t, tot_rnkinp);
		mc_init_arr_vals(tot_rnkinp, my_net->dbg_all_output_rnk_min_cols, BJ_INVALID_SRT_GRP);
		my_net->dbg_all_output_rnk_max_cols = mc_malloc32(num_nod_t, tot_rnkinp);
		mc_init_arr_vals(tot_rnkinp, my_net->dbg_all_output_rnk_max_cols, BJ_INVALID_SRT_GRP);
		
		my_net->dbg_is_input_rnkgrps = mc_malloc32(bool, tot_rnkinp);
		mc_init_arr_vals(tot_rnkinp, my_net->dbg_is_input_rnkgrps, false);

		sorcell** all_rnkcell = my_net->dbg_all_input_rnkcells;
		
		num_nod_t aa;
		for(aa = 0; aa < tot_rnkinp; aa++){
			sorcell* inp_aa = (sorcell*)mc_manageru_addr_to_workeru_addr((mc_addr_t)(all_input[aa]));
			bj_wait_set_pt(pre_sornode, inp_aa, sorcell, all_rnkcell[aa]);
		}
	}

	mck_slog2("end_of_bj_load_shd_ranknet  \n");	
}

