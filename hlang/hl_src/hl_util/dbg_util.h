

/*************************************************************

This file is part of ben-jose.

ben-jose is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

ben-jose is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ben-jose.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2007-2012, 2014-2016. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://github.com/joseluisquiroga/ben-jose

ben-jose is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------

dbg_util.h

func to print a stack trace.

--------------------------------------------------------------*/

#ifndef HL_DBG_UTIL_H
#define HL_DBG_UTIL_H

#include <iostream>
#include <sstream>
#include <fstream>	// used for hl_ofstream

#define hl_null NULL

#define HL_MAX_CALL_STACK_SZ 100
#define HL_PRT_SIZE_T "%lu"
#define HL_MAX_STR_SZ 300

#define hl_c_decl extern "C"

#define HL_STRINGIFY(x) #x
#define HL_TOSTRING(x) HL_STRINGIFY(x)

#define MC_ABORT_MSG(msg) mc_cstr("ABORTING. '" msg "' at " __FILE__ "(" HL_TOSTRING(__LINE__) ")")

#ifdef __cplusplus
hl_c_decl {
#endif

#ifdef FULL_DEBUG
#	define HL_DBG(prm) prm
#else
#	define HL_DBG(prm) /**/ 
#endif

#define HL_MARK_USED(X)  ((void)(&(X)))

#ifdef __cplusplus
}
#endif

typedef std::string hl_string;

hl_string	hl_get_stack_trace( const hl_string & file, int line );
void hl_abort_func(long val, const char* msg);

#define HL_STACK_STR hl_get_stack_trace(__FILE__, __LINE__)
#define DO_GETCHAR()			getchar()
#define CARRIAGE_RETURN		((char)13)

typedef std::ostream hl_ostream;
typedef std::ostringstream hl_ostringstream;
typedef std::ofstream hl_ofstream;


const char* hl_get_ptd_log_fnam();
bool hl_call_assert(char* out_fnam, bool is_assert, bool prt_stck, bool vv_ck, 
				const char* file, int line, const char* ck_str, const char* fmt, ...);


#define HL_CODE(cod) cod
#define HL_DBG_CODE(cod) HL_DBG(cod)

#define HL_CK(vv) HL_DBG( \
	hl_call_assert(hl_null, true, true, vv, __FILE__, __LINE__, #vv, hl_null))

#define HL_CK_PRT(vv, ...) HL_DBG( \
	hl_call_assert(hl_null, true, true, vv, __FILE__, __LINE__, #vv, __VA_ARGS__))

#define HL_CK_LOG(vv, ...) HL_DBG( \
	hl_call_assert(hl_get_ptd_log_fnam(), true, true, vv, __FILE__, __LINE__, #vv, __VA_ARGS__))

#define HL_COND_LOG(cond, ...) HL_DBG( \
	hl_call_assert(hl_get_ptd_log_fnam(), false, false, cond, __FILE__, __LINE__, #cond, __VA_ARGS__))

#define HL_LOG(...) HL_COND_LOG(true, __VA_ARGS__)

#define HL_COND_PRT(cond, ...) HL_DBG( \
	hl_call_assert(hl_null, false, false, cond, __FILE__, __LINE__, #cond, __VA_ARGS__))

#define HL_PRT(...) HL_COND_PRT(true, __VA_ARGS__)

#define HL_PRINTF(...) HL_DBG(printf(__VA_ARGS__))

#define HL_PRT_STACK(cond, ...) HL_DBG( \
	hl_call_assert(hl_null, false, true, cond, __FILE__, __LINE__, #cond, __VA_ARGS__))

#define HL_LOG_STACK(cond, ...) HL_DBG( \
	hl_call_assert(hl_get_ptd_log_fnam(), false, true, cond, __FILE__, __LINE__, #cond, __VA_ARGS__))


#endif		// HL_DBG_UTIL_H


