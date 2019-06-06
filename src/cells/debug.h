

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

------------------------------------------------------------*/


// debug.h

#ifndef MC_DEBUG_H
#define MC_DEBUG_H

#define mc_c_decl extern "C"

#define MC_STRINGIFY(x) #x
#define MC_TOSTRING(x) MC_STRINGIFY(x)

#define MC_ABORT_MSG(msg) mc_cstr("ABORTING. '" msg "' at " __FILE__ "(" MC_TOSTRING(__LINE__) ")")

#ifdef __cplusplus
mc_c_decl {
#endif

#ifdef FULL_DEBUG
#	define MC_DBG(prm) prm
#else
#	define MC_DBG(prm) /**/ 
#endif

#define MC_MARK_USED(X)  ((void)(&(X)))

#ifdef __cplusplus
}
#endif

#endif // MC_DEBUG_H

