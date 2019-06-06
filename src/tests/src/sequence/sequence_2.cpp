

/*************************************************************

This file is part of messaging-cells.

messaging-cells is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

messaging-cells is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with messaging-cells.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2017-2018. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://messaging-cells.github.io/

messaging-cells is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------*/


#ifdef MC_IS_PTD_CODE
#include "stdio.h"
#endif

#include "attribute.h"
#include "cell.hh"

class sequence;

#define MAX_MSVS 50

#define PH_DBG PTD_PRT

typedef uint32_t seq_tok_t;

enum sq_hdlr_idx_t : mck_handler_idx_t {
	idx_invalid = mck_tot_base_cell_classes,
	idx_sequence,
	idx_last_invalid,
	idx_total
};

missive_handler_t sequence_handlers[idx_total];

class sequence : public cell {
public:
	MCK_DECLARE_MEM_METHODS(sequence)

	seq_tok_t last_sent;
	seq_tok_t last_recv;

	sequence(){
		init_sequence();
	}

	~sequence(){}

	void init_sequence(){
		handler_idx = idx_sequence;

		last_sent = 0;
		last_recv = 0;
	}

	void handler(missive* msv);
	void send(cell* dst, seq_tok_t tok);

	virtual 
	grip&	get_available();
};

// For global data. DO NOT USE GLOBAL VARIABLES IF YOU WANT THE PTD (workerus as threads) TO WORK.
class seq_workeru {
public:
	MCK_DECLARE_MEM_METHODS(seq_workeru)

	sequence my_seq;
	grip ava_seq;
};

MCK_DEFINE_ACQUIRE_ALLOC(seq_workeru, 32)	// defines seq_workeru::acquire_alloc

#define glb_seq_workeru ((seq_workeru*)(mck_get_kernel()->user_data))
#define glb_my_seq &(glb_seq_workeru->my_seq)

#define get_my_seq(id) ((sequence*)mc_addr_set_id(id, glb_my_seq))

#define glb_sender get_my_seq(mc_nn_to_id(1))
#define glb_receiver get_my_seq(mc_nn_to_id(0))
#define glb_ava_seq (glb_seq_workeru->ava_seq)

void 
sequence_handler(missive* msv){
	MCK_CALL_HANDLER(sequence, handler, msv);
}

void sequence_init_handlers(){
	missive_handler_t* hndlrs = sequence_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_sequence] = sequence_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
}

void
sequence::handler(missive* msv){
	cell* msv_src = msv->src;
	seq_tok_t tok = (seq_tok_t)msv->tok;
	MC_MARK_USED(msv_src);
	MC_MARK_USED(tok);

	kernel* ker = mck_get_kernel();
	mc_workeru_nn_t nn = ker->get_workeru_nn();

	bool is_sender = (nn == 1);

	PH_DBG("is_sender=%d tok=%d last_recv=%d\n", is_sender, tok, last_recv);
	PTD_CK(tok == (last_recv + 1));
	last_recv = tok;

	if(is_sender){
		PTD_CK(msv_src == glb_receiver);
		PTD_CK(last_recv == last_sent);
		last_sent += 1;
	} else {
		PTD_CK(msv_src == glb_sender);
		last_sent = last_recv;
	}

	bool is_last = (last_sent == MAX_MSVS);
	if(! is_sender && is_last){
		mck_get_kernel()->set_idle_exit();
		return;
	}

	respond(msv, last_sent);

	if(is_sender && is_last){
		mck_get_kernel()->set_idle_exit();
	}
}

void
sequence::send(cell* dst, seq_tok_t tok){
	last_sent = tok;
	
	missive* msv = mc_missive_acquire();
	
	msv->src = this;
	msv->dst = dst;
	msv->tok = tok;
	msv->send();
}

MCK_DEFINE_MEM_METHODS(sequence, 32, glb_ava_seq, 0)

grip&
sequence::get_available(){
	return glb_ava_seq;
}

void ker_func(){
	kernel* ker = mck_get_kernel();
	mc_workeru_nn_t nn = ker->get_workeru_nn();
	MC_MARK_USED(ker);
	MC_MARK_USED(nn);
}

void mc_workerus_main() {
	kernel::init_sys();
	kernel* ker = mck_get_kernel();
	MC_MARK_USED(ker);

	mc_workeru_nn_t nn = ker->get_workeru_nn();
	
	if((nn != 0) && (nn != 1)){
		PH_DBG("SKIP\n");
		kernel::finish_sys();
		return;
	}

	seq_workeru* workeru_dat = seq_workeru::acquire_alloc();
	ker->user_data = workeru_dat;
	ker->user_func = ker_func;

	sequence_init_handlers();

	missive::separate(mc_out_num_workerus);
	agent_ref::separate(mc_out_num_workerus);
	agent_grp::separate(mc_out_num_workerus);

	PH_DBG("started\n");
	if(nn == 1){
		glb_sender->send(glb_receiver, 1);
	}

	kernel::run_sys();

	PH_DBG("SEQ_2 finished\n");
	mck_slog2("SEQUENCE 2 FINISHED !!\n");	

	kernel::finish_sys();
}

