
#include "solver.hh"

#include "dbg_only.hh"
#include "load_cnf.hh"
#include "propag.hh"
#include "stabi.hh"
#include "sornet.hh"

//mc_c_decl int main();

#define TOT_MODS 6

#define DBG_ONLY_IDX 0
#define CNF_LOADER_IDX 1
#define PROPAG_IDX 2
#define STABI_IDX 3
#define SORNET_IDX 4
#define PGROUP_IDX 5

char* all_mod_nams[TOT_MODS] mc_external_data_ram;
mc_addr_t* all_mod_addr;

void
init_module_nams() mc_external_code_ram;

void
init_module_nams(){
	all_mod_nams[DBG_ONLY_IDX] = mc_cstr("module1");
	all_mod_nams[CNF_LOADER_IDX] = mc_cstr("module2");
	all_mod_nams[PROPAG_IDX] = mc_cstr("module3");
	all_mod_nams[STABI_IDX] = mc_cstr("module4");
	all_mod_nams[SORNET_IDX] = mc_cstr("module5");
	all_mod_nams[PGROUP_IDX] = mc_cstr("module6");
}

void
print_module_nams() mc_external_code_ram;

char dbg_slvr1[] mc_external_data_ram = "ALL_MODULE_NAMES=\n";

void
print_module_nams(){
	uint32_t to_m = mck_get_tot_modules();
	uint32_t aa = 0;
	mck_sprt2(dbg_slvr1);
	for(aa = 0; aa < to_m; aa++){
		char* nam0 = mck_get_module_name(aa);
		mck_sprt2(nam0);
		mck_sprt2("\n");
	}
}

void
print_module_addrs() mc_external_code_ram;

void
print_module_addrs(){
	uint32_t aa = 0;
	mck_sprt2("ALL_MODULE_ADDRS=\n");
	for(aa = 0; aa < TOT_MODS; aa++){
		char* nam0 = all_mod_nams[aa];
		mck_sprt2(nam0);
		mck_sprt2(" addr____");
		mck_xprt(all_mod_addr[aa]);
		mck_sprt2("____\n");
	}
}

void
link_all_modules_aux_fn() mc_external_code_ram;

void
link_all_modules_aux_fn(){
	bj_load_main();
}

void mc_workerus_main() {
	kernel::init_sys();

	//PTD_PRT("SOLVER main (KER=%p)\n", (void*)MCK_KERNEL);
	mck_slog2("SOLVER_STARTED\n");	

	bj_print_class_szs();

	init_module_nams();

	all_mod_addr = mc_malloc32(mc_addr_t, TOT_MODS);
	mck_fill_module_external_addresses(TOT_MODS, all_mod_nams, all_mod_addr);

	#ifdef BJ_DBG_ONLY
		PTD_PRT("BJ_DBG_ONLY\n");
		mck_slog2("BJ_DBG_ONLY\n");	
		bool ok = false;
		PTD_CK(DBG_ONLY_IDX < TOT_MODS);
		ok = mck_load_module(all_mod_addr[DBG_ONLY_IDX]);
		if(! ok){
			mck_abort(1, mc_cstr("mck_load_module failed for DBG_ONLY_IDX. \n"));
		}
		bj_dbg_only_main();
	#endif

	#ifdef BJ_DBG_SORNET
		PTD_PRT("BJ_DBG_SORNET\n");
		mck_slog2("BJ_DBG_SORNET\n");	
		bool ok = false;

		PTD_CK(CNF_LOADER_IDX < TOT_MODS);
		ok = mck_load_module(all_mod_addr[CNF_LOADER_IDX]);
		if(! ok){
			mck_abort(1, mc_cstr("mck_load_module failed for CNF_LOADER_IDX. \n"));
		}
		bj_load_main();

		PTD_CK(SORNET_IDX < TOT_MODS);
		ok = mck_load_module(all_mod_addr[SORNET_IDX]);
		if(! ok){
			mck_abort(1, mc_cstr("mck_load_module failed for SORNET_IDX. \n"));
		}
		bj_sornet_main();
	#endif

	
	#ifdef BJ_NORMAL 
		PTD_PRT("BJ_NORMAL\n");
		mck_slog2("BJ_NORMAL\n");	

		bool ok = false;
		PTD_CK(CNF_LOADER_IDX < TOT_MODS);
		ok = mck_load_module(all_mod_addr[CNF_LOADER_IDX]);
		if(! ok){
			mck_abort(1, mc_cstr("mck_load_module failed for CNF_LOADER_IDX. \n"));
		}
		bj_load_main();
		
		PTD_CK(PROPAG_IDX < TOT_MODS);
		ok = mck_load_module(all_mod_addr[PROPAG_IDX]);
		if(! ok){
			mck_abort(1, mc_cstr("mck_load_module_failed_for_PROPAG_IDX. \n"));
		}
		bj_propag_main();
		
		bool ach = bj_nervenet->act_left_side.sync_all_zero_act_neus;
		
		if(! ach){
			PTD_CK(STABI_IDX < TOT_MODS);
			ok = mck_load_module(all_mod_addr[STABI_IDX]);
			if(! ok){
				mck_abort(1, mc_cstr("mck_load_module_failed_for_STABI_IDX. \n"));
			}
			bj_mirrow_main();
			bj_stabi_main();
		}

		PTD_DBG_CODE(bj_nervenet->all_dbg_dat.dbg_prt_all());
		
	#endif

	//PTD_PRT("SOLVER_ENDED\n");
	mck_slog2("SOLVER_ENDED_OK\n");	

	kernel::finish_sys();
}

