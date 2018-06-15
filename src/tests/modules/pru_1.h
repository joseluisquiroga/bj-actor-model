

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

// pru_1.h

#ifndef PRU_1_H
#define PRU_1_H

#include "attribute.h"

#ifdef __cplusplus
mc_c_decl {
#endif

void m1_f1_shd() mc_external_code_ram;
void m1_f2_shd() mc_external_code_ram;

void m1_f3_core() mc_mod1_cod;
void m1_main() mc_mod1_cod;

#ifdef __cplusplus
}
#endif

#endif // PRU_1_H

