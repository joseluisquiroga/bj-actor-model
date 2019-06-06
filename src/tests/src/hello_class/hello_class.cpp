

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

class hello : public cell {
public:
	char f1;
	void handler_1(missive* msv);
};

void 
hello::handler_1(missive* msv){
	mck_slog2("HELLO_CLASS\n");	
}

typedef void (cell::*mc_handler_t)(missive* msv);

void mc_workerus_main() {
	mck_glb_init(false);

	hello ob1;

	mc_handler_t pt_fun = (mc_handler_t)(&hello::handler_1);

	(ob1.*pt_fun)(mc_null);

	mck_glb_finish();
}

