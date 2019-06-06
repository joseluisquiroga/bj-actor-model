
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cell.hh"
#include "manageru_preload.hh"

pre_load_snet* THE_SORT_NET = mc_null;

grip ava_pre_sornode;
grip ava_pre_sort_net;

MCK_DEFINE_MEM_METHODS(pre_sornode, 32, ava_pre_sornode, 0)
MCK_DEFINE_MEM_METHODS(pre_sort_net, 32, ava_pre_sort_net, 0)

missive_handler_t sn_mgr_handlers[mgr_idx_total];
grip* sn_mgr_all_ava[mgr_idx_total];
mc_alloc_kernel_func_t sn_mgr_all_acq[mgr_idx_total];
mc_alloc_kernel_func_t sn_mgr_all_sep[mgr_idx_total];

void sort_net_mgr_init_handlers(){
	missive_handler_t* hndlrs = sn_mgr_handlers;
	mc_init_arr_vals(mgr_idx_total, hndlrs, mc_null);
	hndlrs[mgr_idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(mgr_idx_total);
	kernel::set_cell_handlers(hndlrs);

	mc_init_arr_vals(mgr_idx_total, sn_mgr_all_ava, mc_null);
	mc_init_arr_vals(mgr_idx_total, sn_mgr_all_acq, mc_null);
	mc_init_arr_vals(mgr_idx_total, sn_mgr_all_sep, mc_null);
	
	sn_mgr_all_ava[mgr_idx_pre_sornode] = &(ava_pre_sornode);
	sn_mgr_all_ava[mgr_idx_last_invalid] = mc_pt_invalid_available;
	
	sn_mgr_all_acq[mgr_idx_pre_sornode] = (mc_alloc_kernel_func_t)pre_sornode::acquire_alloc;
	sn_mgr_all_acq[mgr_idx_last_invalid] = kernel::invalid_alloc_func;

	sn_mgr_all_sep[mgr_idx_pre_sornode] = (mc_alloc_kernel_func_t)pre_sornode::separate;
	sn_mgr_all_sep[mgr_idx_last_invalid] = kernel::invalid_alloc_func;
	
	kernel::set_cell_mem_funcs(sn_mgr_all_ava, sn_mgr_all_acq, sn_mgr_all_sep);

	PTD_CK(pre_sornode::ck_cell_id(mgr_idx_pre_sornode));
	
	PTD_PRT("INITED_MEM_FUNCS \n");
}

num_nod_t
get_tot_levels(num_nod_t nn){
	double in = (double)nn;
	double lg = log2(in);
	double lv = lg * (lg + 1) / 2;
	return (num_nod_t)lv;
}

num_nod_t
get_bigger_pow2(num_nod_t nn){
	num_nod_t pp = 1;
	while(pp < nn){ pp <<= 1; }
	return pp;
}

pre_sornode*
create_node(sornod_kind_t knd, num_nod_t up_idx, num_nod_t down_idx, sornet_prms& prms){
	printf("create_node_flg1 \n");
	pre_sornode* nod = bj_pre_sornode_acquire();
	
	MCK_CK(nod != mc_null);
	
	mc_workeru_nn_t ptd_up_nid = 0;
	mc_workeru_nn_t ptd_down_nid = 0;
	MC_MARK_USED(ptd_up_nid);
	MC_MARK_USED(ptd_down_nid);
	
	prms.curr_nod_id++;
	nod->nod_id = prms.curr_nod_id;

	PTD_CK(up_idx < prms.tot_nods);
	PTD_CK(down_idx < prms.tot_nods);

	nod->up_idx = up_idx;
	nod->down_idx = down_idx;

	nod->out_up = prms.arr_nods[up_idx];
	nod->out_down = prms.arr_nods[down_idx];

	prms.arr_nods[up_idx] = nod;
	prms.arr_nods[down_idx] = nod;

	if(nod->out_up != mc_null){
		nod->level = nod->out_up->level + 1;
		PTD_CK(nod->out_down != mc_null);
		PTD_CK(nod->out_down->level == nod->out_up->level);

		ptd_up_nid = nod->out_up->nod_id;
		ptd_down_nid = nod->out_down->nod_id;

		nod->up_conn = nod->out_up->get_conn_kind(up_idx);
		nod->down_conn = nod->out_down->get_conn_kind(down_idx);
	}

	PTD_CK(nod->level >= 0);
	PTD_CK(nod->level < prms.tot_lvs);

	if(THE_SORT_NET != mc_null){
		long num_workerus = THE_SORT_NET->tot_workerus;
		mc_workeru_nn_t& nxt_nn = prms.arr_lvs[nod->level];
		PTD_CK(nxt_nn < num_workerus);

		nod->nod_nn = nxt_nn;
		
		PTD_CK(THE_SORT_NET->all_cnf != mc_null);

		pre_sort_net& local_sort_net = THE_SORT_NET->all_cnf[nxt_nn];
		local_sort_net.tot_pre_sornods++;
		local_sort_net.all_pre_sornods.bind_to_my_left(*nod);

		nxt_nn++;
		if(nxt_nn == num_workerus){ nxt_nn = 0; }
		PTD_CK(nxt_nn < num_workerus);
	}

	PTD_CK(knd != snod_invalid);
	PTD_PRT("%s_NOD (%ld)  %ld[%ld %ld] \t(%ld %ld) \n", ((knd == snod_alte)?("ALT"):("HLF")), nod->nod_id, 
			nod->level, up_idx, down_idx, ptd_up_nid, ptd_down_nid);
	ZNQ_CODE(printf("%s_NOD (%ld)  %ld[%ld %ld] \t(%d %d) \n", ((knd == snod_alte)?("ALT"):("HLF")), 
				nod->nod_id, nod->level, up_idx, down_idx, ptd_up_nid, ptd_down_nid));

	return nod;
}

void
create_net_half_cleaner(num_nod_t pos, num_nod_t sz, sornet_prms& prms){
	if(sz == 1){ return; }
	num_nod_t hlf = sz/2;
	for(num_nod_t ii = 0; ii < hlf; ii++){
		num_nod_t up_idx = pos + ii;
		num_nod_t down_idx = pos + ii + hlf;

		create_node(snod_half, up_idx, down_idx, prms);
	}
}

void
create_net_alt_half_cleaner(num_nod_t pos, num_nod_t sz, sornet_prms& prms){
	if(sz == 1){ return; }
	num_nod_t hlf = sz/2;
	for(num_nod_t ii = 0; ii < hlf; ii++){
		num_nod_t up_idx = pos + ii;
		num_nod_t down_idx = pos + (sz - ii - 1);

		create_node(snod_alte, up_idx, down_idx, prms);
	}
}

void
create_net_bitonic(num_nod_t pos, num_nod_t sz, sornet_prms& prms){
	if(sz == 1){ return; }
	num_nod_t hlf = sz/2;
	create_net_bitonic(pos, hlf, prms);
	create_net_bitonic(pos + hlf, hlf, prms);
	create_net_half_cleaner(pos, sz, prms);
}

void
create_net_merger(num_nod_t pos, num_nod_t sz, sornet_prms& prms){
	if(sz == 1){ return; }
	num_nod_t hlf = sz/2;
	create_net_bitonic(pos, hlf, prms);
	create_net_bitonic(pos + hlf, hlf, prms);
	create_net_alt_half_cleaner(pos, sz, prms);
}

void
create_net_sorter(num_nod_t pos, num_nod_t sz, sornet_prms& prms){
	if(sz == 1){ return; }
	num_nod_t hlf = sz/2;
	create_net_merger(pos, sz, prms);
	create_net_sorter(pos, hlf, prms);
	create_net_sorter(pos + hlf, hlf, prms);
}

void
create_sornet(num_nod_t num_to_sort){
	sornet_prms prms;

	//MUST be a power of 2
	prms.tot_nods = get_bigger_pow2(num_to_sort);
	prms.arr_nods = mc_malloc32(pre_sornode*, prms.tot_nods);
	mc_init_arr_vals(prms.tot_nods, prms.arr_nods, mc_null);

	PTD_CK(THE_SORT_NET != mc_null);
	long num_workerus = THE_SORT_NET->tot_workerus;
	THE_SORT_NET->all_cnf = mc_malloc32(pre_sort_net, num_workerus);
	for(int bb = 0; bb < num_workerus; bb++){ 
		new (&(THE_SORT_NET->all_cnf[bb])) pre_sort_net(); 
	} 

	THE_SORT_NET->tot_pre_sorinput_nod = prms.tot_nods;
	THE_SORT_NET->all_pre_sorinput_nod = prms.arr_nods;

	prms.tot_lvs = get_tot_levels(prms.tot_nods);

	//printf("prms.tot_lvs %ld prms.tot_nods %ld \n", prms.tot_lvs, prms.tot_nods);
	
	prms.arr_lvs = (mc_workeru_nn_t*)calloc(prms.tot_lvs, sizeof(mc_workeru_nn_t));

	create_net_sorter(0, prms.tot_nods, prms);

	ZNQ_CODE(printf("\nTOT_INPUT_NODS=%ld TOT_LVS=%ld \n", prms.tot_nods, prms.tot_lvs));
	PTD_PRT("\nTOT_INPUT_NODS=%ld TOT_LVS=%ld \n", prms.tot_nods, prms.tot_lvs);
}

void
link_all_modules_aux_fn() mc_external_code_ram;

void
link_all_modules_aux_fn(){
	agent_grp aux;
	agent aux2;
	//mc_manageru_main(0, mc_null);
}

