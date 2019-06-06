

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

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "attribute.h"
#include "thread_ptd.hh"
#include "shared.h"
#include "booter.h"

thread_info_t* ALL_THREADS_INFO = mc_null;
int TOT_THREADS = 0;
pthread_t MANAGERU_THREAD_ID = 0;

// =====================================================================================

uint8_t
mc_hex_to_int(uint8_t in) {
	uint8_t out = 0;
	if((in >= '0') && (in <= '9')){
		out = in - '0';
	}
	if((in >= 'a') && (in <= 'f')){
		out = (in - 'a') + 10;
	}
	if((in >= 'A') && (in <= 'F')){
		out = (in - 'A') + 10;
	}
	return out;
}

uint8_t
mc_int_to_hex(uint8_t in) {
	uint8_t out = 0;
	if((in >= 0) && (in <= 9)){
		out = '0' + in;
	}
	if((in >= 10) && (in <= 15)){
		out = 'a' + (in - 10);
	}
	return out;
}

#define mc_lo_mask	0x0f
#define mc_hi_lo_to_byte(hi, lo) (((hi) << 4) | (lo))
#define mc_byte_to_hi(bb) (((bb) >> 4) & mc_lo_mask)
#define mc_byte_to_lo(bb) ((bb) & mc_lo_mask)

uint16_t
mc_hex_bytes_to_uint16(uint8_t* hex_str) {
	uint8_t hi = mc_hi_lo_to_byte(mc_hex_to_int(hex_str[3]), mc_hex_to_int(hex_str[2]));
	uint8_t lo = mc_hi_lo_to_byte(mc_hex_to_int(hex_str[1]), mc_hex_to_int(hex_str[0]));
	uint16_t out = ((((uint16_t)hi) << 8) | ((uint16_t)lo));
	return out;
}

void
mc_uint16_to_hex_bytes(uint16_t ival, uint8_t* hex_str) {
	uint8_t* ibytes = (uint8_t*)(&ival);
	hex_str[0] = mc_int_to_hex(mc_byte_to_lo(ibytes[0]));
	hex_str[1] = mc_int_to_hex(mc_byte_to_hi(ibytes[0]));
	hex_str[2] = mc_int_to_hex(mc_byte_to_lo(ibytes[1]));
	hex_str[3] = mc_int_to_hex(mc_byte_to_hi(ibytes[1]));
	hex_str[4] = '\0';
}

// =====================================================================================

uint16_t
mck_get_thread_idx(){
	if(ALL_THREADS_INFO == mc_null){
		mch_abort_func(2, "get_thread_idx. NULL ALL_THREADS_INFO \n");
		return 0;
	}
	pthread_t slf = pthread_self();
	if(slf == MANAGERU_THREAD_ID){
		mch_abort_func(2, "get_thread_idx. Host thread. \n");
		return 0;
	}

	char thd_name[NAMELEN];
	int rc = pthread_getname_np(slf, thd_name, NAMELEN);
	if(rc != 0){
		mch_abort_func(1, "get_thread_idx. INVALID THREAD NAME \n");
		return 0;
	}
	uint16_t thd_idx = mc_hex_bytes_to_uint16((uint8_t*)thd_name);
	if((thd_idx < 0) || (thd_idx >= TOT_THREADS)){
		mch_abort_func(1, "get_thread_idx. INVALID thd_idx \n");
		return 0;
	}
	return thd_idx;
}

ptd_info_t*
mck_get_ptd_info(){
	if(mc_is_manageru_thread()){
		PTD_CK(mcm_MANAGERU_PTD_INFO != mc_null);
		return mcm_MANAGERU_PTD_INFO;
	}
	uint16_t thd_idx = mck_get_thread_idx();
	ptd_info_t* info = &(ALL_THREADS_INFO[thd_idx].thd_ptd);
	return info;
}

// =====================================================================================

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while (0)

mc_workeru_id_t
mcm_get_addr_workeru_id_fn(void* addr){
	if(mcm_addr_in_manageru(addr)){
		PTD_CK(mcm_MANAGERU_PTD_INFO != mc_null);
		return mcm_MANAGERU_PTD_INFO->ptd_workeru_id;
	}	
	mc_workeru_nn_t idx = mck_get_addr_idx(addr);
	thread_info_t* info = &(ALL_THREADS_INFO[idx]);
	return info->thd_ptd.ptd_workeru_id;
}

void*
mcm_addr_with_fn(mc_workeru_id_t workeru_id, void* addr){
	if(mcm_addr_in_manageru(addr)){
		return mc_null;
	}
	mc_workeru_nn_t idx = mc_id_to_nn(workeru_id);
	void* addr2 = (void*)((uintptr_t)(&(ALL_THREADS_INFO[idx])) + mck_get_addr_offset(addr));
	//PTD_CK((workeru_id != mcm_get_addr_workeru_id_fn(addr)) || (addr2 == addr));
	return addr2;
}

bool 
mcm_call_assert(char* out_fnam, bool is_assert, bool prt_stck, bool cond, 
		const char* fnam, int line, const char* ck_str, const char* fmt, ...)
{
	FILE* out_file = stderr;
	if(out_fnam != mc_null){
		out_file = fopen(out_fnam, "a");
		if(out_file == NULL){
			out_file = stderr;
		}
	}

	bool is_asst = (is_assert && ! cond);
	bool is_prt = (! is_assert && cond);
	bool do_prt = is_asst || is_prt;
	if(do_prt){
		flockfile(stdout);
		flockfile(out_file);
		fflush(stdout); 
		fflush(out_file); 

		if(is_assert || prt_stck){
			fprintf(out_file, "\n------------------------------------------------------------------\n");
		}
		if(MANAGERU_THREAD_ID != 0){
			ptd_info_t* inf = mck_get_ptd_info();
			fprintf(out_file, "%d:%x --> ", inf->ptd_num, inf->ptd_workeru_id);
		}
		if(fnam != mc_null){
			fprintf(out_file, "FILE %s(%d): ", fnam, line);
		}
		if(is_assert && (ck_str != mc_null)){
			fprintf(out_file, "ASSERT '%s' FAILED.\n", ck_str);
		} 
		if(prt_stck){
			mch_ptr_call_stack_trace(out_file);
		}

		if(fmt != NULL){
			char prt_buff[MC_MAX_STR_SZ];
			va_list ap;

			va_start(ap, fmt);
			int size = vsnprintf(prt_buff, MC_MAX_STR_SZ, fmt, ap);
			va_end(ap);

			prt_buff[MC_MAX_STR_SZ - 1] = '\0';

			if(size < 0){ 
				mch_abort_func((mc_addr_t)mcm_call_assert, "mcm_call_assert._ERROR_. \n");
			}

			fprintf(out_file, "%s", prt_buff);
		}
		if(is_assert || prt_stck){
			fprintf(out_file, "\n------------------------------------------------------------------\n\n");
		}
		fflush(out_file); 

		funlockfile(stdout);
		funlockfile(out_file);
	}

	if((out_fnam != mc_null) && (out_file != stderr)){
		fclose(out_file);
	}

	if(is_assert && ! cond){
		//assert(cond);
		mcm_thread_abort();
	}
	return cond;
}

char*
mcm_get_ptd_log_fnam(){
	if(mc_is_manageru_thread()){
		return mc_null;
	}
	uint16_t thd_idx = mck_get_thread_idx();
	char* log_fnam = ALL_THREADS_INFO[thd_idx].thd_log_fnam;
	return log_fnam;
}

void *
thread_start(void *arg){

	thread_info_t *tinfo = (thread_info_t *)arg;
	pthread_t slf = pthread_self();
	int old_cancel;

	pthread_setname_np(slf, tinfo->thd_ptd.ptd_name);

	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_cancel);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_cancel);

	//printf("SELF = %ld \tWORKERU_ID = %d \tNAME = %s \n", slf, mck_get_workeru_id(), tinfo->thd_ptd.ptd_name);

	if(tinfo->thd_ptd.ptd_workeru_func != mc_null){
		(tinfo->thd_ptd.ptd_workeru_func)();
	}

	return mc_null;
}

void 
mcm_thread_abort(){
	if(mc_is_manageru_thread()){
		abort();
	}
	mc_out_abort_ptd();

	ptd_info_t* info = mck_get_ptd_info();
	if(pthread_cancel(info->ptd_id) != 0){
		assert(false && "CANNOT_CANCEL_THREAD !!!!!");
	}
	pause();
	assert(false && "CANNOT_CANCEL_THREAD 2 !!!!!");
}

