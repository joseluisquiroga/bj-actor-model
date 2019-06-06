

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


//----------------------------------------------------------------------------
/*! \file shared.h

\brief C shared structures (by the manageru and all workerus). It is included in global.h and \ref cell.hh.

*/

#ifndef MC_SHARED_DATA_H
#define MC_SHARED_DATA_H

//include <limits.h>
#include <stdint.h>
#include <inttypes.h>

#include "attribute.h"
#include "log.h"
#include "rr_array.h"

#ifdef __cplusplus
mc_c_decl {
#endif

typedef uint8_t mc_bool_t;

//======================================================================
// min max functions

#define mc_min(v1, v2) (((v1) < (v2))?(v1):(v2))

#define mc_max(v1, v2) (((v1) > (v2))?(v1):(v2))

#define mc_issigned(t) (((t)(-1)) < ((t) 0))

#define mc_umaxof(t) (((0x1ULL << ((sizeof(t) * 8ULL) - 1ULL)) - 1ULL) | \
                    (0xFULL << ((sizeof(t) * 8ULL) - 4ULL)))

#define mc_smaxof(t) (((0x1ULL << ((sizeof(t) * 8ULL) - 1ULL)) - 1ULL) | \
                    (0x7ULL << ((sizeof(t) * 8ULL) - 4ULL)))

//define mc_maxof(t) ((unsigned long long) (mc_issigned(t) ? mc_smaxof(t) : mc_umaxof(t)))
#define mc_maxof(t) ((unsigned long) (mc_issigned(t) ? mc_smaxof(t) : mc_umaxof(t)))

	
//======================================================================
// epiphany III

#define mc_e3_xx 32
#define mc_e3_yy 8
#define mc_e3_xx_sz 4
#define mc_e3_yy_sz 4
#define mc_e3_yy_sz_pw2 2

#define mc_e3_num_chip_workerus 16

#define mc_workeru_tot_mem		0x8000

//======================================================================
// address opers

#ifdef MC_IS_PTD_CODE
	#include "shared_ptd.h"
#endif

#ifdef MC_IS_EPH_CODE
	#include "shared_eph.h"
#endif

#ifdef MC_IS_ZNQ_CODE
	#include "shared_znq.h"
#endif

#define MC_INVALID_WORKERU_NN ((mc_workeru_nn_t)(~((mc_workeru_nn_t)0x0)))

typedef mc_addr_t mc_size_t;
	
//======================================================================
// working system struct

#define mc_pw2(pw2) (1 << (pw2))
#define mc_div_pw2(v1, pw2) ((v1) >> (pw2))
#define mc_mod_pw2(v1, pw2) ((v1) & ((1 << (pw2)) - 1))
	
#define mc_null 0x0

#define mc_out_num_workerus mc_e3_num_chip_workerus

struct mc_aligned mc_sys_def { 
	mc_workeru_co_t 	xx;		// absolute xx epiphany space coordinates
	mc_workeru_co_t 	yy;		// absolute yy epiphany space coordinates
	mc_workeru_co_t 	xx_sz;		// this running sys number of ekores in xx axis (sys length)
	uint8_t 	 	yy_sz_pw2;		// this running sys number of ekores in yy axis (sys witdh) is (2 ^ yy_sz_pw2)
};
typedef struct mc_sys_def mc_sys_sz_st;

#ifdef MC_IS_EPH_CODE
	extern mc_sys_sz_st 	mck_system_sz;
	#define MC_SYS_SZ (&mck_system_sz)
#else
	mc_sys_sz_st*
	mc_get_glb_sys_sz();

	#define MC_SYS_SZ mc_get_glb_sys_sz()
#endif

void mc_inline_fn
mc_init_glb_sys_sz(mc_sys_sz_st* sys_sz) {
	sys_sz->xx = mc_e3_xx;
	sys_sz->yy = mc_e3_yy;
	sys_sz->xx_sz = mc_e3_xx_sz;
	sys_sz->yy_sz_pw2 = mc_e3_yy_sz_pw2;
}

void mc_inline_fn
mc_init_glb_sys_sz_with(mc_sys_sz_st* sys_sz, mc_workeru_co_t xx_val, mc_workeru_co_t yy_val, 
				mc_workeru_co_t xx_sz_val, uint8_t yy_sz_pw2_val)
{
	sys_sz->xx = xx_val;
	sys_sz->yy = yy_val;
	sys_sz->xx_sz = xx_sz_val;
	sys_sz->yy_sz_pw2 = yy_sz_pw2_val;
}

#define mc_e3_co_to_pw(co) ((uint8_t)log2l(co))

#define mch_init_glb_sys_sz_with_dev(sys_sz, dev) \
	mc_init_glb_sys_sz_with((sys_sz), (dev)->row, (dev)->col, (dev)->rows, mc_e3_co_to_pw((dev)->cols))
	
// end_of_macro


//======================================================================
// convertion functions
	
// xx and yy are absolute epiphany space coordinates
// ro and co are relative epiphany space coordinates with respect to the 
// 		allocated running workerus (MC_SYS_SZ)
// id is the workeru id absolute in epiphany space 
// nn is a consec with respect to the allocated running workerus (MC_SYS_SZ)

#define mc_pw2_yy_sys (MC_SYS_SZ->yy_sz_pw2)

#define mc_tot_xx_sys (MC_SYS_SZ->xx_sz)
#define mc_tot_yy_sys ((mc_workeru_co_t)(1 << mc_pw2_yy_sys))
#define mc_tot_nn_sys (mc_tot_xx_sys * mc_tot_yy_sys)

#define mc_min_xx_sys (MC_SYS_SZ->xx)
#define mc_max_xx_sys (mc_min_xx_sys + mc_tot_xx_sys)
#define mc_min_yy_sys (MC_SYS_SZ->yy)
#define mc_max_yy_sys (mc_min_yy_sys + mc_tot_yy_sys)

#define mc_id_to_xx(id)	(((id) >> mc_axis_bits) & mc_axis_mask)
#define mc_id_to_yy(id)	((id) & mc_axis_mask)
#define mc_xx_to_ro(xx)	((xx) - mc_min_xx_sys)
#define mc_yy_to_co(yy)	((yy) - mc_min_yy_sys)
#define mc_id_to_ro(id)	mc_xx_to_ro(mc_id_to_xx(id))
#define mc_id_to_co(id)	mc_yy_to_co(mc_id_to_yy(id))
#define mc_ro_to_xx(ro)	((ro) + mc_min_xx_sys)
#define mc_co_to_yy(co)	((co) + mc_min_yy_sys)
#define mc_ro_co_to_nn(ro, co) (((ro) * mc_tot_yy_sys) + (co))
#define mc_xx_yy_to_id(xx, yy) (((xx) << mc_axis_bits) + (yy))
#define mc_ro_co_to_id(ro, co) ((mc_ro_to_xx(ro) << mc_axis_bits) + mc_co_to_yy(co))
#define mc_nn_to_ro(nn)	mc_div_pw2(nn, mc_pw2_yy_sys)
#define mc_nn_to_co(nn)	mc_mod_pw2(nn, mc_pw2_yy_sys)
#define mc_id_to_nn(id) (mc_ro_co_to_nn(mc_id_to_ro(id), mc_id_to_co(id)))
#define mc_nn_to_id(nn) (mc_ro_co_to_id(mc_nn_to_ro(nn), mc_nn_to_co(nn)))

#define mc_xx_in_sys(xx) (((xx) >= mc_min_xx_sys) && ((xx) < mc_max_xx_sys))
#define mc_yy_in_sys(yy) (((yy) >= mc_min_yy_sys) && ((yy) < mc_max_yy_sys))
#define mc_xx_yy_in_sys(xx, yy) (mc_xx_in_sys(xx) && mc_yy_in_sys(yy))
#define mc_id_in_sys(id) (mc_xx_yy_in_sys(mc_id_to_xx(id), mc_id_to_yy(id)))


//======================================================================
// address functions 2

#ifdef MC_IS_EPH_CODE
mc_workeru_id_t mc_inline_fn
mck_get_workeru_id() {
	mc_workeru_id_t koid = 0x0; 
	mc_asm("movfs %0, coreid" : "=r" (koid));
	return koid;
}
#else
mc_workeru_id_t 
mck_get_workeru_id();
#endif

//! Returns true if 'addr' is local to the workeru with id 'koid'
bool mc_inline_fn
mc_addr_in_workeru(mc_addr_t addr, mc_workeru_id_t koid) {
	mc_workeru_id_t addr_koid = mc_addr_get_id(addr);
	return ((addr_koid == 0) || (addr_koid == koid));
}

//! Returns true if 'addr' is in any of the workerus of the epiphany system
bool mc_inline_fn
mc_addr_in_sys(mc_addr_t addr) {
	mc_workeru_id_t addr_koid = mc_addr_get_id(addr);
	if(addr_koid == 0){
		return true;
	}
	return mc_id_in_sys(addr_koid);
}

//! Returns true if 'addr' is in the manageru
#define mc_addr_in_manageru(addr) (! mc_addr_in_sys(addr))

/*! Remote dereference of a 'pt_field' of remote object pointer 'glb_pt' (with id) and 
returns it as a 'typ_nam' pointer
*/
#define mc_dref(typ_nam, glb_pt, pt_field) ((typ_nam*)mc_addr_set_id(mc_addr_get_id(glb_pt), (glb_pt)->pt_field))

/*! Remote dereference of a 'field' of class 'cls_field' in a remote object structure 'base' (not a pointer)
of class 'cls_base'.
*/
#define mc_dref_field(cls_base, base, cls_field, field)	\
	((cls_field*)(uint8_t*)(((uint8_t*)base) + mc_offsetof(&cls_base::field)))




//======================================================================
// sane alignment/access functions

#define MC_IS_ALIGNED_16(ptr) ((((uintptr_t)ptr) & 0x1) == 0)  //!< True if ptr is 16 aligned
#define MC_IS_ALIGNED_32(ptr) ((((uintptr_t)ptr) & 0x3) == 0)  //!< True if ptr is 32 aligned
#define MC_IS_ALIGNED_64(ptr) ((((uintptr_t)ptr) & 0x7) == 0)  //!< True if ptr is 64 aligned
//define MC_IS_ALIGNED(ptr, agn) ((((uintptr_t)ptr) & (agn - 1)) == 0)

mc_opt_sz_fn uint8_t 
mc_get_aligment(void* ptr) mc_external_code_ram;

uint32_t mc_inline_fn
mc_v32_of_p16(uint16_t* p16){
	uint32_t v32 = p16[1];
	v32 <<= 16;
	v32 |= p16[0];
	return v32;
}

//! Warranted set of an off-workeru variable (loops until reading the value). Use only if needed.
#define mc_loop_set_var(var, val) \
	(var) = (val); \
	while((var) != (val)); \
		
// end_macro
	
#define mc_spin_cond(cond) \
	while(cond){ \
		/* SPIN UNTIL SET (may be set by an other workeru) */ \
		PTD_CODE(sched_yield()); \
	} \
	PTD_CK(! (cond)); \
	
// end_macro

//======================================================================
// bitarray

// where 'a' is the byte stream pointer, and b is the bit number.

#define mc_div8(b)	((b)>>3)
#define mc_mod8(b)	((b)&7)

#define mc_get_bit(a, b)		((((uint8_t*)a)[mc_div8(b)] >> mc_mod8(b)) & 1)
#define mc_set_bit(a, b)		(((uint8_t*)a)[mc_div8(b)] |= (1 << mc_mod8(b)))
#define mc_reset_bit(a, b) 		(((uint8_t*)a)[mc_div8(b)] &= ~(1 << mc_mod8(b)))
#define mc_toggle_bit(a, b) 	(((uint8_t*)a)[mc_div8(b)] ^= (1 << mc_mod8(b)))

#define mc_to_bytes(num_bits)	(mc_div8(num_bits) + (mc_mod8(num_bits) > 0))
#define mc_to_bits(num_bytes)	(num_bytes * 8)

#define mc_byte_to_binary_pattern "%c%c%c%c%c%c%c%c"

#define mc_byte_to_binary(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 




#define mc_mem_2K   2048
#define mc_mem_16K   16384
#define mc_mem_32K   32768
#define mc_max_opcodes_func 16384

#define MC_MAGIC_ID 0xaabbccdd
#define MC_MAGIC_END 0x6789abcd

#define MC_ABORT_ERR 				0xdeadbeaf
#define MC_CALL_STACK_TRACE_ERR		0x01234567

#define MC_MAX_CALL_STACK_SZ	20

typedef uint8_t mck_exception_t;

#define mck_invalid_exception 0
#define mck_software_exception 1
#define mck_memory_exception 2

#define MC_NOT_FINISHED_VAL 0x21
#define MC_FINISHED_VAL 	0xf1

#define MC_NOT_WAITING	 	0x0
#define MC_WAITING_ENTER	0xaa
#define MC_WAITING_BUFFER	0xbb

struct mc_aligned mc_off_workeru_shared_data_def { 
	uint32_t 		magic_id;
	mc_workeru_id_t	ck_workeru_id;
	uint8_t 		is_finished;
	uint8_t 		is_waiting;
	void* 			workeru_data;
};
typedef struct mc_off_workeru_shared_data_def mc_off_workeru_st;


//define MC_OUT_BUFF_SZ 	mc_mem_16K
#define MC_OUT_BUFF_SZ 	300
#define MC_OUT_BUFF_MAX_OMC_SZ 500

struct mc_aligned mc_workeru_out_def { 
	uint32_t 		magic_id;
	mc_rrarray_st 	wr_arr;
	mc_rrarray_st 	rd_arr;
	uint8_t 		buff[MC_OUT_BUFF_SZ];
};
typedef struct mc_workeru_out_def mc_workeru_out_st;


struct mc_aligned mc_off_sys_shared_data_def { 
	uint32_t 		magic_id;
	//uint32_t 		dbg_error_code;
	void* 			pt_this_from_znq;
	void* 			pt_this_from_eph;
	void* 			pt_manageru_kernel;
	mc_addr_t		znq_shared_mem_base;
	mc_addr_t		eph_shared_mem_base;

	uint32_t 		tot_modules;

	mc_workeru_id_t	first_load_workeru_id;

	mc_sys_sz_st 	wrk_sys;
	mc_off_workeru_st 	sys_workerus[mc_out_num_workerus];
	mc_workeru_out_st 	sys_out_buffs[mc_out_num_workerus];
};
typedef struct mc_off_sys_shared_data_def mc_off_sys_st;

//! Library EXTERNAL RAM running version of memset
mc_opt_sz_fn uint8_t*
mc_memset(uint8_t* dest, uint8_t val, mc_size_t sz) mc_external_code_ram;

//! Library EXTERNAL RAM running version of memcpy
mc_opt_sz_fn uint8_t*
mc_memcpy(uint8_t* dest, const uint8_t* src, mc_size_t sz) mc_external_code_ram;

//! Library EXTERNAL RAM running version of memmove
mc_opt_sz_fn uint8_t*
mc_memmove(uint8_t* dest, const uint8_t* src, mc_size_t sz) mc_external_code_ram;

//! Library EXTERNAL RAM running version of strlen.
uint16_t 
mc_strlen(char* str) mc_external_code_ram;

//! Library EXTERNAL RAM running version of strcmp.
uint8_t 
mc_strcmp(char* str1, char* str2) mc_external_code_ram;

mc_inline_fn bool
mc_isprint(char cc){
	return ((cc >= ' ' && cc <= '~') ? true : false);
}

//! Inits an array 'arr' of objects of class 'cls' and size 'sz' with 'new' 
#define mc_init_arr_objs(sz, arr, cls) \
	for(int aa = 0; aa < sz; aa++){ \
		new (&(arr[aa])) cls(); \
	} \

// end_macro

//! Inits an array 'arr' of objects of size 'sz' with 'val'
#define mc_init_arr_vals(sz, arr, val) \
	for(int aa = 0; aa < sz; aa++){ \
		arr[aa] = (val); \
	} \

// end_macro

//! Maps a manageru address to a workeru address
mc_addr_t
mc_manageru_addr_to_workeru_addr(mc_addr_t h_addr) mc_external_code_ram;

//! Maps a workeru address to a manageru address
mc_addr_t
mc_workeru_addr_to_manageru_addr(mc_addr_t c_addr) mc_external_code_ram;

//! Maps a manageru pointer to a workeru pointer
#define mc_manageru_pt_to_workeru_pt(pt) (mc_manageru_addr_to_workeru_addr((mc_addr_t)(pt)))

//! Maps a workeru pointer to a manageru pointer
#define mc_workeru_pt_to_manageru_pt(pt) (mc_workeru_addr_to_manageru_addr((mc_addr_t)(pt)))


#ifdef __cplusplus
}
#endif

#endif // MC_SHARED_DATA_H

