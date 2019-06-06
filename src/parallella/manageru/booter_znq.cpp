

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


#include <math.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <e-hal.h>
#include <epiphany-hal-api-local.h>

#include "booter.h"
#include "all_regs.h"
#include "dlmalloc.h"
#include "cell.hh"

#include "workeru_loader_znq.h"

//=====================================================================================

mc_workeru_id_t mch_first_load_workeru_nn = 0;

uint8_t* MCH_EXTERNAL_RAM_BASE_PT = mc_null;

//char* mch_epiphany_elf_path = (mc_cstr("the_epiphany_executable.elf"));

//mspace mch_glb_load_mspace;
mspace mch_glb_alloc_mspace;

e_mem_t mch_glb_emem;
e_epiphany_t mch_glb_dev;

void
print_workeru_info(mc_off_workeru_st* sh_dat_1, e_epiphany_t* dev, unsigned row, unsigned col){
	mck_glb_sys_st* pt_inco = 
		(mck_glb_sys_st*)mc_manageru_eph_loc_addr_to_znq_addr(row, col, (mc_addr_t)(sh_dat_1->workeru_data));
	mch_prt_in_workeru_shd_dat(pt_inco);

	if(pt_inco->dbg_stack_trace != mc_null){
		void** 	pt_trace = (void**)mc_manageru_eph_loc_addr_to_znq_addr(
			row, col, (mc_addr_t)(pt_inco->dbg_stack_trace));;
		mch_prt_workeru_call_stack(mch_epiphany_elf_path, MC_MAX_CALL_STACK_SZ, pt_trace);
	}
}

void
print_exception_case(mc_off_workeru_st* sh_dat_1, e_epiphany_t* dev, unsigned row, unsigned col){
	mck_glb_sys_st* pt_inco = 
		(mck_glb_sys_st*)mc_manageru_eph_loc_addr_to_znq_addr(row, col, (mc_addr_t)(sh_dat_1->workeru_data));

	if(pt_inco->exception_code != mck_invalid_exception){
		//mch_prt_exception(pt_inco);
		mch_prt_in_workeru_shd_dat(pt_inco);
	}
	if(pt_inco->dbg_error_code != 0){
		mch_prt_in_workeru_shd_dat(pt_inco);
	}
	if(pt_inco->dbg_stack_trace != mc_null){
		mc_addr_t eph_addr = (mc_addr_t)(pt_inco->dbg_stack_trace);
		void** 	pt_trace = (void**)mc_manageru_eph_loc_addr_to_znq_addr(row, col, eph_addr);
		mch_prt_workeru_call_stack(mch_epiphany_elf_path, MC_MAX_CALL_STACK_SZ, pt_trace);
	}
}

void
mc_manageru_init(){
	e_epiphany_t & dev = mch_glb_dev;

	e_platform_t platform;

	mcl_module_names_sz = 0;

	mc_link_syms_data_st* lk_dat = &(MC_EXTERNAL_RAM_LOAD_DATA);
	mch_read_eph_link_syms(mch_epiphany_elf_path, lk_dat);

	if(lk_dat->extnl_ram_orig == 0) {
		printf("ERROR: Can't read external memory location from '%s'\n", mch_epiphany_elf_path);
		printf("1- Make sure to set 'mch_epiphany_elf_path' global variable to a valid Epiphany ELF file\n");
		printf("\tbefore calling 'kernel::init_manageru_sys()'\n");
		printf("2- Make sure linker script for '%s' defines LD_EXTERNAL_* symbols\n\n",
			   mch_epiphany_elf_path);
		mch_abort_func(201, "mc_manageru_init (1). ERROR: Bad ELF\n");
	}

	// IMPORTANT NOTE:
	// Vales in HDF MUST match values in the linker script:
	// 			EMEM_BASE_ADDRESS == LD_EXTERNAL_RAM_ORIG
	// 			EMEM_SIZE == LD_EXTERNAL_RAM_SIZE

	// Current HDF: the value of EPIPHANY_HDF enviroment variable because e_initi is called with NULL
	// Current value of EPIPHANY_HDF: /opt/adapteva/esdk/bsps/current/platform.hdf
	// Current value of EMEM_BASE_ADDRESS in HDF: 0x8e000000
	// Current link script: mc-ld-script.ldf
	// Observe that this address is AS SEEN FROM THE EPIPHANY CORES (workerus) side. 
	// 		NOT as seen from the Zynq (manageru) side.

	// sys init

	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	if (e_alloc(&mch_glb_emem, 0, lk_dat->extnl_ram_size)) {
		mch_abort_func(202, "mc_manageru_init (2). ERROR: Can't allocate external memory buffer!\n\n");
	}

	MCH_EXTERNAL_RAM_BASE_PT = ((uint8_t*)mch_glb_emem.base);

	mcz_pt_external_manageru_data_obj = (mc_off_sys_st*)mch_disp_to_pt(lk_dat->extnl_manageru_data_disp);

	//uint8_t* extnl_load_base = mch_disp_to_pt(lk_dat->extnl_load_disp);
	uint8_t* extnl_manageru_alloc_base = mch_disp_to_pt(lk_dat->extnl_manageru_alloc_disp);
	
	//mch_glb_load_mspace = create_mspace_with_base(extnl_load_base, lk_dat->extnl_load_size, 0);
	mch_glb_alloc_mspace = create_mspace_with_base(extnl_manageru_alloc_base, lk_dat->extnl_manageru_alloc_size, 0);

	// dev init
	
	e_open(&dev, 0, 0, platform.rows, platform.cols);

	mc_sys_sz_st* g_sys_sz = MC_SYS_SZ;
	mch_init_glb_sys_sz_with_dev(g_sys_sz, &dev);

	mc_off_sys_st* pt_shd_data = mcz_pt_external_manageru_data_obj;
	MCH_CK(sizeof(*pt_shd_data) == sizeof(mc_off_sys_st));

	//printf("pt_shd_data=%p \n", pt_shd_data);

	memset(pt_shd_data, 0, sizeof(*pt_shd_data));

	// load elf

	load_info_t ld_dat;
	ld_dat.executable = (char*)mch_epiphany_elf_path;
	ld_dat.dev = &dev;
	ld_dat.row = 0;
	ld_dat.col = 0;
	ld_dat.rows = platform.rows;
	ld_dat.cols = platform.cols;
	ld_dat.all_module_names = NULL;
	ld_dat.all_module_addrs = NULL;
	ld_dat.root_nn = mch_first_load_workeru_nn;

	e_reset_group(&dev);

	#ifdef MC_PLL_LOADING
		int err = mcl_load_root(&ld_dat); 
	#else
		int err = mc_load_group(&ld_dat); 
	#endif

	if(err == E_ERR){
		mch_abort_func(203, "mc_manageru_init (3). ERROR: Loading_group_failed.\n");
	}

	// init shared data.

	pt_shd_data->magic_id = MC_MAGIC_ID;
	MCH_CK(pt_shd_data->magic_id == MC_MAGIC_ID);

	pt_shd_data->pt_this_from_znq = pt_shd_data;
	pt_shd_data->wrk_sys = *g_sys_sz;
	MCH_CK(mch_ck_sys_data(&(pt_shd_data->wrk_sys)));

	pt_shd_data->znq_shared_mem_base = (mc_addr_t)MCH_EXTERNAL_RAM_BASE_PT;
	pt_shd_data->eph_shared_mem_base = (mc_addr_t)(lk_dat->extnl_ram_orig);

	//printf("eph_shared_mem_base= %p \n", (void*)(pt_shd_data->eph_shared_mem_base));

	pt_shd_data->pt_manageru_kernel = mc_null;
	pt_shd_data->tot_modules = mcl_module_names_sz;

	pt_shd_data->first_load_workeru_id = mc_nn_to_id(mch_first_load_workeru_nn);
}

void
mc_start_first_workeru(){
	e_epiphany_t & dev = mch_glb_dev;
	mc_workeru_co_t row, col;
	row = mc_nn_to_ro(mch_first_load_workeru_nn);
	col = mc_nn_to_co(mch_first_load_workeru_nn);
	e_start(&dev, row, col);
}

#define mc_init_2d_bool_arr(arr, rows, cols, val) \
{ \
	for(int aa = 0; aa < rows; aa++){ \
		for(int bb = 0; bb < cols; bb++){ \
			arr[aa][bb] = val; \
		} \
	} \
} \


void
mc_manageru_run(){
	e_epiphany_t & dev = mch_glb_dev;

	//mc_link_syms_data_st* lk_dat = &(MC_EXTERNAL_RAM_LOAD_DATA);
	mc_off_sys_st* pt_shd_data = mcz_pt_external_manageru_data_obj;

	mc_workeru_co_t row, col, max_row, max_col;
	mc_workeru_nn_t tot_workerus;
	mc_workeru_id_t workeru_id;
	char f_nm[200];

	//max_row = 1;
	//max_col = 2;
	max_row = dev.rows;
	max_col = dev.cols;

	tot_workerus = max_row * max_col;
	MCH_CK(tot_workerus <= mc_out_num_workerus);

	char* all_f_nam[tot_workerus];
	FILE* all_fps[tot_workerus];
	bool all_f_locks[tot_workerus];

	memset(all_f_nam, 0, (sizeof(char*) * tot_workerus));
	memset(all_fps, 0, (sizeof(FILE*) * tot_workerus));
	memset(all_f_locks, false, (sizeof(bool) * tot_workerus));

	for (row=0; row < max_row; row++){
		for (col=0; col < max_col; col++){
			//workeru_id = (row + platform.row) * 64 + col + platform.col;
			workeru_id = mc_ro_co_to_id(row, col);
			mc_workeru_nn_t num_workeru = mc_id_to_nn(workeru_id);

			//printf("STARTING WORKERU 0x%03x (%2d,%2d) NUM=%d\n", workeru_id, row, col, num_workeru);

			memset(&f_nm, 0, sizeof(f_nm));
			sprintf(f_nm, "log_workeru_%02d.txt", num_workeru);

			all_f_nam[num_workeru] = strdup((const char*)f_nm);
			mch_reset_log_file(all_f_nam[num_workeru]);

			FILE* flog = fopen(f_nm, "a");
			if(flog == NULL){
				fprintf(stderr, "ERROR. Can NOT open file %s\n", f_nm);
				return;
			}
			all_fps[num_workeru] = flog;
			
			// init shared data.
			pt_shd_data->sys_workerus[num_workeru].magic_id = MC_MAGIC_ID;
			MCH_CK(pt_shd_data->sys_workerus[num_workeru].magic_id == MC_MAGIC_ID);

			mc_workeru_out_st* pt_buff = &(pt_shd_data->sys_out_buffs[num_workeru]);

			pt_buff->magic_id = MC_MAGIC_ID;
			MCH_CK(pt_buff->magic_id == MC_MAGIC_ID);

			mc_rr_init(&(pt_buff->rd_arr), MC_OUT_BUFF_SZ, pt_buff->buff, 1);

			// Start one workeru
			//e_start(&dev, row, col);
		}
	}

	#ifdef MC_PLL_LOADING
		mc_start_first_workeru();
	#else
		mc_start_all_workerus();
	#endif

	bool workeru_started[tot_workerus];
	memset(workeru_started, 0, (sizeof(bool) * tot_workerus));

	bool workeru_finished[tot_workerus];
	memset(workeru_finished, 0, (sizeof(bool) * tot_workerus));

	/*
	bool workeru_started[max_row][max_col];
	mc_init_2d_bool_arr(workeru_started, max_row, max_col, 0);

	bool workeru_finished[max_row][max_col];
	mc_init_2d_bool_arr(workeru_finished, max_row, max_col, 0);
	*/

	bool has_work = true;	
	while(has_work){
		sched_yield();				//yield
		has_work = false;

		kernel* ker = MCK_KERNEL;
		if(ker != mc_null){
			ker->handle_manageru_missives();
			has_work = ker->did_work;
		}

		for (row=0; row < max_row; row++){
			for (col=0; col < max_col; col++){
				//workeru_id = (row + platform.row) * 64 + col + platform.col;
				workeru_id = mc_ro_co_to_id(row, col);
				mc_workeru_nn_t num_workeru = mc_id_to_nn(workeru_id);
				mc_off_workeru_st* sh_dat_1 = &(pt_shd_data->sys_workerus[num_workeru]);
				mc_workeru_out_st* pt_buff = &(pt_shd_data->sys_out_buffs[num_workeru]);

				// Wait for workeru program execution to start
				if((sh_dat_1->workeru_data == 0x0) || (sh_dat_1->is_finished == 0x0)){
					has_work = true;
					MCH_CK(sh_dat_1->magic_id == MC_MAGIC_ID);
					continue;
				}
				
				MCH_CK(sh_dat_1->magic_id == MC_MAGIC_ID);
				MCH_CK(	(sh_dat_1->is_finished == MC_NOT_FINISHED_VAL) ||
						(sh_dat_1->is_finished == MC_FINISHED_VAL)
				);

				MCH_CK(sh_dat_1->ck_workeru_id == workeru_id);
				if(! workeru_started[num_workeru] && (sh_dat_1->is_finished == MC_NOT_FINISHED_VAL)){ 
					workeru_started[num_workeru] = true;
					//printf("Waiting for finish 0x%03x (%2d,%2d) NUM=%d\n", 
					//			workeru_id, row, col, num_workeru);
				}

				// wait for finish
				if(sh_dat_1->is_finished == MC_NOT_FINISHED_VAL){
					has_work = true;

					mch_print_out_buffer(all_fps[num_workeru], &(all_f_locks[num_workeru]), 
							&(pt_buff->rd_arr), all_f_nam[num_workeru], num_workeru);

					if(sh_dat_1->is_waiting){
						if(sh_dat_1->is_waiting == MC_WAITING_ENTER){
							print_workeru_info(sh_dat_1, &dev, row, col);
							mch_get_enter(row, col);
						}
						if(sh_dat_1->is_waiting == MC_WAITING_BUFFER){
							mch_print_out_buffer(all_fps[num_workeru], &(all_f_locks[num_workeru]), &(pt_buff->rd_arr), 
										all_f_nam[num_workeru], num_workeru);
						}
						
						int SYNC = (1 << E_SYNC);
						sh_dat_1->is_waiting = MC_NOT_WAITING;
						if (ee_write_reg(&dev, row, col, E_REG_ILATST, SYNC) == E_ERR) {
							printf("ERROR sending SYNC to (%d, %d) \n", row, col);
							break;
						}
					}
				} else {
					MCH_CK(sh_dat_1->is_finished == MC_FINISHED_VAL);
	
					if(! workeru_finished[num_workeru]){
						workeru_finished[num_workeru] = true;

						mch_print_out_buffer(all_fps[num_workeru], &(all_f_locks[num_workeru]), 
								&(pt_buff->rd_arr), all_f_nam[num_workeru], num_workeru);

						MCH_CK(mch_rr_ck_zero(&(pt_buff->rd_arr)));

						printf("Finished %d %p \n", num_workeru, (void*)(uintptr_t)(sh_dat_1->is_finished));
						//print_workeru_info(sh_dat_1, &dev, row, col);
						print_exception_case(sh_dat_1, &dev, row, col);
					}

				}
			}
		}
	} // while

	/*
	mc_sys_sz_st* g_sys_sz = MC_SYS_SZ;

	printf("sys_sz->xx=%d\n", g_sys_sz->xx);
	printf("sys_sz->yy=%d\n", g_sys_sz->yy);
	printf("sys_sz->xx_sz=%d\n", g_sys_sz->xx_sz);
	printf("sys_sz->yy_sz_pw2=%d\n", g_sys_sz->yy_sz_pw2);

	printf("SHD_DATA_addr_as_seen_from_eph=%p\n", pt_shd_data->pt_this_from_eph);
	printf("SHD_DATA_addr_as_seen_from_znq=%p\n", pt_shd_data->pt_this_from_znq);
	printf("SHD_DATA_displacement_from_shd_mem_base_adddr= %p\n", (void*)(lk_dat->extnl_manageru_data_disp));

	printf("pt_shd_data=%p \n", pt_shd_data);
	*/
	//printf("WORKERUS FINISHED \n");

	int nn;
	for (nn=0; nn < tot_workerus; nn++){
		if(all_f_nam[nn] != mc_null){
			free(all_f_nam[nn]);
		}
		if(all_fps[nn] != NULL){
			fclose(all_fps[nn]);
		}
	}
	
	//printf("FREED MEM \n");
}

void
mc_manageru_finish(){
	e_epiphany_t & dev = mch_glb_dev;

	e_reset_group(&dev); 
	e_reset_system();
	
	e_close(&dev);

	//destroy_mspace(mch_glb_load_mspace);
	destroy_mspace(mch_glb_alloc_mspace);

	e_free(&mch_glb_emem);
	e_finalize();

	//prt_manageru_aligns();
}

int main(int argc, char *argv[]) {
	int rr = 0;
	rr = mc_manageru_main(argc, argv);
	return rr;
}

void
mc_start_all_workerus(){
	e_epiphany_t & dev = mch_glb_dev;
	mc_workeru_co_t row, col, max_row, max_col;

	fprintf(stderr, "sizeof(int)=%d \n", sizeof(int));
	fprintf(stderr, "sizeof(uint32_t)=%d \n", sizeof(uint32_t));
	MCH_CK(sizeof(int) == sizeof(uint32_t));

	max_row = dev.rows;
	max_col = dev.cols;

	//int		*pto;
	//off_t to_addr = (off_t)MC_REG_ILATST;
	//if (to_addr >= E_REG_R0){
	//	to_addr = to_addr - E_REG_R0;
	//}
	//int SYNC = (1 << E_SYNC);

	for (row=0; row < max_row; row++){
		for (col=0; col < max_col; col++){
			//pto = (int *) (dev.workeru[row][col].regs.base + to_addr);
			//*pto = SYNC;

			e_start(&dev, row, col);
		}
	}
}

