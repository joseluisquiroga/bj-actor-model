

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


#ifndef MC_RRARRAY_H
#define MC_RRARRAY_H

#include <inttypes.h>

#include "attribute.h"

#ifdef __cplusplus
mc_c_decl {
#endif

struct mc_aligned mc_rrarray_def { 
	uint32_t magic_id;
	uint8_t* data;
	uint8_t* end_data;
	uint8_t* wr_obj;
	uint8_t* rd_obj;
	
	uint32_t num_wr_errs;
	int8_t	 wr_err;
	int8_t	 rd_err;
};
typedef struct mc_rrarray_def mc_rrarray_st;

void
mc_rr_init(mc_rrarray_st* arr, uint16_t size, uint8_t* data, uint8_t reset) mc_external_code_ram;

uint16_t
mc_rr_read_obj(mc_rrarray_st* arr, uint16_t omc_sz, uint8_t* obj) mc_external_code_ram;

uint16_t
mc_rr_write_obj(mc_rrarray_st* arr, uint16_t omc_sz, uint8_t* obj) mc_external_code_ram;

#ifdef __cplusplus
}
#endif

#endif // MC_RRARRAY_H
