

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
/*! \file log.h

\brief Defines log functions and macros.

------------------------------------------------------------*/

#ifndef MC_LOGS_H
#define MC_LOGS_H

#include <inttypes.h>

#include "attribute.h"

#ifdef __cplusplus
mc_c_decl {
#endif

enum mc_out_omc_type_def {
	MC_OUT_LOG,
	MC_OUT_PRT,
	MC_OUT_LOCK_LOG,
	MC_OUT_UNLOCK_LOG,
	MC_OUT_ABORT
};
typedef enum mc_out_omc_type_def mc_out_type_t;

enum mc_type_def {
	MC_CHR,
	MC_I8,
	MC_I16,
	MC_I32,
	MC_UI8,
	MC_UI16,
	MC_UI32,
	MC_X8,
	MC_X16,
	MC_X32
};
typedef enum mc_type_def mc_type_t;

//======================================================================
// log messages

void
mck_aux_sout(bool cond, char* msg, mc_out_type_t outt) mc_external_code_ram;

//! Locks this workeru log file
mc_inline_fn void
mck_lock_log(){
	mck_aux_sout(mc_true, mc_cstr("lck"), MC_OUT_LOCK_LOG);
}

//! Unocks this workeru log file
mc_inline_fn void
mck_unlock_log(){
	mck_aux_sout(mc_true, mc_cstr("ulk"), MC_OUT_UNLOCK_LOG);
}

//! Logs a string in this workeru log file
mc_inline_fn void
mck_slog(char* msg){
	mck_aux_sout(mc_true, msg, MC_OUT_LOG);
}

//! Sames as \ref mck_slog but it does a const_cast<char *> for you so you can use directly it in c++.
#define	mck_slog2(msg) mck_slog(mc_cstr(msg))

//! Conditionally logs a string in this workeru log file
mc_inline_fn void
mck_cond_slog(bool cond, char* msg){
	mck_aux_sout(cond, msg, MC_OUT_LOG);
}

//! Sames as \ref mck_cond_slog but it does a const_cast<char *> for you so you can use directly it in c++.
#define	mck_cond_slog2(cond, msg) mck_cond_slog(cond, mc_cstr(msg))

//! Prints a string to stdout in the manageru.
mc_inline_fn void
mck_sprt(char* msg){
	mck_aux_sout(mc_true, msg, MC_OUT_PRT);
}

//! Sames as \ref mck_sprt but it does a const_cast<char *> for you so you can use directly it in c++.
#define	mck_sprt2(msg) mck_sprt(mc_cstr(msg))

void
mck_aux_iout(uint32_t vv, mc_out_type_t outt, mc_type_t tt) mc_external_code_ram;

//! Aborts PTD
mc_inline_fn void
mc_out_abort_ptd(){
	mck_aux_iout(0, MC_OUT_ABORT, MC_X32);
}

//! Logs an int32_t in this workeru log file
mc_inline_fn void
mck_ilog(int32_t vv){
	mck_aux_iout(vv, MC_OUT_LOG, MC_I32);
}

//! Logs an uint32_t in this workeru log file
mc_inline_fn void
mck_ulog(uint32_t vv){
	mck_aux_iout(vv, MC_OUT_LOG, MC_UI32);
}

//! Logs an uint32_t as a hexadecimal in this workeru log file
mc_inline_fn void
mck_xlog(uint32_t vv){
	mck_aux_iout(vv, MC_OUT_LOG, MC_X32);
}

//! Prints an int32_t to stdout in the manageru.
mc_inline_fn void
mck_iprt(int32_t vv){
	mck_aux_iout(vv, MC_OUT_PRT, MC_I32);
}

//! Prints an uint32_t to stdout in the manageru.
mc_inline_fn void
mck_uprt(uint32_t vv){
	mck_aux_iout(vv, MC_OUT_PRT, MC_UI32);
}

//! Prints an uint32_t as a hexadecimal to stdout in the manageru.
mc_inline_fn void
mck_xprt(uint32_t vv){
	mck_aux_iout(vv, MC_OUT_PRT, MC_X32);
}

#ifdef __cplusplus
}
#endif

#endif // MC_LOGS_H

