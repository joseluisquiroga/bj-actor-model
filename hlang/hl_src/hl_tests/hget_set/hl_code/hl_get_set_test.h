

#ifndef HL_GET_SET_TEST_H
#define HL_GET_SET_TEST_H

#include "hlang.hh"

class cls_gs_data;

class cls_gs_data : public hcell {
public:
	hcell_class(cls_gs_data);
	
	hreference(cls_gs_data, dst);
	hchar(v1);
	hint(v2);
	hlong(v3);
	hreference(cls_gs_data, v4);
	hreference(cls_gs_data, v5);

	hsafe_long(s1);
	hsafe_long(s2);
	hsafe_reference(cls_gs_data, s3);
	hsafe_reference(cls_gs_data, s4);
	
	hnucleus(central);
	
	cls_gs_data(){}

	~cls_gs_data(){}
};

void hl_get_set(int argc, char *argv[]);

#endif		// HL_GET_SET_TEST_H

