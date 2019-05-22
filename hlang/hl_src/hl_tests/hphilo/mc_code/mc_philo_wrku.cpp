
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

	philosopher* my_philo = hg_philosopher_acquire();
	
	hg_globals()->hg_user_data = my_philo;
	
	/*
	if(nn == 0){
		mck_slog2("WORKERU 0 started\n");

		kernel::run_sys();
	} else
	if(nn == 1){
		mck_slog2("WORKERU 1 started\n");
		mc_workeru_id_t dst = mc_nn_to_id(0);

		mc_spin_cond(kernel::get_workeru_kernel(dst) == mc_null);
		
		kernel* ker2 = kernel::get_workeru_kernel(dst);
		PTD_CK(ker2 != mc_null);
		mc_spin_cond(ker2->user_data == mc_null);
		
		hg_glbs_hl_generated_output* dat2 = (hg_glbs_hl_generated_output*)(ker2->user_data);
		PTD_CK(dat2 != mc_null);
		mc_spin_cond(dat2->hg_user_data == mc_null);
		
		cls_snd* obj1 = snd_obj;
		cls_snd* obj2 = (cls_snd*)(dat2->hg_user_data);

		PTD_CK(obj1 != mc_null);
		PTD_CK(obj2 != mc_null);
		
		hg_missive* msv = hg_missive_acquire();
		
		msv->tok = htk_start;
		msv->src = obj1;
		msv->dst = obj2;
		msv->val = (hg_value_t)obj1;
		msv->send();
		
		PTD_PRT("SENT MISSIVE\n");
		mck_slog2("SENT MISSIVE\n");

		kernel::run_sys();
	} 
	*/

	mck_slog2("FINISHED !!\n");	

	kernel::finish_sys();
}

