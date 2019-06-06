

#include <stdio.h>

#include "hlang.hh"
#include "hl_send_val.h"

hdefine_token(pr_tok_snd_start);
hdefine_token(pr_tok_snd_val1);
hdefine_token(pr_tok_snd_val2);
hdefine_token(pr_tok_snd_add);
hdefine_token(pr_tok_snd_finished);

hdefine_const(k0, 98);
hdefine_const(k1, 11);
hdefine_const(k2, 123);

hcell_class_def(cls_snd_val);

hnucleus_def(cls_snd_val, central, (
	hswitch(hmsg_tok_as(char)) /= (
		hcase(htk_start) /= mth_start(), 
		hcase(pr_tok_snd_val1) /= (
			v1 = hmsg_val, 
			hdbg(R"my_code(printf("GOT pr_tok_snd_val1\n");)my_code")
		),
		hcase(pr_tok_snd_val2) /= (
			v2 = hmsg_val_as(int),
			hdbg(R"my_code(printf("GOT pr_tok_snd_val2\n");)my_code")
		),
		hcase(pr_tok_snd_add) /= (
			sum = (v1 + v2),
			hdbg(R"my_code(printf("GOT pr_tok_snd_add\n");)my_code"),
			hsend(hmsg_src, pr_tok_snd_finished, hlit(0))
		),
		hcase(pr_tok_snd_finished) /= (
			hdbg(R"my_code(printf("GOT pr_tok_snd_finished. Ending with abort (no sync). \n");)my_code"),
			hfinished
		)
	),
	hreturn
));


hmethod_def(cls_snd_val, mth_start, (
	hdbg(R"my_code(printf("Started mth_start\n");)my_code"),
	dst = hmsg_ref_as(cls_snd_val),
	hsend(dst, pr_tok_snd_val1, hcon(k1)),
	hsend(dst, pr_tok_snd_val2, hcon(k2)),
	hsend(dst, pr_tok_snd_add, hcon(k0)),
	hdbg(R"my_code(printf("Ended mth_start\n");)my_code")
));

int main(int argc, char *argv[]){
	int rr = 0;
	printf("HGENERATION_STARTING: hl_send_val ==================================== \n");
	
	fprintf(stdout, "######################################################\n");

	HLANG_SYS().allow_send_values("cls_snd_val");
	HLANG_SYS().init_sys(hl_null);
	
	fprintf(stdout, "RUNNING PATH= %s \n", path_get_running_path().c_str());

	HLANG_SYS().generate_cpp_code();

	printf("HGENERATION_FINISHED: hl_send_test ==================================== \n");
	return rr;
}

