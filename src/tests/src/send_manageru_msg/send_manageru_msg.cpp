

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


#include "cell.hh"

#include "resp_conf.h"

void recv_workeru_handler(missive* msg);

void 
recv_workeru_handler(missive* msg){
	PTD_CK(mc_addr_is_local(msg->dst));

	mck_sprt2("WORKERU_GOT_RESPONSE\n");
	PTD_PRT("RCV_RESPONSE. src=%p dst=%p \n", msg->get_source(), msg->dst);
	PTD_LOG("RCV_RESPONSE. src=%p dst=%p \n", msg->get_source(), msg->dst);
	
	// JUST for this test. NEVER call this func directly.
	mck_get_kernel()->set_idle_exit();
}

enum sm_hdlr_idx_t : mck_handler_idx_t {
	idx_invalid = mck_tot_base_cell_classes,
	idx_recv_msg,
	idx_last_invalid,
	idx_total
};

missive_handler_t send_msg_handlers[idx_total];

void send_msg_init_handlers(){
	missive_handler_t* hndlrs = send_msg_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_recv_msg] = recv_workeru_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
}

void mc_workerus_main() {
	kernel::init_sys();

	cell::separate(mc_out_num_workerus);
	missive::separate(mc_out_num_workerus);
	agent_ref::separate(mc_out_num_workerus);
	agent_grp::separate(mc_out_num_workerus);

	// JUST for this test. NEVER overwrite the first cell handler.
	kernel::get_first_cell()->handler_idx = idx_recv_msg; 
	
	kernel* ker = mck_get_kernel();
	MC_MARK_USED(ker);

	if(mck_is_ro_co_workeru(0,0)){
		mck_slog2("WORKERU (0,0) started\n");

		send_msg_init_handlers();

		cell* act1 = kernel::get_first_cell();
		cell* act2 = kernel::get_manageru_cell();

		PTD_CK_PRT((mc_addr_get_id((mc_addr_t)act2) != 0), "act2 = %p\n", act2);

		missive* msv = mc_missive_acquire();
		
		msv->src = act1;
		msv->dst = act2;
		msv->tok = 432;

		mck_sprt2("SND_act2___");
		mck_xprt((mc_addr_t)(act2));
		mck_sprt2("___\n");

		MCK_CK(act2 != mc_null);

		msv->send_to_manageru();

		PTD_PRT("SND_SENT H MISSIVE\n");
		PTD_LOG("SND_SENT H MISSIVE\n");

		#ifndef WITH_RESPONSE
			// JUST for this test. NEVER call this func directly.
			ker->set_idle_exit();
		#endif

		kernel::run_sys();
	}

	mck_slog2("FINISHED SEND_MANAGERU TEST\n");	

	kernel::finish_sys();
}

