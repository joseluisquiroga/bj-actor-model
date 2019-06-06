

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


//include <new>

#include "all_regs.h"

#include "cell.hh"
#include "dyn_mem.h"

kernel* mck_PT_THE_KERNEL = mc_null;

kernel*
mck_get_first_kernel(){
	//mck_PT_THE_KERNEL = mc_malloc32(kernel, 1);
	mck_PT_THE_KERNEL = mc_malloc64(kernel, 1);
	return mck_PT_THE_KERNEL;
}

void 
mck_send_irq(mc_workeru_id_t koid, uint16_t num_irq) {
	unsigned* ilatst = (unsigned*)mc_addr_set_id(koid, (void*) MC_REG_ILATST);
	*ilatst = 1 << num_irq;
}

kernel* //static 
kernel::get_workeru_kernel(mc_workeru_id_t id){
	mck_abort(__LINE__, MC_ABORT_MSG("Invalid kernel::get_workeru_kernel func call\n"));
	return mc_null;
}
