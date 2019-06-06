

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

------------------------------------------------------------

binder.cpp  

binder class.

------------------------------------------------------------*/

#include "binder.hh"
#include "global.h"

mc_size_t
binder::calc_size(){
	// for debug purposes (it only work on local binders)
	binder* loc_ths = (binder*)mck_as_loc_pt(this);
	binder * fst, * lst, * wrk;
	mc_size_t sz = 0;

	fst = loc_ths->bn_right;
	lst = loc_ths;
	for(wrk = fst; wrk != lst; wrk = wrk->bn_right){
		sz++;
	}
	return sz;
}

