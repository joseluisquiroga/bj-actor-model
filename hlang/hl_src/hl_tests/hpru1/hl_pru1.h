

#ifndef TEST_PRU1_H
#define TEST_PRU1_H

#include "hlang.hh"

// ==================================================================
// TEST CODE
// TEST CODE
// TEST CODE
// TEST CODE
// TEST CODE
// TEST CODE

hdeclare_token(pr_tok_pru1);
hdeclare_token(pr_tok_pru2);

hdeclare_const(k0);
hdeclare_const(k1);
hdeclare_const(k2);

class cls_A2;
class cls_A3;

class msv_1 : public hcell {
public:
	hmissive_class(msv_1);
	
	hchar(a1);
	hmethod(mth01);
};


class cls_A1 : public hcell {
public:
	hcell_class(cls_A1);

	hbool(b1);
	hbool(b2);
	hbool(b3);
	hbool(b4);
	hchar(v1);
	hint(v2);
	hlong(o1);
	hlong(o2);
	hlong(o3);
	hreference(msv_1, o4);
	hreference(msv_1, o5);
	hint8_t(o6);
	huint8_t(o7);

	hreference(cls_A2, rA2);
	hreference(cls_A3, rA3);

	hnucleus(central);
	
	hmethod(mth01);
	hmethod(mth02);
	hmethod(mth03);
	hmethod(mth04);
	
	cls_A1(){
		o1 = 1;
		o2 = 2;
		o3 = 3;
	}

	~cls_A1(){}
	
};

class cls_A2 : public hcell {
public:
	hcell_class(cls_A2);
	
	hchar(v1);
	hint(v2);
	hlong(o1);
	hlong(o2);
	
	hreference(cls_A1, r1);
	hreference(cls_A1, rr2);
	
	hmethod(mth01);
	hmethod(mth02);
	hmethod(mth03);
	hmethod(mth04);
	hmethod(mth05);

	hnucleus(nucl_A2);
	
};

class cls_A3 : public hcell {
public:
	hcell_class(cls_A3);
	
	hlong(o1);
	hlong(o2);
	hlong(o3);
	hsafe_reference(msv_1, o4);
	
	hsafe_int(xx1);
	hchar(v1);
	hint(v2);
	hreference(cls_A2, rr2);
	hreference(cls_A2, r3);

	hreference(cls_A1, aa1);
	hreference(cls_A2, aa2);
	hreference(cls_A3, aa3);
	
	hmethod(mth01);
	hmethod(mth02);
	hmethod(mth03);

	hnucleus(nucl_A3);
};

class cls_A4 : public hcell {
public:
	hcell_class(cls_A4);
	
	hchar(a1);
	hreference(msv_1, r1);

	hnucleus(nucl_A4);
};

void hl_test_1(int argc, char *argv[]);


#endif		// TEST_PRU1_H

