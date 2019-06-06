

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

// common.h

#ifndef COMMON_H
#define COMMON_H

#include "attribute.h"

#ifdef __cplusplus
mc_c_decl {
#endif

#define prts(ss) mck_slog2(ss)

//define prts(ss) comm_prt(mc_cstr(ss))

class comm_cla {
public:
	int aa;
	long bb;

	comm_cla() mc_comm_cod;

	~comm_cla() mc_comm_cod;

	void init_comm_cla() mc_comm_cod;

	void method1() mc_comm_cod;
};

void comm_prt(char* val) mc_comm_cod;

void comm_f1_workeru() mc_comm_cod;
void comm_f2_workeru() mc_comm_cod;

#ifdef __cplusplus
}
#endif

#endif // COMMON_H

