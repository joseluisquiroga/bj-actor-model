

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


// shared_znq.h

#ifndef MC_SHARED_DATA_ZNQ_H
#define MC_SHARED_DATA_ZNQ_H

#ifndef MC_IS_ZNQ_CODE
	#error Only compile for zynq
#endif

#include "shared_eph3.h"

#ifdef __cplusplus
mc_c_decl {
#endif

#define mc_addr_has_local_id(addr) (! mc_addr_in_sys((mc_addr_t)addr))
#define mc_addr_is_local(addr) mc_addr_has_local_id(addr)

#define mck_as_glb_pt(pt) ((void*)(pt))
#define mck_as_loc_pt(pt) ((void*)(pt))

#ifdef __cplusplus
}
#endif

#endif // MC_SHARED_DATA_ZNQ_H


