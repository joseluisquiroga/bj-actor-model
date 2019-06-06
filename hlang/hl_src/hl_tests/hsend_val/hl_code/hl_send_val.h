

#ifndef HL_SEND_TEST_H
#define HL_SEND_TEST_H

#include "hlang.hh"

class cls_snd_val;

hdeclare_token(pr_tok_snd_start);
hdeclare_token(pr_tok_snd_val1);
hdeclare_token(pr_tok_snd_val2);
hdeclare_token(pr_tok_snd_add);
hdeclare_token(pr_tok_snd_finished);

hdeclare_const(k0);
hdeclare_const(k1);
hdeclare_const(k2);

class cls_snd_val : public hcell {
public:
	hcell_class(cls_snd_val);
	
	hreference(cls_snd_val, dst);
	hchar(v1);
	hint(v2);
	hlong(sum);

	hnucleus(central);
	
	hmethod(mth_start);
	
	cls_snd_val(){}

	~cls_snd_val(){}
};

void hl_send_test(int argc, char *argv[]);

#endif		// HL_SEND_TEST_H

