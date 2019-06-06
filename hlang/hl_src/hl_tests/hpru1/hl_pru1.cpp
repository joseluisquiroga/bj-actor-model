

#include <stdio.h>
#include <bitset>

#include "hlang.hh"
#include "hl_pru1.h"

hdefine_token(pr_tok_pru1);
hdefine_token(pr_tok_pru2);

hdefine_const(k0, 98);
hdefine_const(k1, 11);
hdefine_const(k2, 123);

hdbg_pre_hh(cls_A1, 
R"cod(
	/* this is just a comment for pre hh cod */
)cod");

hdbg_pos_hh(cls_A1,
R"cod(
	/* this is just a comment for pos hh cod */
)cod");

hdbg_pre_cpp(cls_A1,
R"cod(
	/* this is just a comment for pre cpp cod */
)cod");

hdbg_pos_cpp(cls_A1,
R"cod(
	/* this is just a comment for pos cpp cod */
)cod");

hcell_class_def(cls_A1);
hcell_class_def(cls_A2);
hcell_class_def(cls_A3);
hcell_class_def(cls_A4);

hmissive_class_def(msv_1);

hnucleus_def(cls_A1, central, (
	(o1 = (o1 + o2)),
	mth01(), 
	hreturn
));

hmethod_def(cls_A1, mth01, (
	(o1 = (o1 + o2)),
	(o2 = (o2 + o1)),
	(o1 = o2) = (o4 + o5),
	(o1 = o2) = (o4 + o5),
	(o1 = o2),
	o4++, o5++, 
	hswitch(o1) /= (
		hcase(o2) /= o3++, 
		hcase(o4) /= ++o5, 
		hcase(o1) /= o2--
	), 
	//o1 = (o1, o2),
	//hif((o1 = o1, o2 = o2, o3 = o4)) /= o1,
	hif(b3 && (b4 || b1)) /= (~ o1 + ! o2),
	hif(o3 && (o4 || o1)) /= (~ o1 + ! o2),
	helif(o4) /= ( (~ o1 + ! o2), (o5 && o3)),
	helse /= o2--,
	hfor(o1 < o2, (o1++, o2 = o3 + o4)) /= (o2 || o3),
	hwhile(o4) /= (
		(o1 & o3),
		o2++,
		o3++
	),
	hreturn,
	//mth02(), 
	hcontinue,
	hbreak,
	((o1 < o2) <= (o3 > o4)),
	o4--
));

hmethod_def(cls_A1, mth02, (
	! (hmsg_ref_as(cls_A3)->xx1()),
	(o1 = (o1 + o2)),
	mth01(), 
	hreturn
));

hmethod_def(cls_A1, mth03, (
	hif(o3) /= (~ o1 + ! o2),
	hsend(rA3, pr_tok_pru1, o5),
	(o2 = (o2 + o1)),
	(o2 = (o2 + o1)),
	o1 = hme(),
	mth02()
));

hmethod_def(cls_A1, mth04, (
	mth03(),
	mth02(),
	o1 = hme(),
	o2 = hlit(12345),
	hsend(rA2, pr_tok_pru2, o4)
));

hnucleus_def(cls_A2, nucl_A2, (
	hreturn
));

hmethod_def(cls_A2, mth01, (
	hif(v1) /= (~ o1 + ! o2),
	(r1->o4()) + v2,
	hcon(k2)++
));

hmethod_def(cls_A2, mth02, (
	mth01()
));

hmethod_def(cls_A2, mth03, (
	mth02()
));

hmethod_def(cls_A2, mth04, (
	mth03()
));

hmethod_def(cls_A2, mth05, (
	mth04()
));

hl_string pru_str = "prueba_1";

hnucleus_def(cls_A3, nucl_A3, (
	hreturn
));

hmethod_def(cls_A3, mth01, (
	r3->r1->o4(), 
	r3->r1->o4(),
	aa1 = r3->r1(), 
	r3->r1() = v1,
	r3->r1->mth02(), 
	aa1 = aa2, 
	aa2 = aa3, 
	aa3 = aa1,
	//hcase(r3) /= r3--,
	hmsg_src_as(cls_A1)->o4(),
	hmsg_ref_as(cls_A1)->o4(),
	hmsg_val_as(long)--,
	hmsg_tok_as(char)--,
	hdbg(R"my_code(
		/* my coment */
		// other  comment
	printf("%s %d \n\t", añlskdhgf kajhnfg 
	askjdhf """)
	)my_code"
	)
));

hmethod_def(cls_A3, mth02, (
			/*hif(o3) /= o2++,
			helif(o3) /= (
				hwhile(o3) /= (
					hif(o3) /= o2--,	// FIX_THIS_CASE
					o3++
				),
				o2++
			),*/
	
	hswitch(o1) /= (
		hcase(o2) /= (
			hif(o3 && (o4 || o1)) /= (~ o1 + ! o2),
			helif(o4) /= (
				hwhile(o4) /= (
					(o1 & o3),
					o2++,
					hif(o3) /= (o2++, hbreak),
					hif(o3) /= (o2++, hreturn),
					o3++
				),
				o2++,
				o1 = r3->r1->o4()
			),
			helse /= o2--,
			hfor(o1 < o2, (o1++, o2 = o3 + o4)) /= (
				(o2 || o3),
				o3++,
				hif(o3) /= (o2++, hbreak),
				hif(o3) /= (o2++, hcontinue),
				o2--,
				hfor(o1 < o2, (o1++, o2 = o3 + o4)) /= (o2 || o3)
			),
			hswitch(o1) /= (
				hcase(o2) /= o3++, 
				hcase(o4) /= ++o1, 
				hcase(o1) /= o2--
			) 
		),
		hcase(o4) /= o2--,
		hcase(o1) /= o2--
	),
	aa1 = aa2, 
	hset(aa1, hatt(hid_cls_A1_b4), xx1),
	aa2 = aa3, 
	hget(aa1, hatt(hid_cls_A1_b1), o4),
	aa3 = aa1
));

hmethod_def(cls_A3, mth03, (
	hswitch(o1) /= (
		hcase(o4) /= (
			hif(o3 && (o4 || o1)) /= (~ o1 + ! o2) ,
			helse /= o2--
		)
	),
	aa1 = aa2
));

hnucleus_def(cls_A4, nucl_A4, (
	hacquire(r1),
	hreturn
));

/*hmethod_def(msv_1, mth01, (
	hreturn
));*/


/*
class CLS_AA {};
class CLS_BB : public CLS_AA {};
class CLS_CC : public CLS_BB {};
*/

hdefine_const(kk1, 123);

void hl_test_1(int argc, char *argv[])
{
	fprintf(stdout, "bj_mchl_test_1 \n");
	
	//haddress(CLS_A, e1);
	
	fprintf(stdout, "######################################################\n");

	HLANG_SYS().init_sys(hl_null);
	
	fprintf(stdout, "RUNNING PATH= %s \n", path_get_running_path().c_str());

	HLANG_SYS().generate_cpp_code();
	
	if(argc > 2){
		std::size_t sz = 0;
		long v1 = std::stol(argv[1], &sz, 2);
		long v2 = std::stol(argv[2], &sz, 2);
		long v3 = (v1 & v2);
		std::string b1 = std::bitset<sizeof(long)>(v1).to_string();
		std::string b2 = std::bitset<sizeof(long)>(v2).to_string();
		std::string b3 = std::bitset<sizeof(long)>(v3).to_string();
		//bool all_ok = ((v1 & v2) == 0);
		fprintf(stdout, "%s & %s == %s \n", b1.c_str(), b2.c_str(), b3.c_str());
	}
	
	fprintf(stdout, "\n End of Using bj_mchl_test_1\n");
}

int main(int argc, char *argv[]){
	int rr = 0;
	printf("HGENERATION_STARTING: hcod_compile_pru1 ==================================== \n");
	
	hl_test_1(argc, argv);	

	printf("HGENERATION_FINISHED: hcod_compile_pru1 ==================================== \n");
	return rr;
}

