

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


//define _GNU_SOURCE

#include <execinfo.h>
#include <inttypes.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thread_ptd.hh"
#include "booter.h"
#include "string_hst.hh"

//include "workerus_main.h"
#include "dlmalloc.h"

#include "dyn_mem.h"
#include "cell.hh"

// =====================================================================================

mc_workeru_id_t mch_first_load_workeru_nn = 0;

#define MCM_EXTERNAL_RAM_ORIG 0x8e000000

uint8_t mcm_dlmalloc_heap[MCM_DLMALLOC_HEAP_SZ];

mspace mcm_glb_mspace;

ptd_info_t*	mcm_MANAGERU_PTD_INFO = mc_null;

void
mcm_get_call_stack_trace(size_t trace_strs_sz, char** trace_strs) {
	void* 		trace[trace_strs_sz];

	mc_memset((uint8_t*)trace, 0, trace_strs_sz * sizeof(void*));
	mc_memset((uint8_t*)trace_strs, 0, trace_strs_sz * sizeof(char*));

	size_t trace_sz = backtrace(trace, trace_strs_sz);
	char **stack_strings = backtrace_symbols(trace, trace_sz);
	if(stack_strings != NULL){
		for( size_t ii = 1; ii < trace_sz; ii++ ) {
			if(ii >= trace_strs_sz){ break; }
			trace_strs[ii] = stack_strings[ii];
			fprintf(stderr, "%s \n", stack_strings[ii]);
		}
		free( stack_strings );
	}
}

void 
mch_prt_workeru_call_stack_ptd(thread_info_t& thd_inf){
	char** trace = thd_inf.thd_ptd.ptd_glb_sys_data.mck_dbg_call_nams_stack_trace;
	if(trace == mc_null){
		return;
	}
	if(trace[0] == mc_null){
		return;
	}
	mch_out << "---------------------------------------------------\n";
	mch_out << "STACK_TRACE for workeru num " << std::dec << thd_inf.thd_ptd.ptd_num;
	mch_out << " with workeru_id = 0x" << std::hex << thd_inf.thd_ptd.ptd_workeru_id << "\n";
	for(int aa = 0; aa < MC_MAX_CALL_STACK_SZ; aa++){
		if(trace[aa] == mc_null){ break; }
		mch_out << trace[aa] << "\n";
	}
	mch_out << "---------------------------------------------------\n";
}

bool
ck_all_workeru_ids(){
	if (ALL_THREADS_INFO == NULL){
		mch_abort_func(1, "ck_all_workeru_ids. NULL ALL_THREADS_INFO \n");
	}
	for(int aa = 0; aa < TOT_THREADS; aa++){
		mc_workeru_id_t koid = mc_nn_to_id(aa);
		if(ALL_THREADS_INFO[aa].thd_ptd.ptd_workeru_id != koid){
			mch_abort_func(1, "ck_all_workeru_ids. BAD WORKERU ID \n");
		}
	}
	return true;
}

void
mch_load_map_rec(mc_workeru_id_t parent, mc_load_map_st* mp){
	MCH_CK(mp != mc_null);

	mc_workeru_nn_t nn_workeru = mp->num_workeru;
	mc_workeru_id_t koid = mc_nn_to_id(nn_workeru);

	MCH_CK(nn_workeru >= 0);
	MCH_CK(nn_workeru < TOT_THREADS);
	thread_info_t& thd_inf = ALL_THREADS_INFO[nn_workeru];

	thd_inf.thd_ptd.ptd_map_loaded = mp;
	thd_inf.thd_ptd.ptd_map_parent_workeru_id = parent;
	thd_inf.thd_ptd.ptd_map_tot_children = 0;

	if(mp->childs == mc_null){ return; }

	int aa = 0;
	mc_load_map_st* ch_map = (mp->childs)[aa];
	while(ch_map != mc_null){
		mch_load_map_rec(koid, ch_map);

		aa++;
		ch_map = (mp->childs)[aa];
	}
	thd_inf.thd_ptd.ptd_map_tot_children = aa;
}

void
mch_load_map(){
	//return;
	mc_load_map_st* ld_mp = mc_get_first_load_map();
	if(mck_first_load_map != mc_null){
		ld_mp = mck_first_load_map;
	}
	MCH_CK(ld_mp != mc_null);
	mch_load_map_rec(0, ld_mp);
}

void
mc_manageru_init(){
	ALL_THREADS_INFO = mc_null;
	MANAGERU_THREAD_ID = pthread_self();

	memset(mcm_dlmalloc_heap, 0, sizeof(mcm_dlmalloc_heap));
	mcm_glb_mspace = create_mspace_with_base(mcm_dlmalloc_heap, MCM_DLMALLOC_HEAP_SZ, 0);

	mcm_MANAGERU_PTD_INFO = mc_malloc32(ptd_info_t, 1);

	mc_init_glb_sys_sz(&(mcm_MANAGERU_PTD_INFO->ptd_system_sz));

	mc_workeru_co_t max_row = mc_tot_xx_sys;
	mc_workeru_co_t max_col = mc_tot_yy_sys;
	mc_workeru_id_t workeru_id = mc_ro_co_to_id(max_row + 1, max_col + 1);
	mcm_MANAGERU_PTD_INFO->ptd_workeru_id = workeru_id;
	mcm_MANAGERU_PTD_INFO->ptd_num = ~0;

	memset(&mch_external_ram_load_data, 0, sizeof(mc_link_syms_data_st));

	TOT_THREADS = mc_tot_nn_sys;
	ALL_THREADS_INFO = (thread_info_t *)calloc(TOT_THREADS, sizeof(thread_info_t));
	if (ALL_THREADS_INFO == NULL){
		mch_abort_func(1, "manageru_main. NULL ALL_THREADS_INFO \n");
	}

	printf("TOT_THREADS = %d\n", TOT_THREADS);

	mc_off_sys_st* pt_shd_data = MCK_PT_EXTERNAL_MANAGERU_DATA;
	MCH_CK(sizeof(*pt_shd_data) == sizeof(mc_off_sys_st));
	PTD_64_CODE(printf("sizeof(*pt_shd_data)=%ld\n", sizeof(*pt_shd_data)));
	PTD_32_CODE(printf("sizeof(*pt_shd_data)=%d\n", sizeof(*pt_shd_data)));

	// init shared data.
	memset(pt_shd_data, 0, sizeof(*pt_shd_data));

	pt_shd_data->magic_id = MC_MAGIC_ID;
	MCH_CK(pt_shd_data->magic_id == MC_MAGIC_ID);

	mc_sys_sz_st* sys_sz = MC_SYS_SZ;

	pt_shd_data->wrk_sys = *sys_sz;
	MCH_CK(mch_ck_sys_data(&(pt_shd_data->wrk_sys)));

	pt_shd_data->pt_manageru_kernel = mc_null;

	pt_shd_data->first_load_workeru_id = mc_nn_to_id(mch_first_load_workeru_nn);
}

void
mc_manageru_run()
{
	mc_off_sys_st* pt_shd_data = MCK_PT_EXTERNAL_MANAGERU_DATA;
	//mc_sys_sz_st* sys_sz = MC_SYS_SZ;

	mc_workeru_id_t workeru_id;
	mc_workeru_co_t row, col, max_row, max_col;
	char f_nm[200];

	mc_workeru_nn_t tot_workerus = mc_out_num_workerus;

	char* all_f_nam[tot_workerus];
	FILE* all_fps[tot_workerus];
	bool all_f_locks[tot_workerus];

	memset(all_f_nam, 0, (sizeof(char*) * tot_workerus));
	memset(all_fps, 0, (sizeof(FILE*) * tot_workerus));
	memset(all_f_locks, false, (sizeof(bool) * tot_workerus));

	mch_load_map();
	
	max_row = 1;
	max_col = 2;
	max_row = mc_tot_xx_sys;
	max_col = mc_tot_yy_sys;

	for (row=0; row < max_row; row++){
		for (col=0; col < max_col; col++){
			workeru_id = mc_ro_co_to_id(row, col);
			mc_workeru_nn_t num_workeru = mc_id_to_nn(workeru_id);

			thread_info_t& thd_inf = ALL_THREADS_INFO[num_workeru];
			thd_inf.thd_ptd.ptd_num = num_workeru;
			mc_uint16_to_hex_bytes(thd_inf.thd_ptd.ptd_num, (uint8_t*)(thd_inf.thd_ptd.ptd_name));
			thd_inf.thd_ptd.ptd_workeru_id = workeru_id;
			thd_inf.thd_ptd.ptd_workeru_func = &mc_workerus_main;
			thd_inf.thd_log_fnam = mc_null;

			//printf("STARTING WORKERU 0x%03x (%2d,%2d) NUM=%d\n", workeru_id, row, col, num_workeru);

			if(num_workeru < mc_out_num_workerus){
				memset(&f_nm, 0, sizeof(f_nm));
				sprintf(f_nm, "log_workeru_%02d.txt", num_workeru);
				all_f_nam[num_workeru] = strdup((const char*)f_nm);
				thd_inf.thd_log_fnam = strdup((const char*)f_nm);
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
			}
			
			// Start one workeru ptd thread

			int ss = pthread_create(&thd_inf.thd_ptd.ptd_id, NULL,
								&thread_start, &thd_inf);
			if (ss != 0){
				mch_abort_func(ss, "manageru_main. Cannot pthread_create");
			}
		}
	}

	MCH_CK(ck_all_workeru_ids());

	bool workeru_started[tot_workerus];
	memset(workeru_started, 0, (sizeof(bool) * tot_workerus));

	bool workeru_finished[tot_workerus];
	memset(workeru_finished, 0, (sizeof(bool) * tot_workerus));

	/*
	bool workeru_started[max_row][max_col];
	memset(workeru_started, 0, sizeof(workeru_started));

	bool workeru_finished[max_row][max_col];
	memset(workeru_finished, 0, sizeof(workeru_finished));
	*/

	bool has_work = true;	
	while(has_work){
		sched_yield();				//yield
		has_work = false;

		if(MCH_ABORT_EXEC){
			mch_abort_func(1, "ABORT_CALLED_FROM_PTD_THREAD \n");
		}

		kernel* ker = MCK_KERNEL;
		if(ker != mc_null){
			ker->handle_manageru_missives();
			has_work = ker->did_work;
		}

		for (row=0; row < max_row; row++){
			for (col=0; col < max_col; col++){
				workeru_id = mc_ro_co_to_id(row, col);
				mc_workeru_nn_t num_workeru = mc_id_to_nn(workeru_id);
				mc_off_workeru_st* sh_dat_1 = &(pt_shd_data->sys_workerus[num_workeru]);
				mc_workeru_out_st* pt_buff = &(pt_shd_data->sys_out_buffs[num_workeru]);

				thread_info_t& thd_inf = ALL_THREADS_INFO[num_workeru];

				// Wait for workeru program execution to finish.
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

				//mck_glb_sys_st* inco = &thd_inf.thd_ptd.ptd_glb_sys_data.in_workeru_shd;
				mck_glb_sys_st* inco = &thd_inf.thd_ptd.ptd_glb_sys_data;

				// wait for finish
				if(sh_dat_1->is_finished == MC_NOT_FINISHED_VAL){
					has_work = true;

					mch_print_out_buffer(all_fps[num_workeru], &(all_f_locks[num_workeru]), 
							&(pt_buff->rd_arr), all_f_nam[num_workeru], num_workeru);

					if(sh_dat_1->is_waiting){
						if(sh_dat_1->is_waiting == MC_WAITING_ENTER){
							mch_get_enter(row, col);
						}
						if(sh_dat_1->is_waiting == MC_WAITING_BUFFER){
							mch_print_out_buffer(all_fps[num_workeru], &(all_f_locks[num_workeru]), &(pt_buff->rd_arr), 
												all_f_nam[num_workeru], num_workeru);
						}
						
						sh_dat_1->is_waiting = MC_NOT_WAITING;

						thd_inf.thd_ptd.ptd_glb_sys_data.mck_sync_signal = 1;	// SEND signal
					}
				} else {
					MCH_CK(sh_dat_1->is_finished == MC_FINISHED_VAL);
	
					if(! workeru_finished[num_workeru]){

						workeru_finished[num_workeru] = true;

						mch_print_out_buffer(all_fps[num_workeru], &(all_f_locks[num_workeru]), 
								&(pt_buff->rd_arr), all_f_nam[num_workeru], num_workeru);

						MCH_CK(mch_rr_ck_zero(&(pt_buff->rd_arr)));

						/*printf("Finished\n");
						int err2 = mch_prt_in_workeru_shd_dat(inco);
						if(err2){
							break;
						}*/
						if(inco->exception_code != mck_invalid_exception){
							//mch_prt_exception(inco);
							mch_prt_in_workeru_shd_dat(inco);
						}
						if(inco->dbg_error_code != 0){
							mch_prt_in_workeru_shd_dat(inco);
						}
						mch_prt_workeru_call_stack_ptd(thd_inf);
					}

				}
			}
		}
	} // while

	/*
	printf("sys_sz->xx=%d\n", sys_sz->xx);
	printf("sys_sz->yy=%d\n", sys_sz->yy);
	printf("sys_sz->xx_sz=%d\n", sys_sz->xx_sz);
	printf("sys_sz->yy_sz_pw2=%d\n", sys_sz->yy_sz_pw2);
	*/
	
	int tnum;
	for (tnum = 0; tnum < TOT_THREADS; tnum++) {
		void *res;
		int ss = pthread_join(ALL_THREADS_INFO[tnum].thd_ptd.ptd_id, &res);
		if(ss != 0){
			mch_abort_func(ss, "mc_manageru_finish. Cannot join thread.");
		}
	}

	int nn;
	for (nn=0; nn < mc_out_num_workerus; nn++){
		if(all_f_nam[nn] != mc_null){
			free(all_f_nam[nn]);
		}
		if(all_fps[nn] != NULL){
			fclose(all_fps[nn]);
		}
	}

}

void
mc_manageru_finish()
{

	free(ALL_THREADS_INFO);
	destroy_mspace(mcm_glb_mspace);
}

/*
#include "tak_mak.hh"
void test_dlmalloc_align(){
	unsigned long init_val = 10;
	tak_mak gg(init_val);

	for(int aa = 0; aa < 1000; aa++){
		long val = gg.gen_rand_int32_ie(1, 1000);
		printf("%ld ", val);
	}
	printf("\n\n");
	
}*/

int main(int argc, char *argv[]) {
	int rr = 0;
	rr = mc_manageru_main(argc, argv);
	return rr;
	//test_dlmalloc_align();
}

