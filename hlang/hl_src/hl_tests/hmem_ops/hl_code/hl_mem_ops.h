

#ifndef HL_MEM_OPS_H
#define HL_MEM_OPS_H

#include "hlang.hh"

class msv_1;
class cls_mem_ops;

hdeclare_token(tk_start);
hdeclare_token(tk_write_val);
hdeclare_token(tk_finished);

class msv_1 : public hcell {
public:
	hmissive_class(msv_1);
	
	hlong(vv);
};

class cls_mem_ops : public hcell {
public:
	hcell_class(cls_mem_ops);
	
	hreference(msv_1, dat);
	
	hsafe_long(out);
	hsafe_long(in);

	hnucleus(central);
	
	hmethod(mth_start);
	
	cls_mem_ops(){}

	~cls_mem_ops(){}
};

void hl_mem_ops_test(int argc, char *argv[]);

#endif		// HL_MEM_OPS_H

