

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

dbg_only.hh

Declaration of functions to load cnfs in the core.

--------------------------------------------------------------*/

#ifndef DBG_ONLY_H
#define DBG_ONLY_H

#include "nervenet.hh"

void nervenet_dbg_only_handler(missive* msv) bj_dbg_only_cod;

void bj_dbg_only_main() bj_dbg_only_cod;
void bj_dbg_only_init_handlers() bj_dbg_only_cod;

void bj_dbg_separate() bj_dbg_only_cod;

#endif		// DBG_ONLY_H


