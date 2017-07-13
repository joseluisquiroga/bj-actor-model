
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

mc_addr_t 
mck_get_module_address(uint32_t modl_idx){
	return mc_null;
}

char* 
mck_get_module_name(uint32_t modl_idx){
	return const_cast<char*>("NO_MODULE_NAME");
}

void
mck_fill_module_external_addresses(int user_sz, char** user_order, mc_addr_t* user_ext_addr){
}

bool
mck_load_module(mc_addr_t ext_addr){
	return true;
}

