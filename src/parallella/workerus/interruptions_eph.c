

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


#include "interruptions_eph.h"
#include "shared.h"
#include "global.h"

//pragma GCC diagnostic error "-Wattributes"

uint32_t test_send_irq3 = 0;

void
mck_sync_handler(){	// must be set afer normal start in glb init // IVT_RESET has irq0
	// replaces the boot routines (normal start) that call the main func 
}

void
mck_software_exception_handler(){ 	// ivt_entry_software_exception has irq1
	mck_simple_abort(mck_software_exception_handler, mck_software_exception);
}

void
mck_page_miss_handler(){ 	// ivt_entry_page_miss has irq2
	mck_simple_abort(mck_page_miss_handler, mck_memory_exception);
}

void
mck_timer0_handler(){ 	// ivt_entry_timer0 has irq3
	test_send_irq3++;
}

