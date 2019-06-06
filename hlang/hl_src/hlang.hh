

/*************************************************************

This file is part of ben-jose.

ben-jose is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

ben-jose is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ben-jose.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2007-2012, 2014-2016. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://github.com/joseluisquiroga/ben-jose

ben-jose is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------

hlang.hh

hlang declarations.

--------------------------------------------------------------*/

#ifndef HLANG_H
#define HLANG_H

#include <algorithm>
#include <type_traits>
#include <iostream>
#include <stdio.h>
#include <map>
#include <string>
#include <list>
#include <set>
//include <vector>

#include "dbg_util.h"

#define MAX_BUFF_SZ 0xFFF

#define HL_INVALID_SAFE_IDX 0
#define HL_MAX_SAFE_IDX 63
#define HC_NUM_RESERVED_SAFE_IDX 4
#define HC_GET_NEXT_SAFE_IDX 1
#define HC_SEND_AGAIN_SAFE_IDX 2

extern char PROC_LINK_BUFF[];
extern char FILE_NAME_BUFF[];

FILE* file_open(const char* nm);
bool file_rename(hl_string& old_pth, hl_string& nw_pth);
bool file_remove(hl_string& pth);
bool file_exists(hl_string th_pth);
bool file_touch(hl_string the_pth);
bool file_newer_than(hl_string the_pth, time_t tm1);
void file_update(hl_string& tmp_nm, hl_string& hh_nm);
bool make_dir(hl_string the_pth, mode_t mod = 0700);
bool change_dir(hl_string the_pth);
hl_string path_get_running_path();
hl_string path_to_absolute_path(hl_string pth);
bool path_create(hl_string n_pth);
bool path_begins_with(hl_string the_pth, hl_string the_beg);
bool path_ends_with(hl_string& the_str, hl_string& the_suf);
hl_string path_get_directory(hl_string the_pth, bool add_last_dir_sep);
hl_string path_get_name(hl_string the_pth);
hl_string get_errno_str(long val_errno);
hl_string get_upper_str(const hl_string& lower);

//======================================================================
// bitarray

// where 'a' is the byte stream pointer, and b is the bit number.

#define hl_div8(b)	((b)>>3)
#define hl_mod8(b)	((b)&7)

#define hl_get_bit(a, b)		((((int8_t*)a)[hl_div8(b)] >> hl_mod8(b)) & 1)
#define hl_set_bit(a, b)		(((int8_t*)a)[hl_div8(b)] |= (1 << hl_mod8(b)))
#define hl_reset_bit(a, b) 		(((int8_t*)a)[hl_div8(b)] &= ~(1 << hl_mod8(b)))
#define hl_toggle_bit(a, b) 	(((int8_t*)a)[hl_div8(b)] ^= (1 << hl_mod8(b)))

#define hl_to_bytes(num_bits)	(hl_div8(num_bits) + (hl_mod8(num_bits) > 0))
#define hl_to_bits(num_bytes)	(num_bytes * 8)

//======================================================================
// defs

typedef uint64_t hl_safe_bits_t;
typedef uint8_t hl_safe_idx_t; 

typedef int64_t hc_chip_idx;
#define hl_tok_last 0

#define HC_MAX_LEVELS 10

class hc_system;
class hc_term;
class hc_global;
class hc_steps;
class hc_keyword;
class hc_mth_def;
class hc_mth_call;
class hcell;
class hc_ast_block;

typedef void (*hc_caller_t)();

//======================================================================
// hclass_reg

class hclass_reg {
public:
	hl_string nam;
	std::set<hl_string> attributes;
	std::list<hc_term*> values;
	std::list<hc_term*> references;
	std::list<hc_term*> safe_values;
	std::list<hc_term*> safe_references;
	std::list<hc_mth_def*> methods;
	
	bool with_methods = false;
	bool with_dbg_mem = false;
	bool allow_send_values = false;
	
	hc_mth_def* nucleus = hl_null;
	hc_caller_t initer = hl_null;
	const char* pre_hh_cod = hl_null;
	const char* pos_hh_cod = hl_null;
	const char* pre_cpp_cod = hl_null;
	const char* pos_cpp_cod = hl_null;
	long depth;
	long tot_steps;
	long mth_nucleus_caller_step = 0;
	long mth_handler_return_step = 0;
	long mth_queue_pop_step = 0;
	long mth_safe_wait_step = 0;
	long mth_call_num_step = 0;
	long mth_ret_num_step = 0;
	hl_safe_idx_t tot_safe_attrs = HC_NUM_RESERVED_SAFE_IDX;
	
	hclass_reg(){
		with_methods = false;
		with_dbg_mem = false;
		allow_send_values = false;
	
		nucleus = hl_null;
		initer = hl_null;
		pre_hh_cod = hl_null;
		pos_hh_cod = hl_null;
		pre_cpp_cod = hl_null;
		pos_cpp_cod = hl_null;
		depth = 0;
		tot_steps = 0;
		mth_nucleus_caller_step = 0;
		mth_handler_return_step = 0;
		mth_queue_pop_step = 0;
		mth_safe_wait_step = 0;
		mth_call_num_step = 0;
		mth_ret_num_step = 0;
		tot_safe_attrs = HC_NUM_RESERVED_SAFE_IDX;
	}

	virtual	~hclass_reg(){}

	bool has_attribute(const char* attr){
		auto it = attributes.find(attr);
		return (it != attributes.end());
	}
	
	bool has_value(const char* attr);
	bool has_reference(const char* attr);
	
	void init_methods();
	void init_depth();

	void print_methods(FILE* st);
	
	hl_string get_tmp_hh_name(){
		return nam + "_tmp.hh";
	}
	
	hl_string get_hh_name(){
		return nam + ".hh";
	}
	
	hl_string get_hh_define_str(){
		hl_string out = get_upper_str(nam);
		out += "_HH_FILE";
		return out;
	}
	
	void print_hh_file();
	void print_hh_all_mth_step_id(FILE* st);
	void print_hh_class_top_decl(FILE* ff);
	void print_hh_class_decl_content(FILE* ff);

	hl_string get_tmp_cpp_name(){
		return nam + "_tmp.cpp";
	}
	
	hl_string get_cpp_name(){
		return nam + ".cpp";
	}
	
	void print_cpp_file();
	void print_cpp_release_me_mth(FILE* ff);
	void print_cpp_class_defs(FILE* ff);
	
	void print_cpp_call_mth_case(FILE* st, long idx);
	void print_cpp_ret_mth_case(FILE* st, long idx);
	
	void print_cpp_get_set_switch_code(FILE* ff);
	void print_cpp_handle_reply_code(FILE* ff);
	void print_cpp_replies_switch_code(FILE* ff);
	
	void print_cpp_nucleus_caller_code(FILE* st);
	void print_cpp_handler_return_code(FILE* st);
	void print_cpp_queue_pop_code(FILE* st);
	void print_cpp_call_wait_safe_code(FILE* st);
	void print_cpp_call_mth_code(FILE* st);
	void print_cpp_ret_mth_code(FILE* st);
	
	void print_all_cpp_methods(FILE *st);
};

extern hc_system& HLANG_SYS();

class hc_system {
public:
	long first_att_id_val;
	long first_token_val;
	hl_string first_handler_idx;
	hl_string project_nam;

	std::map<hl_string, hclass_reg*> all_classes;
	
	std::map<hl_string, hc_term*> all_glb_address;
	std::map<hl_string, hc_global*> all_glb_att_id;
	std::map<hl_string, hc_global*> all_glb_token;
	std::map<hl_string, hc_global*> all_glb_const;

	hc_system(){
		init_me();
	}

	virtual	~hc_system(){}

	virtual
	void init_me(int caller = 0){
		project_nam = "hl_generated_output";
		first_att_id_val = 5;
		first_token_val = 11;
		first_handler_idx = "mck_tot_base_cell_classes";
	}
	
	hclass_reg* get_hcell_reg(const char* cls_nm){
		hclass_reg* rg = get_class_reg(cls_nm, hl_null, false, true);
		return rg;
	}
	
	hclass_reg* get_class_reg(const char* cls, hc_caller_t the_initer = hl_null, 
							  bool with_mths = false, bool only_get = false);
	void register_method(const char* cls, hc_mth_def* mth, bool is_nucl, bool can_register);
	void register_value(hcell* obj, hc_term* attr);
	void register_reference(hcell* obj, hc_term* attr);
	
	bool is_missive_class(const char* cls);

	bool has_address(const char* attr);
	void register_address(hc_term* attr);

	hc_global& add_att(const char* nm);
	hc_global& add_tok(const char* nm);
	hc_global& add_con(const char* nm, const char* val);
	hc_global& get_att(const char* nm);
	hc_global& get_tok(const char* nm);
	hc_global& get_con(const char* nm);
	
	void init_all_attributes(FILE* st);
	void init_all_att_id(FILE* st);
	void init_all_token(FILE* st);
	void init_all_methods(FILE* st);
	void init_sys(FILE* st);

	void print_all_methods(FILE* st);
	
	void generate_hh_files();
	void generate_cpp_files();
	void generate_cpp_code();
	
	hl_string get_cpp_idx_last_str(){
		hl_string tot = "idx_last_ivalid_" + project_nam;
		return tot;
	}
	
	hl_string get_cpp_idx_total_str(){
		hl_string tot = "idx_total_" + project_nam;
		return tot;
	}
	
	hl_string get_cpp_dir_name(){
		return project_nam + "_cpp";
	}
	
	hl_string get_glbs_tmp_hh_name(){
		return project_nam + "_glbs_tmp.hh";
	}
	
	hl_string get_glbs_hh_name(){
		return project_nam + "_glbs.hh";
	}
	
	hl_string get_glbs_hh_define_str(){
		hl_string out = get_upper_str(project_nam);
		out += "_GLBS_HH_FILE";
		return out;
	}
	
	hl_string get_glbs_tmp_cpp_name(){
		return project_nam + "_glbs_tmp.cpp";
	}
	
	hl_string get_glbs_cpp_name(){
		return project_nam + "_glbs.cpp";
	}
	
	hl_string get_tmp_hh_name(){
		return project_nam + "_tmp.hh";
	}
	
	hl_string get_hh_name(){
		return project_nam + ".hh";
	}
	
	hl_string get_hh_define_str(){
		hl_string out = get_upper_str(project_nam);
		out += "_HH_FILE";
		return out;
	}
	
	void print_glbs_hh_file();
	void print_hh_file();

	void print_glbs_cpp_dbg_att_id_to_str_func(FILE* ff);
	void print_glbs_cpp_dbg_tok_to_str_func(FILE* ff);
	void print_glbs_cpp_dbg_cell_idx_to_str_func(FILE* ff);
	void print_glbs_cpp_file();
	
	hl_string get_tmp_cpp_name(){
		return project_nam + "_tmp.cpp";
	}
	
	hl_string get_cpp_name(){
		return project_nam + ".cpp";
	}
	
	void print_cpp_file();
	void print_cpp_class_defs(FILE* ff);

	void set_dbg_mem_code_generation(const char* cls_nm){
		hclass_reg* rg = get_hcell_reg(cls_nm);
		if(rg != hl_null){
			rg->with_dbg_mem = true;
		}
	}

	void allow_send_values(const char* cls_nm){
		hclass_reg* rg = get_hcell_reg(cls_nm);
		if(rg != hl_null){
			rg->allow_send_values = true;
		}
	}
	
};


template <bool> struct HC_ILLEGAL_USE_OF_OBJECT;
template <> struct HC_ILLEGAL_USE_OF_OBJECT<true>{};
#define HC_OBJECT_COPY_ERROR HC_ILLEGAL_USE_OF_OBJECT<false>()

#define hc_xstr(a) hc_str(a)
#define hc_str(a) #a

// ~,|,&,^,==, >>, <<, >, <,?,

enum	hc_syntax_op_t {
	hc_invalid_op,

	hc_value_op,
	hc_reference_op,
	
	hc_safe_check_op,

	hc_hmsg_src_op,
	hc_hmsg_ref_op,
	hc_hmsg_tok_op,
	hc_hmsg_val_op,
	
	hc_mth_call_op,
	hc_mth_def_op,
	hc_mth_ret_op,
	
	hc_dbg_op,
	
	hc_acquire_op,	// hacquire
	hc_release_op,	// hrelease
	
	hc_hget_op,	// hget
	hc_hset_op,	// hset
	hc_hsend_op,	// hsend
	hc_hreply_op,	// hreply
	hc_hwait_op,	// hwait
	
	hc_member_op,	// ->
	
	hc_comma_op,	// ,
	hc_then_op,	// /=
	
	hc_hme_op,	// hme
	
	hc_hfor_op,	// hfor
	hc_hwhile_op,	// hwhile
	hc_hif_op,	// hif
	hc_helif_op,	// helif
	hc_helse_op,	// helse
	hc_hswitch_op,	// hswitch
	hc_hcase_op,	// hcase	
	hc_hdefault_op,	// hdefault
	
	hc_hbreak_op,	// hbreak
	hc_hcontinue_op,	// hcontinue
	hc_hreturn_op,	// hreturn
	hc_hfinished_op,	// hfinished
	
	hc_habort_op, // habort

	hc_hthis_op,	// hthis
	hc_hnull_op,	// hnull
	
	hc_hskip_op,	// hskip
	
	hc_assig_op1,	// =
	hc_assig_op2,	// =
	hc_assig_op3,	// =
	hc_assig_op4,	// =
	hc_assig_op5,	// =

	hc_shift_left_op,	// <<
	hc_shift_right_op,	// >>
	
	hc_less_than_op,	// <
	hc_more_than_op,	// >
	hc_less_equal_than_op,	// <=
	hc_more_equal_than_op,	// >=
	hc_equal_op,	// ==
	hc_not_equal_op,	// !=
	hc_and_op,	// &&
	hc_or_op,	// ||
	hc_bit_and_op,	// &
	hc_bit_or_op,	// |
	
	hc_not_op,	// !
	hc_bit_not_op,	// ~

	hc_plus_op,	// +
	hc_minus_op,	// -
	
	hc_pre_inc_op,	// ++
	hc_pre_dec_op,	// --
	hc_post_inc_op,	// ++
	hc_post_dec_op,	// --
	
	hc_total_num_op
};

const char*
hc_get_token(hc_syntax_op_t op);

const char*
hc_get_cpp_token(hc_syntax_op_t op);

#define HC_DEFINE_BINARY_OP_BASE(the_code) \
	hc_binary_term* tm = new hc_binary_term(the_code, this, &o1); \
	return *tm; \

// end_define

#define HC_DEFINE_ASSIG_BASE(the_code) \
	if(o1.has_statements()){ \
		hl_abort("Parameter %s to %s has statements. Invalid grammar.\n", \
					o1.get_name(), get_name()); \
	} \
	hc_binary_term* tm = new hc_binary_term(the_code, this, &o1); \
	tm->has_st = true; \
	return *tm; \

// end_define

#define HC_DEFINE_BINARY_OP(the_op, the_code) \
hc_term& hc_term::operator the_op (hc_term& o1) { \
	HC_DEFINE_BINARY_OP_BASE(the_code) \
} \

// end_define

#define HC_DEFINE_UNARY_OP_BASE(the_code) \
	hc_term* tm = new hc_unary_term(the_code, this); \

// end_define

#define HC_DEFINE_UNARY_OP(the_op, the_code) \
hc_term& hc_term::operator the_op (){ \
	HC_DEFINE_UNARY_OP_BASE(the_code) \
	return *tm; \
} \

// end_define

#define HC_DEFINE_FUNC_OP(the_op, the_class, the_code) \
hc_term& \
the_op(hc_term& o1){ \
	if(o1.has_statements()){ \
		hl_abort("Parameter %s to %s has statements. Invalid grammar.\n", \
					o1.get_name(), #the_op); \
	} \
	the_class* tm = new the_class(the_code, &o1); \
	return *tm; \
} \

// end_define

#define hl_invalid_bit			-1
#define hl_has_safe_bit 		0
#define hl_has_last_bit 		1
#define hl_has_break_bit 		2
#define hl_has_continue_bit 	3
#define hl_has_return_bit 		4
#define hl_has_abort_bit 		5
#define hl_has_case_bit 		6

class hc_term {
public:
	static hc_mth_def* HC_CURRENT_DEFINING_METHOD;
	static long	HC_PRT_TERM_INDENT;
	static long	HC_NUM_LABEL;
	static const char* HC_INVALID_TYPE;
	
	int8_t	flags;
	long num_steps = 0;
	hc_term* next;
	long label_number = 0;
	
	// constructors
	hc_term(){
		flags = 0;
		num_steps = 0;
		next = hl_null;
		label_number = 0;
	}

	virtual	~hc_term(){
	}

	// Don't allow copying (error prone):
	// force use of referenced rows
	
	hc_term(hc_term& other){ 
		hl_abort("func: 'hc_term(hc_term&)' \n");
		//HC_OBJECT_COPY_ERROR;
		//fprintf(stdout, "%s \n", STACK_STR.c_str());
		//bj_abort_func(0, "func: 'hc_term::~hc_term'"); 
	}

	hc_term&	operator , (hc_term& o1);
	hc_term&	operator /= (hc_term& o1);
	
	hc_term&	operator = (hc_term& o1);

	hc_term&	operator << (hc_term& o1);
	hc_term&	operator >> (hc_term& o1);
	
	hc_term&	operator < (hc_term& o1);
	hc_term&	operator > (hc_term& o1);
	hc_term&	operator <= (hc_term& o1);
	hc_term&	operator >= (hc_term& o1);
	hc_term&	operator == (hc_term& o1);
	hc_term&	operator != (hc_term& o1);
	hc_term&	operator && (hc_term& o1);
	hc_term&	operator || (hc_term& o1);
	hc_term&	operator & (hc_term& o1);
	hc_term&	operator | (hc_term& o1);

	hc_term&	operator + (hc_term& o1);
	hc_term&	operator - (hc_term& o1);

	hc_term&	operator ! ();
	hc_term&	operator ~ ();
	
	hc_term&	operator ++ ();
	hc_term&	operator ++ (int xx);
	hc_term&	operator -- ();
	hc_term&	operator -- (int xx);
	
	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	void print_text_code(FILE *st){
		hl_abort("Invalid call to 'print_text_code()'.\n" 
				"NOT_A_METHOD. It has no code to print.\n");
	}

	virtual 
	void print_cpp_code(FILE *st){
		hl_abort("Invalid call to 'print_cpp_code()'.\n" 
				"NOT_A_METHOD. It has no code to print.\n");
	}

	virtual 
	hc_term*	get_src(){
		return this;
	}
	
	virtual 
	void set_num_label(){}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){}

	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_invalid_op;
	}
	
	virtual 
	hc_syntax_op_t	get_cond_oper(){
		return hc_invalid_op;
	}
	
	virtual 
	const char*	get_type(){
		return HC_INVALID_TYPE;
	}	

	virtual 
	hl_string	get_cpp_casted_tmp_msv_val(){
		return HC_INVALID_TYPE;
	}	

	virtual 
	const char*	get_name(){
		return "INVALID_NAME";
	}	

	virtual 
	hl_string	get_value(){
		return "0";
	}	

	virtual 
	hl_string	get_id(){
		return "INVALID_ID";
	}
	
	virtual 
	bool	is_compound(){
		return false;
	}

	virtual 
	bool	has_statements(){
		return false;
	}
	
	virtual 
	hc_term* get_first_step(){
		return this;
	}

	virtual 
	void	set_next(hc_term& nxt);

	virtual 
	void	set_last(hc_term& nxt);
	
	virtual 
	void	set_jumps(hc_syntax_op_t the_op, hc_term& nxt);
	
	virtual
	void	set_safe_idx(hl_safe_idx_t idx){}
	
	virtual 
	hl_safe_idx_t get_safe_idx(){ return HL_INVALID_SAFE_IDX; }
		
	virtual 
	hl_safe_bits_t get_safe_bit_mask(){
		return 0;
	}

	virtual 
	bool 	is_safe_attribute(){ return false; }

	virtual 
	bool 	is_message_reference(){ return false; }

	static
	void print_new_line(FILE *st);

	static
	void print_indent(FILE *st, bool with_case_margin = false);
	
	bool 	is_closed_member(){
		return (get_src() == this);
	}
	
	bool is_cond(){
		return (get_cond_oper() != hc_invalid_op);
	}
	
	bool	is_if_closer(){
		hc_syntax_op_t op = get_cond_oper();
		return ((op != hc_helif_op) && (op != hc_helse_op));
	}
	
	hc_steps* to_steps();
	
	long get_num_label();
	
	void print_label(FILE *st);

	void set_flag(int num_flg){
		hl_set_bit(&flags, num_flg);
	}

	void reset_flag(int num_flg){
		hl_reset_bit(&flags, num_flg);
	}

	bool get_flag(int num_flg){
		return hl_get_bit(&flags, num_flg);
	}
	
	void set_has_last(){
		hl_set_bit(&flags, hl_has_last_bit);
	}

	void reset_has_last(){
		hl_reset_bit(&flags, hl_has_last_bit);
	}

	bool get_has_last(){
		return hl_get_bit(&flags, hl_has_last_bit);
	}
	
	void set_has_safe(){
		hl_set_bit(&flags, hl_has_safe_bit);
	}

	void reset_has_safe(){
		hl_reset_bit(&flags, hl_has_safe_bit);
	}

	bool get_has_safe(){
		return hl_get_bit(&flags, hl_has_safe_bit);
	}
	
	void or_has_safe(hc_term* tm){
		if((tm != hl_null) && tm->get_has_safe()){
			set_has_safe();
		}
	}

	bool with_dbg_mem(){ 
		const char* typ = get_type();
		hclass_reg* rg = HLANG_SYS().get_hcell_reg(typ);
		if(rg != hl_null){
			return rg->with_dbg_mem;
		}
		return false; 
	}
	
	void print_cpp_get_set_case(FILE* ff);
	void print_cpp_reply_case(FILE* st);

	void print_type_reg_comment(FILE* st);
};

hc_term&	hfor(hc_term& the_cond, hc_term& the_end_each_loop);
hc_term&	hif(hc_term& o1);
hc_term&	helif(hc_term& o1);
hc_term&	hwhile(hc_term& o1);
hc_term&	hswitch(hc_term& o1);
hc_term&	hcase(hc_term& o1);
hc_term&	hdbg(const char* cod);
hc_term&	habort(const char* cod);

hc_term& hacquire(hc_term& att);
hc_term& hrelease(hc_term& att);
hc_term& hwait(hc_term& att);

#ifdef FULL_DEBUG
#	define HCK(prm) hdbg("PTD_CK(" prm ")")
#	define HCK_PRT(prm) hdbg("PTD_CK_PRT(" prm ")")
#	define HPRT(prm) hdbg("PTD_PRT(" prm ")")
#	define HLOG(prm) hdbg("PTD_LOG(" prm ")")
#else
#	define HCK(prm) hskip
#	define HCK_PRT(prm) hskip
#	define HPRT(prm) hskip
#	define HLOG(prm) hskip
#endif

bool	hdbg_txt_pre_hh(const char* cls, const char* cod);
bool	hdbg_txt_pos_hh(const char* cls, const char* cod);
bool	hdbg_txt_pre_cpp(const char* cls, const char* cod);
bool	hdbg_txt_pos_cpp(const char* cls, const char* cod);

#define hdbg_pre_hh(cls, cod) bool pre_hh_cod_ ## cls ## _ok = hdbg_txt_pre_hh(#cls, cod)
#define hdbg_pos_hh(cls, cod) bool pos_hh_cod_ ## cls ## _ok = hdbg_txt_pos_hh(#cls, cod)
#define hdbg_pre_cpp(cls, cod) bool pre_cpp_cod_ ## cls ## _ok = hdbg_txt_pre_cpp(#cls, cod)
#define hdbg_pos_cpp(cls, cod) bool pos_cpp_cod_ ## cls ## _ok = hdbg_txt_pos_cpp(#cls, cod)

class hc_global : public hc_term {
public:
	hl_string nam;
	hl_string val;
	
	virtual	~hc_global(){}
	
	hc_global(hl_string the_nam){
		nam = the_nam;
	}

	virtual 
	const char*	get_name(){
		return nam.c_str();
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, " %s ", get_name());
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st){
		fprintf(st, " %s ", get_name());
		return 0;
	}

	virtual 
	hl_string	get_value(){
		return val;
	}	
};

inline
void
ck_closed_param(hc_term* pm, hc_term* tm, bool& has_st){
	HL_CK(tm != hl_null);
	if(pm == hl_null){ return; }
	if(! pm->is_closed_member()){
		hl_abort("Parameter %s to %s not closed. Close it with '()'.\n", 
					pm->get_name(), tm->get_name());
	}
	has_st = (has_st || pm->has_statements());
}


class hc_unary_term : public hc_term {
public:
	bool has_st = false;
	hc_syntax_op_t op;
	hc_term* prm;
	
	virtual	~hc_unary_term(){
		if((prm != hl_null) && prm->is_compound()){
			delete prm;
		}
		prm = hl_null;
	}

	hc_unary_term(hc_syntax_op_t pm_op, hc_term* pm_val){
		init_unary_term(pm_op, pm_val);
	}

	void init_unary_term(hc_syntax_op_t pm_op, hc_term* pm_val)
	{
		HL_CK(pm_val != hl_null);
		has_st = false;
		op = pm_op;
		ck_closed_param(pm_val, this, has_st);
		prm = pm_val;
		num_steps = prm->num_steps;
		
		or_has_safe(prm);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return op;
	}
	
	virtual 
	const char*	get_name(){
		return hc_get_token(op);
	}
	
	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	void set_num_label(){
		HL_CK(prm != hl_null);
		prm->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(prm != hl_null);
		prm->get_safe_attributes(all_safe, owr);
	}
	
	virtual 
	bool	is_compound(){
		return true;
	}
	
	virtual 
	bool	has_statements(){
		return has_st;
	}
};

class hc_case_term : public hc_unary_term {
public:
	hc_term* the_sw_eq;
	
	virtual	~hc_case_term(){
		the_sw_eq = hl_null;
	}

	hc_case_term(hc_syntax_op_t pm_op, hc_term* pm_val) : hc_unary_term(pm_op, pm_val) {
		the_sw_eq = hl_null;
	}

	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);
	
	virtual 
	void	set_jumps(hc_syntax_op_t the_op, hc_term& nxt);
	
};

class hc_steps : public hc_term {
public:
	hc_term*	first_if = hl_null;
	std::list<hc_term*> steps;

	virtual	~hc_steps(){
		first_if = hl_null;
		while(! steps.empty()){
			hc_term* tm = steps.front();
			HL_CK(tm != hl_null);
			steps.pop_front();
			if(tm->is_compound()){
				delete tm;
			}
		}		
	}
	
	hc_steps(){
		first_if = hl_null;
	}
	
	virtual 
	hc_syntax_op_t	get_cond_oper(){
		HL_CK(! steps.empty());
		hc_term* tm = steps.back();
		return tm->get_cond_oper();
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_comma_op;
	}
	
	virtual 
	const char*	get_name(){
		return hc_get_token(get_oper());
	}
	
	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	void set_num_label();

	virtual 
	bool	is_compound(){
		return true;
	}
	
	virtual 
	bool	has_statements(){
		return true;
	}
	
	void append_term(hc_term& o1);
	
	virtual 
	hc_term* get_first_step();
	
	virtual 
	void	set_next(hc_term& nxt);
	
	virtual 
	void	set_last(hc_term& nxt);

	virtual 
	void	set_jumps(hc_syntax_op_t the_op, hc_term& nxt);
	
	void	set_back_next(hc_term& nxt);
	void	append_safe_check(hc_term& stp_tm);
};

class hc_condition : public hc_term {
public:
	hc_term* cond;
	hc_term* if_true;

	virtual	~hc_condition(){
		if((cond != hl_null) && cond->is_compound()){
			delete cond;
		}
		cond = hl_null;
		if((if_true != hl_null) && if_true->is_compound()){
			delete if_true;
		}
		if_true = hl_null;
	}
	
	hc_condition(hc_term* the_cond, hc_term* pm_if_true){
		init_condition(the_cond, pm_if_true);
	}
	
	void init_condition(hc_term* the_cond, hc_term* pm_if_true){
		HL_CK(the_cond != hl_null);
		HL_CK(pm_if_true != hl_null);
		bool has_st = false;
		ck_closed_param(the_cond, this, has_st);
		ck_closed_param(pm_if_true, this, has_st);
		
		cond = the_cond;
		if_true = pm_if_true;
		
		num_steps = cond->num_steps;
		num_steps += if_true->num_steps;

		or_has_safe(cond);
		or_has_safe(if_true);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_then_op;
	}
	
	virtual 
	hc_syntax_op_t	get_cond_oper(){
		HL_CK(cond != hl_null);
		return cond->get_oper();
	}
	
	virtual 
	const char*	get_name(){
		return hc_get_token(get_oper());
	}
	
	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	void set_num_label(){
		HL_CK(cond != hl_null);
		HL_CK(if_true != hl_null);
		cond->set_num_label();
		if_true->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(cond != hl_null);
		HL_CK(if_true != hl_null);
		cond->get_safe_attributes(all_safe, owr);
		if_true->get_safe_attributes(all_safe, owr);
	}

	virtual 
	bool	is_compound(){
		return true;
	}
	
	virtual 
	bool	has_statements(){
		return true;
	}
	
	bool can_add_to(hc_condition& cond);
	
	virtual 
	void	set_next(hc_term& nxt);
	
	virtual 
	void	set_last(hc_term& nxt);
	
	virtual 
	void	set_jumps(hc_syntax_op_t the_op, hc_term& nxt);
	
};

class hc_for_loop : public hc_term {
public:
	hc_condition* owner = hl_null;
	hc_term* cond;
	hc_term* end_each_loop;
	
	virtual	~hc_for_loop(){
		owner = hl_null;
		if((cond != hl_null) && cond->is_compound()){
			delete cond;
		}
		cond = hl_null;
		if((end_each_loop != hl_null) && end_each_loop->is_compound()){
			delete end_each_loop;
		}
		end_each_loop = hl_null;
	}
	
	hc_for_loop(hc_term* the_cond, hc_term* the_end_each_loop){
		owner = hl_null;
		
		HL_CK(the_cond != hl_null);
		HL_CK(the_end_each_loop != hl_null);
		bool has_st = false;
		ck_closed_param(the_cond, this, has_st);
		ck_closed_param(the_end_each_loop, this, has_st);
		cond = the_cond;
		end_each_loop = the_end_each_loop;
		
		num_steps = cond->num_steps;
		num_steps += end_each_loop->num_steps;
		
		or_has_safe(cond);
		or_has_safe(the_end_each_loop);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_hfor_op;
	}
	
	virtual 
	const char*	get_name(){
		return hc_get_token(get_oper());
	}
	
	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	void set_num_label(){
		HL_CK(cond != hl_null);
		HL_CK(end_each_loop != hl_null);
		cond->set_num_label();
		end_each_loop->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(cond != hl_null);
		HL_CK(end_each_loop != hl_null);
		cond->get_safe_attributes(all_safe, owr);
		end_each_loop->get_safe_attributes(all_safe, owr);
	}

	virtual 
	bool	is_compound(){
		return true;
	}
	
	virtual 
	bool	has_statements(){
		return true;
	}
};

class hc_wait_term : public hc_term {
public:
	hc_term* att;
	
	virtual	~hc_wait_term(){
		if((att != hl_null) && att->is_compound()){
			delete att;
		}
		att = hl_null;
	}
	
	hc_wait_term(hc_term* the_att){
		HL_CK(the_att != hl_null);
		bool has_st = false;
		ck_closed_param(the_att, this, has_st);
		att = the_att;
		
		num_steps = att->num_steps;
		
		or_has_safe(att);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_hwait_op;
	}
	
	virtual 
	const char*	get_name(){
		return hc_get_token(get_oper());
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, " %s(%s) ", get_name(), att->get_name());
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st){
		fprintf(st, " /* %s(%s) */", get_name(), att->get_name());
		return 0;
	}

	virtual 
	void set_num_label(){
		HL_CK(att != hl_null);
		att->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(att != hl_null);
		att->get_safe_attributes(all_safe, owr);
	}

	virtual 
	bool	is_compound(){
		return true;
	}
	
	virtual 
	bool	has_statements(){
		return true;
	}
};

class hc_binary_term : public hc_term {
public:
	bool has_st = false;
	hc_syntax_op_t op;
	hc_term* lft;
	hc_term* rgt;
	
	virtual	~hc_binary_term(){
		if((lft != hl_null) && lft->is_compound()){
			delete lft;
		}
		lft = hl_null;
		if((rgt != hl_null) && rgt->is_compound()){
			delete rgt;
		}
		rgt = hl_null;
	}
	
	hc_binary_term(hc_syntax_op_t pm_op, hc_term* pm_lft, hc_term* pm_rgt){
		init_binary_term(pm_op, pm_lft, pm_rgt);
	}
	
	void init_binary_term(hc_syntax_op_t pm_op = hc_invalid_op, 
						  hc_term* pm_lft = hl_null, hc_term* pm_rgt = hl_null)
	{
		//HL_CK(lft != hl_null);
		//HL_CK(rgt != hl_null);
		
		has_st = false;
		op = pm_op;
		ck_closed_param(pm_lft, this, has_st);
		ck_closed_param(pm_rgt, this, has_st);
		lft = pm_lft;
		rgt = pm_rgt;
		
		num_steps = 0;
		if(lft != hl_null){ num_steps += lft->num_steps; }
		if(rgt != hl_null){ num_steps += rgt->num_steps; }
		
		or_has_safe(lft);
		or_has_safe(rgt);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return op;
	}
	
	virtual 
	const char*	get_name(){
		return hc_get_token(op);
	}
	
	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	void set_num_label(){
		HL_CK(lft != hl_null);
		HL_CK(rgt != hl_null);
		lft->set_num_label();
		rgt->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(lft != hl_null);
		HL_CK(rgt != hl_null);
		lft->get_safe_attributes(all_safe, owr);
		rgt->get_safe_attributes(all_safe, owr);
	}
	
	virtual 
	bool	is_compound(){
		return true;
	}
	
	virtual 
	bool	has_statements(){
		return has_st;
	}
};

class hc_send_term : public hc_term {
public:
	hc_syntax_op_t op;
	hc_term* snd_dst;
	hc_term* snd_tok;
	hc_term* snd_att;
	hc_global* snd_req_id;
	
	virtual	~hc_send_term(){
		if((snd_dst != hl_null) && snd_dst->is_compound()){
			delete snd_dst;
		}
		snd_dst = hl_null;
		if((snd_tok != hl_null) && snd_tok->is_compound()){
			delete snd_tok;
		}
		snd_tok = hl_null;
		if((snd_att != hl_null) && snd_att->is_compound()){
			delete snd_att;
		}
		snd_att = hl_null;
		snd_req_id = hl_null;
	}
	
	hc_send_term(hc_syntax_op_t pm_op, hc_term* pm_dst, hc_term* pm_tok, hc_term* pm_att,
		hc_global* pm_idx = hl_null)
	{
		init_send_term(pm_op, pm_dst, pm_tok, pm_att, pm_idx);
	}
	
	void init_send_term(hc_syntax_op_t pm_op = hc_invalid_op, 
						hc_term* pm_dst = hl_null, hc_term* pm_tok = hl_null,
						hc_term* pm_att = hl_null, hc_global* pm_idx = hl_null)
	{
		HL_CK(pm_dst != hl_null);
		HL_CK(pm_tok != hl_null);
		HL_CK(pm_att != hl_null);
		
		bool has_st = false;
		op = pm_op;
		ck_closed_param(pm_dst, this, has_st);
		ck_closed_param(pm_tok, this, has_st);
		ck_closed_param(pm_att, this, has_st);
		snd_dst = pm_dst;
		snd_tok = pm_tok;
		snd_att = pm_att;
		snd_req_id = pm_idx;
		
		num_steps = snd_dst->num_steps;
		num_steps += snd_tok->num_steps;
		num_steps += snd_att->num_steps;
		
		or_has_safe(snd_dst);
		or_has_safe(snd_tok);
		or_has_safe(snd_att);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return op;
	}
	
	virtual 
	const char*	get_name(){
		return hc_get_token(op);
	}
	
	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	void set_num_label(){
		HL_CK(snd_dst != hl_null);
		HL_CK(snd_tok != hl_null);
		HL_CK(snd_att != hl_null);
		snd_dst->set_num_label();
		snd_tok->set_num_label();
		snd_att->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(snd_dst != hl_null);
		HL_CK(snd_tok != hl_null);
		HL_CK(snd_att != hl_null);
		snd_dst->get_safe_attributes(all_safe, owr);
		snd_tok->get_safe_attributes(all_safe, owr);
		snd_att->get_safe_attributes(all_safe, owr);
	}
	
	virtual 
	bool	is_compound(){
		return true;
	}
};

class hc_mem_oper_term : public hc_term {
public:
	hc_syntax_op_t op;
	hc_term* nw_att;
	
	virtual	~hc_mem_oper_term(){
		if((nw_att != hl_null) && nw_att->is_compound()){
			delete nw_att;
		}
		nw_att = hl_null;
	}
	
	hc_mem_oper_term(hc_syntax_op_t pm_op, hc_term* pm_att)
	{
		HL_CK(pm_att != hl_null);
		
		op = pm_op;
		
		bool has_st = false;
		ck_closed_param(pm_att, this, has_st);
		nw_att = pm_att;
		
		num_steps += nw_att->num_steps;
		
		or_has_safe(nw_att);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return op;
	}
	
	virtual 
	const char*	get_name(){
		return hc_get_token(op);
	}
	
	virtual 
	int print_term(FILE *st);

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	void set_num_label(){
		HL_CK(nw_att != hl_null);
		nw_att->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(nw_att != hl_null);
		nw_att->get_safe_attributes(all_safe, owr);
	}
	
	virtual 
	bool	is_compound(){
		return true;
	}
};

template<class obj_t>
class hc_value : public hc_term {
public:
	hc_global* my_id;
	hl_safe_idx_t safe_idx;
	const char* typ;
	const char* nam;
	obj_t	val;
	hcell*	owner;
	
	virtual	~hc_value(){}

	//		 hcell* the_owner = hl_null, obj_t the_val = obj_t())
	
	hc_value(const char* the_typ, const char* the_nam, hc_global* the_id = hl_null, 
			 hcell* the_owner = hl_null, bool safe_vl = false)
	{
		HL_CK(the_owner != hl_null);
		HL_CK(! std::is_pointer<obj_t>::value);
		HL_CK(! std::is_class<obj_t>::value);
		my_id = the_id;
		if(safe_vl && (the_owner != hl_null)){
			set_has_safe();
		}
		safe_idx = HL_INVALID_SAFE_IDX;
		typ = the_typ;
		nam = the_nam;
		val = obj_t();
		
		owner = the_owner;
		HLANG_SYS().register_value(owner, this);
	}
	
	hc_value(hc_value<obj_t>& other){ 
		HC_OBJECT_COPY_ERROR; 
	}

	hc_value(hc_term& other){ 
		HC_OBJECT_COPY_ERROR; 
	}
	
	hc_term&	operator = (hc_term& o1){
		HC_DEFINE_ASSIG_BASE(hc_assig_op2);
	}

	hc_term&	operator = (hc_value<obj_t>& o1) { 
		HC_DEFINE_ASSIG_BASE(hc_assig_op3);
	}

	//void operator ()(obj_t aa) { val = aa; }	// as unary func. used as init func
	hc_term& operator ()() { return *get_src(); }	// as empty func
	
	void  operator = (obj_t aa) { 
		val = aa;
	}

	//operator obj_t() { return ref; }		// cast op
	 
	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_value_op;
	}
	
	virtual 
	const char*	get_type(){
		return typ;
	}	

	virtual 
	hl_string	get_cpp_casted_tmp_msv_val(){
		hl_string tt(typ);
		hl_string vv = "((" + tt + ")hg_tmp_val)";
		return vv;
	}	

	virtual 
	const char*	get_name(){
		return nam;
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		if(get_has_safe()){
			fprintf(st, "<%d>", safe_idx);
		}
		fprintf(st, " %s ", get_name());
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st){
		fprintf(st, " %s ", get_name());
		return 0;
	}

	virtual 
	hc_term*	get_src();

	virtual
	void	set_safe_idx(hl_safe_idx_t idx){
		HL_CK(get_has_safe());
		if(idx > HL_MAX_SAFE_IDX){
			hl_abort("Error in attribute %s. Cannot have more that %d safe attributes per class \n", 
					 get_name(), HL_MAX_SAFE_IDX);
		}
		HL_CK(safe_idx == HL_INVALID_SAFE_IDX);
		safe_idx = idx;
		HL_CK(safe_idx > HL_INVALID_SAFE_IDX);
	}

	virtual 
	hl_safe_idx_t get_safe_idx(){ return safe_idx; }

	virtual 
	hl_safe_bits_t get_safe_bit_mask(){
		hl_safe_bits_t msk = 0;
		if(safe_idx != HL_INVALID_SAFE_IDX){
			hl_set_bit(&msk, safe_idx);
		}
		return msk;
	}
	
	virtual 
	bool 	is_safe_attribute(){ return get_has_safe(); }

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		if(safe_idx != HL_INVALID_SAFE_IDX){
			if(owr == hl_null){
				HL_CK(owner != hl_null);
				owr = owner;
			}
			hl_set_bit(&all_safe, safe_idx);
		}
	}

	virtual 
	hl_string	get_id(){
		if(my_id == hl_null){
			return "INVALID_ID";
		}
		return my_id->nam;
	}	
	
};

class hc_keyword : public hc_term {
public:
	hc_syntax_op_t op;
	
	virtual	~hc_keyword(){}
	
	hc_keyword(hc_syntax_op_t the_op){
		op = the_op;
	}

	virtual 
	const char*	get_name(){
		return hc_get_token(op);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return op;
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, " %s ", get_name());
		return 0;
	}
	
	virtual 
	int print_cpp_term(FILE *st){
		if(op == hc_hfinished_op){
			fprintf(st, R"finish(PTD_ABORT("NORMAL_ABORTING_with_hfinished\n");)finish");
		} else {
			fprintf(st, " %s ", hc_get_cpp_token(op));
		}
		return 0;
	}

	virtual 
	bool	is_compound(){
		return true;
	}

	virtual 
	void	set_jumps(hc_syntax_op_t the_op, hc_term& nxt);
	
};

class hc_cast : public hc_term {
public:
	const char* cast_str;
	hc_term* the_tm;
	
	virtual	~hc_cast(){}
	
	hc_cast(const char* the_cast, hc_term& tm){
		HL_CK(the_cast != hl_null);
		cast_str = the_cast;
		the_tm = tm.get_src();
		
		or_has_safe(the_tm);
	}

	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		HL_CK(cast_str != hl_null);
		fprintf(st, "(cast(%s)", cast_str);
		the_tm->print_term(st);
		fprintf(st, ")");
		return 0;
	}
	
	virtual 
	int print_cpp_term(FILE *st){
		HL_CK(cast_str != hl_null);
		fprintf(st, "((%s)", cast_str);
		the_tm->print_cpp_term(st);
		fprintf(st, ")");
		return 0;
	}

	virtual 
	void set_num_label(){
		HL_CK(the_tm != hl_null);
		the_tm->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(the_tm != hl_null);
		the_tm->get_safe_attributes(all_safe, owr);
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		HL_CK(the_tm != hl_null);
		return the_tm->get_oper();
	}
	
	virtual 
	hc_syntax_op_t	get_cond_oper(){
		HL_CK(the_tm != hl_null);
		return the_tm->get_cond_oper();
	}
	
	virtual 
	const char*	get_type(){
		HL_CK(the_tm != hl_null);
		return the_tm->get_type();
	}	

	virtual 
	const char*	get_name(){
		HL_CK(the_tm != hl_null);
		return the_tm->get_name();
	}	

	virtual 
	hl_string	get_value(){
		HL_CK(the_tm != hl_null);
		return the_tm->get_value();
	}	

	virtual 
	bool	is_compound(){
		return true;
	}

	virtual 
	bool	has_statements(){
		HL_CK(the_tm != hl_null);
		return the_tm->has_statements();
	}
	
	virtual 
	hc_term* get_first_step(){
		HL_CK(the_tm != hl_null);
		return the_tm->get_first_step();
	}

	virtual 
	void	set_next(hc_term& nxt){
		HL_CK(the_tm != hl_null);
		return the_tm->set_next(nxt);
	}

	virtual 
	void	set_last(hc_term& nxt){
		HL_CK(the_tm != hl_null);
		return the_tm->set_last(nxt);
	}
	
	virtual 
	void	set_jumps(hc_syntax_op_t the_op, hc_term& nxt){
		HL_CK(the_tm != hl_null);
		return the_tm->set_jumps(the_op, nxt);
	}
	
};

class hc_literal : public hc_term {
public:
	const char* lit_str;
	
	virtual	~hc_literal(){}
	
	hc_literal(const char* the_lit){
		HL_CK(the_lit != hl_null);
		lit_str = the_lit;
	}

	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		HL_CK(lit_str != hl_null);
		fprintf(st, "(lit %s)", lit_str);
		return 0;
	}
	
	virtual 
	int print_cpp_term(FILE *st){
		HL_CK(lit_str != hl_null);
		fprintf(st, " %s ", lit_str);
		return 0;
	}

	virtual 
	bool	is_compound(){
		return true;
	}
};

class hc_safe_check : public hc_term {
public:
	hl_safe_bits_t safe_pattern;
	hcell* owner;
	
	virtual	~hc_safe_check(){}
	
	hc_safe_check(hl_safe_bits_t all_safe, hc_term& nxt, hcell* owr){
		HL_CK(owr != hl_null);
		safe_pattern = all_safe;
		next = &nxt;
		owner = owr;
	}

	virtual 
	const char*	get_name(){
		return "hc_safe_check";
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, "safe_ck(%#lx)", safe_pattern);
		return 0;
	}
	
	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_safe_check_op;
	}
	
	virtual 
	hc_syntax_op_t	get_cond_oper(){
		HL_CK(next != hl_null);
		return next->get_cond_oper();
	}
	
	virtual 
	void	set_next(hc_term& nxt);

	virtual 
	void	set_last(hc_term& nxt);
	
	virtual 
	bool	is_compound(){
		return true;
	}
};

template<class obj_t>
class hc_reference : public hc_term {
public:
	static obj_t*	HC_GLB_REF;
	
	hc_global* my_id;
	hl_safe_idx_t safe_idx;
	bool has_cast;
	const char* typ;
	const char* nam;
	obj_t*	ref;
	hcell*	owner;
	
	virtual	~hc_reference(){}
	
	hc_reference(const char* the_typ, const char* the_nam, hc_global* the_id = hl_null, 
				 hcell* the_owner = hl_null, bool with_cast = false, bool rf_safe = false){
		HL_CK(! std::is_pointer<obj_t>::value);
		HL_CK(std::is_class<obj_t>::value);
		//HL_CK((std::is_base_of<hcell, obj_t>::value));
		my_id = the_id;
		
		//bool is_sf = HLANG_SYS().is_missive_class(the_typ) || rf_safe;
		bool is_sf = rf_safe;
		if(is_sf && (the_owner != hl_null)){
			set_has_safe();
		}
		safe_idx = HL_INVALID_SAFE_IDX;
		has_cast = with_cast;
		typ = the_typ;
		nam = the_nam;
		ref = hl_null;
		owner = the_owner;
		if(owner != hl_null){
			HLANG_SYS().register_reference(owner, this);
		} else {
			HLANG_SYS().register_address(this);
		}
	}
	
	hc_reference(hc_reference<obj_t>& other){ 
		HC_OBJECT_COPY_ERROR; 
	}

	hc_reference(hc_term& other){ 
		HC_OBJECT_COPY_ERROR; 
	}
	
	hc_term&	operator = (hc_term& o1){
		HC_DEFINE_ASSIG_BASE(hc_assig_op4);
	}

	hc_term&  operator = (hc_reference<obj_t>& o1) { 
		HC_DEFINE_ASSIG_BASE(hc_assig_op5);
	}

	void operator ()(obj_t* aa) { // as unary func. used as init func
		ref = aa; 
	}	
	
	hc_term& operator ()() { return *get_src(); }	// as empty func. top member calls with this.
	
	void  operator = (obj_t* aa) { 
		ref = aa;
	}
	
	static
	obj_t* get_glb_ref(){
		if(HC_GLB_REF == hl_null){
			HC_GLB_REF = new obj_t();
		}
		return HC_GLB_REF;
	}
	
	obj_t const *	operator -> () const {
		if(ref == hl_null){
			ref = get_glb_ref();
		}
		if(ref->rf_src_tm != hl_null){ 
			hl_abort("\nCannot have recursive referencing in hlang.\n"); 
		}
		ref->rf_src_tm = this;
		return ref;
	}
	
    obj_t*	operator -> () {
		if(ref == hl_null){
			ref = get_glb_ref();
		}
		if(ref->rf_src_tm != hl_null){ 
			hl_abort("\nCannot have recursive referencing in hlang.\n"); 
		}
		ref->rf_src_tm = this;
		return ref;
	}
	
	bool is_null(){
		return (ref == hl_null);
	}

	//operator obj_t() { return ref; }		// cast op
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_reference_op;
	}
	
	virtual 
	const char*	get_type(){
		return typ;
	}	

	virtual 
	hl_string	get_cpp_casted_tmp_msv_val(){
		hl_string tt(typ);
		hl_string vv = "((" + tt + "*)hg_tmp_ref)";
		return vv;
	}	

	virtual 
	const char*	get_name(){
		return nam;
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		HL_CK(typ != hl_null);
		if(has_cast){
			fprintf(st, "((%s*)", typ);
		}
		if(get_has_safe()){
			fprintf(st, "<%d>", safe_idx);
		}
		fprintf(st, " %s ", get_name());
		if(has_cast){
			fprintf(st, ")");
		}
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st){
		HL_CK(typ != hl_null);
		if(has_cast){
			fprintf(st, "((%s*)", typ);
		}
		fprintf(st, " %s ", get_name());
		if(has_cast){
			fprintf(st, ")");
		}
		return 0;
	}

	virtual 
	hc_term*	get_src();

	virtual
	void	set_safe_idx(hl_safe_idx_t idx){
		HL_CK(get_has_safe());
		if(idx > HL_MAX_SAFE_IDX){
			hl_abort("Error in attribute %s. Cannot have more that %d safe attributes per class \n", 
					 get_name(), HL_MAX_SAFE_IDX);
		}
		HL_CK(safe_idx == HL_INVALID_SAFE_IDX);
		safe_idx = idx;
		HL_CK(safe_idx > HL_INVALID_SAFE_IDX);
	}
	
	virtual 
	hl_safe_idx_t get_safe_idx(){ return safe_idx; }
	
	virtual 
	hl_safe_bits_t get_safe_bit_mask(){
		hl_safe_bits_t msk = 0;
		if(safe_idx != HL_INVALID_SAFE_IDX){
			hl_set_bit(&msk, safe_idx);
		}
		return msk;
	}
	
	virtual 
	bool 	is_safe_attribute(){ return get_has_safe(); }

	virtual 
	bool 	is_message_reference(){ 
		const char* typ = get_type();
		hclass_reg* rg = HLANG_SYS().get_class_reg(typ);
		return (! rg->with_methods); 
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		if(safe_idx != HL_INVALID_SAFE_IDX){
			if(owr == hl_null){
				HL_CK(owner != hl_null);
				owr = owner;
			}
			hl_set_bit(&all_safe, safe_idx);
		}
	}

	virtual 
	hl_string	get_id(){
		if(my_id == hl_null){
			return "INVALID_ID";
		}
		return my_id->nam;
	}	
	
};

template<class obj_t>
obj_t* hc_reference<obj_t>::HC_GLB_REF = hl_null;

inline
hc_term&
hc_new_cast(const char* cst_str, hc_term& o1){
	hc_cast* tm = new hc_cast(cst_str, o1);
	return *tm;
}

inline
hc_term&
hc_new_literal(const char* the_lit){
	hc_literal* tm = new hc_literal(the_lit);
	return *tm;
}

#define hcast(typ, tm) hc_new_cast(#typ, (tm))

#define hlit(the_lit) hc_new_literal(#the_lit)

#define hdefine_const(nn, vv) hc_term& nn = HLANG_SYS().add_con(#nn, #vv)

#define hdeclare_const(nn) extern hc_term& nn

#define hdefine_token(nn) hc_term& nn = HLANG_SYS().add_tok(#nn)

#define hdeclare_token(nn) extern hc_term& nn

#define hatt_id(nn) hid_ ## nn

#define hdef_att_id(nn, obj) hc_global& hatt_id(nn) = HLANG_SYS().add_att(obj->get_attr_nm("hid_", #nn))

#define hvalue(tt, nn) hdef_att_id(nn, this); \
	hc_value<tt> nn{#tt, #nn, &(hatt_id(nn)), this}

#define hsafe_value(tt, nn) hdef_att_id(nn, this); \
	hc_value<tt> nn{#tt, #nn, &(hatt_id(nn)), this, true}

#define hreference(tt, nn) hdef_att_id(nn, this); \
	hc_reference<tt> nn{#tt, #nn, &(hatt_id(nn)), this}

#define hsafe_reference(tt, nn) hdef_att_id(nn, this); \
	hc_reference<tt> nn{#tt, #nn, &(hatt_id(nn)), this, false, true}

	
#define haddress(tt, nn) hc_reference<tt> nn{#tt, #nn}

#define hatt(nn) HLANG_SYS().get_att(#nn)
#define htok(nn) HLANG_SYS().get_tok(#nn)
#define hcon(nn) HLANG_SYS().get_con(#nn)

#define hbool(nn) hvalue(bool, nn)
#define hchar(nn) hvalue(char, nn)
#define hint(nn) hvalue(int, nn)
#define hlong(nn) hvalue(long, nn)
#define hint8_t(nn) hvalue(int8_t, nn)
#define hint16_t(nn) hvalue(int16_t, nn)
#define hint32_t(nn) hvalue(int32_t, nn)
#define hint64_t(nn) hvalue(int64_t, nn)
#define huint8_t(nn) hvalue(uint8_t, nn)
#define huint16_t(nn) hvalue(uint16_t, nn)
#define huint32_t(nn) hvalue(uint32_t, nn)
#define huint64_t(nn) hvalue(uint64_t, nn)

#define hsafe_bool(nn) hsafe_value(bool, nn)
#define hsafe_char(nn) hsafe_value(char, nn)
#define hsafe_int(nn) hsafe_value(int, nn)
#define hsafe_long(nn) hsafe_value(long, nn)
#define hsafe_int8_t(nn) hsafe_value(int8_t, nn)
#define hsafe_int16_t(nn) hsafe_value(int16_t, nn)
#define hsafe_int32_t(nn) hsafe_value(int32_t, nn)
#define hsafe_int64_t(nn) hsafe_value(int64_t, nn)
#define hsafe_uint8_t(nn) hsafe_value(uint8_t, nn)
#define hsafe_uint16_t(nn) hsafe_value(uint16_t, nn)
#define hsafe_uint32_t(nn) hsafe_value(uint32_t, nn)
#define hsafe_uint64_t(nn) hsafe_value(uint64_t, nn)


#define hl_new_keyword(nn) (*(new hc_keyword(hc_ ## nn ## _op)))

#define hdefault 	hl_new_keyword(hdefault)
#define helse 		hl_new_keyword(helse)
#define hbreak 		hl_new_keyword(hbreak)
#define hcontinue 	hl_new_keyword(hcontinue)
#define hreturn 	hl_new_keyword(hreturn)
#define hfinished	hl_new_keyword(hfinished)

#define hthis	 	hl_new_keyword(hthis)
#define hnull	 	hl_new_keyword(hnull)

#define hskip	 	hl_new_keyword(hskip)

#define hmsg_src 	hl_new_keyword(hmsg_src)
#define hmsg_ref 	hl_new_keyword(hmsg_ref)
#define hmsg_tok 	hl_new_keyword(hmsg_tok)
#define hmsg_val 	hl_new_keyword(hmsg_val)

class hc_mth_ret : public hc_term {
public:
	hl_string nam;
	hc_mth_def* the_mth = hl_null;
	
	virtual	~hc_mth_ret(){}
	
	hc_mth_ret(hc_mth_def* mthdef);
	
	virtual 
	const char*	get_name();
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, "%s", get_name());
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_mth_ret_op;
	}
	
	virtual 
	bool	is_compound(){
		return true;
	}
};

class hc_mth_def : public hc_term {
public:
	const char* cls = hl_null;
	const char* nam = hl_null;
	hc_term* cod = hl_null;
	bool defining = false;
	hc_caller_t caller = hl_null;
	std::list<hc_mth_def*> calls;
	hc_mth_ret* ret = hl_null;
	hclass_reg* my_cls = hl_null;
	
	virtual	~hc_mth_def(){
		if(ret != hl_null){
			delete ret;
		}
		ret = hl_null;
	}
	
	hc_mth_def(const char* the_cls, const char* the_nam, 
			   hc_caller_t the_cllr, bool is_nucl = false, bool can_register = false)
	{
		cls = the_cls;
		nam = the_nam;
		caller = the_cllr;
		
		ret = new hc_mth_ret(this);
		my_cls = hl_null;
		
		HLANG_SYS().register_method(the_cls, this, is_nucl, can_register);
	}
	
	long 	calc_depth();

	virtual 
	const char*	get_name(){
		return nam;
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, " %s() ", get_name());
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st){
		fprintf(st, " %s() ", get_name());
		return 0;
	}

	virtual 
	void set_num_label(){
		HL_CK(cod != hl_null);
		cod->set_num_label();
	}

	virtual 
	void get_safe_attributes(hl_safe_bits_t& all_safe, hcell*& owr){
		HL_CK(cod != hl_null);
		cod->get_safe_attributes(all_safe, owr);
	}
	
	virtual 
	void print_text_code(FILE *st){
		fprintf(st, "CODE_FOR %s\n---------------\n", get_name());
		if(cod == hl_null){
			fprintf(st, "EMPTY_COD for mth\n");
		} else {
			cod->print_term(st);
			fprintf(st, "\n");
		}
	}
	
	virtual 
	void print_cpp_code(FILE *st){
		fprintf(st, "\n// CODE_FOR %s\n//--------------- \n", get_name());
		if(cod == hl_null){
			fprintf(st, "/* EMPTY_COD for mth */\n");
		} else {
			cod->print_cpp_term(st);
			fprintf(st, "\n");
		}
	}
	
	bool	is_nucleus(){
		HL_CK(my_cls != hl_null);
		return (my_cls->nucleus == this);
	}

	virtual 
	hc_term* get_first_step(){
		HL_CK(! is_nucleus());
		HL_CK(cod != hl_null);
		hc_term* tm = cod->get_first_step();
		return tm;
	}
	
	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_mth_def_op;
	}
	
	long get_cod_num_label(){
		if(is_nucleus()){
			hc_term* tm = cod->get_first_step();
			long num_lb = tm->label_number;
			HL_CK(num_lb != 0);
			return num_lb;
		}
		return get_num_label();
	}
	
	void print_hh_step_id(FILE *st);
};

class hc_mth_call : public hc_term {
public:
	hc_mth_def* the_mth = hl_null;
	
	virtual	~hc_mth_call(){}
	
	hc_mth_call(hc_mth_def* mthdef){
		HL_CK(mthdef != hl_null);
		the_mth = mthdef;
	}

	virtual 
	const char*	get_name(){
		HL_CK(the_mth != hl_null);
		return the_mth->get_name();
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, " %s() ", get_name());
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st);

	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_mth_call_op;
	}
	
	virtual 
	bool	is_compound(){
		return true;
	}
};


#define hmethod(mth) \
	static hc_mth_def mth ## _def_reg; \
	hc_term& mth(); \

// end_define

#define hnucleus(mth) hmethod(mth)


#define hmethod_def_base(cls, mth, code, is_nucl) \
	void cls ## _ ## mth ## _ ## caller(); \
	hc_mth_def cls::mth ## _def_reg \
		{#cls, #mth, cls ## _ ## mth ## _ ## caller, is_nucl, cls::hc_can_have_methods}; \
	hc_term& \
	cls::mth(){ \
		hc_mth_call* mc = new hc_mth_call(&cls::mth ## _def_reg); \
		hc_term* rr = mc; \
		if(rf_src_tm != hl_null){ \
			hc_term* tmp = rf_src_tm; \
			rf_src_tm = hl_null; \
			tmp = tmp->get_src(); \
			rr = new hc_binary_term(hc_member_op, tmp, mc); \
		} \
		if((cls::mth ## _def_reg.cod == hl_null) && (cls::mth ## _def_reg.defining)){ \
			fprintf(stdout, "CALLING_METHOD\n%s\n", __PRETTY_FUNCTION__); \
			hc_term& the_code = (code); \
			hc_steps* sts_cod = the_code.get_src()->to_steps(); \
			hc_mth_ret* ret_tm = cls::mth ## _def_reg.ret; \
			sts_cod->append_term(*ret_tm); \
			sts_cod->set_jumps(hc_hreturn_op, *ret_tm); \
			cls::mth ## _def_reg.cod = sts_cod; \
			cls::mth ## _def_reg.num_steps = cls::mth ## _def_reg.cod->num_steps + 1; \
		} \
		if(! cls::mth ## _def_reg.defining){ \
			HL_CK(hc_term::HC_CURRENT_DEFINING_METHOD != hl_null); \
			hc_term::HC_CURRENT_DEFINING_METHOD->calls.push_back(&cls::mth ## _def_reg); \
		} \
		return *rr; \
	} \
	void cls ## _ ## mth ## _ ## caller(){ \
		cls* obj = hc_reference<cls>::get_glb_ref(); \
		HL_CK(hc_term::HC_CURRENT_DEFINING_METHOD == hl_null); \
		hc_term::HC_CURRENT_DEFINING_METHOD = &cls::mth ## _def_reg; \
		cls::mth ## _def_reg.defining = true; \
		obj->cls::mth(); \
		cls::mth ## _def_reg.defining = false; \
		hc_term::HC_CURRENT_DEFINING_METHOD = hl_null; \
		HL_CK(hc_term::HC_CURRENT_DEFINING_METHOD == hl_null); \
	} \

// end_define

#define hmethod_def(cls, mth, code) hmethod_def_base(cls, mth, code, false)

#define hnucleus_def(cls, mth, code) hmethod_def_base(cls, mth, code, true)

class hc_dbg : public hc_term {
public:
	hl_string dbg_cod;
	
	virtual	~hc_dbg(){}
	
	hc_dbg(hl_string the_cod){
		dbg_cod = the_cod;
	}

	virtual 
	const char*	get_name(){
		return "hc_dbg";
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, "hdbg{%s}", dbg_cod.c_str());
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st){
		fprintf(st, " %s ", dbg_cod.c_str());
		return 0;
	}

	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_dbg_op;
	}
	
	virtual 
	bool	is_compound(){
		return true;
	}
};

class hc_abort : public hc_term {
public:
	hl_string ab_msg;
	
	virtual	~hc_abort(){}
	
	hc_abort(hl_string the_msg){
		ab_msg = the_msg;
	}

	virtual 
	const char*	get_name(){
		return "hc_abort";
	}
	
	virtual 
	int print_term(FILE *st){
		HL_CK(st != hl_null);
		fprintf(st, "habort(%s)", ab_msg.c_str());
		return 0;
	}

	virtual 
	int print_cpp_term(FILE *st){
		fprintf(st, "PTD_ABORT(%s)", ab_msg.c_str());
		return 0;
	}

	virtual 
	hc_syntax_op_t	get_oper(){
		return hc_habort_op;
	}
	
	virtual 
	bool	is_compound(){
		return true;
	}
};

#define hme_def(cls) \
    virtual	hc_term&	hme(){ \
		if(hc_pt_me == hl_null){ \
			hc_reference<cls>* tmp = new hc_reference<cls>(#cls, "hme_" #cls, hl_null, this); \
			tmp->ref = this; \
			hc_pt_me = tmp; \
		} \
		return *hc_pt_me; \
	} \

// end_define


class hcell {
public:
	hc_term* 		rf_src_tm = hl_null;
	hc_term* 		hc_pt_me = hl_null;

	hcell(){
		init_me();
	}
	
	virtual	~hcell(){}

	virtual
	void init_me(int caller = 0){
		rf_src_tm = hl_null;
	}

	virtual
	const char* get_class_name(){
		return "hcell";
	}

	const char* get_attr_nm(const char* pfix, const char* sfix);
	
	hme_def(hcell);
	
	hc_term& hget(hc_term& dst, hc_global& idx, hc_term& att);
	hc_term& hset(hc_term& dst, hc_global& idx, hc_term& att);
	hc_term& hsend(hc_term& dst, hc_term& tok, hc_term& att);
	hc_term& hreply(hc_term& att);

};

#define hmissive_class(cls) \
	static bool hc_can_have_methods; \
	static const char* hc_class_name; \
	static hclass_reg* hc_register_ ## cls; \
	virtual	const char* get_class_name(); \

// end_define

#define hmissive_class_def(cls) \
	void cls ## _ref_initer(); \
	hclass_reg* cls::hc_register_ ## cls = HLANG_SYS().get_class_reg(#cls, cls ## _ref_initer); \
	bool cls::hc_can_have_methods = false; \
	const char* cls::hc_class_name = #cls; \
	const char* \
	cls::get_class_name(){ \
		return cls::hc_class_name; \
	} \
	void cls ## _ref_initer(){ \
		hc_reference<cls>::get_glb_ref(); \
	} \

// end_define


#define hcell_class(cls) \
	static bool hc_can_have_methods; \
	static const char* hc_class_name; \
	static hclass_reg* hc_register_ ## cls; \
	static hc_reference<cls>& get_msg_ref(); \
	static hc_reference<cls>& get_msg_src(); \
	virtual	const char* get_class_name(); \
	hme_def(cls); \

// end_define

#define hcell_class_def(cls) \
	void cls ## _ref_initer(); \
	hclass_reg* cls::hc_register_ ## cls = HLANG_SYS().get_class_reg(#cls, cls ## _ref_initer, true); \
	bool cls::hc_can_have_methods = true; \
	const char* cls::hc_class_name = #cls; \
	const char* \
	cls::get_class_name(){ \
		return cls::hc_class_name; \
	} \
	void cls ## _ref_initer(){ \
		hc_reference<cls>::get_glb_ref(); \
	} \
	hc_reference<cls>& cls::get_msg_ref(){ \
		static hc_reference<cls> gref{#cls, "hg_msg_ref", hl_null, hl_null, true}; \
		return gref; \
	} \
	hc_reference<cls>& cls::get_msg_src(){ \
		static hc_reference<cls> gref{#cls, "hg_msg_src", hl_null, hl_null, true}; \
		return gref; \
	} \

// end_define


#define HC_GET_SOURCE() \
	hc_term* src = this; \
	if((owner != hl_null) && (owner->rf_src_tm != hl_null)){ \
		hc_term* tmp = owner->rf_src_tm; \
		owner->rf_src_tm = hl_null; \
		tmp = tmp->get_src(); \
		src = new hc_binary_term(hc_member_op, tmp, this); \
	} \
	return src; \

// end_define

template<class obj_t>
hc_term*	
hc_value<obj_t>::get_src(){
	HC_GET_SOURCE();
}

template<class obj_t>
hc_term*	
hc_reference<obj_t>::get_src(){
	HC_GET_SOURCE();
}

#define hmsg_src_as(cls) (cls::get_msg_src())
#define hmsg_ref_as(cls) (cls::get_msg_ref())
#define hmsg_val_as(typ) hcast(typ, hmsg_val)
#define hmsg_tok_as(typ) hcast(typ, hmsg_tok)

hdeclare_token(htk_release);
hdeclare_token(htk_get);
hdeclare_token(htk_set);
hdeclare_token(htk_start);
hdeclare_token(htk_finished);

#endif		// HLANG_H


