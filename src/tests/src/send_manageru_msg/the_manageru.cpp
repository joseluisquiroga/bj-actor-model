

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


#include <stdio.h>

#include "booter.h"
#include "cell.hh"

#include "resp_conf.h"

char* mch_epiphany_elf_path = (mc_cstr("the_epiphany_executable.elf"));

void recv_manageru_handler(missive* msg);

void 
recv_manageru_handler(missive* msg){
	PTD_PRT("RCV_MSV=%p \n", msg);
	PTD_PRT("RCV_msv=%p SRC=%p DST=%p \n", (void*)msg, msg->src, msg->dst);
	PTD_PRT("RCV_WORKERU_ID=%x \n", mc_addr_get_id(msg->dst));
	PTD_PRT("RCV_GLB_WORKERU_ID=%x \n", MC_WORKERU_INFO->the_workeru_id);
	printf("MANAGERU_RECEIVED_MSV !!!\n");

	PTD_CK(mc_addr_is_local(msg->dst));
	mc_workeru_id_t koid = kernel::get_workeru_id();
	MC_MARK_USED(koid);
	mc_workeru_nn_t konn = kernel::get_workeru_nn();
	MC_MARK_USED(konn);


	PTD_LOG("recv_manageru_handler. workeru_id=%lx workeru_nn=%d src=%p dst=%p \n", 
			koid, konn, msg->get_source(), msg->dst);
	PTD_PRT("RCV_MSV. workeru_id=%lx workeru_nn=%d src=%p dst=%p \n", 
			koid, konn, msg->get_source(), msg->dst);

	#ifdef WITH_RESPONSE
		msg->dst->respond(msg, (msg->tok + 10)); 
		printf("MANAGERU_RESPONDED\n");
	#endif 


	// JUST for this test. NEVER call this func directly.
	mck_get_kernel()->set_idle_exit();
}

enum sm_hdlr_idx_t : mck_handler_idx_t {
	idx_invalid = mck_tot_base_cell_classes,
	idx_recv_msg,
	idx_last_invalid,
	idx_total
};

missive_handler_t send_msg_mgr_handlers[idx_total];

void send_msg_mgr_init_handlers(){
	missive_handler_t* hndlrs = send_msg_mgr_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_recv_msg] = recv_manageru_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(idx_total);
	kernel::set_cell_handlers(hndlrs);
}

void
send_manageru_main(){
	kernel::init_manageru_sys();

	// JUST for this test. NEVER overwrite the first cell handler.
	kernel::get_first_cell()->handler_idx = idx_recv_msg;
	
	send_msg_mgr_init_handlers();

	cell::separate(mc_out_num_workerus);
	missive::separate(mc_out_num_workerus);
	agent_ref::separate(mc_out_num_workerus);
	agent_grp::separate(mc_out_num_workerus);

	mc_size_t off_all_agts = mc_offsetof(&missive_grp_t::all_agts);
	MC_MARK_USED(off_all_agts);

	printf("MANAGERU STARTING ==================================== \n");
	ZNQ_CODE(printf("off_all_agts=%d \n", off_all_agts));

	kernel::run_manageru_sys();
	kernel::finish_manageru_sys();

	printf("ALL FINISHED ==================================== \n");
}

int mc_manageru_main(int argc, char *argv[])
{
	if(argc > 1){
		mch_epiphany_elf_path = argv[1];
		printf("Using workeru executable: %s \n", mch_epiphany_elf_path);
	}
	if(argc > 2){
		printf("LOADING WITH MEMCPY \n");
		MCH_LOAD_WITH_MEMCPY = true;
	}

	send_manageru_main();

	return 0;
}


