
#include "global.h"

#include "thread_emu.hh"
#include "booter.h"

//======================================================================
// off chip shared memory

mc_off_sys_st mcm_external_host_data_obj;

//=====================================================================
// global funcs

void 
mck_abort(mc_addr_t err, char* orig_msg) {
	char msg[300];
	emu_info_t* inf = mck_get_emu_info();
	snprintf(msg, 300, "ABORTED THREAD=%ld \t CORE_ID=%x MSG=%s\n", inf->emu_id, inf->emu_core_id, orig_msg);
	mch_abort_func(err, msg);
}

mck_glb_sys_st*
mck_get_glb_sys(){
	return &(mck_get_emu_info()->emu_glb_sys_data);
}

void 
mck_set_irq0_handler(){
}

