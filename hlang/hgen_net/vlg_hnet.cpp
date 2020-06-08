

#include "hgen_net.h"

using namespace std;

//--------------------------------------------------------------------------------
// tester of verilog module printer

void
runner_print_verilog_target_box::print_help(){
	GH_GLOBALS.compl_sys.print_last_complete_arg();
	FILE* of = GH_GLOBALS.compl_sys.args_compl_output;
	fprintf(of, "<num_in> <num_out> [-o <out_file_name>] [-w <num_direct_chnls> <num_direct_paks> <wrapper_file_name>] [-pb <pw_base>] \n");
}
bool

runner_print_verilog_target_box::get_args(gh_str_list_t& lt_args){
	GH_CK(! lt_args.empty());
	GH_CK(lt_args.front() == "print_verilog_target_box");
	
	bool is_cmpl = GH_GLOBALS.compl_sys.args_orig_has_compl;
	int tot_params = lt_args.size();
	tot_params--;
	if(is_cmpl){
		tot_params--;
	}
	if(tot_params < 2){
		if(! is_cmpl){
			fprintf(stdout, "following args must have the form:\n\t");
		}
		print_help();
		return false;
	}
	
	gh_dec_args(lt_args);
	
	gh_string_t the_arg = lt_args.front(); gh_dec_args(lt_args);
	num_in = atol(the_arg.c_str());
	
	the_arg = lt_args.front(); gh_dec_args(lt_args);
	num_out = atol(the_arg.c_str());
	
	bool did_some = true;
	while(did_some && ! lt_args.empty()){
		did_some = false;
		the_arg = lt_args.front();
		if((the_arg == "-pb") && (lt_args.size() > 1)){
			gh_dec_args(lt_args); did_some = true;

			the_arg = lt_args.front(); gh_dec_args(lt_args);
			pw_base = atol(the_arg.c_str());
		}
		else if((the_arg == "-o") && (lt_args.size() > 1)){
			gh_dec_args(lt_args); did_some = true;

			out_file_name = lt_args.front(); gh_dec_args(lt_args);
		}
		else if((the_arg == "-w") && (lt_args.size() > 3)){
			gh_dec_args(lt_args); did_some = true;

			the_arg = lt_args.front(); gh_dec_args(lt_args);
			num_direct_channels = atol(the_arg.c_str());
			
			the_arg = lt_args.front(); gh_dec_args(lt_args);
			num_direct_packets = atol(the_arg.c_str());
			
			wrapper_file_name = lt_args.front(); gh_dec_args(lt_args);
		}
	}
	
	return true;
}

int
runner_print_verilog_target_box::run_test(gh_str_list_t& lt_args){
	bool go_on = get_args(lt_args);
	if(! go_on){
		return -1;
	}
	bool is_cmpl = GH_GLOBALS.compl_sys.args_orig_has_compl;
	if(is_cmpl){ return -1; }
	
	long bb = pw_base;
	long nin = num_in;
	long nout = num_out;
	
	FILE* ff = stdout;
	if(out_file_name != ""){
		ff = fopen(out_file_name.c_str(), "w");
		if(ff == NULL){
			fprintf(stderr, "Cannot open file %s for writing !!\n", out_file_name.c_str());
			return -1;
		}
	}
	
	FILE* ffw = NULL;
	if(wrapper_file_name != ""){
		ffw = fopen(wrapper_file_name.c_str(), "w");
		if(ffw == NULL){
			fprintf(stderr, "Cannot open file %s for writing !!\n", wrapper_file_name.c_str());
			return -1;
		}
	}
	
	
	fprintf(ff, "// test_get_target base=%ld #in=%ld #out=%ld\n", bb, nin, nout);

	htarget_box* bx = new htarget_box(bb);
	
	long tg_idx = 0;
	long tgs_sz = 0;
	gh_dbg_calc_idx_and_sz(nin, nout, bb, tg_idx, tgs_sz);
	
	slice_vec all_addr;
	bx->init_target_box(tg_idx, nin, nout, all_addr, tgs_sz);

	GH_GLOBALS.CK_LINK_MODE = gh_valid_self_ck_mod;

	fprintf(ff, "/*\n");
	fprintf(ff, "---------------------------------------------\n");
	bx->print_box(ff, gh_full_prt);
	fprintf(ff, "---------------------------------------------\n");
	fprintf(ff, "*/\n");
	
	verilog_file vff;
	vff.fl = ff;
	bx->print_verilog_module_router(vff);
	
	if(ffw != NULL){
		verilog_file wrp_vff;
		wrp_vff.fl = ffw;
		
		bx->print_verilog_module_wrapper(wrp_vff, num_direct_channels, num_direct_packets);
	}
	
	delete bx;
	return 0;
}



int test_verilog(gh_str_list_t& lt_args){
	//bool is_cmpl = GH_GLOBALS.compl_sys.args_orig_has_compl;
	int resp = -1;
			
	gh_str_set_t lv_commds;
	
	lv_commds.insert("print_verilog_target_box");
	lv_commds.insert("print_verilog_network");
	
	std::string cho;
	bool ok1 = gh_args_select_one_of(lt_args, lv_commds, cho);
	if(! ok1){
		return -1;
	}

	bool done = false;
	if(! done && (cho == "print_verilog_target_box")){
		resp = GH_GLOBALS.rnr_print_verilog_target_box.run_test(lt_args);
		done = true;
	}
	if(! done && (cho == "print_verilog_network")){
		resp = GH_GLOBALS.rnr_print_verilog_network.run_test(lt_args);
		done = true;
	}
	
	
	return resp;
}

//--------------------------------------------------------------------------------
// verilog module printer

gh_string_t
hnode::get_verilog_id(){
	gh_string_t o_str = "gh_invalid_verilog_id";
	if(is_1to1()){
		hnode_1to1* th_nd = (hnode_1to1*)this;
		gh_1t1_kind_t k1t1 = th_nd->get_1t1_kind();
		GH_CK(k1t1 == gh_target_1t1);
		
		hnode_target* tg = (hnode_target*)th_nd;
		o_str = gh_vl_tgt + gh_long_to_string(tg->bx_idx);
	} else {
		o_str = gh_vl_nod + gh_long_to_string(addr);
	}
	return o_str;
}

gh_string_t
hnode::get_verilog_send_node_interface_name(hnode** ppout){
	GH_CK(ppout != gh_null);
	GH_CK(*ppout != gh_null);
	gh_string_t nd_id = get_verilog_id();
	if(*ppout == this){
		hnode** pnd = get_pt_out0();
		if(pnd == ppout){
			gh_string_t o_str = nd_id + gh_vl_out0;
			return o_str;
		} 
		pnd = get_pt_out1();
		GH_CK(pnd == ppout);
		gh_string_t o_str = nd_id + gh_vl_out1;
		return o_str;
	}
	//GH_CK((nd_id == dbg_get__in0_verilog_id()) || (nd_id == dbg_get__in1_verilog_id()));
	
	gh_string_t o_str = nd_id + gh_vl_sep + (*ppout)->get_verilog_id();
	return o_str;
}

gh_string_t
hnode::get_verilog_receive_node_interface_name(hnode** ppin){
	GH_CK(ppin != gh_null);
	GH_CK(*ppin!= gh_null);
	gh_string_t nd_id = get_verilog_id();
	if(*ppin == this){
		hnode** pnd = get_pt_in0();
		if(pnd == ppin){
			gh_string_t o_str = gh_vl_in0 + nd_id;
			return o_str;
		} 
		pnd = get_pt_in1();
		GH_CK(pnd == ppin);
		gh_string_t o_str = gh_vl_in1 + nd_id;
		return o_str;
	}
	//GH_CK((nd_id == dbg_get__out0_verilog_id()) || (nd_id == dbg_get__out1_verilog_id()));
	
	gh_string_t o_str = (*ppin)->get_verilog_id() + gh_vl_sep + nd_id;
	return o_str;
}

void
gh_print_verilog_declare_out_channel(verilog_file& vff, gh_string_t itf_nm, bool with_final_comma){
	FILE* ff = vff.fl;
	fprintf(ff, "\t");
	fprintf(ff, "`NS_DECLARE_OUT_CHNL(%s)", itf_nm.c_str());
	if(with_final_comma){
		fprintf(ff, ",");
	}
	fprintf(ff, "\n");
}

void
gh_print_verilog_declare_in_channel(verilog_file& vff, gh_string_t itf_nm, bool with_final_comma){
	FILE* ff = vff.fl;
	fprintf(ff, "\t");
	fprintf(ff, "`NS_DECLARE_IN_CHNL(%s)", itf_nm.c_str());
	if(with_final_comma){
		fprintf(ff, ",");
	}
	fprintf(ff, "\n");
}

void
gh_print_verilog_declare_pakout(verilog_file& vff, gh_string_t itf_nm, bool with_final_comma){
	FILE* ff = vff.fl;
	fprintf(ff, "\t");
	fprintf(ff, "`NS_DECLARE_PAKOUT_CHNL(%s)", itf_nm.c_str());
	if(with_final_comma){
		fprintf(ff, ",");
	}
	fprintf(ff, "\n");
}

void
gh_print_verilog_declare_pakin(verilog_file& vff, gh_string_t itf_nm, bool with_final_comma){
	FILE* ff = vff.fl;
	fprintf(ff, "\t");
	fprintf(ff, "`NS_DECLARE_PAKIN_CHNL(%s)", itf_nm.c_str());
	if(with_final_comma){
		fprintf(ff, ",");
	}
	fprintf(ff, "\n");
}

void 
gh_print_tabs(FILE* ff, long num_tabs){
	for(long aa = 0; aa < num_tabs; aa++){
		fprintf(ff, "\t");
	}
}

void
gh_print_verilog_declare_link_interface(verilog_file& vff, long num_tabs, gh_string_t itf_nm){
	gh_str_set_t& all_itf = vff.declared_interfaces;
	auto it = all_itf.find(itf_nm);
	if(it != all_itf.end()){
		GH_CK(*it == itf_nm);
		return;
	}
	all_itf.insert(itf_nm);
	
	FILE* ff = vff.fl;
	gh_print_tabs(ff, num_tabs);
	fprintf(ff, "`NS_DECLARE_LINK(%s)\n", itf_nm.c_str());
}

void
gh_print_verilog_declare_pakio_link_interface(verilog_file& vff, long num_tabs, gh_string_t itf_nm){
	gh_str_set_t& all_itf = vff.declared_interfaces;
	auto it = all_itf.find(itf_nm);
	if(it != all_itf.end()){
		GH_CK(*it == itf_nm);
		return;
	}
	all_itf.insert(itf_nm);
	
	FILE* ff = vff.fl;
	gh_print_tabs(ff, num_tabs);
	fprintf(ff, "`NS_DECLARE_PAKIO_LINK(%s)\n", itf_nm.c_str());
}

void
gh_print_verilog_assign_link_from_in_channel(verilog_file& vff, long num_tabs, gh_string_t itf_nm_1, gh_string_t itf_nm_2){
	gh_str_set_t& all_itf = vff.assigned_interfaces;
	auto it = all_itf.find(itf_nm_1);
	if(it != all_itf.end()){
		GH_CK(*it == itf_nm_1);
		return;
	}
	all_itf.insert(itf_nm_1);
	
	FILE* ff = vff.fl;
	gh_print_tabs(ff, num_tabs);
	fprintf(ff, "`NS_ASSIGN_LINK_FROM_IN_CHNL(%s, %s)\n", itf_nm_1.c_str(), itf_nm_2.c_str());
}

void
gh_print_verilog_assign_out_channel_from_link(verilog_file& vff, long num_tabs, gh_string_t itf_nm_1, gh_string_t itf_nm_2){
	gh_str_set_t& all_itf = vff.assigned_out_channels;
	auto it = all_itf.find(itf_nm_1);
	if(it != all_itf.end()){
		GH_CK(*it == itf_nm_1);
		return;
	}
	all_itf.insert(itf_nm_1);
	
	FILE* ff = vff.fl;
	gh_print_tabs(ff, num_tabs);
	fprintf(ff, "`NS_ASSIGN_OUT_CHNL_FROM_LINK(%s, %s)\n", itf_nm_1.c_str(), itf_nm_2.c_str());
}

void
gh_print_verilog_instance_send_interface(verilog_file& vff, long num_tabs, gh_string_t itf_nm_1, gh_string_t itf_nm_2, bool with_final_comma){
	FILE* ff = vff.fl;
	gh_print_tabs(ff, num_tabs);
	fprintf(ff, "`NS_INSTA_SND_CHNL(%s, %s)", itf_nm_1.c_str(), itf_nm_2.c_str());
	
	if(with_final_comma){
		fprintf(ff, ",");
	}
	fprintf(ff, "\n");
}

void
gh_print_verilog_instance_receive_interface(verilog_file& vff, long num_tabs, gh_string_t itf_nm_1, gh_string_t itf_nm_2, bool with_final_comma){
	FILE* ff = vff.fl;
	gh_print_tabs(ff, num_tabs);
	fprintf(ff, "`NS_INSTA_RCV_CHNL(%s, %s)", itf_nm_1.c_str(), itf_nm_2.c_str());
	
	if(with_final_comma){
		fprintf(ff, ",");
	}
	fprintf(ff, "\n");
}

void
gh_print_verilog_instance_send_channel_from_channel(verilog_file& vff, long num_tabs, gh_string_t itf_nm_1, gh_string_t itf_nm_2, 
													bool with_final_comma)
{
	FILE* ff = vff.fl;
	gh_print_tabs(ff, num_tabs);
	fprintf(ff, "`NS_INSTA_SND_CHNL_FROM_CHNL(%s, %s)", itf_nm_1.c_str(), itf_nm_2.c_str());
	
	if(with_final_comma){
		fprintf(ff, ",");
	}
	fprintf(ff, "\n");
}

void
gh_print_verilog_instance_receive_channel_from_channel(verilog_file& vff, long num_tabs, gh_string_t itf_nm_1, gh_string_t itf_nm_2, 
													   bool with_final_comma)
{
	FILE* ff = vff.fl;
	gh_print_tabs(ff, num_tabs);
	fprintf(ff, "`NS_INSTA_RCV_CHNL_FROM_CHNL(%s, %s)", itf_nm_1.c_str(), itf_nm_2.c_str());
	
	if(with_final_comma){
		fprintf(ff, ",");
	}
	fprintf(ff, "\n");
}

void
hnode_2to1::print_verilog_2to1_instance(verilog_file& vff, long idx){
	FILE* ff = vff.fl;
	gh_string_t i0_itf_nm = get_verilog_receive_node_interface_name(&in0);
	gh_string_t i1_itf_nm = get_verilog_receive_node_interface_name(&in1);
	gh_string_t o0_itf_nm = get_verilog_send_node_interface_name(&out0);
	
	gh_print_verilog_declare_link_interface(vff, 1, i0_itf_nm);
	gh_print_verilog_declare_link_interface(vff, 1, i1_itf_nm);
	gh_print_verilog_declare_link_interface(vff, 1, o0_itf_nm);

	vff.print_verilog_inc_ready_and();
	fprintf(ff, R"base(
	nd_2to1 it_nd_2to1_%ld (
		`NS_INSTA_GLB_CHNL_VALS(gch, gch_clk, gch_reset, rdy%ld_lv0),
)base", idx, vff.tot_rdy);
	
	gh_print_verilog_instance_send_interface(vff, 2, gh_vl_snd0, o0_itf_nm);
	gh_print_verilog_instance_receive_interface(vff, 2, gh_vl_rcv0, i0_itf_nm);
	gh_print_verilog_instance_receive_interface(vff, 2, gh_vl_rcv1, i1_itf_nm, false);
	
	fprintf(ff, "\t); \n\n");
	
}

gh_string_t
edge::get_verilog_oper(){
	gh_string_t op = "INVALID_OPER";
	if(is_gt()){
		if(is_eq()){
			op = "`NS_GTE_OP";
			return op;
		}
		op = "`NS_GT_OP";
		return op;
	}
	if(is_eq()){
		op = "`NS_LTE_OP";
		return op;
	}
	op = "`NS_LT_OP";
	return op;
}

gh_string_t
edge::get_verilog_ref_val(){
	return gh_long_to_string(slc_edge);
}

gh_string_t
hnode_1to2::get_verilog_oper_1(){
	if(selector.lft.is_undef()){
		GH_CK(! selector.rgt.is_undef());
		return selector.rgt.get_verilog_oper();
	}
	return selector.lft.get_verilog_oper();
}

gh_string_t
hnode_1to2::get_verilog_ref_val_1(){
	if(selector.lft.is_undef()){
		GH_CK(! selector.rgt.is_undef());
		return selector.rgt.get_verilog_ref_val();
	}
	return selector.lft.get_verilog_ref_val();
}

gh_string_t
hnode_1to2::get_verilog_is_range(){
	bool is_itv = get_flag(gh_is_interval);
	gh_string_t is_rng = "`NS_FALSE";
	bool has_udf = selector.lft.is_undef() || selector.rgt.is_undef();
	if(is_itv && ! has_udf){
		is_rng = "`NS_TRUE";
	}
	return is_rng;
}

gh_string_t
hnode_1to2::get_verilog_oper_2(){
	return selector.rgt.get_verilog_oper();
}

gh_string_t
hnode_1to2::get_verilog_ref_val_2(){
	return selector.rgt.get_verilog_ref_val();
}

void
hnode_1to2::print_verilog_1to2_instance(verilog_file& vff, long idx){
	FILE* ff = vff.fl;
	gh_string_t i0_itf_nm = get_verilog_receive_node_interface_name(&in0);
	gh_string_t o0_itf_nm = get_verilog_send_node_interface_name(&out0);
	gh_string_t o1_itf_nm = get_verilog_send_node_interface_name(&out1);
	
	gh_print_verilog_declare_link_interface(vff, 1, i0_itf_nm);
	gh_print_verilog_declare_link_interface(vff, 1, o0_itf_nm);
	gh_print_verilog_declare_link_interface(vff, 1, o1_itf_nm);
	
	gh_string_t op1 = get_verilog_oper_1();
	gh_string_t val1 = get_verilog_ref_val_1();
	gh_string_t is_rng = get_verilog_is_range();
	gh_string_t op2 = get_verilog_oper_2();
	gh_string_t val2 = get_verilog_ref_val_2();
	
	vff.print_verilog_inc_ready_and();
	fprintf(ff, R"base(
	nd_1to2 #(.OPER_1(%s), .REF_VAL_1(%s), .IS_RANGE(%s), .OPER_2(%s), .REF_VAL_2(%s))
	it_nd_1to2_%ld (
		`NS_INSTA_GLB_CHNL_VALS(gch, gch_clk, gch_reset, rdy%ld_lv0),
)base", 
	op1.c_str(),
	val1.c_str(),
	is_rng.c_str(),
	op2.c_str(),
	val2.c_str(),
	idx,
	vff.tot_rdy
	);
	
	gh_print_verilog_instance_send_interface(vff, 2, gh_vl_snd0, o0_itf_nm);
	gh_print_verilog_instance_send_interface(vff, 2, gh_vl_snd1, o1_itf_nm);
	gh_print_verilog_instance_receive_interface(vff, 2, gh_vl_rcv0, i0_itf_nm, false);
	
	fprintf(ff, "\t); \n\n");
	
}

void
gh_print_verilog_assigns_for_ios(verilog_file& vff, ppnode_vec_t& all_io, gh_route_side_t sd, gh_io_kind_t iok){
	FILE* ff = vff.fl;
	fflush(ff);
	for(long ii = 0; ii < (long)all_io.size(); ii++){
		hnode** ppi = all_io[ii];
		if(ppi != gh_null){
			hnode* pi = *ppi;
			GH_CK(pi != gh_null);

			gh_string_t itf_nm;
			if(iok == gh_in_kind){
				itf_nm = pi->get_verilog_receive_node_interface_name(ppi);
			} else {
				GH_CK(iok == gh_out_kind);
				itf_nm = pi->get_verilog_send_node_interface_name(ppi);
			}
			gh_print_verilog_declare_link_interface(vff, 1, itf_nm);
			
			gh_string_t io_nm = get_verilog_io_name(ii, sd, iok);
			
			if(iok == gh_in_kind){
				gh_print_verilog_assign_link_from_in_channel(vff, 1, itf_nm, io_nm);
			} else {
				GH_CK(iok == gh_out_kind);
				gh_print_verilog_assign_out_channel_from_link(vff, 1, io_nm, itf_nm);
			}
		} else {
			fprintf(ff, "// NULL io %ld \n", ii);
		}
	}
	fprintf(ff, "\n\n");
	fflush(ff);
}

void
htarget_box::print_verilog_instance_io_net(verilog_file& vff, gh_string_t io_nm, long lnk_idx, 
										   gh_route_side_t sd, gh_io_kind_t iok, bool with_comma)
{
	gh_string_t lnk_nm = get_verilog_target_link_name(lnk_idx, sd, iok);
	if(iok == gh_in_kind){
		gh_print_verilog_instance_receive_interface(vff, 2, io_nm.c_str(), lnk_nm.c_str(), with_comma);
	} else {
		GH_CK(iok == gh_out_kind);
		gh_print_verilog_instance_send_interface(vff, 2, io_nm.c_str(), lnk_nm.c_str(), with_comma);
	}
}

void
htarget_box::print_verilog_declare_io_link_net(verilog_file& vff, long lnk_idx, gh_route_side_t sd, gh_io_kind_t iok)
{
	gh_string_t lnk_nm = get_verilog_target_link_name(lnk_idx, sd, iok);
	gh_print_verilog_declare_link_interface(vff, 1, lnk_nm);
}

void
htarget_box::print_verilog_declare_pakio_link_net(verilog_file& vff, long lnk_idx, gh_route_side_t sd, gh_io_kind_t iok)
{
	gh_string_t lnk_nm = get_verilog_target_link_name(lnk_idx, sd, iok);
	gh_print_verilog_declare_pakio_link_interface(vff, 1, lnk_nm);
}

void
htarget_box::print_verilog_instance_io_wrapper(verilog_file& vff, gh_string_t io_nm, gh_io_kind_t iok, bool with_comma)
{
	if(iok == gh_in_kind){
		gh_print_verilog_instance_receive_channel_from_channel(vff, 2, io_nm.c_str(), io_nm.c_str(), with_comma);
	} else {
		GH_CK(iok == gh_out_kind);
		gh_print_verilog_instance_send_channel_from_channel(vff, 2, io_nm.c_str(), io_nm.c_str(), with_comma);
	}
}

void
htarget_box::print_verilog_params_for_ios(verilog_file& vff, gh_verilog_print_op_t op, long tot_io, 
										  gh_route_side_t sd, gh_io_kind_t iok, bool with_final_comma)
{
	FILE* ff = vff.fl;
	fflush(ff);
	
	for(long ii = 0; ii < tot_io; ii++){
		gh_string_t io_nm = get_verilog_io_name(ii, sd, iok);
		bool with_comma = (with_final_comma || (ii < (tot_io - 1)));
		if(op == gh_vl_declare_param_op){
			if(iok == gh_in_kind){
				gh_print_verilog_declare_in_channel(vff, io_nm, with_comma);
			} else {
				GH_CK(iok == gh_out_kind);
				gh_print_verilog_declare_out_channel(vff, io_nm, with_comma);
			}
		} else if(op == gh_vl_declare_link_op) {
			print_verilog_declare_io_link_net(vff, ii, sd, iok);
		} else if(op == gh_vl_instance_net_op) {
			print_verilog_instance_io_net(vff, io_nm, ii, sd, iok, with_comma);
		} else {
			GH_CK(op == gh_vl_instance_wrapper_op);
			print_verilog_instance_io_wrapper(vff, io_nm, iok, with_comma);
		}
	}
	fflush(ff);
}

void
gh_print_verilog_code_for_nodes(verilog_file& vff, pnode_vec_t& all_nd){
	//FILE* ff = vff.fl;
	for(long ii = 0; ii < (long)all_nd.size(); ii++){
		hnode* the_nd = all_nd[ii];
		GH_CK(the_nd != gh_null);
		//fprintf(ff, "\t// node: (%p)\n", (void*)the_nd);
		if(the_nd->is_1to2()){
			hnode_1to2* nd = (hnode_1to2*)the_nd;
			nd->print_verilog_1to2_instance(vff, ii);
		} else {
			GH_CK(the_nd->is_2to1());
			hnode_2to1* nd = (hnode_2to1*)the_nd;
			nd->print_verilog_2to1_instance(vff, ii);
		}
	}
}

gh_string_t
htarget_box::get_verilog_router_module_name(){
	GH_CK(target != gh_null);
	gh_string_t ss = gh_vl_tgt + gh_long_to_string(get_index()) + gh_vl_tg_router;
	return ss;
}

gh_string_t
htarget_box::get_verilog_wrapper_module_name(){
	GH_CK(target != gh_null);
	gh_string_t ss =  gh_vl_tgt + gh_long_to_string(get_index()) + gh_vl_tg_wrapper;
	return ss;
}

gh_string_t
htarget_box::get_verilog_core_module_name(){
	GH_CK(target != gh_null);
	gh_string_t ss =  gh_vl_tgt + gh_long_to_string(get_index()) + gh_vl_tg_core;
	return ss;
}

gh_string_t
htarget_box::get_verilog_target_param_name(gh_io_kind_t iok){
	gh_string_t iok_nm = get_verilog_io_kind(iok);
	GH_CK(target != gh_null);
	//gh_string_t ss = gh_vl_tgt + iok_nm + gh_long_to_string(get_index());
	gh_string_t ss = gh_vl_tgt + iok_nm;
	return ss;
}

gh_string_t
htarget_box::get_verilog_target_link_name(long lnk, gh_route_side_t sd, gh_io_kind_t iok){
	long tg = get_index();
	
	bool is_lft_in = ((sd == gh_left_side) && (iok == gh_in_kind));
	bool is_rgt_out = ((sd == gh_right_side) && (iok == gh_out_kind));
	if(is_lft_in){
		tg -= 1;
		iok = gh_out_kind;
	}
	if(is_rgt_out){
		tg -= 1;
		iok = gh_in_kind;
	}
	gh_string_t ss = gh_vl_tgt + gh_long_to_string(tg) + gh_vl_sep + get_verilog_io_name(lnk, sd, iok);
	return ss;
}

void 
htarget_box::print_verilog_target_param(verilog_file& vff){
	gh_string_t itf_in_nm = get_verilog_target_param_name(gh_in_kind);
	gh_string_t itf_out_nm = get_verilog_target_param_name(gh_out_kind);
	gh_print_verilog_declare_in_channel(vff, itf_in_nm);
	gh_print_verilog_declare_out_channel(vff, itf_out_nm, false);
}

void
htarget_box::print_verilog_router_target_assign(verilog_file& vff){
	gh_string_t rcv_nm = get_verilog_target_param_name(gh_in_kind);
	gh_string_t snd_nm = get_verilog_target_param_name(gh_out_kind);
	GH_CK(target != gh_null);
	gh_string_t nd_tg_itf_nm = target->get_verilog_receive_node_interface_name(&(target->in0));
	gh_string_t tg_nd_itf_nm = target->get_verilog_send_node_interface_name(&(target->out0));
	
	gh_print_verilog_assign_out_channel_from_link(vff, 1, snd_nm, nd_tg_itf_nm);
	gh_print_verilog_assign_link_from_in_channel(vff, 1, tg_nd_itf_nm, rcv_nm);
}

void
htarget_box::print_verilog_module_core(verilog_file& vff, runner_print_verilog_network& cfg){
	FILE* ff = vff.fl;
	gh_string_t itf_in_nm = get_verilog_target_param_name(gh_in_kind);
	gh_string_t itf_out_nm = get_verilog_target_param_name(gh_out_kind);
	
	long loc_addr = get_index();
	long src_addr = 0;
	long dst_addr = 0;
	
	gh_string_t src_nd = "hnull";
	gh_string_t dst_nd = "hnull";
	
	auto its = cfg.all_src_probes.find(loc_addr);
	if(its != cfg.all_src_probes.end()){
		src_nd = "hprb";
		dst_addr = its->second;
	}
	auto itd = cfg.all_dst_probes.find(loc_addr);
	if(itd != cfg.all_dst_probes.end()){
		dst_nd = "hprb";
		src_addr = itd->second;
	}
	
	fprintf(ff, R"base(
/*
	THIS IS A NULL CORE FOR THIS TARGET.
	REPLACE THIS FILE WITH THE CODE YOU WISH FOR THIS TARGET
	HAVE IN MIND THAT THE CODE MUST FOLLOW THE ASYNCHRONOUS PROTOCOL OF THE NETWORK
	FOR THE WHOLE NETWORK TO WORK
	IF ONE TARGET FAILS TO FOLOW THE PROTOCOL THE WHOLE NETWORK MIGHT FAIL
*/
		
`include "hglobal.v"

`default_nettype	none

module %s
#(parameter 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	`NS_DECLARE_GLB_CHNL(gch),
	`NS_DECLARE_IN_CHNL(%s),
	`NS_DECLARE_OUT_CHNL(%s),
	`NS_DECLARE_ERR_CHNL(err0)
);
	localparam MY_LOCAL_ADDR = %ld;
	
	wire rdy1;
	%s_source #(.MY_LOCAL_ADDR(MY_LOCAL_ADDR), .PRB_DST_ADDR(%ld))
	it_core_src (
		`NS_INSTA_GLB_CHNL_VALS(gch, gch_clk, gch_reset, rdy1),
		`NS_INSTA_SND_CHNL_FROM_CHNL(snd0, %s)
	);
 
	wire rdy2;
	%s_sink #(.MY_LOCAL_ADDR(MY_LOCAL_ADDR), .PRB_SRC_ADDR(%ld))
	it_core_snk (
		`NS_INSTA_GLB_CHNL_VALS(gch, gch_clk, gch_reset, rdy2),
		`NS_INSTA_RCV_CHNL_FROM_CHNL(rcv0, %s),
		`NS_INSTA_ERR_CHNL(err0, err0)
	);
	
	assign gch_ready = rdy1 && rdy2;
 
endmodule

)base", 
	get_verilog_core_module_name().c_str(), 
	itf_in_nm.c_str(), 
	itf_out_nm.c_str(),
	get_index(),
	src_nd.c_str(),
	dst_addr,
	itf_out_nm.c_str(),
	dst_nd.c_str(),
	src_addr,
	itf_in_nm.c_str() 
);
	
}

void
htarget_box::print_verilog_module_router(verilog_file& vff){
	FILE* ff = vff.fl;
	
	fprintf(ff, R"base(
`include "hglobal.v"

`default_nettype	none

module %s
#(parameter 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	`NS_DECLARE_GLB_CHNL(gch),
)base", get_verilog_router_module_name().c_str());

	print_verilog_all_params(vff, gh_vl_declare_param_op, 0, 0, true);
	
	print_verilog_target_param(vff);

	fprintf(ff, ");\n");

	gh_print_verilog_assigns_for_ios(vff, lft_in, gh_left_side, gh_in_kind);
	gh_print_verilog_assigns_for_ios(vff, lft_out, gh_left_side, gh_out_kind);
	gh_print_verilog_assigns_for_ios(vff, rgt_in, gh_right_side, gh_in_kind);
	gh_print_verilog_assigns_for_ios(vff, rgt_out, gh_right_side, gh_out_kind);
	
	gh_print_verilog_code_for_nodes(vff, all_nodes);
	
	print_verilog_router_target_assign(vff);
	
	vff.print_verilog_ready_final_and("gch_ready");
	
	fprintf(ff, R"base(

 
endmodule

)base");
	
}

void
htarget_box::print_verilog_params_for_direct_channels(verilog_file& vff, gh_verilog_print_op_t op, long tot_core_ios, 
											long num_direct_ios, gh_route_side_t sd, gh_io_kind_t iok, bool with_final_comma)
{
	FILE* ff = vff.fl;
	fflush(ff);
	
	for(long ii = 0; ii < num_direct_ios; ii++){
		long id_io = tot_core_ios + ii;
		gh_string_t io_nm = get_verilog_io_name(id_io, sd, iok);
		bool with_comma = (with_final_comma || (ii < (num_direct_ios - 1)));
		if(op == gh_vl_declare_param_op){
			if(iok == gh_in_kind){
				gh_print_verilog_declare_in_channel(vff, io_nm, with_comma);
			} else {
				GH_CK(iok == gh_out_kind);
				gh_print_verilog_declare_out_channel(vff, io_nm, with_comma);
			}
		} else if(op == gh_vl_declare_link_op) {
			print_verilog_declare_io_link_net(vff, id_io, sd, iok);
		} else {
			GH_CK(op == gh_vl_instance_net_op);
			print_verilog_instance_io_net(vff, io_nm, id_io, sd, iok, with_comma);
		}
	}
	fflush(ff);
}

void
htarget_box::print_verilog_params_for_direct_packets(verilog_file& vff, gh_verilog_print_op_t op, long tot_core_ios, 
										   long num_direct_ios, gh_route_side_t sd, gh_io_kind_t iok, bool with_final_comma)
{
	FILE* ff = vff.fl;
	fflush(ff);
	
	for(long ii = 0; ii < num_direct_ios; ii++){
		long id_io = tot_core_ios + ii;
		gh_string_t io_nm = get_verilog_io_name(id_io, sd, iok);
		bool with_comma = (with_final_comma || (ii < (num_direct_ios - 1)));
		if(op == gh_vl_declare_param_op){
			if(iok == gh_in_kind){
				gh_print_verilog_declare_pakin(vff, io_nm, with_comma);
			} else {
				GH_CK(iok == gh_out_kind);
				gh_print_verilog_declare_pakout(vff, io_nm, with_comma);
			}
		} else if(op == gh_vl_declare_link_op) {
			print_verilog_declare_pakio_link_net(vff, id_io, sd, iok);
		} else {
			GH_CK(op == gh_vl_instance_net_op);
			print_verilog_instance_io_net(vff, io_nm, id_io, sd, iok, with_comma);
		}
	}
	fflush(ff);
}

void
gh_print_verilog_assigns_for_direct_ios(verilog_file& vff, long tot_core_ios, long num_direct_chns, long num_direct_paks){
	FILE* ff = vff.fl;
	fflush(ff);
	for(long ii = 0; ii < num_direct_chns; ii++){
		long id_io = tot_core_ios + ii;
		gh_string_t li = get_verilog_io_name(id_io, gh_left_side, gh_in_kind);
		gh_string_t lo = get_verilog_io_name(id_io, gh_left_side, gh_out_kind);
		gh_string_t ri = get_verilog_io_name(id_io, gh_right_side, gh_in_kind);
		gh_string_t ro = get_verilog_io_name(id_io, gh_right_side, gh_out_kind);
		
		gh_print_tabs(ff, 1);
		fprintf(ff, "`NS_ASSIGN_OUT_CHNL_FROM_IN_CHNL(%s, %s)\n", lo.c_str(), li.c_str());
		gh_print_tabs(ff, 1);
		fprintf(ff, "`NS_ASSIGN_OUT_CHNL_FROM_IN_CHNL(%s, %s)\n", ro.c_str(), ri.c_str());
	}
	for(long ii = 0; ii < num_direct_paks; ii++){
		long id_io = tot_core_ios + num_direct_chns + ii;
		gh_string_t li = get_verilog_io_name(id_io, gh_left_side, gh_in_kind);
		gh_string_t lo = get_verilog_io_name(id_io, gh_left_side, gh_out_kind);
		gh_string_t ri = get_verilog_io_name(id_io, gh_right_side, gh_in_kind);
		gh_string_t ro = get_verilog_io_name(id_io, gh_right_side, gh_out_kind);
		
		gh_print_tabs(ff, 1);
		fprintf(ff, "`NS_ASSIGN_PAKOUT_FROM_PAKIN(%s, %s)\n", lo.c_str(), li.c_str());
		gh_print_tabs(ff, 1);
		fprintf(ff, "`NS_ASSIGN_PAKOUT_FROM_PAKIN(%s, %s)\n", ro.c_str(), ri.c_str());
	}
	fprintf(ff, "\n\n");
	fflush(ff);
}

gh_string_t 
gh_get_core_link_name(long num){
	gh_string_t nm = gh_vl_tg_core_lnk + gh_long_to_string(num);
	return nm;
}

void
htarget_box::print_verilog_instance_core(verilog_file& vff){
	FILE* ff = vff.fl;
	gh_string_t core_rcv = gh_get_core_link_name(0);
	gh_string_t core_snd = gh_get_core_link_name(1);
	gh_string_t itf_in_nm = get_verilog_target_param_name(gh_in_kind);
	gh_string_t itf_out_nm = get_verilog_target_param_name(gh_out_kind);
	
	gh_print_verilog_declare_link_interface(vff, 1, core_rcv);
	gh_print_verilog_declare_link_interface(vff, 1, core_snd);
		
	vff.print_verilog_inc_ready_and();
	fprintf(ff, R"base(
	%s it_core (
		`NS_INSTA_GLB_CHNL_VALS(gch, gch_clk, gch_reset, rdy%ld_lv0),
		`NS_INSTA_ERR_CHNL(err0, err0),
)base", get_verilog_core_module_name().c_str(), vff.tot_rdy);
		
	gh_print_verilog_instance_receive_interface(vff, 2, itf_in_nm.c_str(), core_rcv.c_str(), true);
	gh_print_verilog_instance_send_interface(vff, 2, itf_out_nm.c_str(), core_snd.c_str(), false);
	fprintf(ff, "\t); \n\n");
	
}

void
htarget_box::print_verilog_instance_router(verilog_file& vff){
	FILE* ff = vff.fl;
	gh_string_t core_rcv = gh_get_core_link_name(0);
	gh_string_t core_snd = gh_get_core_link_name(1);
	gh_string_t itf_in_nm = get_verilog_target_param_name(gh_in_kind);
	gh_string_t itf_out_nm = get_verilog_target_param_name(gh_out_kind);
	
	vff.print_verilog_inc_ready_and();
	fprintf(ff, R"base(
	%s it_router (
		`NS_INSTA_GLB_CHNL_VALS(gch, gch_clk, gch_reset, rdy%ld_lv0),
)base", get_verilog_router_module_name().c_str(), vff.tot_rdy);
	
	print_verilog_all_params(vff, gh_vl_instance_wrapper_op, 0, 0, true);
	
	gh_print_verilog_instance_receive_interface(vff, 2, itf_in_nm.c_str(), core_snd.c_str(), true);
	gh_print_verilog_instance_send_interface(vff, 2, itf_out_nm.c_str(), core_rcv.c_str(), false);
	
	fprintf(ff, "\t); \n\n");
}

void
htarget_box::print_verilog_all_params(verilog_file& vff, gh_verilog_print_op_t op, 
									  long num_direct_chns, long num_direct_paks, bool with_final_comma)
{
	FILE* ff = vff.fl;
	bool has_direct = can_verilog_module_have_wrapper() && ((num_direct_chns > 0) || (num_direct_paks > 0));
	bool prt_lst_comma = (with_final_comma || has_direct);
	
	long lft_in_sz = (long)lft_in.size();
	long lft_out_sz = (long)lft_out.size();
	long rgt_in_sz = (long)rgt_in.size();
	long rgt_out_sz = (long)rgt_out.size();
	
	bool end_aft_1 = ((lft_out_sz + rgt_in_sz + rgt_out_sz) == 0);
	bool end_aft_2 = ((rgt_in_sz + rgt_out_sz) == 0);
	bool end_aft_3 = (rgt_out_sz == 0);

	fprintf(ff, "\t\t// ALL_ROUTER_CHNLS wfc=%d hd=%d\n", with_final_comma, has_direct);
	print_verilog_params_for_ios(vff, op, lft_in_sz, gh_left_side, gh_in_kind, (! end_aft_1 || prt_lst_comma));
	print_verilog_params_for_ios(vff, op, lft_out_sz, gh_left_side, gh_out_kind, (! end_aft_2 || prt_lst_comma));
	print_verilog_params_for_ios(vff, op, rgt_in_sz, gh_right_side, gh_in_kind, (! end_aft_3 || prt_lst_comma));
	print_verilog_params_for_ios(vff, op, rgt_out_sz, gh_right_side, gh_out_kind, prt_lst_comma);
	
	if(has_direct){
		GH_CK(rgt_in.size() == rgt_out.size());
		GH_CK(rgt_in.size() == lft_in.size());
		long from_pm = lft_in_sz;
		
		bool has_paks = (num_direct_paks > 0);
		
		if(num_direct_chns > 0){
			bool with_lst_comma = (has_paks || with_final_comma);
			
			fprintf(ff, "\t\t// ALL_DIRECT_CHNLS\n");
			print_verilog_params_for_direct_channels(vff, op, from_pm, num_direct_chns, gh_left_side, gh_in_kind, true);
			print_verilog_params_for_direct_channels(vff, op, from_pm, num_direct_chns, gh_left_side, gh_out_kind, true);
			print_verilog_params_for_direct_channels(vff, op, from_pm, num_direct_chns, gh_right_side, gh_in_kind, true);
			print_verilog_params_for_direct_channels(vff, op, from_pm, num_direct_chns, gh_right_side, gh_out_kind, with_lst_comma);
		}
		
		if(has_paks){
			from_pm += num_direct_chns;
			
			fprintf(ff, "\t\t// ALL_DIRECT_PACKS\n");
			print_verilog_params_for_direct_packets(vff, op, from_pm, num_direct_paks, gh_left_side, gh_in_kind, true);
			print_verilog_params_for_direct_packets(vff, op, from_pm, num_direct_paks, gh_left_side, gh_out_kind, true);
			print_verilog_params_for_direct_packets(vff, op, from_pm, num_direct_paks, gh_right_side, gh_in_kind, true);
			print_verilog_params_for_direct_packets(vff, op, from_pm, num_direct_paks, gh_right_side, gh_out_kind, with_final_comma);
		}
	}

}

void
htarget_box::print_verilog_module_wrapper(verilog_file& vff, long num_direct_chns, long num_direct_paks){
	FILE* ff = vff.fl;
	
	fprintf(ff, R"base(
`include "hglobal.v"

`default_nettype	none

module %s
#(parameter
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	`NS_DECLARE_GLB_CHNL(gch),
	`NS_DECLARE_ERR_CHNL(err0),
)base", get_verilog_wrapper_module_name().c_str());
	
	print_verilog_all_params(vff, gh_vl_declare_param_op, num_direct_chns, num_direct_paks, false);
	
	fprintf(ff, ");\n");

	bool can_have_direct = can_verilog_module_have_wrapper();
	if(can_have_direct){
		long tot_io = (long)lft_in.size();
		
		gh_print_verilog_assigns_for_direct_ios(vff, tot_io, num_direct_chns, num_direct_paks);
	}
	
	print_verilog_instance_core(vff);
	print_verilog_instance_router(vff);
	
	vff.print_verilog_ready_final_and("gch_ready");
	
	fprintf(ff, R"base(

 
endmodule

)base");
	
}

gh_string_t 
gh_get_verilog_targets_link(long tg1, long tg2, gh_route_side_t sd, long num_lnk){
	gh_string_t sd_nm = get_verilog_side(sd);
	gh_string_t str_lnk = gh_vl_tgts_lnk;
	gh_string_t str_tgt = gh_vl_tgt;
	gh_string_t str_sep = gh_vl_tgts_lnk_sep;
	gh_string_t resp = str_lnk + 
		str_tgt + gh_long_to_string(tg1) + str_sep + 
		str_tgt + gh_long_to_string(tg1) + str_sep + 
		sd_nm + str_sep + 
		gh_long_to_string(num_lnk);
	return resp;
}

void
gh_print_verilog_declare_all_link_target_wrapper_param(verilog_file& vff, long tot_io, long tg1, long tg2, gh_route_side_t sd){
	FILE* ff = vff.fl;
	fflush(ff);
	for(long ii = 0; ii < tot_io; ii++){
		gh_string_t lnk_nm = gh_get_verilog_targets_link(tg1, tg2, sd, ii);
		gh_print_tabs(ff, 2);
		fprintf(ff, "`NS_DECLARE_LINK(%s)\n", lnk_nm.c_str());
	}
	fflush(ff);
}

void
htarget_box::print_verilog_instance_wrapper(verilog_file& vff, long num_direct_chns, long num_direct_paks){
	FILE* ff = vff.fl;
	gh_string_t wrp_nm = get_verilog_wrapper_module_name();
	
	print_verilog_all_params(vff, gh_vl_declare_link_op, num_direct_chns, num_direct_paks, false);
	
	vff.print_verilog_inc_ready_and();
	vff.print_verilog_inc_error_selec();
	
	fprintf(ff, R"base(
	%s it_%s (
		`NS_INSTA_GLB_CHNL_VALS(gch, gch_clk, gch_reset, rdy%ld_lv0),
		`NS_INSTA_ERR_CHNL(err0, err%ld_lv0),
)base", wrp_nm.c_str(), wrp_nm.c_str(), vff.tot_rdy, vff.tot_err);
	
	print_verilog_all_params(vff, gh_vl_instance_net_op, num_direct_chns, num_direct_paks, false);

	fprintf(ff, "\t); \n\n");
}

void
hlognet_box::copy_verilog_foundation_files(gh_string_t& net_nam, runner_print_verilog_network& cfg){
	gh_string_t& dir_name = cfg.dir_name;
	
	gh_string_t fnd_dir = dir_name + gh_path_sep + cfg.vl_sub_fnd_dir;
	GH_CK_PRT(gh_file_exists(fnd_dir), "DIRECTORY %s NOT FOUND. THIS MEANS A BAD INSTALLATION OF THE PROGRAM.", fnd_dir.c_str());
	gh_string_t exe_dir = gh_path_get_directory(gh_get_executable_path(), false);
	gh_string_t orig_fnd_dir = exe_dir + gh_path_sep + cfg.vl_exe_fnd_dir;
	GH_CK_PRT(gh_file_exists(orig_fnd_dir), "DIRECTORY %s NOT FOUND. THIS MEANS A BAD INSTALLATION OF THE PROGRAM.", orig_fnd_dir.c_str());
	
	for(long aa = 0; aa < (long)(cfg.vl_all_fnd_files.size()); aa++){
		gh_string_t ff_nam = cfg.vl_all_fnd_files[aa];
		gh_string_t src_path = orig_fnd_dir + gh_path_sep + ff_nam;
		GH_CK_PRT(gh_file_exists(src_path), "FILE %s NOT FOUND. THIS MEANS A BAD INSTALLATION OF THE PROGRAM.", src_path.c_str());
		gh_string_t dst_path = fnd_dir + gh_path_sep + ff_nam;
		gh_copy_file(src_path, dst_path, cfg.vl_cp_file_buff);
		
		print_verilog_file_name_to_command_files(cfg.vl_sub_fnd_dir + gh_path_sep + ff_nam, cfg);
	}
	
	
	gh_string_t dst_mk = dir_name + gh_path_sep + cfg.vl_make_dst_file_nm;
	
	FILE* mkff = fopen(dst_mk.c_str(), "w");
	if(mkff == NULL){
		fprintf(stderr, "Cannot open file %s for writing !!\n", dst_mk.c_str());
		return;
	}
	fprintf(mkff, "\n\n\n");
	fprintf(mkff, "PRJ_NM = %s\n", net_nam.c_str());
	fprintf(mkff, "TOP_MOD = %s\n", net_nam.c_str());
	fclose(mkff);
	
	gh_string_t src_mk = orig_fnd_dir + gh_path_sep + cfg.vl_make_src_file_nm;
	gh_append_file(src_mk, dst_mk, cfg.vl_cp_file_buff);
	
	gh_string_t src_pcf = orig_fnd_dir + gh_path_sep + cfg.vl_pcf_file_nm;
	gh_string_t dst_pcf = dir_name + gh_path_sep + cfg.vl_pcf_file_nm;
	gh_copy_file(src_pcf, dst_pcf, cfg.vl_cp_file_buff);
	
	gh_string_t src_cl = orig_fnd_dir + gh_path_sep + cfg.vl_clean_file_nm;
	gh_string_t dst_cl = dir_name + gh_path_sep + cfg.vl_clean_file_nm;
	gh_copy_file(src_cl, dst_cl, cfg.vl_cp_file_buff);
}

void
hlognet_box::print_verilog_file_name_to_iverilog_file(FILE* ff, gh_string_t nm){
	fprintf(ff, "%s\n", nm.c_str());
}

void
hlognet_box::print_verilog_file_name_to_verilator_file(FILE* ff, gh_string_t nm){
	fprintf(ff, "%s\n", nm.c_str());
}

void
hlognet_box::print_verilog_file_name_to_yosys_file(FILE* ff, gh_string_t nm){
	fprintf(ff, "read_verilog -I./rtl/foundation %s\n", nm.c_str());
}

void
hlognet_box::print_verilog_header_to_yosys_file(FILE* ff){
	fprintf(ff, "yosys -import\n");
	
}

void
hlognet_box::print_verilog_footer_to_yosys_file(FILE* ff, gh_string_t nm){
	fprintf(ff, "synth_ice40 -top %s -json ./$::env(BUILD_DIR)/%s.json;\n", nm.c_str(), nm.c_str());
}

void
hlognet_box::print_verilog_full_net(runner_print_verilog_network& cfg){
	gh_string_t& dir_name = cfg.dir_name;
	long num_elems = cfg.tot_targets;
	
	cfg.print_all_probes();
	
	if(dir_name == ""){
		fprintf(stderr, "Directory name for net generation is empty !!\n");
		return;
	}
	
	if(! gh_file_exists(dir_name)){
		fprintf(stderr, "Cannot find directory %s. Creating dir %s. \n", dir_name.c_str(), dir_name.c_str());
		gh_path_create(dir_name);
	}
	
	gh_string_t rtl_dir = dir_name + gh_path_sep + cfg.vl_sub_rtl_dir;
	gh_string_t net_dir = dir_name + gh_path_sep + cfg.vl_sub_net_dir;
	gh_string_t tgt_dir = dir_name + gh_path_sep + cfg.vl_sub_tgt_dir;
	gh_string_t fnd_dir = dir_name + gh_path_sep + cfg.vl_sub_fnd_dir;

	gh_path_create(net_dir);
	gh_path_create(tgt_dir);
	gh_path_create(fnd_dir);
	
	GH_CK(gh_file_exists(rtl_dir));
	
	print_verilog_config_file(cfg);
	
	gh_string_t ivl_cmm_ff_nam = dir_name + gh_path_sep + gh_ivl_comm_file_nm;
	gh_string_t vtr_cmm_ff_nam = dir_name + gh_path_sep + gh_vrt_comm_file_nm;
	gh_string_t yos_cmm_ff_nam = dir_name + gh_path_sep + gh_yos_comm_file_nm;

	cfg.ivl_comm_fl = fopen(ivl_cmm_ff_nam.c_str(), "w");
	if(cfg.ivl_comm_fl == NULL){
		fprintf(stderr, "Cannot open file %s for writing !!\n", ivl_cmm_ff_nam.c_str());
		return;
	}
	
	cfg.vtr_comm_fl = fopen(vtr_cmm_ff_nam.c_str(), "w");
	if(cfg.vtr_comm_fl == NULL){
		fprintf(stderr, "Cannot open file %s for writing !!\n", vtr_cmm_ff_nam.c_str());
		return;
	}
	
	cfg.yos_comm_fl = fopen(yos_cmm_ff_nam.c_str(), "w");
	if(cfg.yos_comm_fl == NULL){
		fprintf(stderr, "Cannot open file %s for writing !!\n", yos_cmm_ff_nam.c_str());
		return;
	}
	
	gh_string_t net_nam = dir_name + "_net";
	gh_string_t net_ff_nm = net_nam + gh_vl_file_ext;
	gh_string_t net_path = rtl_dir + gh_path_sep + net_ff_nm;

	
	FILE* ff = fopen(net_path.c_str(), "w");
	if(ff == NULL){
		fprintf(stderr, "Cannot open file %s for writing !!\n", net_path.c_str());
		return;
	}
	verilog_file vff;
	vff.fl = ff;

	print_verilog_header_to_yosys_file(cfg.yos_comm_fl);
	
	gh_string_t net_file_nm = cfg.vl_sub_rtl_dir + gh_path_sep + net_ff_nm;
	print_verilog_file_name_to_command_files(net_file_nm, cfg);
	
	slice_vec tgt_addrs;

	GH_CK(num_elems > 1);
	
	init_length(num_elems);
	
	long tot_channels = -1;
	long num_direct_channels = -1;
	long num_direct_packets = 0;

	const char* top_mod_nm = net_nam.c_str();
	fprintf(ff, R"base(
`include "hglobal.v"

`default_nettype	none

module %s
#(parameter 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	input wire i_Clk,
	output o_LED_1,
	output o_LED_2,
	output o_LED_3,
	output o_LED_4
);
	wire gch_clk = i_Clk;
	reg reset = 0;
	wire ready;
	
	`NS_DECLARE_ERR_LINK(oerr) 
	
	%s_impl it_%s_impl(
		`NS_INSTA_GLB_CHNL_VALS(gch, i_Clk, reset, ready),
		`NS_INSTA_ERR_CHNL(gerr, oerr)
	);
	
	`NS_DECLARE_DBG_LINK(dbg1)
	
	assign dbg1_leds[0:0] = oerr_error;
	assign dbg1_leds[1:1] = 0;
	assign dbg1_leds[2:2] = 0;
	assign dbg1_leds[3:3] = 0;
	
	assign o_LED_1 = dbg1_leds[0:0];
	assign o_LED_2 = dbg1_leds[1:1];
	assign o_LED_3 = dbg1_leds[2:2];
	assign o_LED_4 = dbg1_leds[3:3];

	
endmodule

module %s_impl
#(parameter 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	`NS_DECLARE_GLB_CHNL(gch),
	`NS_DECLARE_ERR_CHNL(gerr)
)base", top_mod_nm, top_mod_nm, top_mod_nm, top_mod_nm);

	fprintf(ff, ");\n");
	
	for(long aa = 0; aa < num_elems; aa++){
		htarget_box* bx = get_target_box(aa, tgt_addrs);
		
		bool can_wrp = bx->can_verilog_module_have_wrapper();
		
		gh_string_t tg_ff_nam = bx->get_verilog_router_module_name() + gh_vl_file_ext;
		gh_string_t tg_path = net_dir + gh_path_sep + tg_ff_nam;
		FILE* tg_rtr_ff = fopen(tg_path.c_str(), "w");
		if(tg_rtr_ff == NULL){
			fprintf(stderr, "Cannot open file %s for writing !!\n", tg_path.c_str());
			return;
		}
		print_verilog_file_name_to_command_files(cfg.vl_sub_net_dir + gh_path_sep + tg_ff_nam, cfg);
		
		gh_string_t cor_ff_nam = bx->get_verilog_core_module_name() + gh_vl_file_ext;
		gh_string_t tg_cor_path = tgt_dir + gh_path_sep + cor_ff_nam;
		FILE* tg_cor_ff = fopen(tg_cor_path.c_str(), "w");
		if(tg_cor_ff == NULL){
			fprintf(stderr, "Cannot open file %s for writing !!\n", tg_cor_path.c_str());
			return;
		}
		print_verilog_file_name_to_command_files(cfg.vl_sub_tgt_dir + gh_path_sep + cor_ff_nam, cfg);
		
		gh_string_t wrp_ff_nam = bx->get_verilog_wrapper_module_name() + gh_vl_file_ext;
		gh_string_t tg_wrp_path = net_dir + gh_path_sep + wrp_ff_nam;
		FILE* tg_wrp_ff = fopen(tg_wrp_path.c_str(), "w");
		if(tg_wrp_ff == NULL){
			fprintf(stderr, "Cannot open file %s for writing !!\n", tg_wrp_path.c_str());
			return;
		}
		print_verilog_file_name_to_command_files(cfg.vl_sub_net_dir + gh_path_sep + wrp_ff_nam, cfg);
		
		long tot_lft_out = (long)(bx->lft_out.size());
		if(aa == 0){
			tot_channels = tot_lft_out;
		}
		
		if(can_wrp){
			num_direct_channels = tot_channels - tot_lft_out;
		} else {
			num_direct_channels = 0;
		}
		
		verilog_file tg_cor_vff;
		tg_cor_vff.fl = tg_cor_ff;

		verilog_file tg_rtr_vff;
		tg_rtr_vff.fl = tg_rtr_ff;
		
		verilog_file tg_wrp_vff;
		tg_wrp_vff.fl = tg_wrp_ff;
		
		bx->print_verilog_module_core(tg_cor_vff, cfg);
		bx->print_verilog_module_router(tg_rtr_vff);
		bx->print_verilog_module_wrapper(tg_wrp_vff, num_direct_channels, num_direct_packets);
		
		bx->print_verilog_instance_wrapper(vff, num_direct_channels, num_direct_packets);
		
	}
	
	copy_verilog_foundation_files(net_nam, cfg);

	print_verilog_footer_to_yosys_file(cfg.yos_comm_fl, net_nam);
	
	vff.print_verilog_ready_final_and("gch_ready");
	vff.print_verilog_error_final_selec("gerr");

	fprintf(ff, R"base(

 
endmodule

)base");

}

void
runner_print_verilog_network::print_help(){
	GH_GLOBALS.compl_sys.print_last_complete_arg();
	FILE* of = GH_GLOBALS.compl_sys.args_compl_output;
	fprintf(of, "<dir_name> <tot_targets> \n[-pb <pw_base>] {-t <src> <dst>}* \n");
}
bool

runner_print_verilog_network::get_args(gh_str_list_t& lt_args){
	GH_CK(! lt_args.empty());
	GH_CK(lt_args.front() == "print_verilog_network");
	
	bool is_cmpl = GH_GLOBALS.compl_sys.args_orig_has_compl;
	int tot_params = lt_args.size();
	tot_params--;
	if(is_cmpl){
		tot_params--;
	}
	if(tot_params < 2){
		if(! is_cmpl){
			fprintf(stdout, "following args must have the form:\n\t");
		}
		print_help();
		return false;
	}
	
	gh_dec_args(lt_args);
	
	gh_string_t the_arg = lt_args.front(); gh_dec_args(lt_args);
	dir_name = the_arg;
	
	the_arg = lt_args.front(); gh_dec_args(lt_args);
	tot_targets = atol(the_arg.c_str());
	
	bool did_some = true;
	while(did_some && ! lt_args.empty()){
		did_some = false;
		the_arg = lt_args.front();
		if((the_arg == "-pb") && (lt_args.size() > 1)){
			gh_dec_args(lt_args); did_some = true;

			the_arg = lt_args.front(); gh_dec_args(lt_args);
			pw_base = atol(the_arg.c_str());
		}
		else if((the_arg == "-t") && (lt_args.size() > 2)){
			gh_dec_args(lt_args); did_some = true;

			the_arg = lt_args.front(); gh_dec_args(lt_args);
			gh_addr_t src = fix_addr(atol(the_arg.c_str()));
			
			the_arg = lt_args.front(); gh_dec_args(lt_args);
			gh_addr_t dst = fix_addr(atol(the_arg.c_str()));

			auto its = all_src_probes.find(src);
			auto itd = all_dst_probes.find(dst);
			bool src_free = (its == all_src_probes.end());
			bool dst_free = (itd == all_dst_probes.end()); 
			if(src_free && dst_free){
				all_src_probes[src] = dst;
				all_dst_probes[dst] = src;
			} else {
				gh_string_t serr = "";
				gh_string_t derr = "";
				if(! src_free){
					serr = "(repeated)";
				}
				if(! dst_free){
					derr = "(repeated)";
				}
				fprintf(stderr, "Ignoring -t %ld%s %ld%s \n", src, serr.c_str(), dst, derr.c_str());
			}
		}
	}
	
	return true;
}

int
runner_print_verilog_network::run_test(gh_str_list_t& lt_args){
	bool go_on = get_args(lt_args);
	if(! go_on){
		return -1;
	}
	bool is_cmpl = GH_GLOBALS.compl_sys.args_orig_has_compl;
	if(is_cmpl){ return -1; }
	
	if(tot_targets < 1){
		fprintf(stdout, "Total number of targets must be greater or equal than 1 \n");
		return -1;
	}
	
	hlognet_box* bx = new hlognet_box(pw_base);
	bx->print_verilog_full_net(*this);
	
	delete bx;
	return 0;
}

void
verilog_file::print_verilog_inc_ready_and(){
	FILE* ff = fl;
	gh_vector_t<long>& pend = pending_rdy;
	
	tot_rdy++;
	fprintf(ff, "\twire rdy%ld_lv0; \n", tot_rdy);
	
	long pend_sz = (long)pend.size();
	long aa = 0;
	for(aa = 0; aa < pend_sz; aa++){
		long vv = pend[aa];
		if(vv != 0){
			pend[aa] = 0;
			fprintf(ff, "\twire rdy%ld_lv%ld; \n", tot_rdy, aa + 1);
			fprintf(ff, "\tassign rdy%ld_lv%ld = rdy%ld_lv%ld && rdy%ld_lv%ld; \n", 
					tot_rdy, aa + 1,
					vv, aa,
					tot_rdy, aa
   				);
		} else {
			pend[aa] = tot_rdy;
			break;
		}
	}
	if(aa == pend_sz){
		pend.push_back(tot_rdy);
	}
}

void
verilog_file::print_verilog_ready_final_and(gh_string_t rdy_nm){
	FILE* ff = fl;
	gh_vector_t<long>& pend = pending_rdy;

	fprintf(ff, "\n\n");
	fprintf(ff, "\t// FINAL_READY (%s) \n\n", rdy_nm.c_str());

	fprintf(ff, "\tassign %s = ", rdy_nm.c_str());
	
	long pend_sz = (long)pend.size();
	long aa = 0;
	long num_prt = 0;
	for(aa = 0; aa < pend_sz; aa++){
		long vv = pend[aa];
		if(vv != 0){
			pend[aa] = 0;
			if(num_prt > 0){ fprintf(ff, " && "); }
			fprintf(ff, "rdy%ld_lv%ld", vv, aa);
			num_prt++;
		}
	}
	
	fprintf(ff, ";\n");
}

void
verilog_file::print_verilog_inc_error_selec(){
	FILE* ff = fl;
	gh_vector_t<long>& pend = pending_err;
	
	tot_err++;
	fprintf(ff, "\t`NS_DECLARE_ERR_LINK(err%ld_lv0) \n", tot_err);
	
	long pend_sz = (long)pend.size();
	long aa = 0;
	for(aa = 0; aa < pend_sz; aa++){
		long vv = pend[aa];
		if(vv != 0){
			pend[aa] = 0;
			fprintf(ff, "\t`NS_DECLARE_ERR_LINK(err%ld_lv%ld) \n", tot_err, aa + 1);
			fprintf(ff, "\t`NS_SELECT_ONE_ERR_LINK(err%ld_lv%ld, err%ld_lv%ld, err%ld_lv%ld) \n", 
					tot_err, aa + 1,
					vv, aa,
					tot_err, aa
   				);
		} else {
			pend[aa] = tot_err;
			break;
		}
	}
	if(aa == pend_sz){
		pend.push_back(tot_err);
	}
}

void
verilog_file::print_verilog_error_final_selec(gh_string_t err_nm){
	FILE* ff = fl;
	gh_vector_t<long>& pend = pending_err;

	fprintf(ff, "\n\n");
	fprintf(ff, "\t// FINAL_ERROR_ASSIGNS \n\n");

	long pend_sz = (long)pend.size();
	long aa = 0;
	long prv_prt = -1;
	gh_string_t prv_err = "";
	for(aa = 0; aa < pend_sz; aa++){
		long vv = pend[aa];
		if(vv != 0){
			pend[aa] = 0;
			gh_string_t curr_err = err_nm + gh_long_to_string(aa);
			if(prv_prt >= 0){ 
				fprintf(ff, "\t`NS_DECLARE_ERR_LINK(%s) \n", curr_err.c_str());
				fprintf(ff, "\t`NS_SELECT_ONE_ERR_LINK(%s, %s, err%ld_lv%ld) \n", 
						curr_err.c_str(),
						prv_err.c_str(),
						vv, aa
					);
				prv_err = curr_err;
			} else {
				prv_err = "err" + gh_long_to_string(vv) + "_lv" + gh_long_to_string(aa);
			}
			prv_prt = aa;
		}
	}
	
	if(prv_prt >= 0){
		fprintf(ff, "\t// FINAL_ERROR (%s) \n\n", err_nm.c_str());
		
		//fprintf(ff, "\t`NS_DECLARE_ERR_LINK(%s) \n", err_nm.c_str());
		fprintf(ff, "\t`NS_ASSIGN_ERR_CHNL(%s, %s, %s_my_addr) \n", 
				err_nm.c_str(),
				prv_err.c_str(),
				prv_err.c_str()
			);
	}
	
}

void
runner_print_verilog_network::init_addr_size(){
	double l2 = ceil(log2((double)tot_targets));
	num_bits_address = (long)l2;
}

void
runner_print_verilog_network::init_redun_size(){
	init_addr_size();
	//(ASZ + ASZ + DSZ + RSZ)
	long base = (num_bits_address * 2) + num_bits_data;
	num_bits_redundant = (redundant_percentage * base / 100);
	if(num_bits_redundant <= 0){
		num_bits_redundant = 1;
	}
}

void
hlognet_box::print_verilog_config_file(runner_print_verilog_network& cfg){
	gh_string_t fnd_dir = cfg.dir_name + gh_path_sep + cfg.vl_sub_fnd_dir;
	GH_CK_PRT(gh_file_exists(fnd_dir), "DIRECTORY %s NOT FOUND. THIS MEANS A BAD INSTALLATION OF THE PROGRAM.", fnd_dir.c_str());
	
	gh_string_t cfg_path = fnd_dir + gh_path_sep + cfg.vl_config_file_nm;
	
	cfg.init_redun_size();

	FILE* ff = fopen(cfg_path.c_str(), "w");
	if(ff == NULL){
		fprintf(stderr, "Cannot open file %s for writing !!\n", cfg_path.c_str());
		return;
	}
	
	
	fprintf(ff, R"base(

`ifndef HCONFIG_V_FILE
`define HCONFIG_V_FILE 1
//--------------------------------------------


`define NS_ADDRESS_SIZE %ld
`define NS_DATA_SIZE %ld
`define NS_REDUN_SIZE %ld // MUST BE LESS THAN the full message size ((NS_ADDRESS_SIZE * 2) + NS_DATA_SIZE) 

`define NS_PACKET_SIZE %ld // MUST BE LESS THAN the full message size ((NS_ADDRESS_SIZE * 2) + NS_DATA_SIZE) 

`define NS_1to2_FSZ %ld
`define NS_2to1_FSZ %ld
`define NS_PACKOUT_FSZ %ld  // 1, 2 or 4 ***ONLY***
`define NS_PACKIN_FSZ %ld  // 1, 2 or 4 ***ONLY***

`define NS_REQ_CKS 1
`define NS_ACK_CKS 1

`define NS_DECLARE_ERR_CHNL(nam) `NS_DECLARE_ERR_T%ld_CHNL(nam)
`define NS_DECLARE_ERR_LINK(nam) `NS_DECLARE_ERR_T%ld_LINK(nam)
`define NS_INSTA_ERR_CHNL(chn0, chn1) `NS_INSTA_ERR_T%ld_CHNL(chn0, chn1)
`define NS_DECLARE_ERR_REG(nam) `NS_DECLARE_ERR_T%ld_REG(nam)
`define NS_INIT_ERR_REG(nam) `NS_INIT_ERR_T%ld_REG(nam)
`define NS_SET_ERR_REG(ou, mg) `NS_SET_ERR_T%ld_REG(ou, mg)
`define NS_ASSIGN_ERR_CHNL(ou, mg, err_addr) `NS_ASSIGN_ERR_T%ld_CHNL(ou, mg, err_addr)
`define NS_SELECT_ONE_ERR_LINK(ou, err0, err1) `NS_SELECT_ONE_ERR_T%ld_LINK(ou, err0, err1)


//--------------------------------------------
`endif // HCONFIG_V_FILE

)base",
		cfg.num_bits_address,
		cfg.num_bits_data,
		cfg.num_bits_redundant,
		cfg.num_bits_packet,
		cfg.vl_1to2_fifo_sz,
		cfg.vl_2to1_fifo_sz,
		cfg.vl_packout_fifo_sz,
		cfg.vl_packin_fifo_sz,
		cfg.err_kind,
		cfg.err_kind,
		cfg.err_kind,
		cfg.err_kind,
		cfg.err_kind,
		cfg.err_kind,
		cfg.err_kind,
		cfg.err_kind
	);
	
	fclose(ff);
}

void
runner_print_verilog_network::print_all_probes(){
	gh_addr_map_t& ss = all_src_probes;
    auto its = ss.begin();
	fprintf(stdout, "ALL_SRC_PROBES ========================\n");
	for(its = ss.begin(); its != ss.end(); its++){
		fprintf(stdout, "%ld, %ld \n", its->first, its->second);
	}
	
	gh_addr_map_t& dd = all_dst_probes;
    auto itd = dd.begin();
	fprintf(stdout, "ALL DST_PROBES ========================\n");
	for(itd = dd.begin(); itd != dd.end(); itd++){
		fprintf(stdout, "%ld, %ld \n", itd->first, itd->second);
	}
	
}

