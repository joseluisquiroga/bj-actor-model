

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


#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include "shared.h"
#include "booter.h"

//include "test_align.h"

mc_link_syms_data_st mch_external_ram_load_data;

bool MCH_LOAD_WITH_MEMCPY = false;

bool MCH_ABORT_EXEC = false;

void 
mch_abort_func(long val, const char* msg){
	fprintf(stderr, "\nABORTING_WITH_ERR=%ld %s\n", val, msg);
	PTD_CODE(mch_ptr_call_stack_trace(mc_null););
	
	exit(EXIT_FAILURE);
}

bool 
mch_call_assert(bool vv_ck, const char* file, int line, const char* ck_str, const char* msg){
	
	if(! vv_ck){
		fprintf(stderr, "ASSERT '%s' FAILED\nFILE= %s\nLINE=%d \n", ck_str, file, line);
		//mc_out << get_stack_trace(file, line) << mc_eol;
		if(msg != NULL){
			fprintf(stderr, "MSG=%s\n", msg);
		}
		fprintf(stderr, "------------------------------------------------------------------\n");
	}
	assert(vv_ck);
	return vv_ck;
}

bool
mch_file_append(char* the_pth, char* the_data, long the_sz){
	int fd = 0;
	
	if((fd = open(the_pth, O_RDWR|O_CREAT|O_APPEND, 0777)) == -1){
		return false;
	}
	write(fd, the_data, the_sz);
	close(fd);

	return true;
}

bool mch_ck_sys_data(mc_sys_sz_st* sys1){
	mc_sys_sz_st* sys0 = MC_SYS_SZ;
	MCH_CK(sys1->xx == sys0->xx);
	MCH_CK(sys1->yy == sys0->yy);
	MCH_CK(sys1->xx_sz == sys0->xx_sz);
	MCH_CK(sys1->yy_sz_pw2 == sys0->yy_sz_pw2);
	return true;
}

void
mch_prt_exception(mck_glb_sys_st* sh_dat){
	for(int aa = 0; aa < 5; aa++){
		if(sh_dat->exception_code == mck_software_exception){
			printf("%x. SOFTWARE EXCEPTION!!\n", sh_dat->the_workeru_id);
		}
		if(sh_dat->exception_code == mck_memory_exception){
			printf("%x. MEMORY EXCEPTION!!\n", sh_dat->the_workeru_id);
		}
	}
}

int mch_prt_in_workeru_shd_dat(mck_glb_sys_st* sh_dat){
	if(sh_dat->magic_id != MC_MAGIC_ID){
		fprintf(stderr, "ERROR with inco.magic_id (0x%08x)\n", sh_dat->magic_id);
		return 1;
	}
	if(sh_dat->magic_end != MC_MAGIC_END){
		fprintf(stderr, "ERROR with inco.magic_end (0x%08x)\n", sh_dat->magic_end);
		return 1;
	}
	fprintf(stderr, "----------------------------------------------------------------------------\n");
	fprintf(stderr, "InWORKERU 0x%03x (%d)\n", sh_dat->the_workeru_id, sh_dat->the_workeru_nn);
	
	ZNQ_CODE(fprintf(stderr, "min_sp=0x%08x \n", sh_dat->dbg_min_sp));

	PTD_64_CODE(fprintf(stderr, "dbg_error_code=0x%08lx \n", sh_dat->dbg_error_code));
	PTD_32_CODE(fprintf(stderr, "dbg_error_code=0x%08x \n", sh_dat->dbg_error_code));
	ZNQ_CODE(fprintf(stderr, "dbg_error_code=0x%08x \n", sh_dat->dbg_error_code));

	if(sh_dat->dbg_error_str != mc_null){
		char* the_str = (char*)mc_workeru_pt_to_manageru_pt(sh_dat->dbg_error_str);
		long sz = 200;
		char buff[sz];
		memset(buff, '\0', sz);
		int ll = strnlen(the_str, sz - 10);
		strncpy(buff, the_str, ll);

		fprintf(stderr, "dbg_error_str=%s \n", buff);
	}

	fprintf(stderr, "dbg_progress_flag=0x%08x \n", sh_dat->dbg_progress_flag);

	if(sh_dat->exception_code != mck_invalid_exception){
		for(int aa = 0; aa < 5; aa++){
			if(sh_dat->exception_code == mck_software_exception){
				fprintf(stderr, "%x. SOFTWARE EXCEPTION!!\n", sh_dat->the_workeru_id);
			}
			if(sh_dat->exception_code == mck_memory_exception){
				fprintf(stderr, "%x. MEMORY EXCEPTION!!\n", sh_dat->the_workeru_id);
			}
		}
	}

	fprintf(stderr, "\n");
	
	return 0;
}

bool
mch_rr_ck_zero(mc_rrarray_st* arr){
	if(arr == mc_null){
		return true;
	}
	uint8_t* dat = arr->data;
	while(dat != arr->end_data){
		uint8_t cc = (uint8_t)(*dat);
		MCH_CK(cc == 0);
		MCH_CK(! isprint(cc));
		dat++;
	}
	return true;
}

void
mch_rr_print(mc_rrarray_st* arr){
	if(arr == mc_null){
		return;
	}
	printf("#####################################\n");
	printf("data=\t%p\n", arr->data);
	printf("end_data=\t%p\n", arr->end_data);
	printf("rd_obj=\t%p\n", arr->rd_obj);
	printf("wr_obj=\t%p\n", arr->wr_obj);
	printf("num_wr_errs=\t%d\n", arr->num_wr_errs);
	printf("wr_err=\t%d\n", arr->wr_err);
	printf("rd_err=\t%d\n", arr->rd_err);
	uint8_t* dat = arr->data;
	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	while(dat != arr->end_data){
		char cc = (char)(*dat);
		if(isprint(cc)){
			printf("%c", cc);
		} 
		/*else {
			printf(".");
		}*/
		dat++;
	}
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}

int
mch_type_sz(mc_type_t tt){
	int sz = 0;
	switch(tt){
		case MC_CHR:
		case MC_I8:
		case MC_UI8:
		case MC_X8:
			sz = 1;
			break;
		case MC_I16:
		case MC_UI16:
		case MC_X16:
			sz = 2;
			break;
		case MC_I32:
		case MC_UI32:
		case MC_X32:
			sz = 4;
			break;
	}
	return sz;
}

void
mch_reset_log_file(char* f_nm){
	int log_fd = 0;
	if((log_fd = creat(f_nm, 0777)) == -1){
		fprintf(stderr, "ERROR. Can NOT reset log file %s\n", f_nm);
		mch_abort_func(1, "ERROR. Can NOT reset log file\n");
		return;
	}
	close(log_fd);
}

void
mch_print_out_buffer(FILE* flog, bool* lock, mc_rrarray_st* arr, char* f_nm, mc_workeru_nn_t num_workeru){
	//FILE* flog = fopen(f_nm, "a");
	if((flog == NULL) || (lock == NULL)){
		fprintf(stderr, "ERROR. NULL file pointer for file %s\n", f_nm);
		mch_abort_func(1, "ERROR. NULL file pointer\n");
		return;
	}

	uint8_t obj[MC_OUT_BUFF_MAX_OMC_SZ];
	while(true){
		uint16_t omc_sz = mc_rr_read_obj(arr, MC_OUT_BUFF_MAX_OMC_SZ, obj);
		if(omc_sz == 0){
			if(arr->rd_err != -4){
				break;
			}
		} else {
			if(omc_sz < 2){
				fprintf(stderr, "ERROR. Got unhandled obj in buffer for %s\n", f_nm);
				continue;
			}
			if(obj[0] == MC_OUT_ABORT){
				MCH_ABORT_EXEC = true;
				continue;
				//mch_abort_func(1, "ABORT CALLED FROM PTD THREAD \n");
			}
			if(obj[0] == MC_OUT_LOCK_LOG){
				if(! (*lock)){
					flockfile(flog);
					(*lock) = true;
				}
				continue;
			}
			if(obj[0] == MC_OUT_UNLOCK_LOG){
				if(*lock){
					funlockfile(flog);
					(*lock) = false;
				}
				continue;
			}

			FILE* fpt = stdout;
			if(obj[0] == MC_OUT_LOG){
				fpt = flog;
			} 
			mc_type_t ot = (mc_type_t)obj[1];
			if(ot == MC_CHR){
				fwrite(obj + 2, omc_sz - 2, 1, fpt);
				continue;
			}
			int osz = mch_type_sz(ot);
			int tot = (omc_sz - 2) / osz;
			int aa;
			uint8_t* pt_num = obj + 2;
			for(aa = 0; aa < tot; aa++, pt_num += osz){
				int istrsz = 50;
				char istr[istrsz];
				switch(ot){
					case MC_CHR:
						break;
					case MC_I8:
						snprintf(istr, istrsz, "%" PRId8 , *((int8_t*)pt_num));
						break;
					case MC_I16:
						snprintf(istr, istrsz, "%" PRId16 , *((int16_t*)pt_num));
						break;
					case MC_I32:
						snprintf(istr, istrsz, "%" PRId32 , *((int32_t*)pt_num));
						break;
					case MC_UI8:
						snprintf(istr, istrsz, "%" PRIu8 , *((uint8_t*)pt_num));
						break;
					case MC_UI16:
						snprintf(istr, istrsz, "%" PRIu16 , *((uint16_t*)pt_num));
						break;
					case MC_UI32:
						snprintf(istr, istrsz, "%" PRIu32 , *((uint32_t*)pt_num));
						break;
					case MC_X8:
						snprintf(istr, istrsz, "0x%02x", *((uint8_t*)pt_num));
						break;
					case MC_X16:
						snprintf(istr, istrsz, "0x%04x", *((uint16_t*)pt_num));
						break;
					case MC_X32:
						snprintf(istr, istrsz, "0x%08x", *((uint32_t*)pt_num));
						break;
				}
				int sz2 = strlen(istr);
				fwrite(istr, sz2, 1, fpt);
			}
		}
	}

	if(! (*lock)){
		fflush(flog);
	}
	//fclose(flog);
}

uint8_t*
mch_read_file(char* the_pth, off_t* size){
	int fd;
	uint8_t* pt_str = NULL;

	if(size == NULL){
		return pt_str;
	}
	*size = 0;

	fd = open(the_pth, O_RDONLY, 0744);
	if(fd == -1){
		*size = 0;
		return pt_str;
	}


	off_t pos0 = 0;
	off_t pos1 = lseek(fd, 0, SEEK_END);

	*size = pos1;

	if((pos1 != -1) && (pos1 != 0)){
		pos0 = lseek(fd, 0, SEEK_SET);
		if(pos0 != 0){
			close(fd);
			*size = 0;
			return pt_str;
		}

		pt_str = (uint8_t*)malloc(pos1 + 1);
		ssize_t nr = read(fd, pt_str, pos1);
		if(nr != pos1){
			close(fd);
			*size = 0;
			free(pt_str);
			pt_str = NULL;
			return pt_str;
		}
		pt_str[pos1] = 0;
	}

	close(fd);
	return pt_str;
}

int
mch_write_file(char* the_pth, uint8_t* the_data, long the_sz, int write_once){
	int fd = 0;
	
	if(write_once){
		// old perm 0444
		if((fd = open(the_pth, O_RDWR|O_CREAT|O_EXCL, 0777)) == -1){
			return 0;
		}
	} else {
		if((fd = creat(the_pth, 0777)) == -1){
			return 0;
		}
	}

	write(fd, the_data, the_sz);
	close(fd);

	return 1;
}

void
mch_get_enter(mc_workeru_co_t row, mc_workeru_co_t col){
	// CONTINUE
	printf("WORKERU (%d, %d) WAITING. Type enter\n", row, col);
	getchar();
}

#define MCH_MAX_CALL_STACK_SZ 100

void
mch_ptr_call_stack_trace(FILE* out_fp) {
	if(out_fp == mc_null){
		out_fp = stderr;
	}

	void* trace[MCH_MAX_CALL_STACK_SZ];
	memset((uint8_t*)trace, 0, MCH_MAX_CALL_STACK_SZ * sizeof(void*));

	size_t trace_sz = backtrace(trace, MCH_MAX_CALL_STACK_SZ);

	PTD_64_CODE(fprintf(out_fp, "trace_size=%lu \n", trace_sz));
	PTD_32_CODE(fprintf(out_fp, "trace_size=%u \n", trace_sz));
	ZNQ_CODE(fprintf(out_fp, "trace_size=%u \n", trace_sz));

	char **stack_strings = backtrace_symbols(trace, trace_sz);
	fprintf(out_fp, "STACK_STRACE------------------------------\n");
	for( size_t ii = 1; ii < trace_sz; ii++ ) {
		if(ii >= MCH_MAX_CALL_STACK_SZ){ break; }
		fprintf(out_fp, "%s \n", stack_strings[ii]);
	}
	fprintf(out_fp, "------------------------------(to see names link with -rdynamic)\n");
	free( stack_strings );
	fflush(out_fp);
}

