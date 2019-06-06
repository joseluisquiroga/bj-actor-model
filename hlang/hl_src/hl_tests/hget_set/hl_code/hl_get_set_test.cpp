

#include <stdio.h>

#include "hlang.hh"
#include "hl_get_set_test.h"

hcell_class_def(cls_gs_data);

hdbg_pre_hh(cls_gs_data, 
R"cod(
void gs_dbg_func(void*);
)cod");

hdbg_pre_cpp(cls_gs_data,
R"cod(
void gs_dbg_func(void* pm){
	hg_dbg_get_set_st* dat = (hg_dbg_get_set_st*)pm;
	MC_MARK_USED(dat);
	PTD_CK(dat != hg_null);
	const char* tok_str = hg_dbg_tok_to_str(dat->tok);
	const char* id_str = hg_dbg_att_id_to_str(dat->att_id);
	printf("dbg_get_set: %s %s %ld \n", tok_str, id_str, dat->msg_val);
}
)cod");

hnucleus_def(cls_gs_data, central, (
	hswitch(hmsg_tok_as(char)) /= (
		hcase(htk_start) /= (
			//hdbg_get(s1),
			//hdbg_set(s1),
			dst = hmsg_ref_as(cls_gs_data),
			s1 = hlit(123),
			HPRT(R"("before hset 1\n")"),
			hset(dst, hatt(hid_cls_gs_data_v1), s1),
			HPRT(R"("after hset 1\n")"),
			s1 = hlit(321),
			HPRT(R"("before hset 2\n")"),
			hset(dst, hatt(hid_cls_gs_data_v2), s1),
			HPRT(R"("after hset 2\n")"),
			hget(dst, hatt(hid_cls_gs_data_v1), s2),
			s1 = s2,
			HPRT(R"("GOT s2 = %ld\n", s2)"),
			s1 = hlit(0),
			s2 = hlit(0),
			HPRT(R"("FINISHING\n")"),
			hfinished
		)
	),
	hreturn
));


int main(int argc, char *argv[]){
	int rr = 0;
	printf("HGENERATION_STARTING: hl_get_set_test ==================================== \n");
	
	fprintf(stdout, "######################################################\n");

	HLANG_SYS().set_dbg_mem_code_generation("cls_gs_data");
	HLANG_SYS().init_sys(hl_null);
	
	fprintf(stdout, "RUNNING PATH= %s \n", path_get_running_path().c_str());

	HLANG_SYS().generate_cpp_code();

	printf("HGENERATION_FINISHED: hl_get_set_test ==================================== \n");
	return rr;
}

