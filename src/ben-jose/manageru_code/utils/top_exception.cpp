

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

top_exception.cpp  

top_exception impl.

--------------------------------------------------------------*/

#include <cstdlib>

#include "bj_stream.h"
#include "stack_trace.h"
#include "top_exception.h"

void bj_abort_func(long val, const char* msg){
	bj_err << bj_eol << "ABORTING! " << msg << bj_eol; 
	exit(val);
}

top_exception::top_exception(long the_id, ch_string assrt_str){
	ex_id = the_id;
	ex_stk = STACK_STR;
	ex_assrt = assrt_str;
}

