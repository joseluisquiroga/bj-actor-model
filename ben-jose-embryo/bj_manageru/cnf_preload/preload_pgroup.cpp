
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preload.hh"

pre_item_pgroup*
pre_item_pgroup::get_pnt_of_nxt_add(pre_pgroup* grp){
	pre_item_pgroup* curr = this;
	
	while((curr->pnt != (void*)grp) && (curr->get_pnt()->rgt == curr)){
		curr = curr->get_pnt();
	}

	if(curr->pnt != (void*)grp){
		curr = curr->get_pnt();
		if(curr->rgt == mc_null){
			PTD_CK(curr != mc_null);
			return curr;
		}
		curr = curr->rgt;
	} 
	
	while(curr->lft != mc_null){
		curr = curr->lft;
	}

	PTD_CK(curr != mc_null);
	return curr;
}

void
pre_pgroup::add_item(pre_item_pgroup* itm){
	pre_pgroup* grp = this;
	
	if(fst == mc_null){
		fst = itm;
		lst = itm;
		itm->pnt = grp;
		return;
	}
	
	PTD_CK(lst != mc_null);
	
	pre_item_pgroup* pnt_nxt = lst->get_pnt_of_nxt_add(grp);
	PTD_CK(pnt_nxt != mc_null);
	
	if(pnt_nxt->lft == mc_null){
		pnt_nxt->lft = itm;
		itm->pnt = pnt_nxt;
		lst = itm;
		return;
	} 
	
	PTD_CK(pnt_nxt->rgt == mc_null);

	pnt_nxt->rgt = itm;
	itm->pnt = pnt_nxt;
	lst = itm;
}

void
create_pgroup(pre_pgroup& grp, num_nod_t num_items){
	
	PTD_CK(grp.fst == mc_null);
	PTD_CK(grp.lst == mc_null);
	
	pre_item_pgroup::separate(num_items);
	
	for(num_nod_t aa = 0; aa < num_items; aa++){
		pre_item_pgroup* itm = bj_pre_item_pgroup_acquire();
		
		grp.add_item(itm);
	}
}

void
pre_item_pgroup::prt_nodes(long dd){
	printf("%ld : ", dd);
	if(lft != mc_null){
		lft->prt_nodes(dd + 1);
	}
	if(rgt != mc_null){
		rgt->prt_nodes(dd + 1);
	}
}
	
void
pre_pgroup::prt_nodes(){
	if(fst == mc_null){
		printf("EMPTY\n");
		return;
	}
	
	fst->prt_nodes(1);
	printf("\n");
}
