

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

#include "cell.hh"

#include "thread_ptd.hh"

kernel*
mcm_get_kernel(){
	return &(mck_get_ptd_info()->ptd_THE_KERNEL);
}

void 
mck_send_irq(mc_workeru_id_t koid, uint16_t num_irq) {
	mck_abort((mc_addr_t)mck_send_irq, mc_cstr("mck_send_irq abort"));
}

kernel* //static 
kernel::get_workeru_kernel(mc_workeru_id_t id){
	mc_workeru_nn_t nn = mc_id_to_nn(id);
	PTD_CK((0 <= nn) && (nn < TOT_THREADS));
	thread_info_t* info = &(ALL_THREADS_INFO[nn]);
	if(info->thd_ptd.ptd_glb_sys_data.inited_workeru != id){
		PTD_PRT("kernel::get_workeru_kernel ID=%p ------------\n", (void*)(uintptr_t)id);
		return mc_null;
	}
	return &(info->thd_ptd.ptd_THE_KERNEL);
}

void 
ptd_dbg_prt_ack_arr(int sz, mck_ack_t* arr){
	fprintf(stderr, "{"); 
	for(int aa = 0; aa < sz; aa++){ 
		fprintf(stderr, "%d ", arr[aa]); 
	} 
	fprintf(stderr, "}\n"); 
	fflush(stderr);
}

