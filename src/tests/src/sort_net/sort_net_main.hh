

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

sort_net_main.hh

Declaration of main bj solver function.

--------------------------------------------------------------*/

#ifndef SORT_NET_MAIN_H
#define SORT_NET_MAIN_H

#include "cell.hh"

#define BJ_DBG_TOT_INPUT_SORNODES 3

#define bj_sornet_cod mc_mod0_cod
#define bj_sornet_dat mc_mod0_dat

typedef long num_nod_t;
typedef uint32_t dbg_consec_t;

#define BJ_INVALID_NUM_NODE ((num_nod_t)(-1))

#define BJ_MAX_NODE_SZ mc_maxof(num_syn_t)

#define MAGIC_VAL 987654


#endif		// SORT_NET_MAIN_H


