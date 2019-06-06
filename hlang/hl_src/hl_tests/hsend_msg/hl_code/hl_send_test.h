

#ifndef HL_SEND_TEST_H
#define HL_SEND_TEST_H

#include "hlang.hh"

class msv_1;
class cls_snd;

hdeclare_token(tk_start);
hdeclare_token(tk_write_val);
hdeclare_token(tk_finished);

class msv_1 : public hcell {
public:
	hmissive_class(msv_1);
	
	hlong(vv);
};

class cls_snd : public hcell {
public:
	hcell_class(cls_snd);
	
	hreference(msv_1, dat);
	
	hsafe_long(out);
	hsafe_long(in);

	hnucleus(central);
	
	hmethod(mth_start);
	
	cls_snd(){}

	~cls_snd(){}
};

void hl_send_test(int argc, char *argv[]);

#endif		// HL_SEND_TEST_H

