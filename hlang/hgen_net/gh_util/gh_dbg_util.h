

/*************************************************************

This file is part of hgen_net.

hgen_net is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

hgen_net is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with hgen_net.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2020. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://github.com/hgen_net

hgen_net is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------

gh_dbg_util.h

func to print a stack trace.

--------------------------------------------------------------*/

#ifndef GH_DBG_UTIL_H
#define GH_DBG_UTIL_H

#include <string>

#define gh_null NULL

#define gh_c_decl extern "C"

#define GH_STRINGIFY(x) #x
#define GH_TOSTRING(x) GH_STRINGIFY(x)

#define GH_MAX_STR_SZ 300

// -----------------

#define GH_MAX_CALL_STACK_SZ 100
#define GH_PRT_SIZE_T "%lu"

#define GH_ABORT_MSG(msg) gh_cstr("ABORTING. '" msg "' at " __FILE__ "(" GH_TOSTRING(__LINE__) ")")

#define GH_INFO_STR "Passed " __FILE__ "(" GH_TOSTRING(__LINE__) ")"

#ifdef __cplusplus
gh_c_decl {
#endif

#ifdef FULL_DEBUG
#	define GH_DBG(prm) prm
#else
#	define GH_DBG(prm) /**/ 
#endif

#define GH_MARK_USED(X)  ((void)(&(X)))

#ifdef __cplusplus
}
#endif

std::string gh_get_stack_trace( const std::string & file, int line );
void gh_ptr_call_stack_trace(FILE* out_fp);
void gh_abort_func(bool prt_stk = false);

#define GH_STACK_STR gh_get_stack_trace(__FILE__, __LINE__)

const char* gh_get_ptd_log_fnam();
bool gh_call_assert(char* out_fnam, bool is_assert, bool prt_stck, bool vv_ck, 
				const char* file, int line, const char* ck_str, const char* fmt, ...);

#define gh_abort(...) \
{ \
	fprintf(stderr, "\nABORTING.\n"); \
	gh_ptr_call_stack_trace(gh_null); \
	fprintf(stderr, __VA_ARGS__); \
	gh_abort_func(); \
} \

// end_define

#define GH_CODE(cod) cod
#define GH_DBG_CODE(cod) GH_DBG(cod)

#define GH_CK(vv) GH_DBG( \
	gh_call_assert(gh_null, true, true, vv, __FILE__, __LINE__, #vv, gh_null))

//	(! vv)?(gh_call_assert(gh_null, true, true, vv, __FILE__, __LINE__, #vv, __VA_ARGS__), 0):(0))
//	gh_call_assert(gh_null, true, true, vv, __FILE__, __LINE__, #vv, __VA_ARGS__))

#define GH_CK_PRT(vv, ...) GH_DBG( \
	(! vv)?(gh_call_assert(gh_null, true, true, vv, __FILE__, __LINE__, #vv, __VA_ARGS__), 0):(0))

#define GH_CK_LOG(vv, ...) GH_DBG( \
	gh_call_assert(gh_get_ptd_log_fnam(), true, true, vv, __FILE__, __LINE__, #vv, __VA_ARGS__))

#define GH_COND_LOG(cond, ...) GH_DBG( \
	gh_call_assert(gh_get_ptd_log_fnam(), false, false, cond, __FILE__, __LINE__, #cond, __VA_ARGS__))

#define GH_LOG(...) GH_COND_LOG(true, __VA_ARGS__)

#define GH_COND_PRT(cond, ...) GH_DBG( \
	gh_call_assert(gh_null, false, false, cond, __FILE__, __LINE__, #cond, __VA_ARGS__))

#define GH_PRT(...) GH_COND_PRT(true, __VA_ARGS__)

#define GH_PRINTF(...) GH_DBG(printf(__VA_ARGS__))

#define GH_PRT_STACK(cond, ...) GH_DBG( \
	gh_call_assert(gh_null, false, true, cond, __FILE__, __LINE__, #cond, __VA_ARGS__))

#define GH_LOG_STACK(cond, ...) GH_DBG( \
	gh_call_assert(gh_get_ptd_log_fnam(), false, true, cond, __FILE__, __LINE__, #cond, __VA_ARGS__))


#endif		// GH_DBG_UTIL_H


