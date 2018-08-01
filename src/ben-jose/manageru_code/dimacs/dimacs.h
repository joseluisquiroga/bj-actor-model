

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

dimacs.h

Declaration of functions to read and parse dimacs files.

--------------------------------------------------------------*/

#ifndef DIMACS_H
#define DIMACS_H

#include "bj_stream.h"
#include "print_macros.h"
#include "tools.h"
#include "top_exception.h"

class dima_dims;

#define DIMACS_DBG(prm)	DBG(prm)
//define DIMACS_CK(prm) 	DBG_CK(prm)
#define DIMACS_H_CK(prm) 	DBG_CK(prm)

#define MAX_DIMACS_HEADER_SZ 4096

//=================================================================
// print funtion declarations

DECLARE_PRINT_FUNCS(dima_dims)

//=================================================================
// funtion declarations

//void print_dimacs_of(bj_ostream& os, row<long>& all_lits, long num_cla, long num_var);

//=================================================================
// dimacs_exception

#define first_char(pt_ch_in) (*((char*)pt_ch_in))

typedef enum {
	dix_bad_eq_op,
	dix_bad_creat,
	dix_no_cnf_decl_1,
	dix_no_cnf_decl_2,
	dix_no_cnf_decl_3,
	dix_bad_num_cls,
	dix_bad_format,
	dix_zero_vars,
	dix_zero_cls,
	dix_bad_lit,
	dix_cls_too_long
} di_ex_cod_t;

class dimacs_exception : public top_exception{
public:
	char val;
	long line;
	long pt_pos;
	
	long num_decl_cls;
	long num_decl_vars;
	long num_read_cls;
	long bad_lit;
	
	dimacs_exception(long the_id, char vv = 0, long ll = -1, long pp = -1) : 
			top_exception(the_id)
	{
		val = vv;
		line = ll;
		pt_pos = pp;

		num_decl_cls = 0;
		num_decl_vars = 0;
		num_read_cls = 0;
		bad_lit = 0;
	}
};

//=================================================================
// dima_dims

class dima_dims {
public:
	long		dd_tot_ccls;
	long		dd_tot_vars;
	long		dd_tot_lits;
	long		dd_tot_twolits;

	dima_dims(){
		init_dima_dims();
	}

	~dima_dims(){
	}
	
	void	update_with_sz(long cls_sz){
		dd_tot_ccls++;
		dd_tot_lits += cls_sz;
		if(cls_sz == 2){ dd_tot_twolits++; }
	}
	
	static
	long	max_abs_of(row<long>& all_lits){
		long mm = 0;
		for(long aa = 0; aa < all_lits.size(); aa++){
			long vv = abs_long(all_lits[aa]);
			if(vv > mm){
				mm = vv;
			}
		}
		return mm;
	}

	void	init_dima_dims(long ini_val = 0){
		dd_tot_ccls = ini_val;
		dd_tot_vars = ini_val;
		dd_tot_lits = ini_val;
		dd_tot_twolits = ini_val;
	}

	bool 	is_dd_virgin(long ini_val = 0){
		bool c1 = (dd_tot_ccls == ini_val);
		bool c2 = (dd_tot_vars == ini_val);
		bool c3 = (dd_tot_lits == ini_val);
		bool c4 = (dd_tot_twolits == ini_val);

		bool vv = (c1 && c2 && c3 && c4);
		return vv;
	}

	bj_ostream&	print_dima_dims(bj_ostream& os, bool from_pt = false){
		os << "dd=[";
		os << " tc=" << dd_tot_ccls;
		os << " tv=" << dd_tot_vars;
		os << " tl=" << dd_tot_lits;
		os << " tt=" << dd_tot_twolits;
		os << "]";
		os.flush();
		return os;
	}
};

//=================================================================
// nid_bits

class nid_bits {
public:
	long		nb_set_count;
	bit_row		nb_pos;
	bit_row		nb_neg;

	nid_bits(long sz = 0){
		init_nid_bits(sz);
	}

	~nid_bits(){
	}

	void	clear(){
		nb_set_count = 0;
		nb_pos.clear();
		nb_neg.clear();
	}

	void	init_nid_bits(long sz = 0){
		nb_set_count = 0;
		nb_pos.fill(false, sz, false);
		nb_neg.fill(false, sz, false);
	}

	void	append_nid_bits(long n_sz){
		nb_pos.fill(false, n_sz, true);
		nb_neg.fill(false, n_sz, true);
	}

	void	inc_nid_bits(){
		DIMACS_H_CK(nb_set_count == 0);
		DIMACS_H_CK(nb_pos.size() == nb_neg.size());

		long n_sz = nb_pos.size() + 1;
		nb_pos.fill(false, n_sz, true);
		nb_neg.fill(false, n_sz, true);
	}

	long	size(){
		DIMACS_H_CK(nb_pos.size() == nb_neg.size());
		return nb_pos.size();
	}

	bool	is_true(long nid){
		DIMACS_H_CK(nb_pos.is_valid_idx(abs_long(nid)));
		DIMACS_H_CK(nb_neg.is_valid_idx(abs_long(nid)));

		bool resp = false;
		if(nid > 0){
			resp = nb_pos[nid];
		} else {
			resp = nb_neg[-nid];
		}
		return resp;
	}

	void	reset(long nid){
		DIMACS_H_CK(is_true(nid));
		DIMACS_H_CK(! is_true(-nid));
		if(nid > 0){
			nb_pos[nid] = false;
		} else {
			nb_neg[-nid] = false;
		}
		nb_set_count--;
	}

	void	set(long nid){
		DIMACS_H_CK(! is_true(nid));
		DIMACS_H_CK(! is_true(-nid));
		if(nid > 0){
			nb_pos[nid] = true;
		} else {
			nb_neg[-nid] = true;
		}
		nb_set_count++;
	}

	bool	any_true(long nid){
		return (is_true(nid) || is_true(-nid));
	}
};

inline
void
reset_all(nid_bits& bts, row<long>& rr_all, long first_ii = 0, long last_ii = -1){
	long the_sz = rr_all.size();
	if((last_ii < 0) || (last_ii > the_sz)){
		last_ii = the_sz;
	}
	if((first_ii < 0) || (first_ii > last_ii)){
		first_ii = 0;
	}
	for(long aa = first_ii; aa < last_ii; aa++){
		long nid = rr_all[aa];
		bts.reset(nid);
	}
}

inline
void
set_all(nid_bits& bts, row<long>& rr_all, long first_ii = 0, long last_ii = -1){
	long the_sz = rr_all.size();
	if((last_ii < 0) || (last_ii > the_sz)){
		last_ii = the_sz;
	}
	if((first_ii < 0) || (first_ii > last_ii)){
		first_ii = 0;
	}
	for(long aa = first_ii; aa < last_ii; aa++){
		long nid = rr_all[aa];
		bts.set(nid);
	}
}

//=================================================================
// dimacs_loader

enum added_var_t {
	av_orlit,
	av_andlit,
	av_last
};

enum fix_kind {
	fx_lit_already_in_clause,
	fx_clause_has_both_lit,
	fx_empty_clause,
	fx_clause_has_one_lit,
	k_last_fix
};

#define RANDOM_INIT	false
#define FIRST_LIT_IDX	1

#define EMPTY_CNF_COMMENT	"c the cnf is empty\n"

class dimacs_loader {
private:
	dimacs_loader&  operator = (dimacs_loader& other){
		throw dimacs_exception(dix_bad_eq_op);
	}

	dimacs_loader(dimacs_loader& other){ 
		throw dimacs_exception(dix_bad_creat);
	}
	
public:
	typedef std::istream::pos_type ld_pos_t;

	ch_string		ld_file_name;
	
	bj_ostr_stream	ld_err_msg;

	bool			ld_allow_empty_cnfs;
	
	bool			ld_as_3cnf;

	long			ld_max_in_ccl_sz;

	long			ld_num_line;
	row<char>		ld_content;
	const char*		ld_cursor;

	long			ld_decl_ccls;
	long			ld_decl_vars;

	long			ld_parsed_ccls;
	long			ld_parsed_lits;
	long			ld_parsed_twolits;

	// ---------------------------------------

	nid_bits		ld_dots;

	row<integer>	ld_fixes;

	row_long_t 		ld_lits1;
	row_long_t 		ld_lits2;

	row_row_long_t 		ld_rr_lits1;
	row_row_long_t 		ld_rr_lits2;

	long			ld_last_or_lit;

	long			ld_nud_added_ccls;
	long			ld_nud_added_vars;
	long			ld_nud_added_lits;
	long			ld_nud_added_twolits;

	long			ld_num_ccls;
	long			ld_num_vars;
	long			ld_tot_lits;
	long			ld_tot_twolits;

	dimacs_loader(void* the_brn){
		init_dimacs_loader();
	}

	~dimacs_loader(){
		init_dimacs_loader();
	}

	void	init_parse();
	void	init_dimacs_loader();
	
	bj_ostr_stream& dimacs_err_msg(long num_line, char ch_err, ch_string msg);
	
	void 	read_problem_decl(const char*& pt_in, long& num_var, long& num_ccl, long& line);
	void 	skip_cnf_decl(const char*& pt_in, long line);

	void	parse_header();
	bool	parse_clause(row<integer>& lits);

	void	verif_num_ccls(ch_string& f_nam, long num_decl_ccls, long num_read_ccls);

	bool	fix_lits(row_long_t& lits, bool& add_it);

	bool	fixappend_lits(row_long_t& lits, row_long_t& prepared);

	void	add_lits(row_long_t& lits, row_long_t& prepared, bool is_orig);
	void	append_rr_lits(row_row_long_t& rr_lits, row<long>& prepared);

	void	three_lits(row_long_t& lits, row_long_t& prepared);

	void	lits_opps(row<long>& r_lits);

	void	calc_f_lit_equal_or(long d_nio, row_long_t& or_lits,
			row_row_long_t& rr_nios);

	void	calc_f_lit_equal_and(long d_nio, row_long_t& and_lits,
			row_row_long_t& rr_nios);

	long	calc_f_lit_equal_or_of_ands(long in_lit, row<long>& ccl_lits,
			row_row_long_t& rr_lits);

	void	load_file(ch_string& f_nam);
	
	void	parse_all_ccls(row<long>& inst_ccls);
	void	finish_parse(row<long>& inst_ccls);
	void	parse_content(row<long>& inst_ccls);
	
	void	parse_file(ch_string& f_nam, row<long>& inst_ccls, bool allow_empty = false);
	
	long	get_cursor_pos();
	
	//ch_string 	calc_content_sha();
};

//=================================================================
// print funcs

DEFINE_PRINT_FUNCS(dima_dims)


#endif		// DIMACS_H


