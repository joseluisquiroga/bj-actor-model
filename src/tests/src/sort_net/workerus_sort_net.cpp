
#include "cell.hh"
#include "workerus_sort_net.hh"
#include "manageru_preload.hh"

#define TOT_MODS 5

#define DBG_ONLY_IDX 0
#define CNF_LOADER_IDX 1
#define PROPAG_IDX 2
#define STABI_IDX 3
#define SORNET_IDX 4

//missive_handler_t bj_nil_handlers[1] = { mc_null };

char* all_mod_nams[TOT_MODS] mc_external_data_ram;
mc_addr_t* all_mod_addr;

void
init_module_nams() mc_external_code_ram;

void
init_module_nams(){
	all_mod_nams[DBG_ONLY_IDX] = mc_cstr("module1");
	all_mod_nams[CNF_LOADER_IDX] = mc_cstr("module2");
	all_mod_nams[PROPAG_IDX] = mc_cstr("module3");
	all_mod_nams[STABI_IDX] = mc_cstr("module4");
	all_mod_nams[SORNET_IDX] = mc_cstr("module5");
}

#define BJ_SORNET_TIER 0

MCK_DEFINE_ACQUIRE_ALLOC(sorting_net, 32);	// defines nervenet::acquire_alloc

MCK_DEFINE_MEM_METHODS(sornet_signal, 32, bj_ava_sornet_signals, 0)
MCK_DEFINE_MEM_METHODS(sorcell, 32, bj_ava_sorcells, 0)

BJ_DEFINE_GET_CLS_NAM(sornet_signal)
BJ_DEFINE_GET_CLS_NAM(sorcell)
BJ_DEFINE_GET_CLS_NAM(sorting_net)

sornet_signal::sornet_signal(){
	PTD_CK(bj_sorting_net != mc_null);
	//PTD_DBG_CODE(bj_sorting_net->all_dbg_dat.dbg_tot_new_sornet_signal ++);
	//mck_slog2("alloc__sornet_signal\n");
	//init_me();
} 

sornet_signal::~sornet_signal(){} 

void
sornet_signal::init_me(int caller){
	missive::init_me(caller);
	idx = 0;
	obj = mc_null;
}

sorcell::sorcell(){ 
	PTD_CK(bj_sorting_net != mc_null);
	//PTD_DBG_CODE(bj_sorting_net->all_dbg_dat.dbg_tot_new_sorcell ++);
	//mck_slog2("alloc__sorcell\n");
	init_me();
} 

sorcell::~sorcell(){} 

void
sorcell::init_me(int caller){
	handler_idx = idx_sorcell;

	color = 0;

	dbg_level = 0;

	up_idx = 0;
	up_inp = mc_null;
	up_out = mc_null;

	down_idx = 0;
	down_inp = mc_null;
	down_out = mc_null;
}

sorting_net::sorting_net(){
	MAGIC = MAGIC_VAL;

	handler_idx = idx_sorting_net;

	mc_init_arr_vals(idx_total, srt_handlers, mc_null);

	shd_cnf = mc_null;

	tot_loading = 0;
	tot_loaded = 0;

	tot_sorcells = 0;

	dbg_sornet_curr_cntr = 0;

	net_top = binval_top;
	net_bottom = binval_bottom;

	tot_input_sorcells = 0;
	all_input_sorcells = mc_null;
	tot_rcv_output_sorobjs = 0;
	all_output_sorobjs = mc_null;
}

sorting_net::~sorting_net(){} 


//--------------

void 
sorcell_sornet_handler(missive* msv){
	MCK_CALL_HANDLER(sorcell, sornet_handler, msv);
}

void 
sorting_net_sornet_handler(missive* msv){
	MCK_CALL_HANDLER(sorting_net, sornet_handler, msv);
}

void sort_net_wrk_init_handlers() mc_external_code_ram;

void
sorting_net::init_mem_funcs(){
	mc_init_arr_vals(idx_total, all_ava, mc_null);
	mc_init_arr_vals(idx_total, all_acq, mc_null);
	mc_init_arr_vals(idx_total, all_sep, mc_null);
	
	all_ava[idx_sornet_signal] = &(ava_sornet_signals);
	all_ava[idx_sorcell] = &(ava_sorcells);
	all_ava[idx_last_invalid] = mc_pt_invalid_available;
	
	all_acq[idx_sornet_signal] = (mc_alloc_kernel_func_t)sornet_signal::acquire_alloc;
	all_acq[idx_sorcell] = (mc_alloc_kernel_func_t)sorcell::acquire_alloc;
	all_acq[idx_last_invalid] = kernel::invalid_alloc_func;

	all_sep[idx_sornet_signal] = (mc_alloc_kernel_func_t)sornet_signal::separate;
	all_sep[idx_sorcell] = (mc_alloc_kernel_func_t)sorcell::separate;
	all_sep[idx_last_invalid] = kernel::invalid_alloc_func;
	
	kernel::set_cell_mem_funcs(all_ava, all_acq, all_sep);
	
	PTD_CK(sornet_signal::ck_cell_id(idx_sornet_signal));
	PTD_CK(sorcell::ck_cell_id(idx_sorcell));	
}

void sort_net_wrk_init_handlers(){
	missive_handler_t* hndlrs = bj_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_sorcell] = sorcell_sornet_handler;
	hndlrs[idx_sorting_net] = sorting_net_sornet_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);	

	bj_sorting_net->init_mem_funcs();
}


#define bj_pt_obj_as_bin(pt_oo) (*((binval_t*)(pt_oo)))

int
bj_cmp_bin_objs(void* obj1, void* obj2){
	binval_t v1 = bj_pt_obj_as_bin(obj1);
	binval_t v2 = bj_pt_obj_as_bin(obj2);
	if(v1 < v2){ return -1; }
	if(v1 > v2){ return 1;}
	return 0;
}

bj_cmp_obj_func_t
sorcell::sornet_get_cmp_func(sornet_tok_t tmt_tok){
	return &bj_cmp_bin_objs;
}

void
sorcell::sornet_handler(missive* msv){
	sornet_signal* sn_tmt = (sornet_signal*)msv;
	sornet_tok_t tmt_tok = (sornet_tok_t)(sn_tmt->tok);
	num_nod_t tmt_idx = sn_tmt->idx;
	void* tmt_obj = sn_tmt->obj;

	bj_cmp_obj_func_t fn = sornet_get_cmp_func(tmt_tok);

	if(tmt_idx == up_idx){
		PTD_CK(up_inp == mc_null);
		up_inp = tmt_obj;
	}
	if(tmt_idx == down_idx){
		PTD_CK(down_inp == mc_null);
		down_inp = tmt_obj;
	}

	cell* src = this;
	if((up_inp != mc_null) && (down_inp != mc_null)){
		//int cv = bj_cmp_bin_objs(up_inp, down_inp);
		int cv = (*fn)(up_inp, down_inp);
		if(cv < 0){
			bj_send_sornet_tmt(src, tmt_tok, up_inp, up_out, up_idx);
			bj_send_sornet_tmt(src, tmt_tok, down_inp, down_out, down_idx);
		} else {
			bj_send_sornet_tmt(src, tmt_tok, up_inp, down_out, down_idx);
			bj_send_sornet_tmt(src, tmt_tok, down_inp, up_out, up_idx);
		}

		up_inp = mc_null;
		down_inp = mc_null;

		//PTD_LOG("SCELL %d(%d %d) fired\n", dbg_level, up_idx, down_idx);
	}
}

void
bj_send_sornet_tmt(cell* src, sornet_tok_t tok, void* obj, sorcell* dst, num_nod_t idx){
	PTD_CK(src != mc_null);
	PTD_CK(obj != mc_null);

	sornet_signal* trm = bj_sornet_signal_acquire();

	trm->src = src;
	trm->dst = dst;
	trm->tok = tok;
	trm->idx = idx;
	trm->obj = obj;

	if(trm->dst == mc_null){
		sorting_net* root_net = bj_sorting_net->get_sorting_net(mc_nn_to_id(0));
		trm->dst = root_net;
		trm->tok = bj_tok_sornet_out;
	}

	trm->send();
}

bool
sorting_net::sornet_dbg_send_cntr(){
	PTD_CK(kernel::get_workeru_nn() == 0);
	PTD_CK(all_input_sorcells != mc_null);
	
	dbg_sornet_curr_cntr++;

	if(dbg_sornet_curr_cntr >= dbg_sornet_max_cntr){
		return false;
	}

	mini_bit_arr_t tmp_num = dbg_sornet_curr_cntr;

	cell* src = this;

	PTD_CK(((mini_bit_arr_t)(sizeof(mini_bit_arr_t) * 8)) > ((mini_bit_arr_t)tot_input_sorcells));
	PTD_LOG("send_cntr counter=%d tot_inp=%d sizeof(mini_bit_arr_t)=%d \n", 
			tmp_num, tot_input_sorcells, sizeof(mini_bit_arr_t));

	num_nod_t aa;
	for(aa = 0; aa < tot_input_sorcells; aa++){
		bool vv = mc_get_bit(&tmp_num, aa);
		//PTD_LOG("send_cntr vv=%d \n", vv);

		sorcell* srcll = all_input_sorcells[aa];
		PTD_CK(srcll != mc_null);

		void* obj = mc_null;
		if(vv){
			obj = mck_as_glb_pt(&net_top);
		} else {
			obj = mck_as_glb_pt(&net_bottom);
		}
		PTD_CK(obj != mc_null);
		PTD_CK(srcll != mc_null);
		PTD_CK(src != mc_null);

		bj_send_sornet_tmt(src, bj_tok_sornet_bin, obj, srcll, aa);
	}

	PTD_LOG("send_cntr_end\n");

	return true;
}

bool
sorting_net::sornet_check_order(bj_cmp_obj_func_t fn){
	//PTD_LOG("sornet_check_order_beg\n");

	num_nod_t aa;
	bool sor_ok = true;
	for(aa = 1; aa < tot_input_sorcells; aa++){
		void* o1 = all_output_sorobjs[aa - 1];
		void* o2 = all_output_sorobjs[aa];
		int cv = (*fn)(o1, o2);
		if(cv > 0){
			sor_ok = false;
			break;
		}
	}
	return sor_ok;
}

mini_bit_arr_t
sorting_net::sornet_dbg_bin_get_mini_sorted_arr(){
	mini_bit_arr_t min_arr = 0;

	PTD_CK(((mini_bit_arr_t)(sizeof(mini_bit_arr_t) * 8)) > ((mini_bit_arr_t)tot_input_sorcells));

	num_nod_t aa;
	for(aa = 0; aa < tot_input_sorcells; aa++){
		void* obj = all_output_sorobjs[aa];
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
sorting_net::sornet_dbg_end_step(){
	PTD_CK(kernel::get_workeru_nn() == 0);

	bool srt_ok = sornet_check_order(&bj_cmp_bin_objs);
	if(! srt_ok){
		mck_abort(__LINE__, MC_ABORT_MSG("SORNET_ERROR\n"));
	}
	
	mini_bit_arr_t min_arr = sornet_dbg_bin_get_mini_sorted_arr();
	char* pt_arr = (char*)(&min_arr);
	MC_MARK_USED(pt_arr);
	PTD_PRT("SORTED_OK_ARR_%d_" BJ_SORTED_PATTERN "\n", dbg_sornet_curr_cntr, BJ_SORTED_DATA);
	PTD_LOG("SORTED_OK_ARR_%d_" BJ_SORTED_PATTERN "\n", dbg_sornet_curr_cntr, BJ_SORTED_DATA);
	mck_slog2("sorted_ok_arr_");
	mck_ilog(dbg_sornet_curr_cntr);
	mck_slog2("\n");
	mck_sprt2("sorted_ok_arr_");
	mck_iprt(dbg_sornet_curr_cntr);
	mck_sprt2("\n");

	mc_init_arr_vals(tot_input_sorcells, all_output_sorobjs, mc_null);
	tot_rcv_output_sorobjs = 0;

	sorting_net* my_net = this;
	send(my_net, bj_tok_sornet_start);
}

void
sorting_net::sornet_dbg_bin_handler(missive* msv){
	PTD_CK(kernel::get_workeru_nn() == 0);
	//PTD_LOG("sornet_dbg_bin_handler 1\n");

	sornet_signal* sn_tmt = (sornet_signal*)msv;
	sornet_tok_t tmt_tok = (sornet_tok_t)(sn_tmt->tok);
	num_nod_t tmt_idx = sn_tmt->idx;
	void* tmt_obj = sn_tmt->obj;

	if(tmt_tok == bj_tok_sornet_start){
		bool has_more = sornet_dbg_send_cntr();
		if(! has_more){
			send_sync_to_children(bj_tok_sync_to_children);
		}
	}
	if(tmt_tok == bj_tok_sornet_out){
		PTD_CK(tmt_idx < tot_input_sorcells);
		PTD_CK(all_output_sorobjs[tmt_idx] == mc_null);
		all_output_sorobjs[tmt_idx] = tmt_obj;

		tot_rcv_output_sorobjs++;
		if(tot_rcv_output_sorobjs == tot_input_sorcells){
			sornet_dbg_end_step();
		}
	}
}

void bj_sornet_kernel_func(){
	sorting_net* my_net = bj_sorting_net;
	if(my_net->sync_is_ending){
		kernel::stop_sys(bj_tok_sornet_end);
	}
}

void
sorting_net::sornet_handler(missive* msv){
	mck_token_t tmt_tok = msv->tok;
	if(bj_tok_sync_to_children == tmt_tok){
		send_sync_to_children(bj_tok_sync_to_children);
		return;
	}

	sornet_dbg_bin_handler(msv);
}

void 
sorting_net::send_sync_to_children(sornet_tok_t the_tok)
{
	SYNC_LOG(" SYNCR_STOP_CHILDREN WORKERU=%d \n", kernel::get_workeru_nn());

	mc_load_map_st** my_children = bj_sorting_net->sync_map->childs;
	if(my_children != mc_null){ 
		int aa = 0;
		mc_load_map_st* ch_map = (my_children)[aa];
		while(ch_map != mc_null){
			mc_workeru_nn_t chd_nn = ch_map->num_workeru;
			sorting_net* ch_net = bj_sorting_net->get_sorting_net(mc_nn_to_id(chd_nn));
			send_sync_transmitter(ch_net, the_tok);

			aa++;
			ch_map = (my_children)[aa];
		}
	}

	if(the_tok == bj_tok_sync_to_children){
		PTD_LOG(" SYNCR_END \n");

		sync_is_ending = true;
	}
}

void
sorting_net::send_sync_transmitter(sorting_net* the_dst, sornet_tok_t the_tok)
{
	sornet_signal* trm = bj_sornet_signal_acquire();
	
	trm->src = bj_sorting_net;
	trm->dst = the_dst;
	trm->tok = the_tok;
	trm->send();

	PTD_CODE(mc_workeru_nn_t dbg_dst_nn = mc_id_to_nn(mc_addr_get_id(the_dst)); MC_MARK_USED(dbg_dst_nn););
	SYNC_LOG(" SYNCR_send_transmitter_%s_ [%ld ->> %ld] \n", 
		tok_to_str(the_tok), kernel::get_workeru_nn(), dbg_dst_nn);
}

#define bj_get_loaded_of_sornode(pt) (((pre_sornode*)(pt))->loaded)

#define bj_set_sorcell_pt(nod, out, reset) \
	if(nod != mc_null){ \
		while(bj_get_loaded_of_sornode(nod) == mc_null){ \
			/* SPIN UNTIL SET (may be set by an other workeru) */ \
			PTD_CODE(sched_yield()); \
		} \
		PTD_CK(bj_get_loaded_of_sornode(nod) != mc_null); \
		out = (sorcell*)bj_get_loaded_of_sornode(nod); \
		if(reset){ nod = mc_null; } \
	} \

// end_macro

void bj_load_shd_sornet(){
	sorting_net* my_net = bj_sorting_net;
	pre_sort_net* nn_cnf = bj_sorting_net->shd_cnf;

	num_nod_t tot_sclls = nn_cnf->tot_pre_sornods;

	my_net->tot_sorcells = tot_sclls;

	sorcell::separate(tot_sclls);

	PTD_LOG("Separated_sorcells %ld\n", tot_sclls);

	//num_nod_t tot_pre_sornods;
	//grip	all_pre_sornods;

	binder * fst, * lst, * wrk;

	binder* nn_all_nods = &(nn_cnf->all_pre_sornods); // nn_cnf is already workeru_pt so nn_all_nods is workeru_pt
	fst = (binder*)mc_manageru_pt_to_workeru_pt(nn_all_nods->bn_right);
	lst = nn_all_nods;
	for(wrk = fst; wrk != lst; wrk = (binder*)mc_manageru_pt_to_workeru_pt(wrk->bn_right)){
		pre_sornode* nod = (pre_sornode*)wrk;

		sorcell* scll = bj_sorcell_acquire();

		scll->dbg_level = nod->level;

		PTD_CK(scll->up_inp == mc_null);
		scll->up_idx = nod->up_idx;
		if(nod->out_up != mc_null){
			scll->up_inp = (void*)mc_manageru_pt_to_workeru_pt(nod->out_up);	// up_inp used as tmp
		}

		PTD_CK(scll->down_inp == mc_null);
		scll->down_idx = nod->down_idx;
		if(nod->out_down != mc_null){
			scll->down_inp = (void*)mc_manageru_pt_to_workeru_pt(nod->out_down);	// down_inp used as tmp
		}

		my_net->all_sorcells.bind_to_my_left(*scll);

		PTD_CK(nod->loaded == mc_null);
		nod->loaded = mck_as_glb_pt(scll);
	}

	binder* nn_all_sclls = &(my_net->all_sorcells); // nn_cnf is already workeru_pt so nn_all_sclls is workeru_pt
	fst = (binder*)(nn_all_sclls->bn_right);
	lst = nn_all_sclls;
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		sorcell* scll = (sorcell*)wrk;

		bj_set_sorcell_pt(scll->up_inp, scll->up_out, true);
		bj_set_sorcell_pt(scll->down_inp, scll->down_out, true);

		PTD_CK(scll->up_inp == mc_null);
		PTD_CK(scll->down_inp == mc_null);
	};

	kernel* ker = mck_get_kernel();
	mc_workeru_nn_t nn = kernel::get_workeru_nn();

	if(nn == 0){
		pre_load_snet* pre_cnf = (pre_load_snet*)(ker->manageru_load_data);
		pre_sornode** all_input = 
			(pre_sornode**)mc_manageru_addr_to_workeru_addr((mc_addr_t)(pre_cnf->all_pre_sorinput_nod));

		num_nod_t tot_sorinp = pre_cnf->tot_pre_sorinput_nod;
		//PTD_LOG("TOT_INPUT_SORCELLS=%ld \n", tot_sorinp);
		mck_slog2("TOT_INPUT_SORCELLS=");
		mck_ilog(tot_sorinp);	
		mck_slog2("\n");

		my_net->tot_input_sorcells = tot_sorinp;
		my_net->all_input_sorcells = mc_malloc32(sorcell*, tot_sorinp);
		mc_init_arr_vals(tot_sorinp, my_net->all_input_sorcells, mc_null);

		my_net->all_output_sorobjs = mc_malloc32(void*, tot_sorinp);
		mc_init_arr_vals(tot_sorinp, my_net->all_output_sorobjs, mc_null);

		sorcell** all_sorcell = my_net->all_input_sorcells;
		
		num_nod_t aa;
		for(aa = 0; aa < tot_sorinp; aa++){
			sorcell* inp_aa = (sorcell*)mc_manageru_addr_to_workeru_addr((mc_addr_t)(all_input[aa]));
			bj_set_sorcell_pt(inp_aa, all_sorcell[aa], false);
		}
	}

	//mck_slog2("end_of_bj_load_shd_sornet  \n");	
}

sorting_net*
sorting_net::get_sorting_net(mc_workeru_id_t id){
	sorting_net* rem_net = (sorting_net*)mc_addr_set_id(id, this);
	return rem_net;
}

void
sorting_net::init_sync_cycle(){
	sync_tot_children = mc_map_get_tot_children();
	sync_parent_id = mc_map_get_parent_workeru_id();
	sync_map = mc_map_get_loaded();

	//act_left_side.init_sync();
	//act_right_side.init_sync();
}

char* global_err_string mc_external_data_ram;

void bj_init_sorting_net(){
	kernel* ker = mck_get_kernel();
	mc_workeru_nn_t nn = kernel::get_workeru_nn();
	MC_MARK_USED(nn);

	if(ker->magic_id != MC_MAGIC_ID){
		mck_slog2("BAD_MAGIC\n");
	}
	MCK_CK(ker->magic_id == MC_MAGIC_ID);

	sorting_net* my_net = sorting_net::acquire_alloc();
	if(my_net == mc_null){
		mck_abort(1, mc_cstr("CAN NOT INIT GLB WORKERU DATA"));
	}
	ker->user_data = my_net;

	mck_slog2("manageru_load_data = ");
	mck_xlog((mc_addr_t)ker->manageru_load_data);
	mck_slog2("\n");

	mck_slog2("mgrker_manageru_load_data = ");
	mck_xlog((mc_addr_t)(ker->manageru_kernel->manageru_load_data));
	mck_slog2("\n");

	MCK_CK(ker->manageru_load_data != mc_null);
	
	pre_load_snet* pre_cnf = (pre_load_snet*)(ker->manageru_load_data);
	if(pre_cnf->MAGIC != MAGIC_VAL){
		mck_slog2("BAD_MAGIC_2\n");
	}
	MCK_CK(pre_cnf->MAGIC == MAGIC_VAL);

	pre_sort_net* nn_cnf = 
		(pre_sort_net*)mc_manageru_addr_to_workeru_addr((mc_addr_t)(pre_cnf->all_cnf + nn));
		
	bj_sorting_net->shd_cnf = nn_cnf;
}

void mc_workerus_main() {
	kernel::init_sys();
	mc_workeru_nn_t nn = kernel::get_workeru_nn();

	//PTD_PRT("STARTING_SORNET___ \n");
	mck_slog2("STARTING_SORNET___ \n");
	
	bj_init_sorting_net();
	
	PTD_CK(bj_sorting_net != mc_null);
	
	sort_net_wrk_init_handlers();

	PTD_LOG("SORNET___ %d \n", nn);

	kernel* ker = mck_get_kernel();
	ker->user_func = bj_sornet_kernel_func;

	sorting_net* my_net = bj_sorting_net;
	PTD_CK(my_net != mc_null);

	my_net->init_sync_cycle();

	sornet_signal::separate(my_net->tot_sorcells * 2);

	//mck_slog2("__dbg2.sornet\n");
	
	bj_load_shd_sornet();

	if(nn == 0){
		mini_bit_arr_t max_cntr = (((mini_bit_arr_t)1) << BJ_DBG_TOT_INPUT_SORNODES) + 1;
		my_net->dbg_sornet_max_cntr = max_cntr;
		my_net->send(my_net, bj_tok_sornet_start);

		PTD_PRT("dbg_sornet_max_cntr=%d\n", max_cntr);
		PTD_LOG("dbg_sornet_max_cntr=%d\n", max_cntr);
	}
	kernel::run_sys();
	
	//bj_print_active_cnf(side_left, tiki_invalid, mc_cstr("snt_after"), 3, 0, 
	//	bj_dbg_prt_nd_neu_flag | bj_dbg_prt_nd_pol_flag);

	//PTD_PRT("...............................END_SORNET\n");
	mck_slog2("END_SORNET___");
	mck_ilog(nn);
	mck_slog2("_________________________\n");
	//mck_sprt2("dbg2.sornet.end\n");

	kernel::finish_sys();
}

