

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
/*! \file shared_eph.h

\brief Addressing functions for the epiphany architecture..

------------------------------------------------------------*/

// shared_eph.h

#ifndef MC_SHARED_DATA_EPH_H
#define MC_SHARED_DATA_EPH_H

#if !defined(MC_IS_EPH_CODE) && ! defined(MC_IS_ZNQ_CODE)
	#error Only compile for parallella (epiphany or zynq)
#endif

#include "shared_eph3.h"

#ifdef __cplusplus
mc_c_decl {
#endif

//! True if 'addr' has local id
#define mc_addr_has_local_id(addr) (mc_addr_get_id(addr) == MC_WORKERU_INFO->the_workeru_id)

//! True if 'addr' is local
#define mc_addr_is_local(addr) ((! mc_addr_has_id(addr)) || mc_addr_has_local_id(addr))

//! Sets the id of 'pt' to the local workeru id.
#define mck_as_glb_pt(pt) ((void*)mc_addr_set_id(MC_WORKERU_INFO->the_workeru_id, (pt)))

//! Sets to zero the id of 'pt'.
#define mck_as_loc_pt(pt) ((void*)mc_addr_mask_ad(pt))

#ifdef __cplusplus
}
#endif

#endif // MC_SHARED_DATA_EPH_H


