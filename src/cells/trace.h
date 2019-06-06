

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
/*! \file trace.h

\brief Trace functions.

------------------------------------------------------------*/

#ifndef MCK_TRACE_H
#define MCK_TRACE_H

#include <inttypes.h>
#include "attribute.h"

#ifdef __cplusplus
mc_c_decl {
#endif
	
uint16_t
mck_get_call_stack_trace(int16_t sz_trace, void** trace) mc_external_code_ram;

void 
mck_wait_sync(uint32_t info, int16_t sz_trace, void** trace) mc_external_code_ram;

#define mck_getchar() 	mck_wait_sync(MC_WAITING_ENTER, 0, mc_null)

#ifdef __cplusplus
}
#endif

#endif // MCK_TRACE_H

