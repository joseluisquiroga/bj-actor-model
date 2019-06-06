

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


#include "log.h"
#include "shared.h"
#include "global.h"

void
mck_aux_sout(bool cond, char* msg, mc_out_type_t outt){ 
	if(! cond){
		return;
	}
	if(! mck_has_off_workeru){
		return;
	}

	uint8_t extra = 2;
	uint16_t oln = mc_strlen(msg);
	if(oln > (MC_MAX_STR_SZ - extra)){
		oln = (MC_MAX_STR_SZ - extra);
	}

	uint8_t* out_str = MC_WORKERU_INFO->dbg_out_str;
	mc_memcpy(out_str + extra, (uint8_t*)msg, oln);
	out_str[0] = outt;
	out_str[1] = MC_CHR;
	mck_glb_sys_st* glb_dat = MC_WORKERU_INFO;
	uint16_t ow = mc_rr_write_obj(glb_dat->write_rrarray, oln + extra, out_str);
	while(ow == 0){
		mck_wait_sync(MC_WAITING_BUFFER, 0, mc_null);
		ow = mc_rr_write_obj(glb_dat->write_rrarray, oln + extra, out_str);
	}
}

void
mck_aux_iout(uint32_t vv, mc_out_type_t outt, mc_type_t tt){
	if(! mck_has_off_workeru){
		return;
	}

	uint16_t oln = 2 + sizeof(uint32_t);
	uint8_t msg[oln];
	msg[0] = outt;
	msg[1] = tt;
	uint8_t* pt = (uint8_t*)(&vv);
	msg[2] = pt[0];
	msg[3] = pt[1];
	msg[4] = pt[2];
	msg[5] = pt[3];
	mck_glb_sys_st* glb_dat = MC_WORKERU_INFO;
	uint16_t ow = mc_rr_write_obj(glb_dat->write_rrarray, oln, (uint8_t*)msg);
	while(ow == 0){
		mck_wait_sync(MC_WAITING_BUFFER, 0, mc_null);
		ow = mc_rr_write_obj(glb_dat->write_rrarray, oln, (uint8_t*)msg);
	}
}

