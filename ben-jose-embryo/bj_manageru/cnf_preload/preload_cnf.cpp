
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preload.hh"

pre_load_cnf* THE_CNF = mc_null;

grip ava_pre_sornode;
grip ava_pre_endnode;
grip ava_pre_item_pgroup;
grip ava_pre_pgroup;
grip ava_pre_cnf_node;
grip ava_pre_cnf_net;

MCK_DEFINE_MEM_METHODS(pre_sornode, 32, ava_pre_sornode, 0)
MCK_DEFINE_MEM_METHODS(pre_endnode, 32, ava_pre_endnode, 0)
MCK_DEFINE_MEM_METHODS(pre_item_pgroup, 32, ava_pre_item_pgroup, 0)
MCK_DEFINE_MEM_METHODS(pre_pgroup, 32, ava_pre_pgroup, 0)
MCK_DEFINE_MEM_METHODS(pre_cnf_node, 32, ava_pre_cnf_node, 0)
MCK_DEFINE_MEM_METHODS(pre_cnf_net, 32, ava_pre_cnf_net, 0)

grip* bj_mgr_all_ava[bj_mgr_idx_total];
mc_alloc_kernel_func_t bj_mgr_all_acq[bj_mgr_idx_total];
mc_alloc_kernel_func_t bj_mgr_all_sep[bj_mgr_idx_total];

void bj_mgr_init_mem_funcs(){
	
	mc_init_arr_vals(bj_mgr_idx_total, bj_mgr_all_ava, mc_null);
	mc_init_arr_vals(bj_mgr_idx_total, bj_mgr_all_acq, mc_null);
	mc_init_arr_vals(bj_mgr_idx_total, bj_mgr_all_sep, mc_null);
	
	bj_mgr_all_ava[bj_cell_id(pre_sornode)] = &(ava_pre_sornode);
	bj_mgr_all_ava[bj_cell_id(pre_endnode)] = &(ava_pre_endnode);
	bj_mgr_all_ava[bj_cell_id(pre_item_pgroup)] = &(ava_pre_item_pgroup);
	bj_mgr_all_ava[bj_cell_id(pre_pgroup)] = &(ava_pre_pgroup);
	bj_mgr_all_ava[bj_cell_id(pre_cnf_node)] = &(ava_pre_cnf_node);
	bj_mgr_all_ava[bj_mgr_last_invalid] = mc_pt_invalid_available;
	
	bj_mgr_all_acq[bj_cell_id(pre_sornode)] = (mc_alloc_kernel_func_t)pre_sornode::acquire_alloc;
	bj_mgr_all_acq[bj_cell_id(pre_endnode)] = (mc_alloc_kernel_func_t)pre_endnode::acquire_alloc;
	bj_mgr_all_acq[bj_cell_id(pre_item_pgroup)] = (mc_alloc_kernel_func_t)pre_item_pgroup::acquire_alloc;
	bj_mgr_all_acq[bj_cell_id(pre_pgroup)] = (mc_alloc_kernel_func_t)pre_pgroup::acquire_alloc;
	bj_mgr_all_acq[bj_cell_id(pre_cnf_node)] = (mc_alloc_kernel_func_t)pre_cnf_node::acquire_alloc;
	bj_mgr_all_acq[bj_mgr_last_invalid] = kernel::invalid_alloc_func;

	bj_mgr_all_sep[bj_cell_id(pre_sornode)] = (mc_alloc_kernel_func_t)pre_sornode::separate;
	bj_mgr_all_sep[bj_cell_id(pre_endnode)] = (mc_alloc_kernel_func_t)pre_endnode::separate;
	bj_mgr_all_sep[bj_cell_id(pre_item_pgroup)] = (mc_alloc_kernel_func_t)pre_item_pgroup::separate;
	bj_mgr_all_sep[bj_cell_id(pre_pgroup)] = (mc_alloc_kernel_func_t)pre_pgroup::separate;
	bj_mgr_all_sep[bj_cell_id(pre_cnf_node)] = (mc_alloc_kernel_func_t)pre_cnf_node::separate;
	bj_mgr_all_sep[bj_mgr_last_invalid] = kernel::invalid_alloc_func;
	
	kernel::set_cell_mem_funcs(bj_mgr_all_ava, bj_mgr_all_acq, bj_mgr_all_sep);
	
	PTD_CK(pre_sornode::ck_cell_id(bj_cell_id(pre_sornode)));
	PTD_CK(pre_endnode::ck_cell_id(bj_cell_id(pre_endnode)));
	PTD_CK(pre_item_pgroup::ck_cell_id(bj_cell_id(pre_item_pgroup)));
	PTD_CK(pre_pgroup::ck_cell_id(bj_cell_id(pre_pgroup)));
	PTD_CK(pre_cnf_node::ck_cell_id(bj_cell_id(pre_cnf_node)));
}

void init_node_arr(long sz, pre_cnf_node** arr, node_kind_t kk){
	for(long aa = 0; aa < sz; aa++){
		arr[aa] = bj_pre_cnf_node_acquire();
		arr[aa]->ki = kk;
		if(kk == nd_pos){
			arr[aa]->id = aa + 1;
		}
		if(kk == nd_neg){
			arr[aa]->id = -(aa + 1);
		}
	}
}

pre_cnf_node*
get_lit(long lit_id){
	if(lit_id > 0){
		pre_cnf_node* pos =  THE_CNF->all_pos[lit_id - 1];
		PTD_CK(pos->id == lit_id);
		return pos;
	}
	PTD_CK(lit_id < 0);
	pre_cnf_node* neg =  THE_CNF->all_neg[(-lit_id) - 1];
	PTD_CK(neg->id == lit_id);
	return neg;
}

void
print_cnf(){
	for(long aa = 0; aa < THE_CNF->tot_ccls; aa++){
		pre_cnf_node* ccl =  THE_CNF->all_ccl[aa];

		printf("[");

		binder * fst, * lst, * wrk;
		binder* lits = &(ccl->all_agts);

		fst = lits->bn_right;
		lst = lits;
		for(wrk = fst; wrk != lst; wrk = wrk->bn_right){
			agent_ref* ref = (agent_ref*)wrk;
			pre_cnf_node* lit = (pre_cnf_node*)(ref->glb_agent_ptr);

			printf("%ld ", lit->id);
		}

		printf("] \n");
	}
}

void
manageru_print_nods(){
	printf("ALL_NODS {");
	for(long aa = 0; aa < THE_CNF->tot_tmp_pre_load_nods; aa++){
		pre_cnf_node* nod =  THE_CNF->all_tmp_pre_load_nods[aa];
		printf("[k%d id%ld sz%ld] \n", nod->ki, nod->id, nod->pre_sz);
	}
	printf("} \n");
}

void
print_all_nods(binder& grip){
	binder * fst, * lst, * wrk;
	binder* grp = &(grip);

	fst = grp->bn_right;
	lst = grp;
	for(wrk = fst; wrk != lst; wrk = wrk->bn_right){
		pre_cnf_node* nod = (pre_cnf_node*)wrk;
		printf("[k%d id%ld sz%ld] ", nod->ki, nod->id, nod->pre_sz);
	}
}

void
print_pre_cnf_nets(){
	printf("ALL_CNFS {\n");
	for(long aa = 0; aa < THE_CNF->tot_workerus; aa++){
		pre_cnf_net& cnf =  THE_CNF->all_cnf[aa];

		printf("CNF %ld ======================================= \n", aa);

		//printf("[k%d id%ld sz%ld] \n", nod->ki, nod->id, nod->sz);
		printf("\tCCLS[");
		print_all_nods(cnf.all_pre_neu);
		printf("\t] \n");

		printf("\tPOS[");
		print_all_nods(cnf.all_pre_pos);
		printf("\t] \n");

		printf("\tNEG[");
		print_all_nods(cnf.all_pre_neg);
		printf("\t] \n");
	}
	printf("} \n");
}

int 
cmp_nodes(const void * p1, const void * p2){
	long sz1 = (*((pre_cnf_node **)p1))->pre_sz;
	long sz2 = (*((pre_cnf_node **)p2))->pre_sz;
	if(sz1 < sz2){ return -1; }
	if(sz1 > sz2){ return 1; }
	return 0;
}

void
check_node_sz(pre_node_sz_t sz){
	if(sz > THE_CNF->max_nod_sz){
		mck_abort(9, mc_cstr(
			"CNF node too big. Variable in too many clauses or clause too big. (BJ_MAX_NODE_SZ). Fix CNF."));
	}
	PTD_CK(sz < THE_CNF->max_nod_sz);
}

void
preload_cnf_init(){
	PTD_CK(THE_CNF != mc_null);

	THE_CNF->max_nod_sz = BJ_MAX_NODE_SZ;

	long num_ccls = THE_CNF->tot_ccls;
	long num_vars = THE_CNF->tot_vars;
	long num_lits = THE_CNF->tot_lits;
	long num_workerus = THE_CNF->tot_workerus;

	if(num_ccls == 0){
		return;
	}
	
	THE_CNF->tot_tmp_pre_load_nods = num_ccls + num_vars;
	long num_tmp_pre_load_nods = THE_CNF->tot_tmp_pre_load_nods;

	agent_ref::separate(2 * num_lits);
	pre_cnf_node::separate(num_ccls + (2 * num_vars));

	printf("tot_lits=%ld tot_vars=%ld tot_ccls=%ld \n", 
			THE_CNF->tot_lits, THE_CNF->tot_vars, THE_CNF->tot_ccls);

	THE_CNF->all_ccl = mc_malloc32(pre_cnf_node*, num_ccls);
	THE_CNF->all_pos = mc_malloc32(pre_cnf_node*, num_vars);
	THE_CNF->all_neg = mc_malloc32(pre_cnf_node*, num_vars);
	//THE_CNF->all_tmp_pre_load_nods = mc_malloc32(pre_cnf_node*, num_tmp_pre_load_nods);
	THE_CNF->all_tmp_pre_load_nods = (pre_cnf_node**) malloc(num_tmp_pre_load_nods * sizeof(pre_cnf_node*));

	THE_CNF->all_cnf = mc_malloc32(pre_cnf_net, num_workerus);
	for(int bb = 0; bb < num_workerus; bb++){ 
		new (&(THE_CNF->all_cnf[bb])) pre_cnf_net(); 
	} 

	init_node_arr(num_ccls, THE_CNF->all_ccl, nd_neu);
	init_node_arr(num_vars, THE_CNF->all_pos, nd_pos);
	init_node_arr(num_vars, THE_CNF->all_neg, nd_neg);

	memcpy(THE_CNF->all_tmp_pre_load_nods, THE_CNF->all_ccl, num_ccls * sizeof(pre_cnf_node*));
	memcpy(THE_CNF->all_tmp_pre_load_nods + num_ccls, THE_CNF->all_pos, num_vars * sizeof(pre_cnf_node*));
}

void
preload_cnf(long sz, const long* arr){
	PTD_CK(THE_CNF != mc_null);

	long num_ccls = THE_CNF->tot_ccls;
	long num_workerus = THE_CNF->tot_workerus;

	if(num_ccls == 0){
		return;
	}
	
	long num_tmp_pre_load_nods = THE_CNF->tot_tmp_pre_load_nods;

	long nn = 0;

	PTD_CK(nn < num_ccls);
	pre_cnf_node* ccl = THE_CNF->all_ccl[nn];
	ccl->id = nn;

	for(long ii = 0; ii < sz; ii++){
		long nio_id = arr[ii];
		PTD_CK(ccl != mc_null);
		
		if(nio_id != 0){
			pre_cnf_node* lit = get_lit(nio_id);
			PTD_CK(lit != NULL);

			agent_ref* rli = mc_agent_ref_acquire();
			rli->glb_agent_ptr = lit;
			ccl->all_agts.bind_to_my_left(*rli);
			ccl->pre_sz++;
			check_node_sz(ccl->pre_sz);

			agent_ref* rcl = mc_agent_ref_acquire();
			rcl->glb_agent_ptr = ccl;
			lit->all_agts.bind_to_my_left(*rcl);
			lit->pre_sz++;
			check_node_sz(lit->pre_sz);

			if(nio_id < 0){
				pre_cnf_node* opp = get_lit(-nio_id);
				opp->pre_sz++;
			}
		} else {
			nn++;
			if(nn < num_ccls){
				ccl = THE_CNF->all_ccl[nn];
				ccl->id = nn;
			} else {
				ccl = mc_null;
			}
		}
	}

	qsort(THE_CNF->all_tmp_pre_load_nods, num_tmp_pre_load_nods, sizeof(pre_cnf_node*), cmp_nodes);

	pre_sornode** all_sorinput = THE_CNF->all_pre_sorinput_nod;
	num_nod_t tod_nd = THE_CNF->tot_pre_sorinput_nod;
	MC_MARK_USED(tod_nd);
	
	num_nod_t pol_idx = 0;
	num_nod_t neu_idx = 0;
	
	long kk = 0;
	for(long aa = 0; aa < num_tmp_pre_load_nods; aa++){
		if(kk == num_workerus){ kk = 0; }
		pre_cnf_node* nod = THE_CNF->all_tmp_pre_load_nods[aa];
		pre_cnf_net& wu_cnf = THE_CNF->all_cnf[kk];
		PTD_CK(nod->is_alone());
		wu_cnf.tot_pre_rels += nod->pre_sz;
		switch(nod->ki){
			case nd_pos:
			{
				PTD_CK(nod->id > 0);
				pre_cnf_node* opp = get_lit(-(nod->id));

				nod->opp_nod = opp;
				opp->opp_nod = nod;

				PTD_CK(nod->loaded == mc_null);
				PTD_CK(opp->loaded == mc_null);

				wu_cnf.all_pre_pos.bind_to_my_left(*nod);
				wu_cnf.all_pre_neg.bind_to_my_left(*opp);

				wu_cnf.tot_pre_vars++;
			
				if(all_sorinput != mc_null){
					PTD_CK(pol_idx < tod_nd);
					nod->srt_nd.idx = pol_idx;
					nod->srt_nd.out = all_sorinput[pol_idx];
					pol_idx++;

					PTD_CK(pol_idx < tod_nd);
					opp->srt_nd.idx = pol_idx;
					opp->srt_nd.out = all_sorinput[pol_idx];
					pol_idx++;
				}
			}
			break;
			case nd_neu:
				wu_cnf.all_pre_neu.bind_to_my_left(*nod);
				wu_cnf.tot_pre_neus++;
				wu_cnf.tot_pre_lits += nod->pre_sz;

				if(all_sorinput != mc_null){
					PTD_CK(neu_idx < tod_nd);
					nod->srt_nd.idx = neu_idx;
					nod->srt_nd.out = all_sorinput[neu_idx];
					neu_idx++;
				}
			break;
			default:
				mck_abort(9, mc_cstr("Bad node cnf kind"));
			break;
		}
		kk++;
	}
}

void 
preload_sornet(){
	PTD_CK(THE_CNF != mc_null);

	long num_ccls = THE_CNF->tot_ccls;
	long num_vars = THE_CNF->tot_vars;
	
	num_nod_t num_rnks = mc_max((num_vars * 2), num_ccls);
	num_nod_t num_srts = num_rnks;
	BJ_DBG_SORNET_CODE(
		num_rnks = BJ_DBG_TOT_INPUT_RNKNODES;
		num_srts = BJ_DBG_TOT_INPUT_SORNODES;
	);
	
	create_ranknet(num_rnks);
	create_sornet(num_srts);
}



