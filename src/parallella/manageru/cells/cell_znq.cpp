

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


#include <new>

#include "booter.h"
#include "cell.hh"
#include "dyn_mem.h"

#include "workeru_loader_znq.h"

kernel* mch_PT_THE_KERNEL = mc_null;

kernel*
mch_get_first_kernel(){
	mch_PT_THE_KERNEL = mc_malloc64(kernel, 1);
	return mch_PT_THE_KERNEL;
}

void 
mck_send_irq(mc_workeru_id_t koid, uint16_t num_irq) {
	mch_abort_func((mc_addr_t)mck_send_irq, "ERROR. Host cannot send irqs.");
}

kernel* //static 
kernel::get_workeru_kernel(mc_workeru_id_t id){
	mc_workeru_nn_t nn = mc_id_to_nn(id);
	mc_off_sys_st* pt_shd_data = mcz_pt_external_manageru_data_obj;
	mck_glb_sys_st* glb_dat = 
		(mck_glb_sys_st*)mc_eph_addr_to_znq_addr((mc_addr_t)(pt_shd_data->sys_workerus[nn]).workeru_data);
	if(glb_dat == mc_null){
		return mc_null;	//not inited yet
	}
	if(glb_dat->inited_workeru != id){
		return mc_null;	//not inited yet
	}
	kernel* ker = (kernel*)(glb_dat->pt_workeru_kernel);
	ker = (kernel*)mc_eph_addr_to_znq_addr((mc_addr_t)ker);
	return ker;
}
