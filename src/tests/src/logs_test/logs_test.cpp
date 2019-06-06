

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


#include "data_logs.h"
#include "cell.hh"

void mc_workerus_main() {
	mck_glb_init(false);

	mc_workeru_id_t koid = mck_get_workeru_id();
	mc_workeru_nn_t num_workeru = mc_id_to_nn(koid);

	char** john = (char**)(all_log_tests[num_workeru]);
	long john_sz = all_log_tests_sz[num_workeru];
	long ii;
	for(ii = 0; ii < john_sz; ii++){
		mck_slog(john[ii]);
	}

	mck_glb_finish();
}


