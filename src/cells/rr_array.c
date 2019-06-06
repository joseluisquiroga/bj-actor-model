

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


#include "shared.h"
#include "global.h"
#include "rr_array.h"

//==============================================================================
// rrarray funcs

#define MC_CRC16_POLY 0x8408

uint16_t 
mc_crc16(uint8_t *data_p, uint16_t length) mc_external_code_ram;

uint16_t 
mc_get_v16_of_p8(uint8_t* p8) mc_external_code_ram;

void
mc_set_v16_of_p8(uint8_t* p8, uint16_t v16) mc_external_code_ram;

uint16_t
mc_rr_get_v16(mc_rrarray_st* arr, uint8_t** pt_dat) mc_external_code_ram;

void
mc_rr_set_v16(mc_rrarray_st* arr, uint8_t** pt_dat, uint16_t v16) mc_external_code_ram;



uint16_t 
mc_crc16(uint8_t *data_p, uint16_t length){
	uint8_t ii;
	uint16_t data;
	uint16_t crc = 0xffff;

	if (length == 0) {
		return (~crc);
	}

	do{
		for (ii = 0, data = (uint16_t)0xff & *data_p++;
				ii < 8; 
				ii++, data >>= 1)
		{
				if ((crc & 0x0001) ^ (data & 0x0001)) {
					crc = (crc >> 1) ^ MC_CRC16_POLY;
				} else { 
					crc >>= 1; 
				}
		}
	} while (--length);

	crc = ~crc;
	data = crc;
	crc = (crc << 8) | (data >> 8 & 0xff);

	return (crc);
}

uint16_t 
mc_get_v16_of_p8(uint8_t* p8){
	uint16_t v16 = p8[1];
	v16 <<= 8;
	v16 |= p8[0];
	return v16;
}

void
mc_set_v16_of_p8(uint8_t* p8, uint16_t v16){
	uint16_t low = v16 & 0x00ff;
	uint16_t hi = v16 & 0xff00;
	hi >>= 8;
	p8[1] = (uint8_t)hi;
	p8[0] = (uint8_t)low;
}

void
mc_rr_init(mc_rrarray_st* arr, uint16_t sz, uint8_t* dat, uint8_t reset){
	if(arr == mc_null){
		return;
	}
	mc_loop_set_var(arr->magic_id, MC_MAGIC_ID);
	mc_loop_set_var(arr->data, dat);
	if(reset){
		mc_memset(arr->data, 0, sz);
	}
	
	mc_loop_set_var(arr->end_data, (dat + sz));
	mc_loop_set_var(arr->wr_obj, arr->data);
	mc_loop_set_var(arr->rd_obj, arr->data);
	
	mc_loop_set_var(arr->num_wr_errs, 0);
	mc_loop_set_var(arr->wr_err, 0);
	mc_loop_set_var(arr->rd_err, 0);
}

uint16_t
mc_rr_get_v16(mc_rrarray_st* arr, uint8_t** pt_dat){
	if(arr == mc_null){
		return 0;
	}
	uint8_t* dat = *pt_dat;
	uint8_t asz[2];
	int pos;
	for(pos = 0; pos < 2; pos++){
		if(dat == arr->end_data){
			dat = arr->data;
		}
		asz[pos] = *dat;
		dat++;
	}
	*pt_dat = dat;
	uint16_t osz = mc_get_v16_of_p8(asz);
	return osz;
}

void
mc_rr_set_v16(mc_rrarray_st* arr, uint8_t** pt_dat, uint16_t v16){
	if(arr == mc_null){
		return;
	}
	uint8_t* dat = *pt_dat;
	uint8_t asz[2];
	mc_set_v16_of_p8(asz, v16);
	int pos;
	for(pos = 0; pos < 2; pos++){
		if(dat == arr->end_data){
			dat = arr->data;
		}
		*dat = asz[pos];
		dat++;
	}
	*pt_dat = dat;
}

uint16_t
mc_rr_read_obj(mc_rrarray_st* arr, uint16_t omc_sz, uint8_t* obj){
	if(arr == mc_null){
		return 0;
	}
	uint8_t* dat = arr->rd_obj;
	uint16_t osz = mc_rr_get_v16(arr, &dat);
	if(osz == 0){
		mc_loop_set_var(arr->rd_err, -1);
		return 0;
	}
	if(osz > omc_sz){
		mc_loop_set_var(arr->rd_err, -2);
		return 0;
	}
	if(osz > (arr->end_data - arr->data)){
		mc_loop_set_var(arr->rd_err, -3);
		return 0;
	}
	uint16_t ii;
	for(ii = 0; ii < osz; ii++){
		if(dat == arr->end_data){
			dat = arr->data;
		}
		obj[ii] = *dat;
		dat++;
	}
	uint16_t tcrc16 = mc_rr_get_v16(arr, &dat);
	uint16_t rcrc16 = mc_crc16(obj, osz);
	if(tcrc16 != rcrc16){
		mc_loop_set_var(arr->rd_err, -4);
		return 0;
	}
	uint8_t* dat2 = arr->rd_obj;
	while(dat2 != dat){
		if(dat2 == arr->end_data){
			dat2 = arr->data;
		}
		(*dat2) = 0;
		dat2++;
	}
	mc_loop_set_var(arr->rd_obj, dat2);
	return osz;
}


uint16_t
mc_rr_write_obj(mc_rrarray_st* arr, uint16_t omc_sz, uint8_t* obj){
	if(arr == mc_null){
		return 0;
	}
	uint32_t nme = arr->num_wr_errs;
	uint16_t osz = omc_sz;
	if(osz > ((arr->end_data - arr->data) - (2 * sizeof(uint16_t)))){
		mc_loop_set_var(arr->wr_err, -1);
		nme++;
		mc_loop_set_var(arr->num_wr_errs, nme);
		return 0;
	}
	if(osz <= 0){
		mc_loop_set_var(arr->wr_err, -2);
		nme++;
		mc_loop_set_var(arr->num_wr_errs, nme);
		return 0;
	}
	uint16_t w_sz = omc_sz + (3 * sizeof(uint16_t));
	uint8_t* dat2 = arr->wr_obj;
	for(; w_sz != 0; w_sz--){
		if(dat2 == arr->end_data){
			dat2 = arr->data;
		}
		if((*dat2) != 0){
			dat2 = 0;
			break;
		}
		dat2++;
	}
	if(dat2 == 0){
		mc_loop_set_var(arr->wr_err, -3);
		nme++;
		mc_loop_set_var(arr->num_wr_errs, nme);
		return 0;
	}
	uint8_t* dat = arr->wr_obj;
	mc_rr_set_v16(arr, &dat, omc_sz);
	uint16_t ii;
	for(ii = 0; ii < osz; ii++){
		if(dat == arr->end_data){
			dat = arr->data;
		}
		*dat = obj[ii];
		dat++;
	}
	uint16_t rcrc16 = mc_crc16(obj, osz);
	mc_rr_set_v16(arr, &dat, rcrc16);
	mc_loop_set_var(arr->wr_obj, dat); 
	return osz;
}


