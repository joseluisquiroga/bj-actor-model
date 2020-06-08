

#ifndef GEN_HNET_H
#define GEN_HNET_H


#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <array>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <sstream>

#include "gh_dbg_util.h"
#include "gh_file_utils.h"

//using namespace std;

#define gh_vector_t std::vector 

#define GH_INVALID_IDX -1
#define GH_INVALID_ADDR -1
#define GH_INVALID_COMPLETE_IDX -100
#define GH_INVALID_LNK_NUM -1

#define GH_DEFAULT_LOGNET_NUM_BITS_DATA 3
#define GH_DEFAULT_LOGNET_NUM_BITS_PACKET 2
#define GH_DEFAULT_LOGNET_REDUNDANT_PERCENTAGE 10

#define GH_DEFAULT_LOGNET_VL_1to2_FIFO_SIZE 1
#define GH_DEFAULT_LOGNET_VL_2to1_FIFO_SIZE 2
#define GH_DEFAULT_LOGNET_VL_PACKOUT_FIFO_SIZE 4
#define GH_DEFAULT_LOGNET_VL_PACKIN_FIFO_SIZE 4


#define GH_BASE_TWO 2

#define GH_MAX_OUTS 64

#define GH_BUFF_SIMU_SZ GH_GLOBALS.node_buff_sz_simu


#define GH_NULL_OUT_ADDR gh_null

#define gh_min(v1, v2) (((v1) < (v2))?(v1):(v2))
#define gh_max(v1, v2) (((v1) > (v2))?(v1):(v2))

typedef const char* gh_c_str_t;

#define gh_vl_sep "_"
#define gh_vl_snd "snd"
#define gh_vl_rcv "rcv"
#define gh_vl_src "src"
#define gh_vl_dst "dst"
#define gh_vl_dat "dat"
#define gh_vl_req "req"
#define gh_vl_ack "ack"
#define gh_vl_in0 "in0_"
#define gh_vl_in1 "in1_"
#define gh_vl_out0 "_out0"
#define gh_vl_out1 "_out1"
#define gh_vl_tgt "tg_"
#define gh_vl_nod "nd_"
#define gh_vl_tg_router "_router"
#define gh_vl_tg_wrapper "_wrapper"
#define gh_vl_tg_core "_core"
#define gh_vl_tgts_lnk "_out_lnk_"
#define gh_vl_tgts_lnk_sep "_"
#define gh_vl_tg_core_lnk "tg_cor_lnk_"
#define gh_vl_err_tg "err_tg_"

#define gh_vl_file_ext ".v"
#define gh_ivl_comm_file_nm "ivl_commands.ivl"
#define gh_vrt_comm_file_nm "vtr_commands.vtr"
#define gh_yos_comm_file_nm "yos_commands.tcl"

#define gh_vl_snd0 "snd0" // must match 1to2 and 2to1 modules interfaces
#define gh_vl_snd1 "snd1" // must match 1to2 and 2to1 modules interfaces
#define gh_vl_rcv0 "rcv0" // must match 1to2 and 2to1 modules interfaces
#define gh_vl_rcv1 "rcv1" // must match 1to2 and 2to1 modules interfaces

//======================================================================
// bitarray

// where 'a' is the byte stream pointer, and b is the bit number.

#define gh_div8(b)	((b)>>3)
#define gh_mod8(b)	((b)&7)

#define gh_get_bit(a, b)		((((int8_t*)a)[gh_div8(b)] >> gh_mod8(b)) & 1)
#define gh_set_bit(a, b)		(((int8_t*)a)[gh_div8(b)] |= (1 << gh_mod8(b)))
#define gh_reset_bit(a, b) 		(((int8_t*)a)[gh_div8(b)] &= ~(1 << gh_mod8(b)))
#define gh_toggle_bit(a, b) 	(((int8_t*)a)[gh_div8(b)] ^= (1 << gh_mod8(b)))

#define gh_to_bytes(num_bits)	(gh_div8(num_bits) + (gh_mod8(num_bits) > 0))
#define gh_to_bits(num_bytes)	(num_bytes * 8)

//======================================================================

enum gh_dbg_call_t {
	gh_invalid_call,
	gh_call_01,
	gh_call_02,
	gh_call_03,
	gh_call_04,
	gh_call_05,
	gh_call_06,
	gh_call_07,
	gh_call_08,
	gh_call_09,
	gh_call_10,
	gh_call_11,
	gh_call_12,
	gh_call_13,
	gh_call_14
};

enum gh_prt_mode_t {
	gh_addr_prt,
	gh_full_prt,
	gh_full_pt_prt,
	gh_info_prt
};

enum gh_hnode_kind_t {
	gh_invalid_nod,
	gh_1_to_1_nod,
	gh_1_to_2_nod,
	gh_2_to_1_nod
};

enum gh_1t1_kind_t {
	gh_invalid_1t1,
	gh_direct_1t1,
	gh_target_1t1,
	gh_source_1t1,
	gh_sink_1t1
};

enum gh_nk_lnk_mod_t {
	gh_invalid_ck_mod,
	gh_strict_ck_mod,
	gh_valid_self_ck_mod,
	gh_soft_ck_mod
};

enum gh_io_kind_t {
	gh_in_kind,
	gh_out_kind
};

enum gh_route_side_t {
	gh_invalid_side,
	gh_left_side,
	gh_right_side
};

enum gh_slice_type_t {
	gh_invalid_slc,
	gh_inc_slc,
	gh_dec_slc
};

enum gh_1to2_kind_t {
	gh_invalid_rou,
	gh_sm_to_bm_rou,
	gh_2to2_nd0_rou,
	gh_2to2_nd1_rou,
	gh_3to2_nd0_rou,
	gh_3to2_nd1_rou,
	gh_3to2_nd2_rou,
	gh_lgnet_in_rou,
	gh_tgt_out_rou,
	gh_bs_tgt_out_rou,
	gh_bs_tgt_lft_in_rou,
	gh_bs_tgt_rgt_in_rou,
	gh_bs_tgt_bx_in_rou
};

enum gh_dbg_st_t {
	gh_min_min_dbg_st,
	gh_min_max_dbg_st,
	gh_max_min_dbg_st,
	gh_max_max_dbg_st
};

typedef int gh_flag_idx_t;
typedef uint8_t gh_flags_t;
typedef long gh_target_addr_t;
typedef long gh_addr_t;

class thd_data;

class hgen_globals;
class hmessage;
class hnode;
class hnode_1to1;
class hnode_direct;
class hnode_target;
class hnode_src;
class hnode_snk;
class hnode_1to2;
class hnode_2to1;
class hnode_box;
class hroute_box;
class htarget_box;
class hlognet_box;


#define gh_is_gt_cmp		0
#define gh_is_eq_cmp		1
#define gh_is_undef			2
#define gh_is_lognet_io 	3
#define gh_is_interval 		4
#define gh_is_sm2bm_out		5
#define gh_last_flg 		6

class edge {
public:
	gh_flags_t slc_flg = 0;
	gh_addr_t slc_edge = 0;
	
	edge(){
		slc_flg = 0;
		slc_edge = 0;
	}

	void set_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		gh_set_bit(&slc_flg, num_flg);
	}

	void reset_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		gh_reset_bit(&slc_flg, num_flg);
	}

	bool get_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		return gh_get_bit(&slc_flg, num_flg);
	}
	
	bool is_undef(){
		bool udf = get_flag(gh_is_undef);
		GH_CK(! udf || (slc_edge == 0));
		return udf;
	}

	bool is_eq(){
		return get_flag(gh_is_eq_cmp);
	}

	bool is_gt(){
		return get_flag(gh_is_gt_cmp);
	}
	
	bool is_out(){
		return get_flag(gh_is_sm2bm_out);
	}

	gh_addr_t get_first_addr_in();
	
	edge get_compl();
	
	bool in_edge(gh_addr_t addr);

	gh_string_t get_print_str();
	
	void print_edge(FILE* ff){
		fprintf(ff, "%s", get_print_str().c_str());
		fflush(ff);
	}
	
	gh_string_t get_verilog_oper();
	gh_string_t get_verilog_ref_val();
};


typedef hnode* pnode_t;
typedef gh_vector_t<int> int_vec_t;
typedef gh_vector_t<pnode_t> pnode_vec_t;
typedef gh_vector_t<pnode_t*> ppnode_vec_t;
typedef gh_vector_t<gh_addr_t> addr_vec_t;
typedef gh_vector_t<hnode_target*> ptarget_vec_t;
typedef gh_vector_t<edge> edge_vec_t;
typedef gh_vector_t<thd_data*> pthread_data_vec_t;

class interval {
public:
	edge lft;
	edge rgt;
	
	gh_string_t get_print_str();
	
	long dbg_get_num_test_in();
	gh_addr_t dbg_get_min_in();
	gh_addr_t dbg_get_max_in();
	
	bool in_interval(gh_addr_t addr);
	
	void print_interval(FILE* ff){
		fprintf(ff, "%s", get_print_str().c_str());
		fflush(ff);
	}
};

class slice_vec : public edge_vec_t {
public:

	bool is_dec(){
		if((long)size() < 2){
			return false;
		}
		if(front().slc_edge > back().slc_edge){
			return true;
		}
		return false;
	}
	
	void init_slice_vec_with(gh_addr_t slc_idx, addr_vec_t& all_rel_pos);
	void init_sub_slices_with(gh_addr_t slc_idx, slice_vec& all_slices, addr_vec_t& all_rel_pos);
	
	interval get_top_interval(long idx, long tot_elems);
	interval get_interval(long idx);

	void init_slice_vec(gh_addr_t slc_idx, slice_vec& all_slices, 
					  long sz, long pw_b, gh_route_side_t sd, bool has_z, const char* dbg_str);

	
	void print_slice_vec(FILE* ff, const char* dbg_nm = gh_null);
	void print_all_intervals(FILE* ff);
	
	void print_dbg_info_slice_vec(FILE* ff, gh_addr_t slc_idx, slice_vec& all_slices, 
					  long sz, long pw_b, gh_route_side_t sd, bool has_z, const char* dbg_str);
};

void gh_init_rel_pos(addr_vec_t& all_rel_pos, long sz, long pw_b, gh_route_side_t sd, bool has_z);

void gh_run_m_to_n(long mm, long nn, bool has_zr);

typedef std::set<gh_string_t> gh_str_set_t;
typedef std::list<gh_string_t> gh_str_list_t;
typedef std::map<gh_addr_t, gh_addr_t> gh_addr_map_t;

class runner_get_binnet_m_to_n {
public:
	long 	dag_mm = 0;
	long 	dag_nn = 0;
	bool	has_zr = false;
	
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	void print_help();
};

class runner_init_slices {
public:
	long 	slices_sz = 0;
	long 	slices_idx = 0;
	bool	has_zr = false;
	gh_route_side_t	slices_sd = gh_left_side;
	
	bool	has_sub_slices = false;
	long 	sub_slices_sz = 0;
	long 	sub_slices_idx = 0;
	bool	sub_has_zr = false;
	gh_route_side_t	sub_slices_sd = gh_left_side;
	
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	void print_help(int hpl_case);
};

class runner_init_lognet_box {
public:
	int 	tot_args = 0;
	long 	pw_base = GH_BASE_TWO;
	long 	tot_targets = 0;
	gh_prt_mode_t	print_op = gh_full_prt;
	gh_addr_t src_addr = GH_INVALID_ADDR;
	gh_addr_t dst_addr = GH_INVALID_ADDR;
	
	
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	virtual void print_help();
	
	bool base_get_args(gh_str_list_t& lt_args, gh_string_t cllr, int num_pm);
	hlognet_box* create_lognet();
};

class runner_check_path : public runner_init_lognet_box {
public:
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	virtual void print_help();
};

class runner_check_all_paths_from : public runner_init_lognet_box {
public:
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	virtual void print_help();
};

class runner_check_all_to_all_paths : public runner_init_lognet_box {
public:
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	virtual void print_help();
};

class runner_threads_simu : public runner_init_lognet_box {
public:
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	virtual void print_help();
};

class runner_print_verilog_target_box {
public:
	long 	pw_base = GH_BASE_TWO;
	long 	num_in = 0;
	long 	num_out = 0;
	gh_string_t out_file_name = "";
	
	long 	num_direct_channels = 0;
	long 	num_direct_packets = 0;
	gh_string_t wrapper_file_name = "";
	
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	void print_help();
};

class runner_print_verilog_network {
public:
	
	gh_buffer_t vl_cp_file_buff;
	gh_string_t vl_exe_fnd_dir = gh_string_t("vlg_fnd");
	gh_string_t vl_sub_rtl_dir = (gh_string_t("rtl"));
	gh_string_t vl_sub_net_dir = (gh_string_t("rtl")) + (gh_string_t(gh_path_sep)) + (gh_string_t("hnet"));
	gh_string_t vl_sub_tgt_dir = (gh_string_t("rtl")) + (gh_string_t(gh_path_sep)) + (gh_string_t("targets"));
	gh_string_t vl_sub_fnd_dir = (gh_string_t("rtl")) + (gh_string_t(gh_path_sep)) + (gh_string_t("foundation"));
	
	gh_string_t vl_make_src_file_nm = gh_string_t("TEMP_Makefile");
	gh_string_t vl_make_dst_file_nm = gh_string_t("Makefile");
	gh_string_t vl_pcf_file_nm = gh_string_t("GO_BOARD.pcf");
	gh_string_t vl_clean_file_nm = gh_string_t("clean.sh");
	gh_string_t vl_config_file_nm = gh_string_t("hconfig.v");
	
	std::array<gh_string_t, 12> vl_all_fnd_files = { 
		"calc_redun.v",
		"debouncer.v",
		"hglobal.v",
		"hnull_sink.v",
		"hnull_source.v",
		"hprb_sink.v",
		"hprb_source.v",
		"nd_1to2.v",
		"nd_2to1.v",
		"pakin.v",
		"pakout.v",
		"tree_nand.v"
	};
	
	long num_bits_data = GH_DEFAULT_LOGNET_NUM_BITS_DATA;
	long num_bits_address = 0;
	long num_bits_packet = GH_DEFAULT_LOGNET_NUM_BITS_PACKET;
	long num_bits_redundant = 0;
	long redundant_percentage = GH_DEFAULT_LOGNET_REDUNDANT_PERCENTAGE;
	
	long vl_1to2_fifo_sz = GH_DEFAULT_LOGNET_VL_1to2_FIFO_SIZE;
	long vl_2to1_fifo_sz = GH_DEFAULT_LOGNET_VL_2to1_FIFO_SIZE;
	long vl_packout_fifo_sz = GH_DEFAULT_LOGNET_VL_PACKOUT_FIFO_SIZE;
	long vl_packin_fifo_sz = GH_DEFAULT_LOGNET_VL_PACKIN_FIFO_SIZE;

	long err_kind = 2;
	
	FILE* ivl_comm_fl = NULL;
	FILE* vtr_comm_fl = NULL;
	FILE* yos_comm_fl = NULL;
	
	long 	pw_base = GH_BASE_TWO;
	long 	tot_targets = 1;
	gh_string_t dir_name = "";
	
	gh_addr_map_t all_src_probes;
	gh_addr_map_t all_dst_probes;
	
	int run_test(gh_str_list_t& lt_args);
	bool get_args(gh_str_list_t& lt_args);
	void print_help();
	
	void init_addr_size();
	void init_redun_size();
	
	gh_addr_t fix_addr(gh_addr_t adr){
		if(adr < 0){ 
			fprintf(stderr, "Fixing addr %ld below 0 to 0\n", adr);
			adr = 0; 
		}
		gh_addr_t max = (tot_targets - 1);
		if(adr > max){ 
			fprintf(stderr, "Fixing addr %ld above %ld to %ld\n", adr, max, max);
			adr = max;
		}
		return adr;
	}
	
	void print_all_probes();
};

class autocomplete_sys {
public:
	const char* args_output_name = "autocomplete_choices.txt";
	FILE*	args_compl_output = stdout;
	
	bool			args_orig_has_compl = false;
	int 			args_orig_argc = -1;
	char**			args_orig_argv = gh_null;
	int 			args_orig_compl_idx = GH_INVALID_COMPLETE_IDX;
	gh_string_t 	args_orig_cmpl_idx_arg = "";
	
	void init_autocomplete_sys(gh_str_list_t& lt_args, int argc, char** argv);
	void print_last_complete_arg();
};

class hgen_globals {
public:
	long DBG_LV = 0;
	gh_nk_lnk_mod_t CK_LINK_MODE = gh_soft_ck_mod;
	
	autocomplete_sys compl_sys;

	runner_get_binnet_m_to_n 		rnr_get_binnet_m_to_n;
	runner_init_slices				rnr_init_slices;
	runner_init_lognet_box			rnr_init_lognet_box;
	runner_threads_simu				rnr_threads_simu;
	runner_check_path				rnr_check_path;
	runner_check_all_paths_from		rnr_check_all_paths_from;
	runner_check_all_to_all_paths	rnr_check_all_to_all_paths;
	
	runner_print_verilog_target_box	rnr_print_verilog_target_box;
	runner_print_verilog_network	rnr_print_verilog_network;
	
	const char* dbg_LI_quarter = "LI";
	const char* dbg_LO_quarter = "LO";
	const char* dbg_RI_quarter = "RI";
	const char* dbg_RO_quarter = "RO";

	const char* dbg_curr_tgt_quarter = gh_null;

	// USED IN runner_threads_simu:
	pthread_data_vec_t 		all_thread_data_simu;
	ptarget_vec_t* 			all_tgt_simu = gh_null;
	long 					tot_src_msg_simu = 0;
	bool 					all_thread_inited_simu = false;
	
	bool		dbg_prt_gen_info = false;
	bool 		buffed_nodes_simu = true;
	bool 		dbg_prt_choo_simu = false;
	long 		idx_test_simu = 0;
	addr_vec_t 	dbg_nodes_prt_simu;
	long 		dbg_prt_disp_all_addr_simu = 0;
	long 		node_buff_sz_simu = 3;
	
	hgen_globals(){}
	
	virtual ~hgen_globals(){
	}
	
};

extern hgen_globals GH_GLOBALS;

#define GH_LI GH_GLOBALS.dbg_LI_quarter
#define GH_LO GH_GLOBALS.dbg_LO_quarter
#define GH_RI GH_GLOBALS.dbg_RI_quarter
#define GH_RO GH_GLOBALS.dbg_RO_quarter

#define GH_QRT(qq) ((GH_GLOBALS.dbg_curr_tgt_quarter != gh_null)?(GH_GLOBALS.dbg_curr_tgt_quarter):(qq))

inline gh_route_side_t
gh_get_opp_side(gh_route_side_t rt_sd){
	switch(rt_sd){
		case gh_invalid_side: 
			return gh_invalid_side;
		case gh_left_side: 
			return gh_right_side;
		case gh_right_side: 
			return gh_left_side;
		default:
			break;
	}
	return gh_invalid_side;
}

inline const char*
gh_dbg_get_side_str(gh_route_side_t dd){
	switch(dd){
		case gh_invalid_side: 
			return "gh_invalid_side";
		case gh_left_side: 
			return "lft";
		case gh_right_side: 
			return "rgt";
		default:
			break;
	}
	return "GH_INVALID_SIDE";
};

inline const char*
gh_dbg_get_slice_tp_str(gh_slice_type_t dd){
	switch(dd){
		case gh_invalid_slc: 
			return "gh_invalid_slc";
		case gh_inc_slc: 
			return "inc";
		case gh_dec_slc: 
			return "dec";
		default:
			break;
	}
	return "GH_INVALID_SIDE";
};

inline const char*
gh_dbg_get_rou_kind_str(gh_1to2_kind_t kk){
	switch(kk){
		case gh_invalid_rou:
			return "gh_invalid_rou";
		case gh_sm_to_bm_rou:
			return "sm_bm";
		case gh_2to2_nd0_rou:
			return "22_0";
		case gh_2to2_nd1_rou:
			return "22_1";
		case gh_3to2_nd0_rou:
			return "32_0";
		case gh_3to2_nd1_rou:
			return "32_1";
		case gh_3to2_nd2_rou:
			return "32_2";
		case gh_lgnet_in_rou:
			return "lgnet_in";
		case gh_tgt_out_rou:
			return "tgt_out";
		case gh_bs_tgt_out_rou:
			return "bt_out";
		case gh_bs_tgt_lft_in_rou:
			return "bt_lfi";
		case gh_bs_tgt_rgt_in_rou:
			return "bt_rgi";
		case gh_bs_tgt_bx_in_rou:
			return "bt_bxi";
		default:
			break;
	}
	return "GH_INVALID_ROU_KIND";
};

inline const char*
gh_dbg_st_case_str(gh_dbg_st_t kk){
	switch(kk){
		case gh_min_min_dbg_st:
			return "mm";
		case gh_min_max_dbg_st:
			return "mM";
		case gh_max_min_dbg_st:
			return "Mm";
		case gh_max_max_dbg_st:
			return "MM";
	}
	return "gh_INVALID_RNG_CASE";
};

gh_addr_t gh_calc_power(long base, gh_addr_t adr);

#define gh_is_msg_busy		0
#define gh_is_msg_head		1
#define gh_is_msg_tail		2

class hmessage {
public:
	gh_flags_t mg_flg = 0;
	
	long mg_val = 0;
	gh_addr_t mg_sra = GH_INVALID_ADDR;
	gh_addr_t mg_src = GH_INVALID_ADDR;
	gh_addr_t mg_dst = GH_INVALID_ADDR;
	
	void copy_mg_to(hmessage& mg, hnode* dbg_src_nod, hnode* dbg_dst_nod);
	
	void set_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		gh_set_bit(&mg_flg, num_flg);
	}

	void reset_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		gh_reset_bit(&mg_flg, num_flg);
	}

	bool get_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		return gh_get_bit(&mg_flg, num_flg);
	}
	
	void init_as_empty_buff(){
		mg_flg = 0;
		set_flag(gh_is_msg_head);
		set_flag(gh_is_msg_tail);
	}
	
	void print_message(FILE* ff){
		fprintf(ff, "{(<%ld>%ld -> %ld) val=%ld}", mg_sra, mg_src, mg_dst, mg_val);
		fflush(ff);
	}
	
};

typedef gh_vector_t<hmessage> msg_vec_t;

class hmsg_queue : public msg_vec_t {
public:
	long hd_idx = 0;
	long tl_idx = 0;
	
	long nxt_pos(long pos){
		pos++;
		if(pos == (long)size()){
			return 0;
		}
		return pos;
	}
	
	hmessage& get_head(){
		return at(hd_idx);
	}
	
	void inc_head(){
		hd_idx = nxt_pos(hd_idx);
	}
	
	hmessage& get_tail(){
		return at(tl_idx);
	}
	
	void inc_tail(){
		tl_idx = nxt_pos(tl_idx);
	}
	
	//void run_idx_simu(long& mg_bf_idx, hmessage& mg_in, hmessage& mg_out, hnode& the_in, hnode& the_out, bool& the_req, bool& the_ack);
	
};

#define gh_long_to_string(num) std::to_string(num)

inline gh_string_t 
get_verilog_side(gh_route_side_t sd){
	gh_string_t sd_nm = "lft";
	if(sd == gh_right_side){
		sd_nm = "rgt";
	} 
	return sd_nm;
}

inline gh_string_t 
get_verilog_io_kind(gh_io_kind_t iok){
	gh_string_t iok_nm = gh_vl_rcv;
	if(iok == gh_out_kind){
		iok_nm = gh_vl_snd;
	} 
	return iok_nm;
}

inline gh_string_t 
get_verilog_io_name(long num, gh_route_side_t sd, gh_io_kind_t iok){
	gh_string_t iok_nm = get_verilog_io_kind(iok);
	gh_string_t sd_nm = get_verilog_side(sd);
	gh_string_t ss = iok_nm + gh_vl_sep + sd_nm + gh_long_to_string(num);
	return ss;
}

#define gh_call_get_ppio(get_ppio_fn) \
	hnode** ppio = get_ppio_fn(); \
	if(ppio == gh_null){ return gh_null; } \
	return *ppio; \

// end_macro

#define gh_get_io_verilog_id(io_fn) \
	hnode* nd = io_fn(); \
	if(nd == gh_null){ return "gh_invalid_verilog_id"; } \
	return nd->get_verilog_id(); \

// end_macro


class verilog_file {
public:
	FILE* fl = NULL;
	gh_str_set_t declared_interfaces;
	gh_str_set_t assigned_interfaces;
	gh_str_set_t assigned_out_channels;
	long tot_rdy = 0;
	gh_vector_t<long> pending_rdy;
	long tot_err = 0;
	gh_vector_t<long> pending_err;
	
	void print_verilog_inc_ready_and();
	void print_verilog_ready_final_and(gh_string_t rdy_nm);

	void print_verilog_inc_error_selec();
	void print_verilog_error_final_selec(gh_string_t err_nm);
	
};


inline bool gh_is_1to1(gh_hnode_kind_t kk){ return (kk == gh_1_to_1_nod); }
inline bool gh_is_1to2(gh_hnode_kind_t kk){ return (kk == gh_1_to_2_nod); }
inline bool gh_is_2to1(gh_hnode_kind_t kk){ return (kk == gh_2_to_1_nod); }

class hnode {
public:
	void* 	simu_data;
	gh_flags_t node_flags = 0;
	gh_addr_t addr = GH_INVALID_ADDR;
	
	const char* dbg_tgt_quarter = gh_null;

	long dbg_idx = GH_INVALID_IDX;
	long dbg_tot = GH_INVALID_IDX;
	
	interval selector;
	
	hnode(){
		simu_data = gh_null;
		node_flags = 0;
		addr = GH_INVALID_ADDR;
		
		//dbg_tgt_addr = GH_GLOBALS.dbg_curr_tgt_addr;
		dbg_tgt_quarter = GH_GLOBALS.dbg_curr_tgt_quarter;
	}
	virtual ~hnode(){}

	void set_selector_interval(long tgt_idx, slice_vec& tgt_addrs, long tot_elems);
	void set_selector_edge(long tgt_idx, slice_vec& tgt_addrs, long tot_tgt);
	void print_selector_info(FILE* ff);
	
	gh_string_t get_selector_str();
	
	bool in_interval(gh_addr_t addr);
	gh_addr_t get_diff_simu();
	gh_addr_t get_min_simu();
	gh_addr_t get_max_simu();
	
	virtual int
	print_node(FILE* ff, gh_prt_mode_t md);
	
	virtual bool 	ck_connections(){ return false; }
	
	virtual gh_hnode_kind_t
	get_kind(){
		return gh_invalid_nod;
	}
	
	bool is_1to1(){ return (get_kind() == gh_1_to_1_nod); }
	bool is_1to2(){ return (get_kind() == gh_1_to_2_nod); }
	bool is_2to1(){ return (get_kind() == gh_2_to_1_nod); }
	
	hnode_target* as_target();
	
	virtual bool
	has_io(hnode** io){
		return false;
	}

	virtual bool 	get_ack0(){ GH_CK("get_ack0" && false); return false; }
	virtual bool 	get_ack1(){ GH_CK("get_ack1" && false); return false; }
	virtual bool 	get_req0(){ GH_CK("get_req0" && false); return false; }
	virtual bool 	get_req1(){ GH_CK("get_req1" && false); return false; }
	
	virtual hnode** 	get_pt_in0(){ return gh_null; }
	virtual hnode** 	get_pt_in1(){ return gh_null; }
	virtual hnode** 	get_pt_out0(){ return gh_null; }
	virtual hnode** 	get_pt_out1(){ return gh_null; }

	hnode* 	get_in0(){ gh_call_get_ppio(get_pt_in0); }
	hnode* 	get_in1(){ gh_call_get_ppio(get_pt_in1); }
	hnode* 	get_out0(){ gh_call_get_ppio(get_pt_out0); }
	hnode* 	get_out1(){ gh_call_get_ppio(get_pt_out1); }

	virtual hnode** set_in0(hnode* nd){ return gh_null; }
	virtual hnode** set_in1(hnode* nd){ return gh_null; }
	virtual hnode** set_out0(hnode* nd){ return gh_null; }
	virtual hnode** set_out1(hnode* nd){ return gh_null; }

	virtual hmessage* get_message0(){ return gh_null; }
	virtual hmessage* get_message1(){ return gh_null; }

	bool 	ireq(hnode& in_nd){
		if(in_nd.get_out0() == this){
			return in_nd.get_req0();
		}
		if(in_nd.get_out1() == this){
			return in_nd.get_req1();
		}
		GH_CK(false);
		return false;
	}
	
	bool 	oack(hnode& out_nd){
		if(out_nd.get_in0() == this){
			return out_nd.get_ack0();
		}
		if(out_nd.get_in1() == this){
			return out_nd.get_ack1();
		}
		GH_CK(false);
		return false;
	}
	
	virtual void set_direct_idx(gh_io_kind_t kk, ppnode_vec_t& all_io, long idx){}
	
	hmessage*	get_message_of(hnode* out){
		if(get_out0() == out){
			return get_message0();
		}
		if(get_out1() == out){
			return get_message1();
		}
		return gh_null;
	}
	
	hnode** re_link_in(hnode* old_nd, hnode* nw_nd){
		GH_CK(old_nd != this);
		GH_CK(get_in0() != get_in1());
		hnode** ppn = gh_null;
		if(get_in0() == old_nd){ ppn = set_in0(nw_nd); GH_CK(get_in0() == nw_nd); return ppn; }
		if(get_in1() == old_nd){ ppn = set_in1(nw_nd); GH_CK(get_in1() == nw_nd); return ppn; }
		GH_CK(false && "Bad_re_link_in");
		return gh_null;
	}
	
	hnode** re_link_out(hnode* old_nd, hnode* nw_nd){
		GH_CK(old_nd != this);
		GH_CK(get_out0() != get_out1());
		hnode** ppn = gh_null;
		if(get_out0() == old_nd){ ppn = set_out0(nw_nd); GH_CK(get_out0() == nw_nd); return ppn; }
		if(get_out1() == old_nd){ ppn = set_out1(nw_nd); GH_CK(get_out1() == nw_nd); return ppn; }
		GH_CK(false && "Bad_re_link_out");
		return gh_null;
	}

	void set_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		gh_set_bit(&node_flags, num_flg);
	}

	void reset_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		gh_reset_bit(&node_flags, num_flg);
	}

	bool get_flag(gh_flag_idx_t num_flg){
		GH_CK(num_flg >= 0);
		GH_CK(num_flg < gh_last_flg);
		return gh_get_bit(&node_flags, num_flg);
	}
	
	void run_head_queue_simu(hmsg_queue& qq, hmessage& mg_in, bool& the_ack);
	void run_tail_queue_simu(hmsg_queue& qq, hmessage& mg_out, hnode& the_out, bool& the_req);

	bool ck_link(hnode* lnk, gh_io_kind_t kk);
	
	void print_addr(FILE* ff, char pfx = '#');
	
	void create_thread_simu(long idx);

	gh_string_t get_verilog_id();

	gh_string_t dbg_get__in0_verilog_id(){ gh_get_io_verilog_id(get_in0); }
	gh_string_t dbg_get__in1_verilog_id(){ gh_get_io_verilog_id(get_in1); }
	gh_string_t dbg_get__out0_verilog_id(){ gh_get_io_verilog_id(get_out0); }
	gh_string_t dbg_get__out1_verilog_id(){ gh_get_io_verilog_id(get_out1); }
	
	gh_string_t get_verilog_send_node_interface_name(hnode** ppout);
	gh_string_t get_verilog_receive_node_interface_name(hnode** ppin);
	
};

void gh_print_tabs(FILE* ff, long num_tabs);
void gh_print_verilog_declare_link_interface(verilog_file& ff, long num_tabs, gh_string_t itf_nm);
void gh_print_verilog_instance_send_interface(verilog_file& ff, long num_tabs, gh_string_t itf_nm_1, gh_string_t itf_nm_2, 
											  bool with_final_comma = true);
void gh_print_verilog_instance_receive_interface(verilog_file& ff, long num_tabs, gh_string_t itf_nm_1, gh_string_t itf_nm_2, 
												 bool with_final_comma = true);

class hnode_1to1 : public hnode {
public:
	hmessage msg0;
	
	hmsg_queue buff0;
	
	bool ack0 = false;
	bool req0 = false;
	
	hnode* in0 = gh_null;
	hnode* out0 = gh_null;

	hnode_1to1(){
		buff0.resize(GH_BUFF_SIMU_SZ);
		buff0.front().init_as_empty_buff();
		
		in0 = gh_null;
		out0 = gh_null;
	}
	virtual ~hnode_1to1(){}
	
	void init_to_me(){
		in0 = this;
		out0 = this;
	}

	virtual bool 	get_ack0(){ return ack0; }
	virtual bool 	get_req0(){ return req0; }
	
	virtual hnode** 	get_pt_in0(){ return &in0; }
	virtual hnode** 	get_pt_out0(){ return &out0; }

	virtual hnode** set_in0(hnode* nd){ in0 = nd; return &in0; }
	virtual hnode** set_out0(hnode* nd){ out0 = nd; return &out0; }

	virtual hmessage* get_message0(){ return &(msg0); }
	
	virtual bool 	ck_connections(){ 
		bool c1 = ck_link(in0, gh_in_kind);
		bool c2 = ck_link(out0, gh_out_kind);
		return (c1 && c2);
	}
	
	virtual gh_hnode_kind_t
	get_kind(){
		return gh_1_to_1_nod;
	}
	
	virtual gh_1t1_kind_t
	get_1t1_kind(){
		return gh_invalid_1t1;
	}
	
	virtual bool
	has_io(hnode** io){
		return ((&in0 == io) || (&out0 == io));
	}
	
	virtual int
	print_node(FILE* ff, gh_prt_mode_t md);

	//void run_1to1_simu();
};

class hnode_direct : public hnode_1to1 {
public:
	ppnode_vec_t* in_vec = gh_null;
	long in_idx = GH_INVALID_IDX;
	
	ppnode_vec_t* out_vec = gh_null;
	long out_idx = GH_INVALID_IDX;

	hnode_direct(){
		init_hnode_direct();
	}
	
	virtual ~hnode_direct(){
		init_hnode_direct();
	}
	
	void init_hnode_direct(){
		in_vec = gh_null;
		in_idx = GH_INVALID_IDX;
		
		out_vec = gh_null;
		out_idx = GH_INVALID_IDX;
	}
	
	virtual void 
	set_direct_idx(gh_io_kind_t kk, ppnode_vec_t& all_io, long idx){
		GH_CK(idx != GH_INVALID_IDX);
		if(kk == gh_out_kind){
			GH_CK(&out0 == all_io[idx]);
			out_vec = &all_io;
			out_idx = idx;
		} else {
			GH_CK(kk == gh_in_kind);
			GH_CK(&in0 == all_io[idx]);
			in_vec = &all_io;
			in_idx = idx;
		}
	}
	
	virtual gh_1t1_kind_t
	get_1t1_kind(){
		return gh_direct_1t1;
	}
	
	virtual int
	print_node(FILE* ff, gh_prt_mode_t md);
};

class hnode_target : public hnode_1to1 {
public:
	long bx_idx = GH_INVALID_IDX;
	
	bool is_source_simu = false;
	gh_addr_t curr_dest_simu = GH_INVALID_ADDR;
	gh_dbg_st_t curr_st_simu = gh_min_min_dbg_st;

	long num_msg_snt_simu = 0;
	long num_msg_rcv_simu = 0;
	
	long dbg_lft_in_sz = 0;
	long dbg_lft_out_sz = 0;
	long dbg_rgt_in_sz = 0;
	long dbg_rgt_out_sz = 0;

	hnode_target(){}
	virtual ~hnode_target(){}
	
	bool inc_st_simu();
	
	hnode_target* dbg_choose_target_simu();
	hnode_target* test0_choose_tgt_simu();
	hnode_target* test1_choose_tgt_simu();
	hnode_target* test2_choose_tgt_simu();
	hnode_target* test3_choose_tgt_simu();
	hnode_target* test4_choose_tgt_simu();
	hnode_target* test5_choose_tgt_simu();

	void dbg_choose_msg_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	bool dbg_inc_tgt_simu();
	void dbg_nxt_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	void dbg_calc_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	
	void test0_choose_msg_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	void test1_choose_msg_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	void test2_choose_msg_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	void test3_choose_msg_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	void test4_choose_msg_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	void test5_choose_msg_src_dst_simu(gh_addr_t& src, gh_addr_t& dst);
	
	void run_target_simu();
	
	virtual gh_1t1_kind_t
	get_1t1_kind(){
		return gh_target_1t1;
	}
	
	virtual int
	print_node(FILE* ff, gh_prt_mode_t md);
};

class hnode_1to2 : public hnode {
public:
	gh_1to2_kind_t rou_kind = gh_invalid_rou;
	gh_dbg_call_t dbg_kind = gh_invalid_call;
	
	hmessage msg0;
	hmessage msg1; 
	
	hmsg_queue buff0;
	hmsg_queue buff1;
	
	edge o_eg0;
	edge o_eg1;
	
	bool ack0 = false;
	bool req0 = false;
	bool req1 = false;
	
	hnode* in0 = gh_null;
	hnode* out0 = gh_null;
	hnode* out1 = gh_null;

	hnode_1to2(){
		buff0.resize(GH_BUFF_SIMU_SZ);
		buff0.front().init_as_empty_buff();
		buff1.resize(GH_BUFF_SIMU_SZ);
		buff1.front().init_as_empty_buff();
		
		in0 = gh_null;
		out0 = gh_null;
		out1 = gh_null;
	}
	virtual ~hnode_1to2(){}
	
	void init_to_me(){
		in0 = this;
		out0 = this;
		out1 = this;
	}

	bool is_out0(hnode** ppo){
		return (&out0 == ppo);
	}
	
	bool is_out1(hnode** ppo){
		return (&out1 == ppo);
	}
	
	void	set_outs_biggest_idx();
	edge	get_biggest_idx();

	edge	get_out0_biggest_idx(){
		GH_CK(out0 != gh_null);
		GH_CK(out0->is_1to2());
		if(out0 == this){
			return o_eg0;
		}
		hnode_1to2* oo = (hnode_1to2*)out0;
		return oo->get_biggest_idx();
	}
	
	hnode_1to2* get_1to2_out0(){
		GH_CK(out0 != gh_null);
		GH_CK(out0->is_1to2());
		return (hnode_1to2*)out0;
	}
	
	hnode_1to2* get_1to2_out1(){
		GH_CK(out1 != gh_null);
		GH_CK(out1->is_1to2());
		return (hnode_1to2*)out1;
	}
	
	virtual bool 	get_ack0(){ return ack0; }
	virtual bool 	get_req0(){ return req0; }
	virtual bool 	get_req1(){ return req1; }
	
	virtual hnode** 	get_pt_in0(){ return &in0; }
	virtual hnode** 	get_pt_out0(){ return &out0; }
	virtual hnode** 	get_pt_out1(){ return &out1; }
	
	virtual hnode** set_in0(hnode* nd){ in0 = nd; return &in0; }
	virtual hnode** set_out0(hnode* nd){ out0 = nd; return &out0; }
	virtual hnode** set_out1(hnode* nd){ out1 = nd; return &out1; }
	
	virtual hmessage* get_message0(){ return &(msg0); }
	virtual hmessage* get_message1(){ return &(msg1); }
	
	virtual bool 	ck_connections(){ 
		bool c1 = ck_link(in0, gh_in_kind);
		bool c2 = ck_link(out0, gh_out_kind);
		bool c3 = ck_link(out1, gh_out_kind);
		return (c1 && c2 && c3);
	}
	
	virtual gh_hnode_kind_t
	get_kind(){
		return gh_1_to_2_nod;
	}
	
	virtual bool
	has_io(hnode** io){
		return ((&in0 == io) || (&out0 == io) || (&out1 == io));
	}
	
	virtual int
	print_node(FILE* ff, gh_prt_mode_t md);
	
	void run_1to2_simu();
	void run_1to2_buff_simu();
	
	const char* dbg_kind_to_str(gh_dbg_call_t cll);
	
	gh_string_t get_verilog_oper_1();
	gh_string_t get_verilog_ref_val_1();
	gh_string_t get_verilog_is_range();
	gh_string_t get_verilog_oper_2();
	gh_string_t get_verilog_ref_val_2();
	
	void print_verilog_1to2_instance(verilog_file& ff, long idx);
};

class hnode_2to1 : public hnode {
public:
	hmessage msg0;
	
	hmsg_queue buff0;
	
	bool choose0 = true;

	bool ack0 = false;
	bool ack1 = false;
	bool req0 = false;
	
	hnode* in0 = gh_null;
	hnode* in1 = gh_null;
	hnode* out0 = gh_null;

	hnode_2to1(){
		buff0.resize(GH_BUFF_SIMU_SZ);
		buff0.front().init_as_empty_buff();
		
		choose0 = true;
		in0 = gh_null;
		in1 = gh_null;
		out0 = gh_null;
	}
	
	virtual ~hnode_2to1(){}
	
	void init_to_me(){
		in0 = this;
		in1 = this;
		out0 = this;
	}
	
	virtual bool 	get_ack0(){ return ack0; }
	virtual bool 	get_ack1(){ return ack1; }
	virtual bool 	get_req0(){ return req0; }
	
	virtual hnode** 	get_pt_in0(){ return &in0; }
	virtual hnode** 	get_pt_in1(){ return &in1; }
	virtual hnode** 	get_pt_out0(){ return &out0; }
	
	virtual hnode** set_in0(hnode* nd){ in0 = nd; return &in0; }
	virtual hnode** set_in1(hnode* nd){ in1 = nd; return &in1; }
	virtual hnode** set_out0(hnode* nd){ out0 = nd; return &out0; }
	
	virtual hmessage* get_message0(){ return &(msg0); }
	
	virtual bool 	ck_connections(){ 
		bool c1 = ck_link(in0, gh_in_kind);
		bool c2 = ck_link(in1, gh_in_kind);
		bool c3 = ck_link(out0, gh_out_kind);
		return (c1 && c2 && c3);
	}
	
	virtual gh_hnode_kind_t
	get_kind(){
		return gh_2_to_1_nod;
	}
	
	virtual bool
	has_io(hnode** io){
		return ((&in0 == io) || (&in1 == io) || (&out0 == io));
	}
	
	virtual int
	print_node(FILE* ff, gh_prt_mode_t md);
	
	bool ck_out_interval(char dbg_in);
	
	void run_2to1_simu();
	void run_2to1_buff_simu();

	void print_verilog_2to1_instance(verilog_file& ff, long idx);
	
};

long 	gh_get_first_null_idx(ppnode_vec_t& vec);
bool 	gh_is_free_io(hnode** io);

inline hnode* 	
gh_set_io(ppnode_vec_t& all_io, long idx_io, hnode* nd){
	GH_CK(idx_io < (long)all_io.size());
	hnode** ppo = all_io[idx_io];
	GH_CK(gh_is_free_io(ppo));
	hnode* po = *ppo;
	*ppo = nd;
	return po;
}

void gh_connect_out_to_in(ppnode_vec_t& all_out, long out, ppnode_vec_t& all_in, long in);
void gh_connect_outputs_to_inputs(ppnode_vec_t& out, ppnode_vec_t& in);
bool gh_move_io(gh_io_kind_t kk, ppnode_vec_t& src, ppnode_vec_t& dst);
void gh_copy_nodes(pnode_vec_t& src, pnode_vec_t& dst, bool clr_src);
void gh_move_nodes(pnode_vec_t& src, pnode_vec_t& dst);

class hnode_box {
public:
	long pw_base = 0;
	
	pnode_vec_t all_direct;
	pnode_vec_t all_nodes;
	ppnode_vec_t inputs;
	ppnode_vec_t outputs;
	
	hnode_box(){
	}
	
	virtual ~hnode_box(){
		release_nodes();
		clear_box();
	}

	void 	clear_box(){
		// CAREFUL. Pointers are not released.
		all_direct.clear();
		all_nodes.clear();
		inputs.clear();
		outputs.clear();
	}
	
	void 	init_all_addr();
	void 	init_all_direct();
	void 	release_nodes();
	
	void 	fill_with_directs(ppnode_vec_t& all_in, ppnode_vec_t& all_out);
	
	virtual
	void 	print_box(FILE* ff, gh_prt_mode_t md);

	long get_base(){
		GH_CK(pw_base > 1);
		return pw_base;
	}
	
	hnode_1to2* add_1to2(gh_1to2_kind_t kk, const char* dbg_qrt = gh_null, gh_dbg_call_t dbg_case = gh_invalid_call);
	hnode_2to1* add_2to1(const char* dbg_qrt = gh_null);
	hnode_direct* add_direct();
	
	hnode_box* convert_net_from_1to2_to_2to1();
	
	bool move_nodes_to(hnode_box& bx);
	
	hnode* set_output(long out, hnode* nd){
		return gh_set_io(outputs, out, nd);
	}

	hnode* set_input(long in, hnode* nd){
		return gh_set_io(inputs, in, nd);
	}

	void connect_output_to_node_input(long out, hnode_2to1& nd, long in){
		hnode* po = set_output(out, &nd);
		if(in == 0){
			GH_CK(nd.in0 == &nd);
			nd.in0 = po;
		} else {
			GH_CK(nd.in1 == &nd);
			nd.in1 = po;
		}
	}
	
	void connect_node_output_to_input(hnode_1to2& nd, long out, long in){
		hnode* pi = set_input(in, &nd);
		if(out == 0){
			GH_CK(nd.out0 == &nd);
			nd.out0 = pi;
		} else {
			GH_CK(nd.out1 == &nd);
			nd.out1 = pi;
		}
	}
	
	void remove_connected_directs();
	void connect_outputs_to_box_inputs(hnode_box& bx);
	
	void init_sm_to_bm_edges(slice_vec* out_addrs);
	
};

void gh_print_verilog_declare_out_channel(verilog_file& ff, gh_string_t itf_nm, bool with_final_comma = true);
void gh_print_verilog_declare_in_channel(verilog_file& ff, gh_string_t itf_nm, bool with_final_comma = true);
void gh_print_verilog_declare_pakout(verilog_file& ff, gh_string_t itf_nm, bool with_final_comma = true);
void gh_print_verilog_declare_pakin(verilog_file& ff, gh_string_t itf_nm, bool with_final_comma = true);

hnode_box*
gh_get_binnet_sm_to_bm(long num_in, long num_out, const char* dbg_qrt, gh_dbg_call_t dbg_case);
	
hnode_box*
gh_get_binnet_m_to_n(long num_in, long num_out, slice_vec* out_addr, 
					 const char* dbg_qrt, gh_dbg_call_t dbg_case);

class hroute_box : public hnode_box {
public:
	hroute_box(long pnt_base) {
		pw_base = pnt_base;
	}
	
	virtual ~hroute_box(){
		release_nodes();
	}
	
	void 	init_route_box(long num_in, long num_out, slice_vec& out_addr, const char* dbg_qrt);
	bool 	ck_route_box(long num_in, long num_out, int dbg_case);
	
	void 	init_as_2to2_route_box(slice_vec* out_addr, const char* dbg_qrt);
	void 	init_as_3to2_route_box(slice_vec* out_addr, const char* dbg_qrt);
	void 	init_as_2to3_route_box(slice_vec* out_addr, const char* dbg_qrt);

	hnode_1to2*	get_1to2_node(long idx){
		GH_CK(idx < (long)all_nodes.size());
		hnode* nd = all_nodes[idx];
		GH_CK(nd->is_1to2());
		return (hnode_1to2*)nd;
	}
};
	
void gh_print_io(FILE* ff, gh_prt_mode_t md, ppnode_vec_t& all_io);
void gh_init_quarter(ppnode_vec_t qrt, gh_addr_t tg_addr, char* qrt_nm);
void gh_print_verilog_assigns_for_ios(verilog_file& ff, ppnode_vec_t& all_io, gh_route_side_t sd, gh_io_kind_t iot);
void gh_print_verilog_code_for_nodes(verilog_file& ff, pnode_vec_t& all_nd);

enum gh_verilog_print_op_t {
	gh_vl_declare_param_op,
	gh_vl_declare_link_op,
	gh_vl_instance_net_op,
	gh_vl_instance_wrapper_op
};

class htarget_box : public hnode_box {
public:
	hnode_target* target = gh_null;
	ppnode_vec_t lft_in;
	ppnode_vec_t lft_out;
	ppnode_vec_t rgt_in;
	ppnode_vec_t rgt_out;
	
	htarget_box(long pnt_base) {
		pw_base = pnt_base;
		
		target = gh_null;
	}
	
	virtual ~htarget_box(){
		del_htarget_box();
	}
	
	long 	get_index(){
		GH_CK(target != gh_null);
		return target->bx_idx;
	}
	
	void 	del_htarget_box();

	void 	join_box_outputs(long fst_idx1, hnode_box* bx1, long fst_idx2, hnode_box* bx2, 
							 long num_idx, ppnode_vec_t& all_out, const char* dbg_qrt);
	void 	resize_with_directs(long nw_side_sz);

	void 	init_basic_target_box(long tgt_idx, long lft_ht, long rgt_ht, slice_vec& tgt_addrs, long tot_tgt);
	void 	init_target_box(long tgt_idx, long lft_sz, long rgt_sz, slice_vec& tgt_addrs, long tot_tgt);
	bool 	ck_target_box(long lft_ht, long rgt_ht);
	
	void 	move_target_to(hlognet_box& bx);
	
	void	dbg_init_tgt_szs();

	virtual
	void 	print_box(FILE* ff, gh_prt_mode_t md);
	
	bool 	can_verilog_module_have_wrapper(){
		bool can_wrp = (lft_in.size() == lft_out.size());
		GH_CK(! can_wrp || (rgt_in.size() == rgt_out.size()));
		GH_CK(! can_wrp || (rgt_in.size() == lft_in.size()));
		return can_wrp;
	}
	
	gh_string_t get_verilog_router_module_name();
	gh_string_t get_verilog_wrapper_module_name();
	gh_string_t get_verilog_core_module_name();
	gh_string_t get_verilog_target_param_name(gh_io_kind_t iok);
	gh_string_t get_verilog_target_link_name(long lnk, gh_route_side_t sd, gh_io_kind_t iok);

	void 	print_verilog_target_param(verilog_file& ff);
	void 	print_verilog_router_target_assign(verilog_file& ff);
	
	void 	print_verilog_module_core(verilog_file& ff, runner_print_verilog_network& cfg);
	void 	print_verilog_instance_core(verilog_file& ff);
	void 	print_verilog_module_router(verilog_file& ff);
	void 	print_verilog_instance_router(verilog_file& ff);
	void 	print_verilog_module_wrapper(verilog_file& ff, long num_direct_chns, long num_direct_paks);
	void 	print_verilog_instance_wrapper(verilog_file& ff, long num_direct_chns, long num_direct_paks);
	
	void 	print_verilog_all_params(verilog_file& vff, gh_verilog_print_op_t op, 
									 long num_direct_chns, long num_direct_paks, bool with_final_comma);
	
	void 	print_verilog_instance_io_net(verilog_file& vff, gh_string_t io_nm, long lnk_idx, 
										  gh_route_side_t sd, gh_io_kind_t iok, bool with_final_comma);
	void 	print_verilog_declare_io_link_net(verilog_file& vff, long lnk_idx, gh_route_side_t sd, gh_io_kind_t iok);
	void 	print_verilog_declare_pakio_link_net(verilog_file& vff, long lnk_idx, gh_route_side_t sd, gh_io_kind_t iok);
	
	void 	print_verilog_instance_io_wrapper(verilog_file& vff, gh_string_t io_nm, gh_io_kind_t iok, bool with_final_comma);
	
	void	print_verilog_params_for_ios(verilog_file& vff, gh_verilog_print_op_t op, long tot_io, 
										 gh_route_side_t sd, gh_io_kind_t iok, bool with_final_comma);
	void	print_verilog_params_for_direct_packets(verilog_file& vff, gh_verilog_print_op_t op, long tot_core_ios, 
										   long num_direct_ios, gh_route_side_t sd, gh_io_kind_t iok, bool with_final_comma);
	void	print_verilog_params_for_direct_channels(verilog_file& vff, gh_verilog_print_op_t op, long tot_core_ios, 
											long num_direct_ios, gh_route_side_t sd, gh_io_kind_t iok, bool with_final_comma);

};

void 	gh_calc_num_io(long base, long length, long idx, long& num_in, long& num_out);

class hlognet_box : public hnode_box {
public:
	long tot_targets;
	long height;
	ptarget_vec_t all_targets;
	
	hlognet_box(long pnt_base) {
		pw_base = pnt_base;

		tot_targets = 0;
		height = 0;
	}
	
	virtual ~hlognet_box(){
		release_nodes();
		release_targets();
	}
	
	void 	release_targets();
	
	void 	init_all_target_addr();

	void 	init_length(long num_elems);
	void 	init_lognet_box(long num_elems, slice_vec& out_addr, const char* dbg_qrt = gh_null);
	bool 	ck_lognet_box(long num_elems);
	
	void 	init_as_io(slice_vec& tgt_addrs);

	bool 	ck_lognet_path(gh_addr_t src, gh_addr_t dst, bool dbg_prt);
	bool 	ck_lognet_all_paths_from(gh_addr_t src_addr, bool dbg_prt);
	bool 	ck_lognet_all_to_all_paths(bool dbg_prt);
	
	htarget_box* get_target_box(long idx, slice_vec& out_addr);

	virtual
	void 	print_box(FILE* ff, gh_prt_mode_t md);

	void 	wait_all_inited_simu();
	void 	wait_all_threads_ended_simu();
	void 	run_hlognet_simu();

	void 	print_targets(FILE* ff, gh_prt_mode_t md);

	void 	copy_verilog_foundation_files(gh_string_t& net_nam, runner_print_verilog_network& cfg);

	void	print_verilog_file_name_to_iverilog_file(FILE* ff, gh_string_t nm);
	void	print_verilog_file_name_to_verilator_file(FILE* ff, gh_string_t nm);
	void	print_verilog_file_name_to_yosys_file(FILE* ff, gh_string_t nm);

	void	print_verilog_header_to_yosys_file(FILE* ff);
	void	print_verilog_footer_to_yosys_file(FILE* ff, gh_string_t nm);
	
	void	print_verilog_file_name_to_command_files(gh_string_t nm, runner_print_verilog_network& cfg){
		print_verilog_file_name_to_iverilog_file(cfg.ivl_comm_fl, nm);
		print_verilog_file_name_to_verilator_file(cfg.vtr_comm_fl, nm);
		print_verilog_file_name_to_yosys_file(cfg.yos_comm_fl, nm);
	}
	
	void	print_verilog_config_file(runner_print_verilog_network& cfg);
	void 	print_verilog_full_net(runner_print_verilog_network& cfg);
};

gh_string_t gh_get_verilog_targets_link(long tg1, long tg2, gh_route_side_t sd, long num_lnk);
void gh_dbg_calc_idx_and_sz(long num_in, long num_out, long bs, long& tg_idx, long& tgs_sz);

void gh_print_str_set(FILE* ff, const gh_str_set_t& the_set);
bool gh_str_is_prefix(const std::string& the_str, const std::string& pfx);

void gh_args_get_list(gh_str_list_t& lt_args, int argc, char *argv[]);
bool gh_args_get_candidates(const gh_str_set_t& map, const std::string& search_for, gh_str_set_t& all_cand);
void gh_args_print_candidates(const gh_str_set_t& all_cand);
void gh_args_print(gh_str_list_t& lt_args);
void gh_dec_args(gh_str_list_t& lt_args, int num_dec = 1);
char** gh_args_get_tail(char *argv[]);
bool gh_args_is_complete_command(gh_str_list_t& lt_args);
int gh_args_get_complete_index();
bool gh_args_select_one_of(gh_str_list_t& lt_args, gh_str_set_t& choices, std::string& sel);

void* run_node_simu(void* pm);

int test_hlogne2(gh_str_list_t& lt_args);
int test_verilog(gh_str_list_t& lt_args);

int mini_test_copy_file(int argc, char *argv[]);

#endif // GEN_HNET_H

