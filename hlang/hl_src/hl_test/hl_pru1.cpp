

#include <stdio.h>

#include "hlang.hh"
#include "hl_pru1.h"

hcell_class_def(cls_A1);
hcell_class_def(cls_A2);
hcell_class_def(cls_A3);
hcell_class_def(cls_A4);

hnucleus_def(cls_A1, central, (
	(r1 = (o1 + o2)),
	mth01(), 
	hreturn
));

hmethod_def(cls_A1, mth01, (
	(t1 = bj_tok_pru_get),
	(r1 = (o1 + o2)),
	(o2 = (r2 + r1)),
	(r1 = r2) = (r4 + r5),
	(o1 = o2) <<= (o4 + o5),
	(o1 <<= o2),
	o4, o5, 
	hswitch(o1) >> (
		hcase(o2) >> o3++, 
		hcase(o4) >> ++o5,
		hcase(o1) >> o2
	),
	hif(o3) >> (~ o1 + ! o2),
	helif(o4) >> (o5 && o3),
	hfor(o4) >> (o2 || o3),
	hwhile(o4) >> (o1 & o3),
	hreturn,
	//mth02(), 
	hcontinue,
	hbreak,
	helse >> o2,
	((o1 < o2) <= (o3 > o4)),
	o4
));

hmethod_def(cls_A1, mth02, (
	(r1 = (o1 + o2)),
	mth01(), 
	hreturn
));

hmethod_def(cls_A1, mth03, (
	hif(o3) >> (~ o1 + ! o2),
	(o2 = (r2 + r1)),
	(o2 <<= (r2 + r1)),
	r1 = hme()
));

hmethod_def(cls_A2, mth01, (
	hif(v1) >> (~ o1 + ! o2),
	(r1->o4) + v2
));

hmethod_def(cls_A3, mth01, (
	r3->r1->o4(), 
	r3->r1->mth02(), 
	r3->r1->o4(),
	aa1 = aa2, 
	aa2 <<= aa3, 
	aa3 = aa1,
	hmsg_src(cls_A1)->o4(),
	hmsg_ref(cls_A1)->o4(),
	hmsg_val(long),
	hmsg_tok(char)
));

class CLS_AA {};
class CLS_BB : public CLS_AA {};
class CLS_CC : public CLS_BB {};

void hl_test_1(int argc, char *argv[])
{
	printf("bj_mchl_test_1 \n");
	
	hexternal(CLS_A, e1);
	hconst(long, k1, 123);
	
	printf("######################################################\n");
	printf("######################################################\n");
	
	HLANG_SYS().init_all_attributes();
	HLANG_SYS().call_all_registered_methods();
	
	printf("\n End of Using bj_mchl_test_1\n");
}

