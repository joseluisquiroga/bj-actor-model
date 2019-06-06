

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

load_cnf.hh

Declaration of functions to load cnfs in the workeru.

--------------------------------------------------------------*/

#ifndef LOAD_CNF_H
#define LOAD_CNF_H

#include "nervenet.hh"

void nervenet_load_handler(missive* msv) bj_load_cod;
void polaron_load_handler(missive* msv) bj_load_cod;
void synapse_load_handler(missive* msv) bj_load_cod;

void print_childs() bj_load_cod;

void bj_load_main() bj_load_cod;
void bj_load_init_handlers() bj_load_cod;
void bj_load_init_cnf() bj_load_cod;
void bj_load_shd_cnf() bj_load_cod;

void bj_init_end_cells(binder* nn_all_endnods) bj_load_cod;

void bj_load_shd_nods(num_nod_t tot_wu_endnods, binder* nn_all_endnods, binder* nn_all_ecells, 
					  num_nod_t tot_wu_nods, binder* nn_all_nods, binder* nn_all_sclls, 
					  void* invalid_val) bj_load_cod;
					  
void bj_init_ends_srt_sornet() bj_load_cod;
void bj_init_ends_rnk_sornet() bj_load_cod;
	
void bj_load_shd_sornet() bj_load_cod;
void bj_load_shd_ranknet() bj_load_cod;


#endif		// LOAD_CNF_H


