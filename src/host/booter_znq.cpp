

#include <math.h>
#include <sched.h>
#include <string.h>

#include <e-hal.h>
#include <epiphany-hal-api-local.h>

#include "booter.h"
#include "core_loader_znq.h"

#include "dlmalloc.h"

//=====================================================================================

uint8_t* BJH_EXTERNAL_RAM_BASE_PT = bj_null;

//char* bjh_epiphany_elf_path = (const_cast<char *>("the_epiphany_executable.elf"));

mspace bjh_glb_load_mspace;
mspace bjh_glb_alloc_mspace;

e_mem_t bjh_glb_emem;
e_epiphany_t bjh_glb_dev;

void
print_core_info(bj_off_core_st* sh_dat_1, e_epiphany_t* dev, unsigned row, unsigned col){
	bj_in_core_st inco;
	memset(&inco, 0, sizeof(bj_in_core_st));
	e_read(dev, row, col, (uint32_t)(sh_dat_1->core_data), &inco, sizeof(inco));
	int err = bjh_prt_in_core_shd_dat(&inco);
	if(err){
		bjh_abort_func((bj_addr_t)(print_core_info), "print_core_info failed");
	}
	
	void* 	trace[BJ_MAX_CALL_STACK_SZ];
	memset(trace, 0, sizeof(trace));
	if(inco.dbg_stack_trace != bj_null){
		e_read(dev, row, col, (uint32_t)(inco.dbg_stack_trace), trace, sizeof(trace));
	}
	bjh_prt_core_call_stack(bjh_epiphany_elf_path, BJ_MAX_CALL_STACK_SZ, trace);
}

void
bj_host_init(){
	e_epiphany_t & dev = bjh_glb_dev;

	e_platform_t platform;

	bj_link_syms_data_st* lk_dat = &(BJ_EXTERNAL_RAM_LOAD_DATA);
	bjh_read_eph_link_syms(bjh_epiphany_elf_path, lk_dat);

	if(lk_dat->extnl_ram_orig == 0) {
		printf("ERROR: Can't read external memory location from '%s'\n", bjh_epiphany_elf_path);
		printf("Make sure linker script for '%s' defines LD_EXTERNAL_* symbols\n\n", bjh_epiphany_elf_path);
		bjh_abort_func(201, "host_init_1. ERROR: Bad ELF\n");
	}

	// IMPORTANT NOTE:
	// Vales in HDF MUST match values in the linker script:
	// 			EMEM_BASE_ADDRESS == LD_EXTERNAL_RAM_ORIG
	// 			EMEM_SIZE == LD_EXTERNAL_RAM_SIZE

	// Current HDF: the value of EPIPHANY_HDF enviroment variable because e_initi is called with NULL
	// Current value of EPIPHANY_HDF: /opt/adapteva/esdk/bsps/current/platform.hdf
	// Current value of EMEM_BASE_ADDRESS in HDF: 0x8e000000
	// Current link script: bj-ld-script.ldf
	// Observe that this address is AS SEEN FROM THE EPIPHANY side. NOT as seen from the Zynq (host) side.

	// sys init

	e_init(NULL);
	e_reset_system();
	e_get_platform_info(&platform);

	if (e_alloc(&bjh_glb_emem, 0, lk_dat->extnl_ram_size)) {
		bjh_abort_func(202, "host_init_2. ERROR: Can't allocate external memory buffer!\n\n");
	}

	BJH_EXTERNAL_RAM_BASE_PT = ((uint8_t*)bjh_glb_emem.base);

	uint8_t* extnl_load_base = bjh_disp_to_pt(lk_dat->extnl_load_disp);
	uint8_t* extnl_alloc_base = bjh_disp_to_pt(lk_dat->extnl_alloc_disp);
	
	bjh_glb_load_mspace = create_mspace_with_base(extnl_load_base, lk_dat->extnl_load_size, 0);
	bjh_glb_alloc_mspace = create_mspace_with_base(extnl_alloc_base, lk_dat->extnl_alloc_size, 0);

	// dev init
	
	e_open(&dev, 0, 0, platform.rows, platform.cols);

	bj_sys_sz_st* g_sys_sz = BJK_GLB_SYS_SZ;
	bjh_init_glb_sys_sz_with_dev(g_sys_sz, &dev);

	bj_off_sys_st* pt_shd_data = (bj_off_sys_st*)bjh_disp_to_pt(lk_dat->extnl_data_disp);
	BJH_CK(sizeof(*pt_shd_data) == sizeof(bj_off_sys_st));

	printf("pt_shd_data=%p \n", pt_shd_data);

	memset(pt_shd_data, 0, sizeof(*pt_shd_data));

	// load elf

	load_info_t ld_dat;
	ld_dat.executable = (char*)bjh_epiphany_elf_path;
	ld_dat.dev = &dev;
	ld_dat.row = 0;
	ld_dat.col = 0;
	ld_dat.rows = platform.rows;
	ld_dat.cols = platform.cols;
	ld_dat.all_module_names = NULL;
	ld_dat.all_module_addrs = NULL;

	e_reset_group(&dev);
	int err = bj_load_group(&ld_dat); 
	if(err == E_ERR){
		bjh_abort_func(203, "host_init_3. ERROR: Loading_group_failed.\n");
	}

	// init shared data.

	pt_shd_data->magic_id = BJ_MAGIC_ID;
	BJH_CK(pt_shd_data->magic_id == BJ_MAGIC_ID);

	pt_shd_data->pt_this_from_znq = pt_shd_data;
	pt_shd_data->wrk_sys = *g_sys_sz;
	BJH_CK(bjh_ck_sys_data(&(pt_shd_data->wrk_sys)));
}

void
bj_host_run(){
	e_epiphany_t & dev = bjh_glb_dev;

	bj_link_syms_data_st* lk_dat = &(BJ_EXTERNAL_RAM_LOAD_DATA);
	bj_off_sys_st* pt_shd_data = (bj_off_sys_st*)bjh_disp_to_pt(lk_dat->extnl_data_disp);

	bj_core_co_t row, col, max_row, max_col;
	bj_core_nn_t tot_cores;
	bj_core_id_t core_id;
	char f_nm[200];

	max_row = 1;
	max_col = 2;
	max_row = dev.rows;
	max_col = dev.cols;

	tot_cores = max_row * max_col;
	BJH_CK(tot_cores <= bj_out_num_cores);

	char* all_f_nam[tot_cores];
	memset(&all_f_nam, 0, sizeof(all_f_nam));

	for (row=0; row < max_row; row++){
		for (col=0; col < max_col; col++){
			//core_id = (row + platform.row) * 64 + col + platform.col;
			core_id = bj_ro_co_to_id(row, col);
			bj_core_nn_t num_core = bj_id_to_nn(core_id);

			printf("STARTING CORE 0x%03x (%2d,%2d) NUM=%d\n", core_id, row, col, num_core);

			memset(&f_nm, 0, sizeof(f_nm));
			sprintf(f_nm, "log_core_%02d.txt", num_core);

			all_f_nam[num_core] = strdup((const char*)f_nm);
			
			// init shared data.
			pt_shd_data->sys_cores[num_core].magic_id = BJ_MAGIC_ID;
			BJH_CK(pt_shd_data->sys_cores[num_core].magic_id == BJ_MAGIC_ID);

			bj_core_out_st* pt_buff = &(pt_shd_data->sys_out_buffs[num_core]);

			pt_buff->magic_id = BJ_MAGIC_ID;
			BJH_CK(pt_buff->magic_id == BJ_MAGIC_ID);

			bj_rr_init(&(pt_buff->rd_arr), BJ_OUT_BUFF_SZ, pt_buff->buff, 1);

			// Start one core
			e_start(&dev, row, col);
		}
	}

	bool core_started[max_row][max_col];
	memset(core_started, 0, sizeof(core_started));

	bool core_finished[max_row][max_col];
	memset(core_finished, 0, sizeof(core_finished));

	bool has_work = true;	
	while(has_work){
		sched_yield();				//yield
		has_work = false;
		for (row=0; row < max_row; row++){
			for (col=0; col < max_col; col++){
				//core_id = (row + platform.row) * 64 + col + platform.col;
				core_id = bj_ro_co_to_id(row, col);
				bj_core_nn_t num_core = bj_id_to_nn(core_id);
				bj_off_core_st* sh_dat_1 = &(pt_shd_data->sys_cores[num_core]);
				bj_core_out_st* pt_buff = &(pt_shd_data->sys_out_buffs[num_core]);

				// Wait for core program execution to start
				if((sh_dat_1->core_data == 0x0) || (sh_dat_1->is_finished == 0x0)){
					has_work = true;
					BJH_CK(sh_dat_1->magic_id == BJ_MAGIC_ID);
					continue;
				}
				
				BJH_CK(sh_dat_1->magic_id == BJ_MAGIC_ID);
				BJH_CK(	(sh_dat_1->is_finished == BJ_NOT_FINISHED_VAL) ||
						(sh_dat_1->is_finished == BJ_FINISHED_VAL)
				);

				BJH_CK(sh_dat_1->ck_core_id == core_id);
				if(! core_started[row][col] && (sh_dat_1->is_finished == BJ_NOT_FINISHED_VAL)){ 
					core_started[row][col] = true;
					printf("Waiting for finish 0x%03x (%2d,%2d) NUM=%d\n", 
								core_id, row, col, num_core);
				}

				// wait for finish
				if(sh_dat_1->is_finished == BJ_NOT_FINISHED_VAL){
					has_work = true;
					bjh_print_out_buffer(&(pt_buff->rd_arr), all_f_nam[num_core], num_core);
					if(sh_dat_1->is_waiting){
						if(sh_dat_1->is_waiting == BJ_WAITING_ENTER){
							print_core_info(sh_dat_1, &dev, row, col);
							bjh_get_enter(row, col);
						}
						if(sh_dat_1->is_waiting == BJ_WAITING_BUFFER){
							bjh_print_out_buffer(&(pt_buff->rd_arr), all_f_nam[num_core], num_core);
						}
						
						int SYNC = (1 << E_SYNC);
						sh_dat_1->is_waiting = BJ_NOT_WAITING;
						if (ee_write_reg(&dev, row, col, E_REG_ILATST, SYNC) == E_ERR) {
							printf("ERROR sending SYNC to (%d, %d) \n", row, col);
							break;
						}
					}
				} else {
					BJH_CK(sh_dat_1->is_finished == BJ_FINISHED_VAL);
	
					if(! core_finished[row][col]){
						core_finished[row][col] = true;

						bjh_print_out_buffer(&(pt_buff->rd_arr), all_f_nam[num_core], num_core);
						BJH_CK(bjh_rr_ck_zero(&(pt_buff->rd_arr)));

						printf("Finished\n");
						print_core_info(sh_dat_1, &dev, row, col);
					}

				}
			}
		}
	} // while

	bj_sys_sz_st* g_sys_sz = BJK_GLB_SYS_SZ;

	printf("sys_sz->xx=%d\n", g_sys_sz->xx);
	printf("sys_sz->yy=%d\n", g_sys_sz->yy);
	printf("sys_sz->xx_sz=%d\n", g_sys_sz->xx_sz);
	printf("sys_sz->yy_sz_pw2=%d\n", g_sys_sz->yy_sz_pw2);

	printf("SHD_DATA_addr_as_seen_from_eph=%p\n", pt_shd_data->pt_this_from_eph);
	printf("SHD_DATA_addr_as_seen_from_znq=%p\n", pt_shd_data->pt_this_from_znq);
	printf("SHD_DATA_displacement_from_shd_mem_base_adddr= %p\n", (void*)(lk_dat->extnl_data_disp));

	printf("pt_shd_data=%p \n", pt_shd_data);

	int nn;
	for (nn=0; nn < tot_cores; nn++){
		if(all_f_nam[nn] != bj_null){
			free(all_f_nam[nn]);
		}
	}
	
}

void
bj_host_finish(){
	e_epiphany_t & dev = bjh_glb_dev;

	e_reset_group(&dev); 
	e_reset_system();
	
	e_close(&dev);
	
	destroy_mspace(bjh_glb_load_mspace);
	destroy_mspace(bjh_glb_alloc_mspace);

	e_free(&bjh_glb_emem);
	e_finalize();

	//prt_host_aligns();
}

int main(int argc, char *argv[]) {
	int rr = 0;
	rr = bj_host_main(argc, argv);
	return rr;
}

