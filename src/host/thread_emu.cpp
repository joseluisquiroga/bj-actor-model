
//define _GNU_SOURCE

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "attribute.h"
#include "thread_emu.hh"
#include "shared.h"
#include "booter.h"

thread_info_t* ALL_THREADS_INFO = mc_null;
int TOT_THREADS = 0;
pthread_t HOST_THREAD_ID = 0;

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
	if(slf == HOST_THREAD_ID){
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

emu_info_t*
mck_get_emu_info(){
	if(mc_is_host_thread()){
		EMU_CK(mcm_HOST_EMU_INFO != mc_null);
		return mcm_HOST_EMU_INFO;
	}
	uint16_t thd_idx = mck_get_thread_idx();
	emu_info_t* info = &(ALL_THREADS_INFO[thd_idx].thd_emu);
	return info;
}

// =====================================================================================

#define handle_error_en(en, msg) \
		do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
		do { perror(msg); exit(EXIT_FAILURE); } while (0)

mc_core_id_t
mcm_get_addr_core_id_fn(void* addr){
	if(mcm_addr_in_host(addr)){
		EMU_CK(mcm_HOST_EMU_INFO != mc_null);
		return mcm_HOST_EMU_INFO->emu_core_id;
	}	
	mc_core_nn_t idx = mck_get_addr_idx(addr);
	thread_info_t* info = &(ALL_THREADS_INFO[idx]);
	return info->thd_emu.emu_core_id;
}

void*
mcm_addr_with_fn(mc_core_id_t core_id, void* addr){
	if(mcm_addr_in_host(addr)){
		return mc_null;
	}
	mc_core_nn_t idx = mc_id_to_nn(core_id);
	void* addr2 = (void*)((uintptr_t)(&(ALL_THREADS_INFO[idx])) + mck_get_addr_offset(addr));
	//EMU_CK((core_id != mcm_get_addr_core_id_fn(addr)) || (addr2 == addr));
	return addr2;
}

bool 
mcm_call_assert(bool is_assert, bool prt_stck, bool cond, 
		const char* file, int line, const char* ck_str, const char* fmt, ...)
{
	bool is_asst = (is_assert && ! cond);
	bool is_prt = (! is_assert && cond);
	bool do_prt = is_asst || is_prt;
	if(do_prt){
		emu_info_t* inf = mck_get_emu_info();
		if(is_assert || prt_stck){
			fprintf(stderr, "------------------------------------------------------------------\n");
		}
		if(file != mc_null){
			fprintf(stderr, "FILE %s(%d): ", file, line);
		}
		fprintf(stderr, "%d:%x --> ", inf->emu_num, inf->emu_core_id);
		if(is_assert){
			fprintf(stderr, "ASSERT '%s' FAILED.\n", ck_str);
		} 
		if(prt_stck){
			mch_ptr_call_stack_trace();
		}

		if(fmt != NULL){
			char pp[MC_MAX_STR_SZ];
			va_list ap;

			va_start(ap, fmt);
			int size = vsnprintf(pp, MC_MAX_STR_SZ, fmt, ap);
			va_end(ap);

			pp[MC_MAX_STR_SZ - 1] = '\0';

			if(size < 0){ 
				mch_abort_func((mc_addr_t)mcm_printf, "mcm_printf. ERROR. \n");
			}

			fprintf(stderr, "%s", pp);
		}
		if(is_assert || prt_stck){
			fprintf(stderr, "------------------------------------------------------------------\n");
		}
		fflush(stderr); 
	}
	if(is_assert){
		assert(cond);
	}
	return cond;
}

void
mcm_log(const char *fmt, ...){
	//EMU_CK(! mc_is_host_thread());

	char pp[MC_MAX_STR_SZ];
	va_list ap;

	va_start(ap, fmt);
	int size = vsnprintf(pp, MC_MAX_STR_SZ, fmt, ap);
	va_end(ap);

	pp[MC_MAX_STR_SZ - 1] = '\0';

	if(size < 0){ 
		mch_abort_func((mc_addr_t)mcm_log, "mcm_log. ERROR. \n");
	}

	mck_slog2(pp);
}

void
mcm_printf(const char *fmt, ...){
	//EMU_CK(! mc_is_host_thread());

	char pp[MC_MAX_STR_SZ];
	va_list ap;

	va_start(ap, fmt);
	int size = vsnprintf(pp, MC_MAX_STR_SZ, fmt, ap);
	va_end(ap);

	pp[MC_MAX_STR_SZ - 1] = '\0';

	if(size < 0){ 
		mch_abort_func((mc_addr_t)mcm_printf, "mcm_printf. ERROR. \n");
	}

	emu_info_t* inf = mck_get_emu_info();

	printf("%d:%x --> %s", inf->emu_num, inf->emu_core_id, pp);
	fflush(stdout); 
}

void *
thread_start(void *arg){

	thread_info_t *tinfo = (thread_info_t *)arg;
	pthread_t slf = pthread_self();

	pthread_setname_np(slf, tinfo->thd_emu.emu_name);

	//printf("SELF = %ld \tCORE_ID = %d \tNAME = %s \n", slf, mck_get_core_id(), tinfo->thd_emu.emu_name);

	if(tinfo->thd_emu.emu_core_func != mc_null){
		(tinfo->thd_emu.emu_core_func)();
	}

	return mc_null;
}

