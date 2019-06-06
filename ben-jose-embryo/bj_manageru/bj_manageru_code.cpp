
#include <stdio.h>
#include <typeindex>

#include "cell.hh"
#include "dimacs.h"
#include "preload.hh"
#include "file_funcs.h"

//define BJ_WITH_MANAGERU_TESTS

#ifdef BJ_WITH_MANAGERU_TESTS
#include "nervenet.hh"
#include "load_cnf.hh"
#include "tak_mak.hh"
#endif

void bj_test(int argc, char *argv[]) mc_external_code_ram;
int bj_manageru_main(int argc, char *argv[]) mc_external_code_ram;
void bj_mgr_init_handlers() mc_external_code_ram;

//------------------------------------------------------------
// Global vars

char* mch_epiphany_elf_path = (mc_cstr("the_epiphany_executable.elf"));

missive_handler_t bj_mgr_handlers[bj_mgr_idx_total];

void bj_mgr_init_handlers(){
	missive_handler_t* hndlrs = bj_mgr_handlers;
	mc_init_arr_vals(bj_mgr_idx_total, hndlrs, mc_null);
	hndlrs[bj_mgr_last_invalid] = kernel::invalid_handler_func;

	kernel::set_tot_cell_subclasses(bj_mgr_idx_total);
	kernel::set_cell_handlers(hndlrs);

	bj_mgr_init_mem_funcs();
}

int bj_manageru_main(int argc, char *argv[])
{
	if(argc > 1){
		mch_epiphany_elf_path = argv[1];
		printf("Using workeru executable: %s \n", mch_epiphany_elf_path);
	}

	if(argc < 3){
		printf("args: <epiphany_elf> <cnf_file_path>\n");
		return 1;
	}
	ch_string f_nam = argv[2];

	if(! file_exists(f_nam)){
		printf("Cannot find file %s \n", f_nam.c_str() );
		return 1;
	}

	printf("BJ_MAX_NODE_SZ=%lu\n", BJ_MAX_NODE_SZ);

	row<long> all_lits;
	all_lits.clear();

	bool load_ok = true;
	dimacs_loader	the_loader(NULL);
	try{
		the_loader.parse_file(f_nam, all_lits, true);
	} catch (const top_exception& ex1){
		load_ok = false;
	}

	if(load_ok){
		//std::cout << all_lits << "\n";
		std::cout << "LOAD CNN SUCCEDED f_nam=" << f_nam << "\n";
	} else {
		std::cout << "LOAD CNN FAILED f_nam=" << f_nam << "\n";
	}

	const long* arr = all_lits.get_c_array();
	long sz = all_lits.get_c_array_sz();

	kernel::init_manageru_sys();

	THE_CNF = mc_malloc32(pre_load_cnf, 1);
	new (THE_CNF) pre_load_cnf(); 

	bj_mgr_init_handlers();
	
	THE_CNF->tot_ccls = the_loader.ld_num_ccls;
	THE_CNF->tot_vars = the_loader.ld_num_vars;
	THE_CNF->tot_lits = the_loader.ld_tot_lits;
	THE_CNF->tot_workerus = mc_tot_nn_sys;

	preload_cnf_init();
	PTD_CODE(preload_sornet());
	preload_cnf(sz, arr);

	void* workeru_cnf_pt = (void*)mc_manageru_addr_to_workeru_addr((mc_addr_t)THE_CNF);
	kernel::get_sys()->manageru_load_data = workeru_cnf_pt;

	printf("LITS_SZ=%ld tot_lits=%ld tot_vars=%ld tot_ccls=%ld tot_workerus=%ld \n", all_lits.size(),
			the_loader.ld_tot_lits, the_loader.ld_num_vars, the_loader.ld_num_ccls, THE_CNF->tot_workerus);

	printf("THE_CNF=%p \n", workeru_cnf_pt);
	printf("magic = %ld \n", THE_CNF->MAGIC);

	//manageru_print_nods();

	//print_cnf();
	//print_workeru_cnfs();

	kernel::run_manageru_sys();
	kernel::finish_manageru_sys();

	return 0;
}

//ifdef MC_IS_PTD_CODE

#if defined(MC_IS_PTD_CODE) && defined(BJ_WITH_MANAGERU_TESTS)

void bj_test_1(int argc, char *argv[])
{
	printf("bj_test_1 \n");
	binder b1;
	binder b2;
	synapse s1;

	s1.handler_idx = 55;
	
	b1.bind_to_my_left(b2);
	b1.bind_to_my_left(s1.right_handle); // test

	if(b1.bn_right == &b2){
		printf("b2 is FIRST\n");
	}
	if(b1.bn_left == &s1.right_handle){ // test
		printf("syn is LAST\n");
	}
	binder* lft = b1.bn_left;
	synapse* pt_s1 = bj_get_syn_of_rgt_handle(lft);

	PTD_CK(bj_ck_is_synapse(pt_s1));
	printf("syn_idx = %d\n", pt_s1->handler_idx);

	printf("BJ_MANAGERU_TESTS_FINISHED\n");
}

typedef uint16_t test_int_t;

void bj_test_2(int argc, char *argv[])
{
	printf("bj_test_2 \n");
	unsigned long imax = mc_maxof(test_int_t);

	printf("imax = %lu\n", imax);
}

void call_nerv_pt_fn(nervenode* nn, synapse* snp, bj_callee_t mth){
	callee_prms pms;
	pms.snp = snp;
	pms.sd = side_right;
	pms.rec = false;

	(nn->*mth)(pms);
	//(nn->*mth)(snp, side_right, false);
}

void 
neuron::pru_callee(callee_prms& pms){
	//printf("pru_callee %ld %d \n", id, pru_attr);
	printf("pru_callee %ld \n", id);
}

void bj_test_3(int argc, char *argv[])
{
	printf("bj_test_3 \n");
	neuron nn1;
	synapse snp1;

	//nn1.pru_attr = 321;
	nn1.id = 1234;
	snp1.owner = &nn1;

	call_nerv_pt_fn(&nn1, &snp1, (bj_callee_t)(&neuron::pru_callee));
	//call_nerv_pt_fn(&nn1, &snp1, &neuron::pru_callee);
}

void bj_test_4(int argc, char *argv[])
{
	printf("bj_test_4 \n");
	long aa = 1234;
	long bb = (aa >> 3);

	printf("aa = %ld bb = %ld\n", aa, bb);
}

void bj_test_5(int argc, char *argv[])
{
	printf("bj_test_5 \n");
	mc_flags_t ff = 0;

	mc_set_flag(ff, mc_flag1);
	printf("flags=%p ff_pt=%p \n", (void*)(uintptr_t)ff, (void*)(&ff));
	mc_set_flag(ff, mc_flag3);
	printf("flags=%p ff_pt=%p \n", (void*)(uintptr_t)ff, (void*)(&ff));
}

void bj_test_6(int argc, char *argv[])
{
	printf("bj_test_6 \n");
	if(argc < 2){
		printf("%s <num>\n", argv[0]);
		return; 
	}
	num_nod_t num_to_sort = atol(argv[1]);
	num_to_sort = get_bigger_pow2(num_to_sort);
	printf("pow2 = %ld \n", num_to_sort);
}

void bj_test_7(int argc, char *argv[])
{
	printf("bj_test_7 \n");
	if(argc < 4){
		printf("Usage: %s <val> <min> <max>\n", argv[0]);
		return;
	}
	
	long val = atol(argv[1]);
	long min = atol(argv[2]);
	long max = atol(argv[3]);
	num_nod_t inter = (num_nod_t)to_interval(val, min, max);
	//long inter = to_interval(val, min, max);
	printf("(%ld %ld %ld) fixed = %ld \n", val, min, max, inter);
}

#endif

#ifdef BJ_WITH_MANAGERU_TESTS

void bj_mc_test_1(int argc, char *argv[])
{
	printf("bj_mc_test_1 \n");
	if(argc > 1){
		mch_epiphany_elf_path = argv[1];
		printf("Using workeru executable: %s \n", mch_epiphany_elf_path);
	}

	if(argc < 3){
		printf("Usage: %s <epiphany_elf> <num>\n", argv[0]);
		return;
	}
	
	num_nod_t num_to_sort = atol(argv[2]);
	num_to_sort = get_bigger_pow2(num_to_sort);
	if(num_to_sort < 4){ num_to_sort = 4; }

	kernel::init_manageru_sys();
	create_sornet(num_to_sort);
	kernel::finish_manageru_sys();
}

void bj_mc_test_2(int argc, char *argv[])
{
	printf("bj_mc_test_2 \n");
	if(argc < 3){
		printf("%s <min> <max> \n", argv[0]);
		return; 
	}
}

#define BJ_DBG_MAX_NOD_SZ 20

void bj_mc_test_3(int argc, char *argv[])
{
	/*
	printf("bj_mc_test_3 \n");
	kernel::init_manageru_sys();
	bj_mgr_init_handlers();
	
	kernel* ker = mck_get_kernel();
	
	//nervenet* my_net_pt = bj_nervenet_acquire();
	
	nervenet my_net;
	ker->user_data = &my_net;

	//nervenode* nn1_pt = bj_nervenode_acquire();
	//nervenode& nn1 = *nn1_pt;
	nervenode nn1;
	
	tak_mak gg;
	char *line = NULL;
	size_t len = 0;
	char dbg_str1[BJ_DBG_STR_CAP];

	nn1.id = 1234;
	nn1.sz = (num_syn_t)(gg.gen_rand_int32_ie(1, BJ_DBG_MAX_NOD_SZ));
	printf("SZ=%d \n", nn1.sz);

	nn1.stabi_arr_dat = mc_malloc32(num_nod_t, nn1.sz);

	for(num_tier_t aa = 0; aa < nn1.sz; aa++){
		printf("Type a number \n");
		ssize_t read = getline(&line, &len, stdin);
		if(read == -1){ break; }
		num_nod_t num = atol(line);
		
		//printf("num=%ld \n", num);
		nn1.stabi_insert_color(num);
		char* str_dat = bj_dbg_stabi_col_arr_to_str(nn1.stabi_arr_sz, nn1.stabi_arr_dat, 
													(BJ_DBG_STR_CAP - 2), dbg_str1);
		printf("dat=%s \n", str_dat);
	}
	
	kernel::finish_manageru_sys();
	*/
}

void bj_mc_test_4(int argc, char *argv[])
{
	printf("bj_mc_test_4 \n");
	if(argc > 1){
		mch_epiphany_elf_path = argv[1];
		printf("Using workeru executable: %s \n", mch_epiphany_elf_path);
	}

	if(argc < 3){
		printf("Usage: %s <epiphany_elf> <num>\n", argv[0]);
		return;
	}
	
	num_nod_t num_item = atol(argv[2]);
	
	kernel::init_manageru_sys();
	bj_mgr_init_handlers();
	pre_pgroup* grp = bj_pre_pgroup_acquire();
	create_pgroup(*grp, num_item);
	grp->prt_nodes();
	kernel::finish_manageru_sys();
}

void pru_lambda()
{
	std::function<int(int aa, int bb)> ff = [&] (int aa, int bb) -> int { return aa + bb; }; 
}

#endif

int mc_manageru_main(int argc, char *argv[])
{
	int rr = 0;

	//PTD_CODE(bj_test_1(argc, argv));
	//PTD_CODE(bj_test_2(argc, argv));
	//PTD_CODE(bj_test_3(argc, argv));
	//PTD_CODE(bj_test_4(argc, argv));
	//PTD_CODE(bj_test_5(argc, argv));
	//PTD_CODE(bj_test_6(argc, argv));
	//PTD_CODE(bj_test_7(argc, argv));
	//bj_mc_test_1(argc, argv);
	//bj_mc_test_2(argc, argv);
	//bj_mc_test_3(argc, argv);
	//bj_mc_test_4(argc, argv);
	//bj_mc_test_5(argc, argv);
	
	rr = bj_manageru_main(argc, argv);

	printf("MANAGERU_CODE_FINISHED ==================================== \n");
	return rr;
}

