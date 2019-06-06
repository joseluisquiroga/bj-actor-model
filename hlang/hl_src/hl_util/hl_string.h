

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

hl_string.h

func to print a stack trace.

--------------------------------------------------------------*/

#ifndef HL_STRING_H
#define HL_STRING_H

#include <iostream>
#include <sstream>
#include <fstream>	// used for hl_ofstream

#define hl_null NULL

#define hl_c_decl extern "C"

#define HL_STRINGIFY(x) #x
#define HL_TOSTRING(x) HL_STRINGIFY(x)

typedef std::ostream hl_ostream;
typedef std::ostringstream hl_ostringstream;
typedef std::ofstream hl_ofstream;
typedef std::string hl_string;
typedef unsigned char hl_uchar_t;

typedef hl_string::size_type hl_str_pos_t;

#define DO_GETCHAR()			getchar()
#define CARRIAGE_RETURN		((char)13)
#define HL_MAX_STR_SZ 300

#endif		// HL_STRING_H


