

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

propag.hh

Declaration of functions to load cnfs in the workeru.

--------------------------------------------------------------*/

#ifndef PROPAG_H
#define PROPAG_H

#include "nervenet.hh"

void neuron_propag_handler(missive* msv) bj_propag_cod;
void synapse_propag_handler(missive* msv) bj_propag_cod;
void nervenet_propag_handler(missive* msv) bj_propag_cod;

void bj_propag_kernel_func() bj_propag_cod;
void bj_propag_init_handlers() bj_propag_cod;
void bj_propag_main() bj_propag_cod;

int cmp_nervenodes(nervenode* nod1, nervenode* nod2) bj_propag_cod;

#endif		// PROPAG_H


