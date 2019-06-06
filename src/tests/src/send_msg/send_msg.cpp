

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
/*! \file send_msg.cpp

\brief Simple send missive example.

\details 

\include send_msg.cpp

------------------------------------------------------------*/

#include "cell.hh"

/*! 
\brief This function will handle messages for a \ref cell when it has zero in 
\ref cell::handler_idx and \ref kernel::all_handlers has been set to \ref send_msg_handlers 
with \ref kernel::set_cell_handlers
*/
void recv_cell_handler(missive* msg);

enum send_msg_tok_t : mck_token_t {
	tok_invalid = mck_tok_last + 1,
	tok_ping
};

void 
recv_cell_handler(missive* msg){
	PTD_CODE(
		PTD_CK(mc_addr_is_local(msg->dst));
		mc_workeru_id_t koid = kernel::get_workeru_id();
		MC_MARK_USED(koid);
		mc_workeru_nn_t konn = kernel::get_workeru_nn();
		MC_MARK_USED(konn);
		PTD_LOG("recv_cell_handler. workeru_id=%lx workeru_nn=%d src=%p dst=%p \n", 
				koid, konn, msg->get_source(), msg->dst);
		PTD_PRT("recv_cell_handler. workeru_id=%lx workeru_nn=%d src=%p dst=%p \n", 
				koid, konn, msg->get_source(), msg->dst);
	)

	PTD_PRT("GOT_MISSIVE\n");
	mck_slog2("GOT_MISSIVE\n");
	
	// JUST for this test. NEVER call this func directly.
	mck_get_kernel()->set_idle_exit();
}

enum sm_hdlr_idx_t : mck_handler_idx_t {
	idx_invalid = mck_tot_base_cell_classes,
	idx_recv_msg,
	idx_last_invalid,
	idx_total
};

/*! 
\brief This will be \ref kernel::all_handlers when \ref kernel::set_cell_handlers gets called.
*/
missive_handler_t send_msg_handlers[idx_total];

void send_msg_init_handlers(){
	missive_handler_t* hndlrs = send_msg_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_recv_msg] = recv_cell_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
}

void mc_workerus_main() {
	kernel::init_sys();

	// JUST for this test. NEVER overwrite the first cell handler.
	kernel::get_first_cell()->handler_idx = idx_recv_msg; 
	
	send_msg_init_handlers();

	cell::separate(mc_out_num_workerus);
	missive::separate(mc_out_num_workerus);
	agent_ref::separate(mc_out_num_workerus);
	agent_grp::separate(mc_out_num_workerus);

	kernel* ker = mck_get_kernel();
	MC_MARK_USED(ker);

	if(mck_is_ro_co_workeru(0,0)){
		mck_slog2("WORKERU (0,0) started\n");

		kernel::run_sys();
	}
	if(mck_is_ro_co_workeru(0,1)){
		mck_slog2("WORKERU (0,1) started\n");
		mc_workeru_id_t dst = mc_ro_co_to_id(0, 0);

		cell* act1 = kernel::get_first_cell();
		cell* act2 = kernel::get_first_cell(dst);

		missive* msv = mc_missive_acquire();
		
		msv->tok = tok_ping;
		msv->src = act1;
		msv->dst = act2;
		msv->send();
		mck_slog2("SENT MISSIVE\n");

		// JUST for this test. NEVER call this func directly.
		ker->set_idle_exit();
		
		kernel::run_sys(false);
	}

	mck_slog2("FINISHED !!\n");	

	kernel::finish_sys();
}

