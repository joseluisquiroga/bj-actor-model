
#include "cell.hh"
#include "nervenet.hh"

/*
-------------------------------------------------------------
PLLA class_sizes:
synset__40__
tierset__32__
base_transmitter__32__
synapse__48__
side_state__104__
nervenode__240__
neuron__240__
polaron__248__
sorcell__56__
endcell__??__
pgrp_item__??__
pgroup__??__
tierdata__56__
netstate__48__
dbg_stats__32__
nervenet__360__
void_pt__4__
binder__8__
num_syn_t__1__
grip__8__
num_tier_t__4__
mc_flags_t__1__
kernel__328__
agent__16__
cell__24__
missive__32__
agent_ref__24__
agent_grp__32__
test_cls__8__
-------------------------------------------------------------
*/

missive_handler_t bj_nil_handlers[1] = { mc_null };

MCK_DEFINE_ACQUIRE_ALLOC(nervenet, 32);	// defines nervenet::acquire_alloc

MCK_DEFINE_MEM_METHODS(base_transmitter, 32, bj_ava_base_transmitters, 0)
MCK_DEFINE_MEM_METHODS(synset, 32, bj_ava_synsets, 0)
MCK_DEFINE_MEM_METHODS(tierset, 32, bj_ava_tiersets, bj_num_sep_tiersets)
MCK_DEFINE_MEM_METHODS(synapse, 32, bj_ava_synapses, 0)
MCK_DEFINE_MEM_METHODS(neuron, 32, bj_ava_neurons, 0)
MCK_DEFINE_MEM_METHODS(polaron, 32, bj_ava_polarons, 0)
MCK_DEFINE_MEM_METHODS(sorcell, 32, bj_ava_sorcells, 0)
MCK_DEFINE_MEM_METHODS(endcell, 32, bj_ava_endcells, 0)
MCK_DEFINE_MEM_METHODS(pgrp_item, 32, bj_ava_pgrp_items, 0)
MCK_DEFINE_MEM_METHODS(pgroup, 32, bj_ava_pgroups, 0)
MCK_DEFINE_MEM_METHODS(tierdata, 32, bj_ava_tierdatas, bj_num_sep_tierdatas)
MCK_DEFINE_MEM_METHODS(layerdata, 32, bj_ava_layerdatas, bj_num_sep_layerdatas)

BJ_DEFINE_GET_CLS_NAM(synset)
BJ_DEFINE_GET_CLS_NAM(tierset)
BJ_DEFINE_GET_CLS_NAM(base_transmitter)
BJ_DEFINE_GET_CLS_NAM(synapse)
BJ_DEFINE_GET_CLS_NAM(nervenode)
BJ_DEFINE_GET_CLS_NAM(neuron)
BJ_DEFINE_GET_CLS_NAM(polaron)
BJ_DEFINE_GET_CLS_NAM(sorcell)
BJ_DEFINE_GET_CLS_NAM(endcell)
BJ_DEFINE_GET_CLS_NAM(pgrp_item)
BJ_DEFINE_GET_CLS_NAM(pgroup)
BJ_DEFINE_GET_CLS_NAM(tierdata)
BJ_DEFINE_GET_CLS_NAM(layerdata)
BJ_DEFINE_GET_CLS_NAM(nervenet)

void
nervenet::init_mem_funcs(){
	mc_init_arr_vals(idx_total, all_ava, mc_null);
	mc_init_arr_vals(idx_total, all_acq, mc_null);
	mc_init_arr_vals(idx_total, all_sep, mc_null);
	
	all_ava[idx_base_transmitter] = &(ava_base_transmitters);
	all_ava[idx_synset] = &(ava_synsets);
	all_ava[idx_tierset] = &(ava_tiersets);
	all_ava[idx_synapse] = &(ava_synapses);
	all_ava[idx_neuron] = &(ava_neurons);
	all_ava[idx_polaron] = &(ava_polarons);
	all_ava[idx_sorcell] = &(ava_sorcells);
	all_ava[idx_endcell] = &(ava_endcells);
	all_ava[idx_pgrp_item] = &(ava_pgrp_items);
	all_ava[idx_pgroup] = &(ava_pgroups);
	all_ava[idx_tierdata] = &(ava_tierdatas);
	all_ava[idx_layerdata] = &(ava_layerdatas);
	all_ava[idx_last_invalid] = mc_pt_invalid_available;
	
	all_acq[idx_base_transmitter] = (mc_alloc_kernel_func_t)base_transmitter::acquire_alloc;
	all_acq[idx_synset] = (mc_alloc_kernel_func_t)synset::acquire_alloc;
	all_acq[idx_tierset] = (mc_alloc_kernel_func_t)tierset::acquire_alloc;
	all_acq[idx_synapse] = (mc_alloc_kernel_func_t)synapse::acquire_alloc;
	all_acq[idx_neuron] = (mc_alloc_kernel_func_t)neuron::acquire_alloc;
	all_acq[idx_polaron] = (mc_alloc_kernel_func_t)polaron::acquire_alloc;
	all_acq[idx_sorcell] = (mc_alloc_kernel_func_t)sorcell::acquire_alloc;
	all_acq[idx_pgrp_item] = (mc_alloc_kernel_func_t)pgrp_item::acquire_alloc;
	all_acq[idx_pgroup] = (mc_alloc_kernel_func_t)pgroup::acquire_alloc;
	all_acq[idx_tierdata] = (mc_alloc_kernel_func_t)tierdata::acquire_alloc;
	all_acq[idx_layerdata] = (mc_alloc_kernel_func_t)layerdata::acquire_alloc;
	all_acq[idx_last_invalid] = kernel::invalid_alloc_func;

	all_sep[idx_base_transmitter] = (mc_alloc_kernel_func_t)base_transmitter::separate;
	all_sep[idx_synset] = (mc_alloc_kernel_func_t)synset::separate;
	all_sep[idx_tierset] = (mc_alloc_kernel_func_t)tierset::separate;
	all_sep[idx_synapse] = (mc_alloc_kernel_func_t)synapse::separate;
	all_sep[idx_neuron] = (mc_alloc_kernel_func_t)neuron::separate;
	all_sep[idx_polaron] = (mc_alloc_kernel_func_t)polaron::separate;
	all_sep[idx_sorcell] = (mc_alloc_kernel_func_t)sorcell::separate;
	all_sep[idx_pgrp_item] = (mc_alloc_kernel_func_t)pgrp_item::separate;
	all_sep[idx_pgroup] = (mc_alloc_kernel_func_t)pgroup::separate;
	all_sep[idx_tierdata] = (mc_alloc_kernel_func_t)tierdata::separate;
	all_sep[idx_layerdata] = (mc_alloc_kernel_func_t)layerdata::separate;
	all_sep[idx_last_invalid] = kernel::invalid_alloc_func;
	
	kernel::set_cell_mem_funcs(all_ava, all_acq, all_sep);

	PTD_CK(base_transmitter::ck_cell_id(idx_base_transmitter));
	PTD_CK(synset::ck_cell_id(idx_synset));
	PTD_CK(tierset::ck_cell_id(idx_tierset));
	PTD_CK(synapse::ck_cell_id(idx_synapse));
	PTD_CK(neuron::ck_cell_id(idx_neuron));
	PTD_CK(polaron::ck_cell_id(idx_polaron));
	PTD_CK(sorcell::ck_cell_id(idx_sorcell));
	PTD_CK(pgrp_item::ck_cell_id(idx_pgrp_item));
	PTD_CK(pgroup::ck_cell_id(idx_pgroup));
	PTD_CK(tierdata::ck_cell_id(idx_tierdata));
	PTD_CK(layerdata::ck_cell_id(idx_layerdata));
}

//===================================================================================================
#ifdef MC_IS_PTD_CODE

#define BJ_AUX_STR_MIN_SZ 3

#define bj_aux_stabi_append_str(str, num_prt, pt_str, nxt_sz, ...) \
	num_prt = snprintf(pt_str, nxt_sz, __VA_ARGS__); \
	if(num_prt > (nxt_sz - BJ_AUX_STR_MIN_SZ)){ \
		return str; \
	} \
	pt_str = strchr(pt_str, '\0'); \
	nxt_sz -= num_prt; \

// end_of_macro

char* bj_dbg_stabi_col_arr_to_str(num_syn_t sz1, num_nod_t* arr1, int sz_str, char* str){
	PTD_CK(str != mc_null);
	PTD_CK(sz_str > BJ_AUX_STR_MIN_SZ);

	memset(str, 0, sz_str);
	str[sz_str - 1] = '\0';

	char* pt_str = str;
	int num_prt = 0;
	int nxt_sz = sz_str;

	bj_aux_stabi_append_str(str, num_prt, pt_str, nxt_sz, "[");

	if(arr1 != mc_null){
		for(num_syn_t aa = 0; (aa < sz1); aa++){
			bj_aux_stabi_append_str(str, num_prt, pt_str, nxt_sz, "%ld ", arr1[aa]);
		}
	}

	bj_aux_stabi_append_str(str, num_prt, pt_str, nxt_sz, "]");

	return str;
}

void
nervenode::dbg_prt_snp_id(callee_prms& pms){
	bj_dbg_str_stream* out = (bj_dbg_str_stream*)(pms.aux);
	*out << get_ki_str() << id;
}

void
nervenode::dbg_prt_active_synset(net_side_t sd, tier_kind_t tiki, char* prefix, num_tier_t num_ti){
	char* ts = bj_dbg_tier_kind_to_str(tiki);

	PTD_LOG(" %s_%s_t%d_%s_%d_%s \n", ts, prefix, num_ti, get_ki_str(), id, "GET_RID_OF_PRT_SYNSET");
}


#endif
//---------------------------------------------------------------------------------------------------

nervenet::nervenet(){
	MAGIC = MAGIC_VAL;

	tot_wu_neus = 0;
	tot_wu_vars = 0;
	tot_wu_lits = 0;
	tot_wu_rels = 0;

	num_sep_tiersets = 10;
	num_sep_tierdatas = 10;
	num_sep_layerdatas = 10;

	handler_idx = idx_nervenet;

	mc_init_arr_vals(idx_total, all_net_handlers, mc_null);

	shd_cnf = mc_null;

	tot_loading = 0;
	tot_loaded = 0;

	act_left_side.my_side = side_left;
	act_right_side.my_side = side_right;
	
	stabi_min_pol = mc_null;
	stabi_max_pol = mc_null;
	
	stabi_wu_flags = 0;
	
	stabi_wu_tot_nod_sep = 0;
	stabi_wu_num_rcv_sep = 0;

	tot_wu_sorcells = 0;
	tot_wu_rnkcells = 0;
	tot_wu_srt_endcells = 0;
	tot_wu_rnk_endcells = 0;
	
	dbg_sornet_curr_cntr = 0;
	dbg_sornet_max_cntr = 0;
	dbg_all_input_vals = mc_null;

	dbg_net_top = binval_top;
	dbg_net_bottom = binval_bottom;

	dbg_tot_input_sorcells = 0;
	dbg_all_input_sorcells = mc_null;
	dbg_all_input_srt_min_grps = mc_null;
	dbg_all_input_srt_max_grps = mc_null;
	
	dbg_tot_grp_items = 0;
	dbg_idx_grp_test = 0;
	
	dbg_tot_rcv_output_sorobjs = 0;
	
	dbg_all_srt_endcells = mc_null;
	
	dbg_all_output_sorobjs = mc_null;
	dbg_all_output_sorcols = mc_null;

	dbg_tot_input_rnkcells = 0;
	dbg_all_input_rnkcells = mc_null;
	
	dbg_all_input_rnkgrps = mc_null;
	dbg_all_input_rnk_min_grps = mc_null;
	dbg_all_input_rnk_max_grps = mc_null;
	
	dbg_all_output_rnk_min_cols = mc_null;
	dbg_all_output_rnk_max_cols = mc_null;
	dbg_is_input_rnkgrps = mc_null;
}

nervenet::~nervenet(){} 

base_transmitter::base_transmitter(){
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_base_transmitter ++);
	mck_slog2("alloc__base_transmitter\n");
	//init_me();
} 

base_transmitter::~base_transmitter(){} 

void
base_transmitter::init_me(int caller){
	missive::init_me(caller);
	d.prp.wrk_side = side_invalid;
	d.prp.wrk_tier = BJ_INVALID_NUM_TIER;
	//PTD_LOG_STACK((kernel::get_workeru_nn() == 15), "INIT_BASE_TRANSMITTER (%p) \n", (void*)this); 
}

void
base_transmitter::init_sornet_transmitter(){
	d.srt.knd = sorkind_invalid;
	d.srt.idx = 0;
	d.srt.inp = mc_null;
	d.srt.max_col = BJ_INVALID_SRT_GRP;
	d.srt.min_col = BJ_INVALID_SRT_GRP;
	d.srt.min_grp = BJ_INVALID_SRT_GRP;
	d.srt.max_grp = BJ_INVALID_SRT_GRP;
}

//--------------

synset::synset(){
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_synset ++);
	mck_slog2("alloc__synset\n");
	init_me();
} 

synset::~synset(){} 

void
synset::init_me(int caller){
	PTD_DBG_CODE(ss_flags = 0);

	num_ss_recv = 0;
	num_ss_ping = 0;

	tot_syn = 0;
}

void 
synset::add_left_synapse(synapse* snp){
	PTD_CK(snp != mc_null);
	tot_syn++;
	all_syn.bind_to_my_left(*snp);
}

void 
synset::add_right_synapse(synapse* snp){
	PTD_CK(snp != mc_null);
	tot_syn++;
	all_syn.bind_to_my_left(snp->right_handle);
}

tierset::tierset(){
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_tierset ++);
	mck_slog2("alloc__tierset\n");
	init_me();
} 

tierset::~tierset(){} 

void
tierset::init_me(int caller){
	ti_id = BJ_INVALID_NUM_TIER;
}

synapse::synapse(){
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_synapse ++);
	mck_slog2("alloc__synapse\n");
	init_me();
} 

synapse::~synapse(){} 

void
synapse::init_me(int caller){
	handler_idx = idx_synapse;

	owner = mc_null;
	mate = mc_null;
}

side_state::side_state(){ 
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_side_state ++);
	mck_slog2("alloc__side_state\n");
	init_me();
} 

side_state::~side_state(){} 

void
side_state::init_me(int caller){
	//side_kind = side_invalid;
	propag_num_tier = 0;

	propag_source = mc_null;

	step_prev_tot_active = 0;

	step_flags = 0;

	step_num_complete = 0;
	step_num_ping = 0;
}

nervenode::nervenode(){ 
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_nervenode ++);
	mck_slog2("alloc__nervenode\n");
	init_me();
} 

nervenode::~nervenode(){} 

void
nervenode::init_me(int caller){
	ki = nd_invalid; 
	id = 0; 
	sz = 0;
	creat_tier = 0;

	//left_side.side_kind = side_left;
	//right_side.side_kind = side_right;

	stabi_num_tier = 0;
	
	stabi_flags = 0;
	stabi_tmp_tier = BJ_INVALID_NUM_TIER;
	stabi_arr_sz = 0;
	stabi_arr_dat = mc_null;
	stabi_prv_arr_sz = 0;
	stabi_prv_arr_dat = mc_null;
	
	stabi_idx = 0;
	stabi_out = mc_null;
}

neuron::neuron(){
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_neuron ++);
	mck_slog2("alloc__neuron\n");
	init_me();
} 

neuron::~neuron(){} 

void
neuron::init_me(int caller){
	handler_idx = idx_neuron;
}

polaron::polaron(){ 
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_polaron ++);
	mck_slog2("alloc__polaron\n");
	init_me();
} 

polaron::~polaron(){} 

void
polaron::init_me(int caller){
	handler_idx = idx_polaron;
	opp = mc_null; 
}

sorcell::sorcell(){ 
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_sorcell ++);
	mck_slog2("alloc__sorcell\n");
	init_me();
} 

sorcell::~sorcell(){} 

void
sorcell::init_me(int caller){
	handler_idx = idx_sorcell;

	MC_DBG(
		nod_id = 0;
		level = 0;
	);
	
	srt_sz = 0;
	edge_flags = 0;
}

endcell::endcell(){ 
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_endcell ++);
	mck_slog2("alloc__endcell\n");
	init_me();
} 

endcell::~endcell(){} 

void
endcell::init_me(int caller){
	handler_idx = idx_endcell;

	end_ki = nd_invalid;
	out_tok = bj_tok_sornet_invalid;
	
	nxt_neu = mc_null;
	nxt_pol = mc_null;
}

pgrp_item::pgrp_item(){ 
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_pgrp_item ++);
	mck_slog2("alloc__pgrp_item\n");
	init_me();
} 

pgrp_item::~pgrp_item(){} 

void
pgrp_item::init_me(int caller){
	handler_idx = idx_pgrp_item;

	pnt = mc_null;
	lft = mc_null;
	rgt = mc_null;

	dat = mc_null;
}

pgroup::pgroup(){ 
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_pgroup ++);
	mck_slog2("alloc__pgroup\n");
	init_me();
} 

pgroup::~pgroup(){} 

void
pgroup::init_me(int caller){
	handler_idx = idx_pgroup;

	up = mc_null;
	fst = mc_null;
	lst = mc_null;
}

void bj_print_loaded_poles(grip& all_pol, node_kind_t ki) {
	binder * fst, * lst, * wrk;

	binder* pt_all_pol = &(all_pol);
	fst = (binder*)(pt_all_pol->bn_right);
	lst = (binder*)mck_as_loc_pt(pt_all_pol);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		polaron* my_pol = (polaron*)wrk;
		PTD_CK(my_pol->ki == ki);

		binder* nn_all_snp = &(my_pol->left_side.step_active_set.all_syn);

		mck_slog2("LDPOL.");
		mck_ilog(my_pol->id);
		mck_slog2("{");

		binder * fst2, * lst2, * wrk2;
		fst2 = (binder*)(nn_all_snp->bn_right);
		lst2 = (binder*)mck_as_loc_pt(nn_all_snp);
		for(wrk2 = fst2; wrk2 != lst2; wrk2 = (binder*)(wrk2->bn_right)){
			synapse* my_snp = (synapse*)wrk2;
			MCK_CK(my_snp->mate != mc_null);

			synapse* mt_snp = (synapse*)(my_snp->mate);
			neuron* my_neu = (neuron*)(mt_snp->owner);
			MCK_CK(my_neu->ki == nd_neu);

			mck_ilog(my_neu->id);
			mck_slog2(" ");

			//mck_slog2("nxt2__________");
			//mck_xlog((mc_addr_t)(wrk2->bn_right));
			//mck_slog2("__________\n");
		}

		mck_slog2("}\n");
	}
}

void 
bj_print_loaded_cnf() {
	nervenet* my_net = bj_nervenet;

	binder * fst, * lst, * wrk;

	binder* pt_all_neu = &(my_net->all_wu_active_neu);
	fst = (binder*)(pt_all_neu->bn_right);
	lst = (binder*)mck_as_loc_pt(pt_all_neu);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		neuron* my_neu = (neuron*)wrk;
		PTD_CK(my_neu->ki == nd_neu);

		binder* nn_all_snp = &(my_neu->left_side.step_active_set.all_syn);

		mck_slog2("n");
		mck_ilog(my_neu->id);
		mck_slog2("[");

		binder * fst2, * lst2, * wrk2;
		fst2 = (binder*)(nn_all_snp->bn_right);
		lst2 = (binder*)mck_as_loc_pt(nn_all_snp);
		for(wrk2 = fst2; wrk2 != lst2; wrk2 = (binder*)(wrk2->bn_right)){
			synapse* my_snp = (synapse*)wrk2;
			MCK_CK(my_snp->mate != mc_null);

			synapse* mt_snp = (synapse*)(my_snp->mate);
			polaron* my_pol = (polaron*)(mt_snp->owner);
			MCK_CK((my_pol->id <= 0) || (my_pol->ki == nd_pos));
			MCK_CK((my_pol->id >= 0) || (my_pol->ki == nd_neg));

			mck_ilog(my_pol->id);
			mck_slog2(" ");
		}

		mck_slog2("]\n");
	}

	mck_lock_log();
	mck_slog2("LD_POLARONS={\n");
	bj_print_loaded_poles(my_net->all_wu_active_pos, nd_pos);
	bj_print_loaded_poles(my_net->all_wu_active_neg, nd_neg);
	mck_slog2("\n}\n");
	mck_unlock_log();
}

char* net_side_to_str(net_side_t sd){
	char* resp = mc_null;
	switch(sd){
	case side_invalid:
		resp = mc_cstr("side_invalid");
	break;
	case side_left:
		resp = mc_cstr("lft");
	break;
	case side_right:
		resp = mc_cstr("rgt");
	break;
	}
	return resp;
}

char* node_kind_to_str(node_kind_t ki){
	char* resp = mc_cstr("UNDEFINED_NODE_KIND !!!");
	switch(ki){
	case nd_invalid:
		resp = mc_cstr("nd_invalid");
	break;
	case nd_pos:
		resp = mc_cstr("nd_pos");
	break;
	case nd_neg:
		resp = mc_cstr("nd_neg");
	break;
	case nd_neu:
		resp = mc_cstr("nd_neu");
	break;
	}
	return resp;
}

char* bj_tok_to_str(mck_token_t tok){
	char* resp = mc_cstr("UNKNOWN_SYNC_TOK");
	
	switch(tok){
	// SYNC TOKENS
	case bj_tok_sync_invalid:
		resp = mc_cstr("bj_tok_sync_invalid");
	break;
	case bj_tok_sync_add_tier:
		resp = mc_cstr("bj_tok_sync_add_tier");
	break;
	case bj_tok_sync_inert_child:
		resp = mc_cstr("bj_tok_sync_inert_child");
	break;
	case bj_tok_sync_confl_up_neu:
		resp = mc_cstr("bj_tok_sync_confl_up_neu");
	break;
	case bj_tok_sync_confl_up_pol:
		resp = mc_cstr("bj_tok_sync_confl_up_pol");
	break;
	case bj_tok_sync_confl_down_neu:
		resp = mc_cstr("bj_tok_sync_confl_down_neu");
	break;
	case bj_tok_sync_confl_down_pol:
		resp = mc_cstr("bj_tok_sync_confl_down_pol");
	break;
	case bj_tok_sync_to_children:
		resp = mc_cstr("bj_tok_sync_to_children");
	break;
	case bj_tok_sync_end:
		resp = mc_cstr("bj_tok_sync_end");
	break;
	// LOAD TOKENS
	case bj_tok_load_invalid:
		resp = mc_cstr("bj_tok_load_invalid");
	break;
	case bj_tok_load_nw_syn:
		resp = mc_cstr("bj_tok_load_nw_syn");
	break;
	case bj_tok_load_no_lits:
		resp = mc_cstr("bj_tok_load_no_lits");
	break;
	case bj_tok_load_end:
		resp = mc_cstr("bj_tok_load_end");
	break;
	// PROPAG TOKENS
	case bj_tok_propag_invalid:
		resp = mc_cstr("bj_tok_propag_invalid");
	break;
	case bj_tok_propag_start:
		resp = mc_cstr("bj_tok_propag_start");
	break;
	case bj_tok_propag_ping:
		resp = mc_cstr("bj_tok_propag_ping");
	break;
	case bj_tok_propag_charge_all:
		resp = mc_cstr("bj_tok_propag_charge_all");
	break;
	case bj_tok_propag_charge_src:
		resp = mc_cstr("bj_tok_propag_charge_src");
	break;
	case bj_tok_propag_tier_done:
		resp = mc_cstr("bj_tok_propag_tier_done");
	break;
	case bj_tok_propag_end:
		resp = mc_cstr("bj_tok_propag_end");
	break;
	// STABI TOKENS
	case bj_tok_stabi_invalid:
		resp = mc_cstr("bj_tok_stabi_invalid");
	break;
	case bj_tok_stabi_start:
		resp = mc_cstr("bj_tok_stabi_start");
	break;
	case bj_tok_stabi_sep_bcast:
		resp = mc_cstr("bj_tok_stabi_sep_bcast");
	break;
	case bj_tok_stabi_range_bcast:
		resp = mc_cstr("bj_tok_stabi_range_bcast");
	break;
	case bj_tok_stabi_sep_neu:
		resp = mc_cstr("bj_tok_stabi_sep_neu");
	break;
	case bj_tok_stabi_sep_pol:
		resp = mc_cstr("bj_tok_stabi_sep_pol");
	break;
	case bj_tok_stabi_ping:
		resp = mc_cstr("bj_tok_stabi_ping");
	break;
	case bj_tok_stabi_color:
		resp = mc_cstr("bj_tok_stabi_color");
	break;
	case bj_tok_stabi_tier_done:
		resp = mc_cstr("bj_tok_stabi_tier_done");
	break;
	case bj_tok_stabi_end:
		resp = mc_cstr("bj_tok_stabi_end");
	break;
	// SORNET TOKENS
	case bj_tok_sornet_invalid:
		resp = mc_cstr("bj_tok_sornet_invalid");
	break;
	case bj_tok_sornet_start:
		resp = mc_cstr("bj_tok_sornet_start");
	break;
	case bj_tok_sornet_sep_out:
		resp = mc_cstr("bj_tok_sornet_sep_out");
	break;
	case bj_tok_sornet_cll_out:
		resp = mc_cstr("bj_tok_sornet_cll_out");
	break;
	case bj_tok_sornet_num_out:
		resp = mc_cstr("bj_tok_sornet_num_out");
	break;
	case bj_tok_sornet_bin:
		resp = mc_cstr("bj_tok_sornet_bin");
	break;
	case bj_tok_sornet_num:
		resp = mc_cstr("bj_tok_sornet_num");
	break;
	case bj_tok_sornet_rank_start:
		resp = mc_cstr("bj_tok_sornet_rank_start");
	break;
	case bj_tok_sornet_rank_jump:
		resp = mc_cstr("bj_tok_sornet_rank_jump");
	break;
	case bj_tok_sornet_rank_step:
		resp = mc_cstr("bj_tok_sornet_rank_step");
	break;
	case bj_tok_sornet_rank_cll:
		resp = mc_cstr("bj_tok_sornet_rank_cll");
	break;
	case bj_tok_sornet_rank_cll_rdy:
		resp = mc_cstr("bj_tok_sornet_rank_cll_rdy");
	break;
	case bj_tok_sornet_rank_sep:
		resp = mc_cstr("bj_tok_sornet_rank_sep");
	break;
	case bj_tok_sornet_rank_sep_rdy:
		resp = mc_cstr("bj_tok_sornet_rank_sep_rdy");
	break;
	case bj_tok_sornet_rank_obj:
		resp = mc_cstr("bj_tok_sornet_rank_obj");
	break;
	case bj_tok_sornet_rank_out:
		resp = mc_cstr("bj_tok_sornet_rank_out");
	break;
	case bj_tok_sornet_end:
		resp = mc_cstr("bj_tok_sornet_end");
	break;
	}
	
	return resp;
}

char* sync_tok_to_str(sync_tok_t tok){
	return bj_tok_to_str(tok);
}

char* load_tok_to_str(load_tok_t tok){
	return bj_tok_to_str(tok);
}

char* propag_tok_to_str(propag_tok_t tok){
	return bj_tok_to_str(tok);
}

char* stabi_tok_to_str(stabi_tok_t tok){
	return bj_tok_to_str(tok);
}

char* sornet_tok_to_str(sornet_tok_t tok){
	return bj_tok_to_str(tok);
}

netstate::netstate(){ 
	init_me();
} 

netstate::~netstate(){} 

void
netstate::init_me(int caller){
	my_side = side_invalid;

	init_sync();

	tok_confl = bj_tok_sync_invalid;
	nod_confl = mc_null;
	ti_confl = BJ_INVALID_NUM_TIER;

	//PTD_PRT("SYNC_INIT_DATA TOT_CHLD=%d \n", sync_tot_children);
}

void
netstate::init_sync(){
	sync_all_zero_act_neus = false;
	sync_is_inactive = false;
	sync_wait_tier = 0;
	sync_is_ending = false;
}

tierdata::tierdata(){
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_tierdata ++);
	mck_slog2("alloc__tierdata\n");
	init_me();
} 

tierdata::~tierdata(){} 

void
tierdata::init_me(int caller){
	tdt_id = BJ_INVALID_NUM_TIER;
	tdt_flags = 0;

	num_inert_chdn = 0;
	num_zero_act_neus_chdn = 0;

	inp_neus = BJ_INVALID_NUM_NODE;
	off_neus = 0;
	rcv_neus = 0;
	stl_neus = 0;
	dly_neus = 0;
}

layerdata::layerdata(){
	PTD_CK(bj_nervenet != mc_null);
	PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_tot_new_layerdata ++);
	mck_slog2("alloc__layerdata\n");
	init_me();
} 

layerdata::~layerdata(){} 

void
layerdata::init_me(int caller){
}

void
nervenet::init_sync_cycle(){
	sync_tot_children = mc_map_get_tot_children();
	sync_parent_id = mc_map_get_parent_workeru_id();
	sync_map = mc_map_get_loaded();

	act_left_side.init_sync();
	act_right_side.init_sync();
}

void 
ptd_prt_tok_codes(){
	for(mck_token_t aa = bj_tok_sync_invalid; aa <= bj_tok_propag_end; aa++){
		if(is_sync_tok(aa)){
			PTD_PRT("%d: %s \n", aa, sync_tok_to_str((sync_tok_t)aa));
		}
		else if(is_load_tok(aa)){
			PTD_PRT("%d: %s \n", aa, load_tok_to_str((load_tok_t)aa));
		}
		else if(is_propag_tok(aa)){
			PTD_PRT("%d: %s \n", aa, propag_tok_to_str((propag_tok_t)aa));
		}
		else if(is_stabi_tok(aa)){
			PTD_PRT("%d: %s \n", aa, stabi_tok_to_str((stabi_tok_t)aa));
		}
	}
}

tierset*
side_state::dbg_get_tiset(grip& all_ti, num_tier_t nti){
	if(all_ti.is_alone()){
		return mc_null;
	}

	if(nti == BJ_INVALID_NUM_TIER){
		return mc_null;
	}

	if(nti == BJ_LAST_TIER){
		PTD_CK(((tierset*)all_ti.bn_left)->ti_id != BJ_INVALID_NUM_TIER);
		return (tierset*)all_ti.bn_left;
	}

	tierset* dat = mc_null;
	binder * fst, * lst, * wrk;

	binder* tis = &(all_ti);
	fst = (binder*)(tis->bn_left);
	lst = (binder*)mck_as_loc_pt(tis);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_left)){
		tierset* tidat = (tierset*)wrk;
		if(tidat->ti_id >= nti){
			dat = tidat;
		}
		if(tidat->ti_id < nti){
			break;
		}
	}

	return dat;
}

void 
dbg_call_all_synapses(binder* nn_all_snp, bj_callee_t mth, net_side_t sd, bool from_rec){
	PTD_CK(mth != mc_null);

	binder * fst, * lst, * wrk;

	fst = (binder*)(nn_all_snp->bn_right);
	lst = (binder*)mck_as_loc_pt(nn_all_snp);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		synapse* my_snp = mc_null;
		if(sd == side_left){
			my_snp = (synapse*)wrk;
		} else {
			PTD_CK(sd == side_right);
			my_snp = bj_get_syn_of_rgt_handle(wrk);
		}
		PTD_CK(my_snp != mc_null);
		PTD_CK(bj_ck_is_synapse(my_snp));

		callee_prms pms;
		pms.snp = my_snp;
		pms.sd = sd;
		pms.rec = from_rec;

		(my_snp->owner->*mth)(pms);
		//(my_snp->owner->*mth)(my_snp, sd, from_rec);
	}
}

void
synset::dbg_rec_call_all(bj_callee_t mth, net_side_t sd){
	MCK_CHECK_SP();
	dbg_call_all_synapses(&(all_syn), mth, sd, true);

	binder * fst, * lst, * wrk;

	binder* grps = &(all_grp);
	fst = (binder*)(grps->bn_right);
	lst = (binder*)mck_as_loc_pt(grps);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		synset* sub_grp = (synset*)wrk;
		//PTD_CK(sub_grp->parent == this);
		sub_grp->dbg_rec_call_all(mth, sd);
	}
}

void
nervenode::dbg_prt_syn(callee_prms& pms){
	MCK_CK(pms.snp->mate != mc_null);

	synapse* mt_snp = (synapse*)(pms.snp->mate);
	nervenode* the_nod = mt_snp->owner;

	mck_ilog(the_nod->id);
	mck_slog2(" ");
}

void
netstate::dbg_prt_all_tiers(tier_kind_t tiki, char* prefix, num_tier_t num_ti){
	char* ts = bj_dbg_tier_kind_to_str(tiki);
	binder * fst, * lst, * wrk;

	grip& all_tiers = get_all_tiers(tiki);

	mck_lock_log();

	mck_slog2(ts);
	mck_slog2("_");
	if(prefix != mc_null){
		mck_slog2(prefix);
	}
	mck_slog2("_t");
	mck_ilog(num_ti);
	mck_slog2("_");
	mck_slog2(net_side_to_str(my_side));
	mck_slog2("_all_tier=[ ");

	binder* tis = &(all_tiers);
	fst = (binder*)(tis->bn_left);
	lst = (binder*)mck_as_loc_pt(tis);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_left)){
		tierdata* tda = (tierdata*)wrk;
		MC_MARK_USED(tda);
		mck_slog2("_t");
		mck_ilog(tda->tdt_id);
		mck_slog2("_[");
		mck_ilog(tda->inp_neus);
		mck_slog2(",");
		mck_ilog(tda->off_neus);
		mck_slog2(",");
		mck_ilog(tda->rcv_neus);
		mck_slog2(",");
		mck_ilog(tda->stl_neus);
		mck_slog2(",");
		mck_ilog(tda->dly_neus);
		mck_slog2("] ");
	}

	mck_slog2("] \n");

	mck_unlock_log();
}

void
nervenode::dbg_prt_nod(net_side_t sd, tier_kind_t tiki, char* prefix, num_pulse_t num_pul, 
					   num_tier_t num_ti)
{
	if(num_ti == BJ_INVALID_NUM_TIER){
		return;
	}

	bj_callee_t mth = &nervenode::dbg_prt_syn;

	side_state& ne_stt = get_side_state(sd);
	tierset* all_ti = ne_stt.dbg_get_tiset(ne_stt.propag_tiers, num_ti);
	
	bool is_mpty = ((all_ti == mc_null) && ne_stt.step_active_set.is_synset_empty());

	if(is_mpty){
		return;
	}

	mck_lock_log();

	if(prefix != mc_null){
		mck_slog2(prefix);
	}
	if(tiki != tiki_stabi){
		mck_slog2("_");
		mck_slog2(net_side_to_str(sd));
	}

	mck_slog2("_t");
	mck_ilog(num_ti);
	mck_slog2("_p");
	mck_ilog(num_pul);
	if(ki == nd_neu){
		mck_slog2("n");
	} else {
		mck_slog2("p");
	}
	mck_ilog(id);
	mck_slog2("_");

	if(ki == nd_neu){
		mck_slog2("[");
	} else {
		mck_slog2("{");
	}

	if(all_ti != mc_null){
		binder * fst_2, * lst_2, * wrk_2;

		binder* tis = &(ne_stt.propag_tiers);
		fst_2 = (binder*)(all_ti);
		lst_2 = (binder*)mck_as_loc_pt(tis);
		for(wrk_2 = fst_2; wrk_2 != lst_2; wrk_2 = (binder*)(wrk_2->bn_right)){
			tierset* tidat = (tierset*)wrk_2;
			dbg_call_all_synapses(&(tidat->ti_all), mth, sd, false);
			mck_slog2("+ ");
		}
	}

	ne_stt.step_active_set.dbg_rec_call_all(mth, sd);

	if(ki == nd_neu){
		mck_slog2("]");
	} else {
		mck_slog2("}");
	}

	if((tiki == tiki_propag) && (ki == nd_neu) && ne_stt.neu_all_ping(tiki)){ mck_slog2("*"); }
	if(tiki == tiki_stabi){
		if(mc_get_flag(stabi_flags, bj_stabi_send_pings_flag)){ mck_slog2("#"); }
		mck_ilog(left_side.step_prev_tot_active);
		if(stabi_is_rdy_to_srt()){ mck_slog2("!"); }
		mck_slog2("_st"); mck_ilog(stabi_num_tier); 
		mck_slog2("_"); mck_ilog(stabi_idx); 
		mck_slog2("_"); mck_ilog(stabi_col.min_idx); 
		mck_slog2("_"); mck_ilog(stabi_col.max_idx); 
	}

	if(stabi_arr_dat != mc_null){
		PTD_DBG_CODE(
			mck_slog2(" id=");
			bj_dbg_stabi_col_arr_to_str(stabi_arr_sz, stabi_arr_dat, 
				BJ_DBG_STR_CAP, bj_nervenet->dbg_str1);
			mck_slog(bj_nervenet->dbg_str1);
		);
	}

	mck_slog2("\n");

	mck_unlock_log();
}

void 
bj_print_active_cnf(net_side_t sd, tier_kind_t tiki, char* prefix, num_pulse_t num_pul, num_tier_t num_ti,
		mc_flags_t prt_flgs)
{
	nervenet* my_net = bj_nervenet;

	binder * fst, * lst, * wrk;

	bool with_neus = mc_get_flag(prt_flgs, bj_dbg_prt_nd_neu_flag);
	if(with_neus){ 
		binder* pt_all_neu = &(my_net->all_wu_active_neu);
		fst = (binder*)(pt_all_neu->bn_right);
		lst = (binder*)mck_as_loc_pt(pt_all_neu);
		for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
			neuron* my_neu = (neuron*)wrk;
			PTD_CK(my_neu->ki == nd_neu);

			my_neu->dbg_prt_nod(sd, tiki, prefix, num_pul, num_ti);
		}
	}

	bool with_pols = mc_get_flag(prt_flgs, bj_dbg_prt_nd_pol_flag);
	if(with_pols){ 
		binder* pt_all_pos = &(my_net->all_wu_active_pos);
		fst = (binder*)(pt_all_pos->bn_right);
		lst = (binder*)mck_as_loc_pt(pt_all_pos);
		for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
			nervenode* my_nd = (nervenode*)wrk;
			PTD_CK(my_nd->ki == nd_pos);

			my_nd->dbg_prt_nod(sd, tiki, prefix, num_pul, num_ti);
		}

		binder* pt_all_neg = &(my_net->all_wu_active_neg);
		fst = (binder*)(pt_all_neg->bn_right);
		lst = (binder*)mck_as_loc_pt(pt_all_neg);
		for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
			nervenode* my_nd = (nervenode*)wrk;
			PTD_CK(my_nd->ki == nd_neg);

			my_nd->dbg_prt_nod(sd, tiki, prefix, num_pul, num_ti);
		}
	}

	bool with_pol_ids = mc_get_flag(prt_flgs, bj_dbg_prt_id_pol_flag);
	if(with_pol_ids){ 
		mck_lock_log();
		if(prefix != mc_null){
			mck_slog2(prefix);
		}
		mck_slog2("_ALL_POL={");

		binder* pt_all_pol = &(my_net->all_wu_active_pos);
		fst = (binder*)(pt_all_pol->bn_right);
		lst = (binder*)mck_as_loc_pt(pt_all_pol);
		for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
			polaron* my_pos = (polaron*)wrk;
			PTD_CK(my_pos->ki == nd_pos);

			mck_ilog(my_pos->id);
			mck_slog2(" ");
		}

		pt_all_pol = &(my_net->all_wu_active_neg);
		fst = (binder*)(pt_all_pol->bn_right);
		lst = (binder*)mck_as_loc_pt(pt_all_pol);
		for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
			polaron* my_neg = (polaron*)wrk;
			PTD_CK(my_neg->ki == nd_neg);

			mck_ilog(my_neg->id);
			mck_slog2(" ");
		}

		mck_slog2("}\n");
		mck_unlock_log();
	}
}

dbg_stats::dbg_stats(){
	init_me();
}

dbg_stats::~dbg_stats(){}

void
dbg_stats::init_me(int caller){
	dbg_tot_new_synset = 0;
	dbg_tot_new_tierset = 0;
	dbg_tot_new_base_transmitter = 0;
	dbg_tot_new_synapse = 0;
	dbg_tot_new_side_state = 0;
	dbg_tot_new_nervenode = 0;
	dbg_tot_new_neuron = 0;
	dbg_tot_new_polaron = 0;
	dbg_tot_new_sorcell = 0;
	dbg_tot_new_endcell = 0;
	dbg_tot_new_pgrp_item = 0;
	dbg_tot_new_pgroup = 0;
	dbg_tot_new_tierdata = 0;
	dbg_tot_new_layerdata = 0;
}

void
dbg_stats::dbg_prt_all(){
	PTD_LOG("dbg_tot_new_synset = %d \n", dbg_tot_new_synset);
	PTD_LOG("dbg_tot_new_tierset = %d \n", dbg_tot_new_tierset);
	PTD_LOG("dbg_tot_new_base_transmitter = %d \n", dbg_tot_new_base_transmitter);
	PTD_LOG("dbg_tot_new_synapse = %d \n", dbg_tot_new_synapse);
	PTD_LOG("dbg_tot_new_side_state = %d \n", dbg_tot_new_side_state);
	PTD_LOG("dbg_tot_new_nervenode = %d \n", dbg_tot_new_nervenode);
	PTD_LOG("dbg_tot_new_neuron = %d \n", dbg_tot_new_neuron);
	PTD_LOG("dbg_tot_new_polaron = %d \n", dbg_tot_new_polaron);
	PTD_LOG("dbg_tot_new_sorcell = %d \n", dbg_tot_new_sorcell);
	PTD_LOG("dbg_tot_new_endcell = %d \n", dbg_tot_new_endcell);
	PTD_LOG("dbg_tot_new_pgrp_item = %d \n", dbg_tot_new_pgrp_item);
	PTD_LOG("dbg_tot_new_pgroup = %d \n", dbg_tot_new_pgroup);
	PTD_LOG("dbg_tot_new_tierdata = %d \n", dbg_tot_new_tierdata);
	PTD_LOG("dbg_tot_new_layerdata = %d \n", dbg_tot_new_tierdata);
}

class mc_aligned test_cls {
public:
	num_syn_t		field_1;

	test_cls(){};
	~test_cls(){};
};

void bj_print_class_szs(){
	if(kernel::get_workeru_nn() == 0){
		mck_slog2("-------------------------------------------------------------\n");
		mck_slog2("class_sizes:\n");
		mck_slog2("synset__");
		mck_ilog(sizeof(synset));
		mck_slog2("__\n");
		mck_slog2("tierset__");
		mck_ilog(sizeof(tierset));
		mck_slog2("__\n");
		mck_slog2("base_transmitter__");
		mck_ilog(sizeof(base_transmitter));
		mck_slog2("__\n");
		mck_slog2("synapse__");
		mck_ilog(sizeof(synapse));
		mck_slog2("__\n");
		mck_slog2("side_state__");
		mck_ilog(sizeof(side_state));
		mck_slog2("__\n");
		mck_slog2("nervenode__");
		mck_ilog(sizeof(nervenode));
		mck_slog2("__\n");
		mck_slog2("neuron__");
		mck_ilog(sizeof(neuron));
		mck_slog2("__\n");
		mck_slog2("polaron__");
		mck_ilog(sizeof(polaron));
		mck_slog2("__\n");
		mck_slog2("sorcell__");
		mck_ilog(sizeof(sorcell));
		mck_slog2("__\n");
		mck_slog2("endcell__");
		mck_ilog(sizeof(endcell));
		mck_slog2("__\n");
		mck_slog2("pgrp_item__");
		mck_ilog(sizeof(pgrp_item));
		mck_slog2("__\n");
		mck_slog2("pgroup__");
		mck_ilog(sizeof(pgroup));
		mck_slog2("__\n");
		mck_slog2("tierdata__");
		mck_ilog(sizeof(tierdata));
		mck_slog2("__\n");
		mck_slog2("layerdata__");
		mck_ilog(sizeof(layerdata));
		mck_slog2("__\n");
		mck_slog2("netstate__");
		mck_ilog(sizeof(netstate));
		mck_slog2("__\n");
		mck_slog2("dbg_stats__");
		mck_ilog(sizeof(dbg_stats));
		mck_slog2("__\n");
		mck_slog2("nervenet__");
		mck_ilog(sizeof(nervenet));
		mck_slog2("__\n");
		mck_slog2("void_pt__");
		mck_ilog(sizeof(void*));
		mck_slog2("__\n");
		mck_slog2("binder__");
		mck_ilog(sizeof(binder));
		mck_slog2("__\n");
		mck_slog2("num_syn_t__");
		mck_ilog(sizeof(num_syn_t));
		mck_slog2("__\n");
		mck_slog2("grip__");
		mck_ilog(sizeof(grip));
		mck_slog2("__\n");
		mck_slog2("num_tier_t__");
		mck_ilog(sizeof(num_tier_t));
		mck_slog2("__\n");
		mck_slog2("mc_flags_t__");
		mck_ilog(sizeof(mc_flags_t));
		mck_slog2("__\n");
		mck_slog2("kernel__");
		mck_ilog(sizeof(kernel));
		mck_slog2("__\n");
		mck_slog2("agent__");
		mck_ilog(sizeof(agent));
		mck_slog2("__\n");
		mck_slog2("cell__");
		mck_ilog(sizeof(cell));
		mck_slog2("__\n");
		mck_slog2("missive__");
		mck_ilog(sizeof(missive));
		mck_slog2("__\n");
		mck_slog2("agent_ref__");
		mck_ilog(sizeof(agent_ref));
		mck_slog2("__\n");
		mck_slog2("agent_grp__");
		mck_ilog(sizeof(agent_grp));
		mck_slog2("__\n");
		mck_slog2("test_cls__");
		mck_ilog(sizeof(test_cls));
		mck_slog2("__\n");

		mck_slog2("-------------------------------------------------------------\n");
	}
}

netstate& 
nervenet::get_active_netstate(net_side_t sd){
	PTD_CK(sd != side_invalid);

	netstate* out_stt = &act_left_side;
	if(sd == side_right){
		out_stt = &act_right_side;
	}
	PTD_CK(out_stt != mc_null);
	return *out_stt;
}

side_state& 
nervenode::get_side_state(net_side_t sd){
	PTD_CK(sd != side_invalid);

	side_state* out_stt = &left_side;
	if(sd == side_right){
		out_stt = &right_side;
	}
	PTD_CK(out_stt != mc_null);
	return *out_stt;
}

synapse*
get_synapse_from_binder(net_side_t sd, binder* bdr){
	EPH_CODE(MCK_CK(! mc_addr_has_id(bdr)));
	synapse* my_snp = mc_null;
	if(sd == side_left){
		my_snp = (synapse*)bdr;
	} else {
		PTD_CK(sd == side_right);
		my_snp = bj_get_syn_of_rgt_handle(bdr);
	}
	PTD_CK(my_snp != mc_null);
	PTD_CK(bj_ck_is_synapse(my_snp));
	MCK_CK(my_snp->owner->ki != nd_invalid);
	return my_snp;
}

void 
with_all_synapses(binder* nn_all_snp, bj_callee_t mth, net_side_t sd, 
			bool from_rec, bool dbg_mates, void* aux)
{
	PTD_CK(mth != mc_null);

	binder * fst, * lst, * wrk, * nxt;

	fst = (binder*)(nn_all_snp->bn_right);
	lst = (binder*)mck_as_loc_pt(nn_all_snp);
	for(wrk = fst; wrk != lst; wrk = nxt){
		nxt = (binder*)(wrk->bn_right);
		synapse* my_snp = get_synapse_from_binder(sd, wrk);

		if(dbg_mates){
			PTD_DBG_CODE(my_snp = my_snp->mate);
		}

		callee_prms pms;
		pms.snp = my_snp;
		pms.sd = sd;
		pms.rec = from_rec;
		pms.aux = aux;

		(my_snp->owner->*mth)(pms);
	}
}

void
synset::transmitter_send_all_rec(bj_callee_t mth, net_side_t sd){
	MCK_CHECK_SP();
	//MCK_CK(! mc_addr_has_id(this));
	//MCK_CK(! mc_addr_has_id(&(all_syn)));
	//MCK_CK(! mc_addr_has_id(&(all_grp)));
	with_all_synapses(&(all_syn), mth, sd, true /* IS_FROM_REC */);

	binder * fst, * lst, * wrk;

	binder* grps = &(all_grp);
	fst = (binder*)(grps->bn_right);
	lst = (binder*)mck_as_loc_pt(grps);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		synset* sub_grp = (synset*)wrk;
		//PTD_CK(sub_grp->parent == this);
		sub_grp->transmitter_send_all_rec(mth, sd);
	}
}

void
netstate::init_sync_transmitter(sync_transmitter& trm, num_tier_t the_ti, 
								nervenode* the_cfl_src, bool has_zan)
{
	trm.d.syc.wrk_side = side_invalid;
	trm.d.syc.wrk_tier = BJ_INVALID_NUM_TIER;
	trm.d.syc.cfl_src = mc_null;
	trm.d.syc.has_zero_act_neus = has_zan;
	
	trm.d.syc.wrk_side = my_side;
	trm.d.syc.wrk_tier = the_ti;
	trm.d.syc.cfl_src = the_cfl_src;
}

void
netstate::send_sync_transmitter(tier_kind_t tiki, nervenet* the_dst, sync_tok_t the_tok, num_tier_t the_ti, 
			nervenode* the_cfl_src, bool has_zan)
{
	PTD_CODE(char* ts = bj_dbg_tier_kind_to_str(tiki); MC_MARK_USED(ts););
	
	sync_transmitter* trm = bj_sync_transmitter_acquire();
	init_sync_transmitter(*trm, the_ti, the_cfl_src, has_zan);

	trm->src = bj_nervenet;
	trm->dst = the_dst;
	trm->tok = the_tok;
	
	trm->send();

	PTD_CODE(mc_workeru_nn_t dbg_dst_nn = mc_id_to_nn(mc_addr_get_id(the_dst)); MC_MARK_USED(dbg_dst_nn););
	SYNC_LOG(" %s_SYNCR_send_transmitter_%s_t%d_%s_ [%ld ->> %ld] \n", ts, 
		net_side_to_str(my_side), the_ti, sync_tok_to_str(the_tok), kernel::get_workeru_nn(), dbg_dst_nn);
	PTD_CK(the_ti != BJ_INVALID_NUM_TIER);
}

nervenet*
nervenet::get_nervenet(mc_workeru_id_t id){
	nervenet* rem_net = (nervenet*)mc_addr_set_id(id, this);
	return rem_net;
}

bool
nervenode::is_tier_complete(signal_data* dat){
	side_state& stt = get_side_state(dat->sd);
	return stt.is_full();
}

bool
polaron::is_tier_complete(signal_data* dat){
	side_state& pol_stt = get_side_state(dat->sd);
	side_state& opp_stt = opp->get_side_state(dat->sd);

	bool both_full = pol_stt.is_full() && opp_stt.is_full();
	return both_full;
}

void
side_state::step_reset_complete(){
	PTD_LOG("::step_reset_complete flags=%p flgs_pt=%p \n", (void*)(uintptr_t)step_flags, 
			(void*)(&step_flags));

	//step_flags = 0;
	mc_reset_flag(step_flags, bj_stt_charge_all_flag);

	step_num_complete = 0;
	step_num_ping = 0;
	step_prev_tot_active = step_active_set.tot_syn;
}

void
synset::reset_rec_tree(){
	MCK_CHECK_SP();
	while(! all_grp.is_alone()){
		synset* sub_grp = (synset*)(binder*)(all_grp.bn_right);
		//PTD_CK(sub_grp->parent == this);
		sub_grp->reset_rec_tree();

		PTD_CK(sub_grp->all_grp.is_alone());
		all_syn.move_all_to_my_right(sub_grp->all_syn);
		sub_grp->release();
	}
}

void
nervenet::send_all_neus(mck_token_t tok){
	nervenet* my_net = this;

	binder * fst, * lst, * wrk;

	binder* pt_all_neu = &(all_wu_active_neu);
	fst = (binder*)(pt_all_neu->bn_right);
	lst = (binder*)mck_as_loc_pt(pt_all_neu);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		neuron* my_neu = (neuron*)wrk;
		PTD_CK(my_neu->ki == nd_neu);
		my_net->send(my_neu, tok);
	}
}

void
netstate::broadcast_tier(tier_kind_t tiki, tierdata& lti, int dbg_caller){
	mc_workeru_id_t pnt_id = bj_nervenet->sync_parent_id;
	if(pnt_id != 0){
		nervenet* pnt_net = bj_nervenet->get_nervenet(pnt_id);
		send_sync_transmitter(tiki, pnt_net, bj_tok_sync_add_tier, lti.tdt_id, mc_null);
	} 
	send_sync_to_children(bj_tok_sync_add_tier, lti.tdt_id, tiki, mc_null);
}

void
netstate::inc_tier_rcv(nervenode* nd, tier_kind_t tiki, num_tier_t the_ti, grip& all_ti){
	PTD_CODE(char* ts = bj_dbg_tier_kind_to_str(tiki); MC_MARK_USED(ts););

	num_tier_t ti = prv_num_tier(the_ti);
	tierdata& tda = get_tier(tiki, all_ti, ti, 14);

	SYNC_LOG(" %s_TO_DELAY_t%d_%s_%ld %s_%s ((%d > 0) && (%d == %d) && (t%d >= t%d)) %s \n", 
		ts, ti, node_kind_to_str(nd->ki), nd->id, ts, ((to_dly)?("INC_STILL"):("")), 
		step_prev_tot_active, step_num_ping, step_prev_tot_active, lti.tdt_id, ti,
		((nd == mc_null)?(""):(mc_get_flag(nd->stabi_flags, bj_stabi_send_pings_flag))?("SEND_PINGS"):(""))
	);
	SYNC_CODE(bj_nervenet->act_left_side.dbg_prt_all_tiers(tiki, mc_cstr("TO_DELAY_"), ti));

	tda.inc_rcv();
}

void
tierdata::delay_binder(binder& bdr, int dbg_caller){
	PTD_CK_PRT(bdr.is_alone(), "cllr=%d", dbg_caller);
	dly_neus++;
	all_delayed.bind_to_my_left(bdr);
}

bool
neuron::is_to_delay(tier_kind_t tiki, net_side_t sd, num_tier_t the_ti, grip& all_ti){
	return (can_delay(tiki, sd) && bj_can_delay_tier(the_ti, all_ti));
}

bool
bj_can_delay_tier(num_tier_t the_ti, grip& all_ti)
{ 
	num_tier_t ti = prv_num_tier(the_ti);

	tierdata& lti = get_last_tier(all_ti);
	lti.update_tidat();

	if(lti.tdt_id < ti){
		return true;
	}
	
	return false;
}

void
inc_tier(tier_kind_t tiki, grip& all_ti, net_side_t sd, int dbg_caller){
	PTD_CODE(char* ts = bj_dbg_tier_kind_to_str(tiki); MC_MARK_USED(ts););

	tierdata& lti = get_last_tier(all_ti);

	lti.update_tidat();

	tierdata* ti_dat = bj_tierdata_acquire();
	ti_dat->tdt_id = lti.tdt_id + 1;

	all_ti.bind_to_my_left(*ti_dat);

	PTD_LOG("%s_INC_NET_TIER_%s_t%d (cllr=%d)\n", ts, net_side_to_str(sd), ti_dat->tdt_id, dbg_caller);

	mck_slog2("inc_tier__");
	mck_ilog(ti_dat->tdt_id);
	mck_slog2("__\n");

	ti_dat->update_tidat();

	lti.proc_delayed(tiki, all_ti, sd, true);
	ti_dat->all_delayed.move_all_to_my_right(lti.all_delayed);
	ti_dat->dly_neus = lti.dly_neus;
	lti.dly_neus = 0;

	PTD_CK(lti.all_delayed.is_alone());

}

tierdata&
netstate::get_tier(tier_kind_t tiki, grip& all_ti, num_tier_t nti, int dbg_caller){
	PTD_CK(! all_ti.is_alone());
	PTD_CK(nti != BJ_INVALID_NUM_TIER);

	tierdata* dat = mc_null;
	binder * fst, * lst, * wrk;

	binder* tis = &(all_ti);
	if(nti == BJ_LAST_TIER){
		dat = (tierdata*)(binder*)(tis->bn_left);
	}

	if(dat == mc_null){
		fst = (binder*)(tis->bn_left);
		lst = (binder*)mck_as_loc_pt(tis);
		for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_left)){
			tierdata* tidat = (tierdata*)wrk;
			if(tidat->tdt_id == nti){
				dat = tidat;
				break;
			}
			if(tidat->tdt_id < nti){
				break;
			}
		}
	}

	if(dat == mc_null){
		mck_slog2("CALC_TIER__");
		mck_ilog(dbg_caller);
		mck_slog2("__\n");
		while(get_last_tier(all_ti).tdt_id < nti){
			inc_tier(tiki, all_ti, my_side, dbg_caller);
		}
		tierdata& lti = get_last_tier(all_ti);
		broadcast_tier(tiki, lti, dbg_caller);
		dat = (tierdata*)(all_ti.bn_left);
	}

	PTD_CK(dat != mc_null);
	PTD_CK_PRT((dat->tdt_id == nti), "(%d == %d)",  dat->tdt_id, nti);

	dat->update_tidat();
	return *dat;
}

neuron*
bj_get_neu(binder* wrk, tier_kind_t tiki, net_side_t sd){
	side_state* stt = (side_state*)wrk;
	nervenode* nd = mc_null;
	if(sd == side_left){
		nd = bj_get_nod_of_pt_lft_st(stt);
	} else {
		PTD_CK(sd == side_right);
		nd = bj_get_nod_of_pt_rgt_st(stt);
	}
	PTD_CK(nd->ki == nd_neu);
	return ((neuron*)nd);
}

void
neuron::get_delayed_data(tier_kind_t tiki, net_side_t sd, 
					num_tier_t& the_ti, mck_token_t& tok)
{
	side_state& stt = get_side_state(sd);
	switch(tiki){
		case tiki_propag: 
			the_ti = stt.propag_num_tier;
			tok = bj_tok_propag_tier_done;
		break;
		case tiki_stabi: 
			the_ti = stabi_num_tier;
			tok = bj_tok_stabi_tier_done;
		break;
		default: 
		break;
	}
}

bool
neuron::can_delay(tier_kind_t tiki, net_side_t sd){
	side_state& sd_stt = get_side_state(sd);
	bool to_dly = sd_stt.neu_all_ping(tiki);
	if(tiki == tiki_stabi){
		to_dly = mc_get_flag(stabi_flags, bj_stabi_send_pings_flag);
	}
	return to_dly;
}

void
tierdata::proc_delayed(tier_kind_t tiki, grip& all_ti, net_side_t sd, bool star_nxt_ti){
	binder * fst, * lst, * wrk, * nxt;

	netstate& nstt = bj_nervenet->get_active_netstate(sd);

	binder* dlyd = &(all_delayed);
	fst = (binder*)(dlyd->bn_right);
	lst = (binder*)mck_as_loc_pt(dlyd);
	for(wrk = fst; wrk != lst; wrk = nxt){
		nxt = (binder*)(wrk->bn_right);

		if(! star_nxt_ti){
			dly_neus--;
			wrk->let_go();
			continue;
		}

		neuron* nd = bj_get_neu(wrk, tiki, sd);
		
		num_tier_t the_ti = BJ_INVALID_NUM_TIER;
		mck_token_t tok = BJ_INVALID_TOKEN;
		nd->get_delayed_data(tiki, sd, the_ti, tok);
		
		bool to_dly = nd->is_to_delay(tiki, sd, the_ti, all_ti);
		PTD_CK(! to_dly);
		if(! to_dly){
			nstt.inc_tier_rcv(nd, tiki, the_ti, all_ti);

			dly_neus--;
			wrk->let_go();

			signal_data dat;
			dat.tok = tok;
			dat.sd = sd;
			dat.ti = the_ti - 1;

			switch(tiki){
				case tiki_propag: 
					nd->propag_start_nxt_tier(&dat);
				break;
				case tiki_stabi: 
					nd->stabi_start_nxt_tier(&dat);
				break;
				default: 
				break;
			}

			SYNC_LOG(" %s_SYNCR_UPDTING_DELAYED_%s_t%d_%s_%ld \n", bj_dbg_tier_kind_to_str(tiki), 
				net_side_to_str(dat.sd), dat.ti, node_kind_to_str(nd->ki), nd->id);
		}
	}

	PTD_CK(all_delayed.is_alone());
}

grip&
netstate::get_all_tiers(tier_kind_t tiki){
	grip* all_tiers_pt = mc_null;
	switch(tiki){
		case tiki_propag: 
			all_tiers_pt = &(all_propag_tiers);
		break;
		case tiki_stabi: 
			all_tiers_pt = &(all_stabi_tiers);
		break;
		default: 
		break;
	}
	PTD_CK(all_tiers_pt != mc_null);
	return *all_tiers_pt;
}

void 
tierdata::update_tidat(){
	if(tdt_id == 0){
		return;
	}
	tierdata* prv = (tierdata*)(bn_left);
	if((inp_neus == BJ_INVALID_NUM_NODE) && prv->got_all_neus()){
		PTD_CK(prv->inp_neus != BJ_INVALID_NUM_NODE);
		inp_neus = prv->inp_neus - prv->off_neus;
		PTD_CK(inp_neus != BJ_INVALID_NUM_NODE);
		PTD_LOG(" UPDATED_TIER_%d inp_neus=%d \n", tdt_id, inp_neus);
		//PTD_CK(stl_neus >= 0);
	}
}

char*
nervenode::get_ki_str(){
	if(ki == nd_neu){ return mc_cstr("n"); }
	return mc_cstr("p");
}

char* 
bj_dbg_tier_kind_to_str(tier_kind_t tiki){
	char* resp = mc_null;
	switch(tiki){
	case tiki_invalid:
		resp = mc_cstr("inv");
	break;
	case tiki_propag:
		resp = mc_cstr("prp");
	break;
	case tiki_stabi:
		resp = mc_cstr("stb");
	break;
	}
	PTD_CK(resp != mc_null);
	return resp;
}

void
nervenet::sync_handler(tier_kind_t tiki, missive* msv){
	PTD_CODE(char* ts = bj_dbg_tier_kind_to_str(tiki); MC_MARK_USED(ts););

	mck_token_t msv_tok = msv->tok;

	sync_transmitter* sy_tmt = (sync_transmitter*)msv;
	net_side_t tmt_sd = sy_tmt->d.syc.wrk_side;
	num_tier_t tmt_ti = sy_tmt->d.syc.wrk_tier;
	nervenode* tmt_cfl = sy_tmt->d.syc.cfl_src;
	bool has_zan = sy_tmt->d.syc.has_zero_act_neus;

	PTD_CK(tmt_ti != BJ_INVALID_NUM_TIER);
	PTD_CK(tmt_sd != side_invalid);

	netstate& nst = get_active_netstate(tmt_sd);

	grip& all_tiers = nst.get_all_tiers(tiki);

	tierdata& lti = get_last_tier(all_tiers);

	lti.update_tidat();

	SYNC_LOG(" %s_SYNCR_RECV_%s_t%d_%s_ [%d <<- %d] lti=%d cf=%p i_chdn=(%d of %d) z_chdn=(%d of %d) %s\n", 
		ts, net_side_to_str(tmt_sd), tmt_ti, sync_tok_to_str((sync_tok_t)(msv_tok)), 
		kernel::get_workeru_nn(), mc_id_to_nn(mc_addr_get_id(msv->src)), 
		lti.tdt_id, tmt_cfl, lti.num_inert_chdn, bj_nervenet->sync_tot_children,
		lti.num_zero_act_neus_chdn, bj_nervenet->sync_tot_children,
		((tmt_ti < lti.tdt_id)?("OLDER_TIER"):(""))
	);

	if((tmt_ti < lti.tdt_id) && (msv_tok != bj_tok_sync_to_children)){
		PTD_CK_PRT((msv_tok != bj_tok_sync_to_children), "(%d < %d) \n", tmt_ti, lti.tdt_id);
		return;
	}

	tierdata& tdt = nst.get_tier(tiki, all_tiers, tmt_ti, 12);
	MC_MARK_USED(tdt);
	//PTD_CK(is_last_tier(all_tiers, tdt));

	switch(msv_tok){
		case bj_tok_sync_add_tier:
		break;
		case bj_tok_sync_inert_child:
			tdt.num_inert_chdn++;
			if(has_zan){
				tdt.num_zero_act_neus_chdn++;
			}
			SYNC_LOG(" %s_SYNCR_RCV_INERT_%s_t%d_dt%d_nt%d_ chdn=(%d of %d)\n", ts, 
				net_side_to_str(tmt_sd), tmt_ti, tdt.tdt_id, nst.sync_wait_tier,
				tdt.num_inert_chdn, bj_nervenet->sync_tot_children);
		break;

		case bj_tok_sync_confl_up_neu:
			nst.send_up_confl_tok(bj_tok_sync_confl_up_neu, tmt_ti, tmt_cfl);
		break;
		case bj_tok_sync_confl_up_pol:
			nst.send_up_confl_tok(bj_tok_sync_confl_up_pol, tmt_ti, tmt_cfl);
		break;
		case bj_tok_sync_confl_down_neu:
			nst.send_sync_to_children(bj_tok_sync_confl_down_neu, tmt_ti, tiki, tmt_cfl);
		break;
		case bj_tok_sync_confl_down_pol:
			nst.send_sync_to_children(bj_tok_sync_confl_down_pol, tmt_ti, tiki, tmt_cfl);
		break;
		case bj_tok_sync_to_children:
			nst.send_sync_to_children(bj_tok_sync_to_children, tmt_ti, tiki, mc_null, has_zan);
		break;
		default:
			mck_abort(1, mc_cstr("BAD_SYNC_TOK"));
		break;
	}

	nst.update_sync_inert(tiki);
}

void
netstate::update_sync_inert(tier_kind_t tiki, bool remove_full){ 
	PTD_CODE(char* ts = bj_dbg_tier_kind_to_str(tiki); MC_MARK_USED(ts););

	if(sync_is_inactive){
		SYNC_LOG(" %s_SYNCR_sync_is_inactive_%s_t%d_\n", ts, net_side_to_str(my_side), sync_wait_tier);
		return;
	}
	grip& all_tiers = get_all_tiers(tiki);

	tierdata& wt_tdt = get_tier(tiki, all_tiers, sync_wait_tier, 13);
	PTD_CK(wt_tdt.tdt_id == sync_wait_tier);
	PTD_CK(sync_wait_tier >= 0);

	if(! wt_tdt.got_all_neus()){ 
		SYNC_LOG(" %s_SYNCR_NO_snd_to_pnt_%s_t%d_not_got_all_NEUS [%d,%d,%d,%d,%d] \n", 
			ts, net_side_to_str(my_side), sync_wait_tier, 
			wt_tdt.inp_neus, wt_tdt.off_neus, wt_tdt.rcv_neus, wt_tdt.stl_neus, wt_tdt.dly_neus
		);
		return; 
	}
	PTD_CK(wt_tdt.inp_neus != BJ_INVALID_NUM_NODE);

	nervenet* the_net = bj_nervenet;

	if(! is_last_tier(all_tiers, wt_tdt)){
		SYNC_LOG(" %s_SYNCR_NO_snd_to_pnt_%s_t%d_not_is_last_tier \n", 
				ts, net_side_to_str(my_side), wt_tdt.tdt_id);
		PTD_CK(wt_tdt.all_delayed.is_alone());
		if(remove_full && ! wt_tdt.is_first_tier(all_tiers)){
			wt_tdt.prv_tier().release();
		}
		sync_wait_tier++;
		return;
	}

	mc_workeru_nn_t tot_chdn = the_net->sync_tot_children;

	if(wt_tdt.num_inert_chdn < tot_chdn){
		SYNC_LOG(" %s_SYNCR_NO_snd_to_pnt_%s_t%d_not_all_inert_chdn (%d < %d) \n", ts, 
			net_side_to_str(my_side), sync_wait_tier, wt_tdt.num_inert_chdn, tot_chdn);
		return;
	}
	PTD_CK(wt_tdt.num_inert_chdn == tot_chdn);

	if(! wt_tdt.is_inert() && ! wt_tdt.is_tidat_empty()){
		SYNC_LOG(" %s_SYNCR_NO_snd_to_pnt_%s_t%d_not_is_inert [%d,%d,%d,%d,%d] \n", ts, 
			net_side_to_str(my_side), sync_wait_tier, 
			wt_tdt.inp_neus, wt_tdt.off_neus, wt_tdt.rcv_neus, wt_tdt.stl_neus, wt_tdt.dly_neus
		);
		return;
	}

	bool already_sent = mc_get_flag(wt_tdt.tdt_flags, bj_sent_inert_flag);
	if(already_sent){ 
		SYNC_LOG(" %s_SYNCR_already_sent_%s_t%d_\n", ts, net_side_to_str(my_side), sync_wait_tier);
		return; 
	}
	mc_set_flag(wt_tdt.tdt_flags, bj_sent_inert_flag);

	SYNC_LOG(" %s_SYNCR_GOT_is_inert_%s_t%d [%d,%d,%d,%d,%d] \n", ts, net_side_to_str(my_side), wt_tdt.tdt_id,
		wt_tdt.inp_neus, wt_tdt.off_neus, wt_tdt.rcv_neus, wt_tdt.stl_neus, wt_tdt.dly_neus
	);
	
	bool has_zan = ((wt_tdt.num_zero_act_neus_chdn == tot_chdn) && (wt_tdt.num_active_neus() == 0));

	mc_workeru_id_t pnt_id = the_net->sync_parent_id;
	if(pnt_id != 0){
		nervenet* pnt_net = the_net->get_nervenet(pnt_id);
		send_sync_transmitter(tiki, pnt_net, bj_tok_sync_inert_child, wt_tdt.tdt_id, mc_null, has_zan);
	} else {
		send_sync_to_children(bj_tok_sync_to_children, wt_tdt.tdt_id, tiki, mc_null, has_zan);
	}
}

synapse*
synset::get_first_snp(net_side_t sd){
	if(all_syn.is_alone()){
		return mc_null;
	}
	binder* fst = (binder*)(all_syn.bn_right);
	synapse* snp = get_synapse_from_binder(sd, fst);
	return snp;
}

int
bj_cmp_num_syn(num_syn_t const & n1, num_syn_t const & n2){ 
	if(n1 == n2){ return 0; }
	if(n1 < n2){ return -1; }
	return 1;
}

bool 
bj_is_sorted(grip& grp, bj_cmp_func_t cmp_fn, int ord){
	binder* all_bn = &grp;
	binder * fst, * lst, * wrk;

	bool sted = true;

	fst = (binder*)(all_bn->bn_right);
	lst = (binder*)mck_as_loc_pt(all_bn);
	for(wrk = fst; wrk != lst; wrk = (binder*)(wrk->bn_right)){
		if(wrk == fst){
			continue;
		}
		int vc = (*cmp_fn)(wrk->bn_left, wrk);
		if(vc == 0){ continue; }
		if(ord < 0){
			if(vc > 0){
				sted = false;
				break;
			}
		} else {
			if(vc < 0){
				sted = false;
				break;
			}
		}
	}

	return sted;
}

int 
bj_dbg_cmp_synset_by_arr_id(binder* obj1, binder* obj2){
	return 0;
}

int 
bj_dbg_cmp_synset_by_tot_syn(binder* obj1, binder* obj2){
	return 0;
}

void
with_all_nervenodes(binder* nn_all_nod, bj_callee_prm_t mth, void* prm){
	PTD_CK(mth != mc_null);

	binder * fst, * lst, * wrk, * nxt;

	fst = (binder*)(nn_all_nod->bn_right);
	lst = (binder*)mck_as_loc_pt(nn_all_nod);
	for(wrk = fst; wrk != lst; wrk = nxt){
		nxt = (binder*)(wrk->bn_right);
		nervenode* my_nod = (nervenode*)wrk;

		(my_nod->*mth)(prm);
	}
}

base_transmitter* 
base_transmitter::clone_transmitter(){
	base_transmitter* orig = this;
	base_transmitter* tmt = bj_base_transmitter_acquire();
	binder bdr;
	
	bdr = *((binder*)tmt);	// keep mem management ok
	*tmt = *orig;
	*((binder*)tmt) = bdr;	// keep mem management ok
	
	return tmt;
}

void 
nervenet::send_all_children(base_transmitter& tmt_orig, char* dbg_str){
	nervenet* src = this;
	
	mc_load_map_st** my_children = sync_map->childs;
	if(my_children != mc_null){ 
		int aa = 0;
		mc_load_map_st* ch_map = (my_children)[aa];
		while(ch_map != mc_null){
			mc_workeru_nn_t chd_nn = ch_map->num_workeru;
			nervenet* ch_net = get_nervenet(mc_nn_to_id(chd_nn));
			
			base_transmitter* tmt = tmt_orig.clone_transmitter();
			
			tmt->src = src;
			tmt->dst = ch_net;
			
			tmt->send();

			if(dbg_str != mc_null){
				mc_workeru_nn_t dbg_dst_nn = mc_id_to_nn(mc_addr_get_id(ch_net)); 
				MC_MARK_USED(dbg_dst_nn);
				PTD_LOG(" %s_CHDN_MSV [%ld ->> %ld] \n", dbg_str, kernel::get_workeru_nn(), dbg_dst_nn);
			}
			
			aa++;
			ch_map = (my_children)[aa];
		}
	}
}

void 
netstate::send_sync_to_children(sync_tok_t the_tok, num_tier_t the_ti, tier_kind_t tiki, 
								nervenode* the_cfl, bool has_zan)
{
	PTD_CODE(char* ts = bj_dbg_tier_kind_to_str(tiki); MC_MARK_USED(ts););
	SYNC_LOG(" %s_SYNCR_STOP_CHILDREN_%s_t%d_ WORKERU=%d \n", ts, net_side_to_str(my_side), 
			the_ti, kernel::get_workeru_nn());

	PTD_CK(the_ti != BJ_INVALID_NUM_TIER);

	if(the_cfl != mc_null){
		PTD_CK((the_tok == bj_tok_sync_confl_down_neu) || (the_tok == bj_tok_sync_confl_down_pol));
		tok_confl = the_tok;
		nod_confl = the_cfl;
		ti_confl = the_ti;

		PTD_LOG(" %s_SYNCR_CONFLICT_%s_t%d_%s confl_pt=%p \n", ts, 
				net_side_to_str(my_side), the_ti, 
				sync_tok_to_str(bj_tok_sync_confl_down_pol), (void*)nod_confl);
	}

	sync_transmitter base_tmt;
	init_sync_transmitter(base_tmt, the_ti, the_cfl, has_zan);
	base_tmt.tok = the_tok;
	bj_nervenet->send_all_children(base_tmt);
	
	if(the_tok == bj_tok_sync_to_children){
		PTD_LOG(" %s_SYNCR_END_%s_t%d \n", ts, net_side_to_str(my_side), the_ti);

		sync_is_ending = true;
		sync_is_inactive = true;
		if(has_zan){ sync_all_zero_act_neus = true; }
	}
}

