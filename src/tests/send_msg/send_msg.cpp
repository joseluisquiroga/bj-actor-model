
#include "actor.hh"

void recv_actor_handler(missive* msg);
void actor_handler(missive* msg);

void
wait_inited_state(bj_core_id_t dst_id){
	bjk_glb_sys_st* in_shd = BJK_GLB_SYS;
	uint8_t* loc_st = &(in_shd->the_core_state);
	uint8_t* rmt_st = (uint8_t*)bj_addr_set_id(dst_id, loc_st);
	while(*rmt_st != bjk_inited_state);
}

void 
recv_actor_handler(missive* msg){
	BJK_UPDATE_MIN_SP();
	EMU_CK(bj_addr_is_local(msg->dst));
	bj_core_id_t koid = kernel::get_core_id();
	BJ_MARK_USED(koid);
	bj_core_nn_t konn = kernel::get_core_nn();
	BJ_MARK_USED(konn);
	bjk_slog2("GOT MISSIVE\n");
	EMU_LOG("recv_actor_handler. core_id=%lx core_nn=%d src=%p dst=%p \n", koid, konn, msg->get_source(), msg->dst);
	EMU_PRT("recv_actor_handler. core_id=%lx core_nn=%d src=%p dst=%p \n", koid, konn, msg->get_source(), msg->dst);
	
	bjk_get_kernel()->set_idle_exit();
}

void bj_cores_main() {
	kernel::init_sys();

	kernel::set_handler(recv_actor_handler, bjk_handler_idx(actor));

	actor::separate(bj_out_num_cores);
	missive::separate(bj_out_num_cores);
	agent_ref::separate(bj_out_num_cores);
	agent_grp::separate(bj_out_num_cores);

	kernel* ker = bjk_get_kernel();
	BJ_MARK_USED(ker);

	if(bjk_is_core(0,0)){
		bjk_slog2("CORE (0,0) started\n");
		kernel::run_sys();
	}
	if(bjk_is_core(0,1)){
		bjk_slog2("CORE (0,1) started\n");
		bj_core_id_t dst = bj_ro_co_to_id(0, 0);
		
		actor* act1 = kernel::get_core_actor();
		actor* act2 = kernel::get_core_actor(dst);

		missive* msv = missive::acquire();
		msv->src = act1;
		msv->dst = act2;
		msv->send();
		bjk_slog2("SENT MISSIVE\n");

		ker->set_idle_exit();
		kernel::run_sys();
	}

	bjk_slog2("FINISHED !!\n");	
	//bjk_xlog((bj_addr_t)ker->host_kernel);
	//bjk_slog2(" is the HOST_KERNEL\n");	

	kernel::finish_sys();
}

