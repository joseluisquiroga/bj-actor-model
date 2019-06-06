

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

string_hst.hh

Wrapper for string class.

------------------------------------------------------------*/


#ifndef MC_STRING_H
#define MC_STRING_H

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>	// used for mc_ofstream

typedef std::string mch_string_t;
typedef std::ostream mch_ostream_t;
typedef std::ostringstream mch_ostr_stream_t;
typedef std::ofstream mch_ofstream_t;

#define mch_out std::cout
#define mch_err std::cerr

#define mch_eol std::endl

#endif // MC_STRING_H


