

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

stabi.hh

Declaration of functions to load cnfs in the workeru.

--------------------------------------------------------------*/

#ifndef STABI_H
#define STABI_H

#include "nervenet.hh"

#define BJ_STABI_SUB_MODULE_MIRROW 1
#define BJ_STABI_SUB_MODULE_STABI 2

#define calc_cap_stabi_arr(tot_syn) (4 * (tot_syn))

#define set_stabi_arr(cap, arr, ii, val) \
	if(ii < cap){ arr[ii] = val; } else { mck_abort(0xdeadbeaf, mc_cstr("ERROR. Invalid_propag_arr_set_index.")); }


void nervenet_mirrow_handler(missive* msv) bj_stabi_cod;

void bj_mirrow_init_handlers() mc_external_code_ram;
void bj_mirrow_main() bj_stabi_cod;

void neuron_stabi_handler(missive* msv) bj_stabi_cod;
void synapse_stabi_handler(missive* msv) bj_stabi_cod;
void nervenet_stabi_handler(missive* msv) bj_stabi_cod;

void bj_stabi_kernel_func() bj_stabi_cod;
void bj_stabi_init_handlers() mc_external_code_ram;
void bj_stabi_main() mc_external_code_ram;

#endif		// STABI_H


