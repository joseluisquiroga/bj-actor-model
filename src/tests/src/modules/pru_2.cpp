

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


#include "cell.hh"
#include "pru_2.h"
#include "common.h"

void m2_f1_shd(){
	prts("m2_f1_shd \n");		
}

void m2_f2_shd(){
	prts("m2_f2_shd \n");		
}

char* msg2 mc_mod2_dat = mc_cstr("m2_f3_workeru \n");
void m2_f3_workeru(){
	prts(msg2);
}


void m2_main() {
	prts("STARTED m2_main !!\n");	

	m2_f1_shd();
	m2_f3_workeru();
	m2_f2_shd();

	comm_f1_workeru();

	prts("FINISHED m2_main !!\n");	
}

