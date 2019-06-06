

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


#include <stdio.h>

#include "booter.h"
#include <cell.hh>

int mc_manageru_main(int argc, char *argv[])
{
	missive_grp_t grp1;
	grp1.tot_agts = 123;

	missive_grp_t* grp2 = &grp1;
	uint8_t* tot1 = &(grp2->tot_agts);
	uint8_t* tot2 = (uint8_t*)(((uint8_t*)grp2) + mc_offsetof(&missive_grp_t::tot_agts));
	printf("offset of all_agts in missive_grp_t ==  %lu\n", mc_offsetof(&missive_grp_t::all_agts));
	printf("tot1 %d \n", *tot1);
	printf("tot2 %d \n", *tot2);

	return 0;
}

void mc_workerus_main(){
}
