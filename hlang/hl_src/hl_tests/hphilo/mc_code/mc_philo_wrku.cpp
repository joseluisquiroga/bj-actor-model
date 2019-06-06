
#include "cell.hh"

#include "hl_generated_output.hh"

void test_init_mem(){
	hg_chopstick_init_mem_funcs();
	hg_philosopher_init_mem_funcs();
}

hg_glbs_hl_generated_output*
hg_globals(){
	return ((hg_glbs_hl_generated_output *)(kernel::get_sys()->user_data));
}

void test_init_handlers(){
	missive_handler_t* hndlrs = hg_handlers;
	mc_init_arr_vals(idx_total_hl_generated_output, hndlrs, mc_null);
	
	hndlrs[idx_chopstick] = chopstick_handler;
	hndlrs[idx_philosopher] = philosopher_handler;
	
	hndlrs[idx_last_ivalid_hl_generated_output] = kernel::invalid_handler_func;
	kernel::set_cell_handlers(hndlrs);
}

#define left_chp_nn(nn) (nn)
#define right_chp_nn(nn) ((nn == 15)?(0):(nn + 1))

#define left_phl_nn(nn) ((nn == 0)?(15):(nn - 1))
#define right_phl_nn(nn) ((nn == 15)?(0):(nn + 1))

class philo_global_info {
public:
	philosopher* my_philo = hg_null;
	chopstick* my_stick = hg_null;
};


philo_global_info*
get_remote_philo_dat(mc_workeru_id_t rem_id){
	kernel* rem_ker = mck_get_kernel_by_id(rem_id);
	PTD_CK(rem_ker != mc_null);
	mc_spin_cond(rem_ker->user_data == mc_null);
	hg_glbs_hl_generated_output* rem_dat = (hg_glbs_hl_generated_output*)(rem_ker->user_data);
	PTD_CK(rem_dat != mc_null);
	mc_spin_cond(rem_dat->hg_user_data == mc_null);
	
	philo_global_info* rem_ph_dat = (philo_global_info*)(rem_dat->hg_user_data);
	PTD_CK(rem_ph_dat != mc_null);
	return rem_ph_dat;
}

void mc_workerus_main() {
	kernel::init_sys();
	kernel* ker = mck_get_kernel();
	MC_MARK_USED(ker);

	mc_workeru_nn_t nn = ker->get_workeru_nn();
	MC_MARK_USED(nn);
	
	hg_glbs_hl_generated_output* workeru_dat = hg_glbs_hl_generated_output::acquire_alloc();
	if(workeru_dat == mc_null){
		mck_abort(1, mc_cstr("CAN NOT INIT GLB WORKERU DATA"));
	}
	
	ker->user_data = workeru_dat;

	workeru_dat->init_mem_funcs();
	hg_missive_init_mem_funcs();
	
	test_init_mem();
	test_init_handlers();

	cell::separate(mc_out_num_workerus);
	missive::separate(mc_out_num_workerus);
	agent_ref::separate(mc_out_num_workerus);
	agent_grp::separate(mc_out_num_workerus);

	philo_global_info ph_dat;
	
	hg_acquire(philosopher, ph_dat.my_philo);
	hg_acquire(chopstick, ph_dat.my_stick);
	
	//CELL_INIT_BEGINING. NEVER INIT CELL THIS WAY. Only for testing purposes.
	hg_missive null_msv;
	
	ph_dat.my_philo->hg_step = hg_init_philosopher_step_id;
	ph_dat.my_philo->handler(&null_msv);
	
	ph_dat.my_stick->hg_step = hg_init_chopstick_step_id;
	ph_dat.my_stick->handler(&null_msv);
	//CELL_INIT_END. NEVER INIT CELL THIS WAY. Only for testing purposes.
	
	hg_globals()->hg_user_data = &ph_dat;
	
	mc_workeru_id_t	lft_stk_id = mc_nn_to_id(left_chp_nn(nn));
	mc_workeru_id_t	rgt_stk_id = mc_nn_to_id(right_chp_nn(nn));
	mc_workeru_id_t	lft_phi_id = mc_nn_to_id(left_phl_nn(nn));
	mc_workeru_id_t	rgt_phi_id = mc_nn_to_id(right_phl_nn(nn));

	philo_global_info* mgr_dat = get_remote_philo_dat(mc_nn_to_id(0));
	ph_dat.my_philo->manager = mgr_dat->my_philo;
	
	philo_global_info* lft_stk_dat = get_remote_philo_dat(lft_stk_id);
	philo_global_info* rgt_stk_dat = get_remote_philo_dat(rgt_stk_id);
	philo_global_info* lft_phi_dat = get_remote_philo_dat(lft_phi_id);
	philo_global_info* rgt_phi_dat = get_remote_philo_dat(rgt_phi_id);
	
	ph_dat.my_philo->lft_stick = lft_stk_dat->my_stick;
	ph_dat.my_philo->rgt_stick = rgt_stk_dat->my_stick;
	ph_dat.my_philo->lft_philo = lft_phi_dat->my_philo;
	ph_dat.my_philo->rgt_philo = rgt_phi_dat->my_philo;
	
	PTD_PRT("PHILO_STARTED %ld\n", nn);
	hg_missive* msv = hg_missive_acquire();
	
	msv->tok = pr_tok_eat;
	msv->src = ph_dat.my_philo;
	msv->dst = ph_dat.my_philo;
	msv->val = (hg_value_t)0;
	msv->send();
	
	kernel::run_sys();

	mck_slog2("FINISHED !!\n");	

	kernel::finish_sys();
}

