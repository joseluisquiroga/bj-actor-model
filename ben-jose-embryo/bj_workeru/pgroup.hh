

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

pgroup.hh

pgroup defs and funcs.

--------------------------------------------------------------*/

#ifndef PGROUP_H
#define PGROUP_H

#include "nervenet.hh"

class end_of_func {};
class end_of_handler {};


#define __fsm_func_init() if(__fsm_current_line < (__fsm_num_call + __LINE__)){

#define __fsm } if(__fsm_current_line == (__fsm_num_call + __LINE__)){


void neuron_pgroup_handler(missive* msv) bj_pgroup_cod;
void synapse_pgroup_handler(missive* msv) bj_pgroup_cod;
void nervenet_pgroup_handler(missive* msv) bj_pgroup_cod;

void bj_pgroup_kernel_func() bj_pgroup_cod;
void bj_pgroup_init_handlers() mc_external_code_ram;
void bj_pgroup_main() mc_external_code_ram;

#endif		// stabi_h


