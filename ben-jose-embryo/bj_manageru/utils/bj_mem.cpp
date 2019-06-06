

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

mem.cpp  

mem trace funcs and other.

--------------------------------------------------------------*/

#include <cassert>

#include "bj_mem.h"
#include "bj_stream.h"
#include "ch_string.h"
#include "stack_trace.h"

//======================================================================
// Assert 

bool 
bj_call_assert(bool vv_ck, const char* file, int line, 
			const char* ck_str, const char* msg){
	if(! vv_ck){
		bj_out << "ASSERT '" << ck_str << "' FAILED" << bj_eol;
		bj_out << get_stack_trace(file, line) << bj_eol;
		if(msg != NULL_PT){
			bj_out << "MSG=" << msg << bj_eol;
		}
	}
	assert(vv_ck);
	return vv_ck;
}

