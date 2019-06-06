

/*************************************************************

This file is part of messaging-cells.

messaging-cells is free software: you can redistribute it and/or modify
it under the terms of the version 3 of the GNU General Public 
License as published by the Free Software Foundation.

messaging-cells is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with messaging-cells.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------------

Copyright (C) 2017-2018. QUIROGA BELTRAN, Jose Luis.
Id (cedula): 79523732 de Bogota - Colombia.
See https://messaging-cells.github.io/

messaging-cells is free software thanks to The Glory of Our Lord 
	Yashua Melej Hamashiaj.
Our Resurrected and Living, both in Body and Spirit, 
	Prince of Peace.

------------------------------------------------------------*/
/*! \file philo.cpp

\brief Implementation of the classical concurrency example of the 
<a href="https://en.wikipedia.org/wiki/Dining_philosophers_problem">eating philosophers</a>.

\details 

\include philo.cpp

------------------------------------------------------------*/

#include "cell.hh"

//define PHILO_EPH_DBG

#ifdef MC_IS_PTD_CODE
#define PHILO_WITH_DBG
#endif

#ifdef PHILO_WITH_DBG
	#include "stdio.h"

	bool dbg_all_idle_prt[16] mc_external_code_ram = {
		false, false, false, false, 
		false, false, false, false, 
		false, false, false, false, 
		false, false, false, false
	};
	bool dbg_all_full[16] mc_external_code_ram = {
		false, false, false, false, 
		false, false, false, false, 
		false, false, false, false, 
		false, false, false, false
	};

	void prt_idle() mc_external_code_ram;
	void prt_full() mc_external_code_ram;
	void prt_all_philo() mc_external_code_ram;

	bool prt_recv_msgs mc_external_code_ram = false;
	bool prt_send_msgs mc_external_code_ram = false;

	#define PH_DBG_COD(prm) prm
#else
	#define PH_DBG_COD(prm) 
#endif // END_OF_PHILO_WITH_DBG

class chopstick;
class philosopher;

#define MAX_BITES 10

#define PH_DBG PTD_PRT

enum ph_hdlr_idx_t : mck_handler_idx_t {
	idx_invalid = mck_tot_base_cell_classes,
	idx_chopstick,
	idx_philosopher,
	idx_last_invalid,
	idx_total
};

missive_handler_t philo_handlers[idx_total];

enum philo_tok_t : mck_token_t {
	tok_invalid = mck_tok_last + 1,
	tok_eat,
	tok_take,
	tok_taken,
	tok_not_taken,
	tok_drop,
	tok_droped,
	tok_not_droped, 
	tok_yes_full
};

//ifdef PHILO_WITH_DBG
	char*
	tok_to_str(philo_tok_t tok) mc_external_code_ram;

	char*
	tok_to_str(philo_tok_t tok){
		switch(tok){
		case tok_invalid:
			return mc_cstr("invalid");
		break;
		case tok_eat:
			return mc_cstr("eat");
		break;
		case tok_take:
			return mc_cstr("take");
		break;
		case tok_taken:
			return mc_cstr("taken");
		break;
		case tok_not_taken:
			return mc_cstr("not_taken");
		break;
		case tok_drop:
			return mc_cstr("drop");
		break;
		case tok_droped:
			return mc_cstr("droped");
		break;
		case tok_not_droped:
			return mc_cstr("not_droped");
		break;
		case tok_yes_full:
			return mc_cstr("yes_fll");
		break;
		default:
			mck_abort(1, mc_cstr("BAD_PHILO_TOK"));
		break;
		}
		return mc_cstr("NO_TOK");
	}
//endif // END_OF_PHILO_WITH_DBG

/*! \class chopstick
\brief Class for chopsticks (\ref philo.cpp example). 

*/
class chopstick : public cell {
public:
	MCK_DECLARE_MEM_METHODS(chopstick)

	cell* owner;

	cell* last_src;
	philo_tok_t last_sent;
	philo_tok_t last_recv;

	chopstick(){
		init_chopstick();
	}

	~chopstick(){}

	void init_chopstick(){
		handler_idx = idx_chopstick;
		owner = mc_null;

		last_src = mc_null;
		last_sent = tok_invalid;
		last_recv = tok_invalid;
	}

	void handler(missive* msv);

	virtual 
	grip&	get_available();
};

/*! \class philosopher
\brief Class for philosophers (\ref philo.cpp example ). 

*/
class philosopher : public cell {
public:
	MCK_DECLARE_MEM_METHODS(philosopher)

	chopstick* left;
	chopstick* right;

	int	num_bites;
	bool lft_ph_full;
	bool rgt_ph_full;

	mc_workeru_id_t	lft_stk_id;
	mc_workeru_id_t	rgt_stk_id;
	chopstick*		lft_stick;
	chopstick*		rgt_stick;
	mc_workeru_id_t	lft_phi_id;
	mc_workeru_id_t	rgt_phi_id;
	philosopher*	lft_philo;
	philosopher*	rgt_philo;

	philo_tok_t last_sent;
	philo_tok_t last_sent_lft;
	philo_tok_t last_sent_rgt;
	philo_tok_t last_recv;
	philo_tok_t last_recv_lft;
	philo_tok_t last_recv_rgt;

	philosopher(){
		init_philosopher();
	}

	~philosopher(){}

	void init_philosopher(){
		handler_idx = idx_philosopher;

		left = mc_null;
		right = mc_null;

		num_bites = 0;
		lft_ph_full = false;
		rgt_ph_full = false;

		lft_stk_id = ~0;
		rgt_stk_id = ~0;
		lft_stick = mc_null;
		rgt_stick = mc_null;
		lft_phi_id = ~0;
		rgt_phi_id = ~0;
		lft_philo = mc_null;
		rgt_philo = mc_null;

		last_sent = tok_invalid;
		last_sent_lft = tok_invalid;
		last_sent_rgt = tok_invalid;
		last_recv = tok_invalid;
		last_recv_lft = tok_invalid;
		last_recv_rgt = tok_invalid;
	}

	void handler(missive* msv);

	void send(cell* dst, philo_tok_t tok);

	void send_full();

	bool can_exit(){
		return ((left == mc_null) && (right == mc_null) && (num_bites == MAX_BITES) && rgt_ph_full && lft_ph_full);
	}

	void call_exit();

	virtual 
	grip&	get_available();
};

// For global data. DO NOT USE GLOBAL VARIABLES IF YOU WANT THE PTD (workerus as threads) TO WORK.
class philo_workeru {
public:
	MCK_DECLARE_MEM_METHODS(philo_workeru)

	philo_workeru(){		// NEED THIS SO THAT no memset func call
		init_philo_workeru();
	}		
	~philo_workeru(){}		// NEED THIS SO THAT no memset func call

	chopstick stick;
	philosopher philo;
	grip ava_chopstick;
	grip ava_philosopher;
	
	mc_size_t from_manageru_work_sz;
	mc_size_t to_manageru_work_sz;
	mc_size_t in_work_sz;
	mc_size_t local_work_sz;
	mc_size_t out_work_sz;
	mc_size_t sent_work_sz;
	mc_size_t cls_available_cell_sz;
	mc_size_t cls_available_missive_sz;
	mc_size_t cls_available_agent_ref_sz;
	mc_size_t cls_available_agent_grp_sz;

	grip* all_ava[idx_total];
	mc_alloc_kernel_func_t all_acq[idx_total];
	mc_alloc_kernel_func_t all_sep[idx_total];

	void init_philo_workeru(){
		from_manageru_work_sz = 0;
		to_manageru_work_sz = 0;
		in_work_sz = 0;
		local_work_sz = 0;
		out_work_sz = 0;
		sent_work_sz = 0;
		cls_available_cell_sz = 0;
		cls_available_missive_sz = 0;
		cls_available_agent_ref_sz = 0;
		cls_available_agent_grp_sz = 0;

		mc_init_arr_vals(idx_total, all_ava, mc_null);
		mc_init_arr_vals(idx_total, all_acq, mc_null);
		mc_init_arr_vals(idx_total, all_sep, mc_null);
		
		all_ava[idx_chopstick] = &ava_chopstick;
		all_ava[idx_philosopher] = &ava_philosopher;
		all_ava[idx_last_invalid] = mc_pt_invalid_available;
		
		all_acq[idx_chopstick] = (mc_alloc_kernel_func_t)chopstick::acquire_alloc;
		all_acq[idx_philosopher] = (mc_alloc_kernel_func_t)philosopher::acquire_alloc;
		all_acq[idx_last_invalid] = kernel::invalid_alloc_func;

		all_sep[idx_chopstick] = (mc_alloc_kernel_func_t)chopstick::separate;
		all_sep[idx_philosopher] = (mc_alloc_kernel_func_t)philosopher::separate;
		all_sep[idx_last_invalid] = kernel::invalid_alloc_func;
		
		kernel::set_tot_cell_subclasses(idx_total);
		kernel::set_cell_mem_funcs(all_ava, all_acq, all_sep);
	}
};

MCK_DEFINE_ACQUIRE_ALLOC(philo_workeru, 32);	// defines philo_workeru::acquire_alloc

#define glb_philo_workeru ((philo_workeru*)(mck_get_kernel()->user_data))

//! Returns the local \ref chopstick 
#define glb_stick (&(glb_philo_workeru->stick))

//! Returns the local \ref philosopher
#define glb_philo (&(glb_philo_workeru->philo))

#define glb_ava_sticks (glb_philo_workeru->ava_chopstick)
#define glb_ava_philos (glb_philo_workeru->ava_philosopher)

#define left_chp_nn(nn) (nn)
#define right_chp_nn(nn) ((nn == 15)?(0):(nn + 1))

#define left_phl_nn(nn) ((nn == 0)?(15):(nn - 1))
#define right_phl_nn(nn) ((nn == 15)?(0):(nn + 1))

//! Returns the \ref chopstick in workeru with 'id'
#define get_stick(id) ((chopstick*)mc_addr_set_id(id, glb_stick))

//! Returns the \ref philosopher in workeru with 'id'
#define get_philo(id) ((philosopher*)mc_addr_set_id(id, glb_philo))

MCK_DEFINE_MEM_METHODS(chopstick, 32, glb_ava_sticks, 0)
MCK_DEFINE_MEM_METHODS(philosopher, 32, glb_ava_philos, 0)

grip&
chopstick::get_available(){
	PTD_LOG("MEM___chopstick::get_available\n");
	return glb_ava_sticks;
}

grip&
philosopher::get_available(){
	PTD_LOG("MEM___philosopher::get_available\n");
	return glb_ava_philos;
}


#ifdef PHILO_WITH_DBG
	philo_workeru* 
	dbg_all_philo[16] mc_external_code_ram = {
		mc_null, mc_null, mc_null, mc_null, 
		mc_null, mc_null, mc_null, mc_null, 
		mc_null, mc_null, mc_null, mc_null, 
		mc_null, mc_null, mc_null, mc_null
	};
#endif // END_OF_PHILO_WITH_DBG

void 
philosopher_handler(missive* msv){
	MCK_CALL_HANDLER(philosopher, handler, msv);
}

void 
chopstick_handler(missive* msv){
	MCK_CALL_HANDLER(chopstick, handler, msv);
}

void philo_init_handlers(){
	missive_handler_t* hndlrs = philo_handlers;
	mc_init_arr_vals(idx_total, hndlrs, mc_null);
	hndlrs[idx_chopstick] = chopstick_handler;
	hndlrs[idx_philosopher] = philosopher_handler;
	hndlrs[idx_last_invalid] = kernel::invalid_handler_func;

	kernel::set_cell_handlers(hndlrs);
}

void
chopstick::handler(missive* msv){
	cell* msv_src = msv->src;
	philo_tok_t tok = (philo_tok_t)msv->tok;

	last_src = msv_src;
	last_recv = tok;

	mc_workeru_nn_t nn = mck_get_kernel()->get_workeru_nn();
	PH_DBG_COD(
		if(prt_recv_msgs){
			PH_DBG("CHOP %d RECV %s \n", nn, tok_to_str(tok));
		}
	)
	if((nn == 0) || (nn >= 14)){
		#ifdef PHILO_EPH_DBG
			mc_workeru_id_t src_id = mc_addr_get_id(msv_src);
			mck_slog2("CHOP_RECV____");
			mck_slog2(tok_to_str(tok));
			mck_slog2("___");
			mck_xlog((mc_addr_t)msv_src);
			mck_slog2("___");
			mck_ilog(mc_id_to_nn(src_id));
			mck_slog2("___\n");

			if(nn == 15){
				//kernel* ker = mck_get_kernel();
				bool ini0 = mck_is_id_inited(mc_nn_to_id(0));
				//long osz = ker->out_work.calc_size();
				//mck_ack_t loc_dst_ack_pt = (ker->pw0_routed_ack_arr)[0];
				mck_slog2("ADDR_INI____");
				mck_xlog((mc_addr_t)(&(MC_WORKERU_INFO->inited_workeru)));
				mck_slog2("___\n");

				mck_slog2("INI_0____");
				mck_ilog(ini0);
				mck_slog2("___\n");
			}
		#endif
	}

	switch(tok){
		case tok_take:
			PTD_CK(owner != msv_src);
			if(owner == mc_null){
				owner = msv_src;
				last_sent = tok_taken;
				respond(msv, tok_taken);
			} else {
				PTD_CK(owner != mc_null);
				last_sent = tok_not_taken;
				respond(msv, tok_not_taken);
			}
		break;
		case tok_drop:
			PTD_CK(owner == msv_src);
			owner = mc_null;
			last_sent = tok_droped;
			respond(msv, tok_droped);
		break;
		default:
			mck_abort(1, mc_cstr("BAD_STICK_TOK"));
		break;
	}
	
	//mck_get_kernel()->set_idle_exit();
}

void
philosopher::send(cell* dst, philo_tok_t tok){
	PH_DBG_COD(
		if(prt_send_msgs){
			PH_DBG("PHIL SEND %d \n", tok);
		}
	)

	last_sent = tok;
	if(dst == lft_stick){ last_sent_lft = tok; }
	if(dst == rgt_stick){ last_sent_rgt = tok; }

	missive* msv = mc_missive_acquire();
	
	msv->src = this;
	msv->dst = dst;
	msv->tok = tok;
	msv->send();
}

void
philosopher::handler(missive* msv){
	cell* msv_src = msv->src;
	philo_tok_t tok = (philo_tok_t)msv->tok;
	mc_workeru_nn_t nn = mck_get_kernel()->get_workeru_nn();
	MC_MARK_USED(nn);

	PH_DBG_COD(
		if(prt_recv_msgs){
			PH_DBG("PHIL %d RECV %s \n", nn, tok_to_str(tok));
		}
	)

	last_recv = tok;

	if(msv_src == lft_stick){ last_recv_lft = tok; }
	if(msv_src == rgt_stick){ last_recv_rgt = tok; }

	switch(tok){
		case tok_eat:
			PTD_CK(msv_src == this);
			PTD_CK(left == mc_null);
			PTD_CK(right == mc_null);
			PTD_CK(num_bites < MAX_BITES);
			send(lft_stick, tok_take);
		break;
		case tok_taken:
			PTD_CK((msv_src == lft_stick) || (msv_src == rgt_stick));
			if(msv_src == lft_stick){
				PTD_CK(left == mc_null);
				PTD_CK(right == mc_null);
				left = lft_stick;
				send(rgt_stick, tok_take);
	
				#ifdef PHILO_EPH_DBG
					if(nn == 15){
						mck_slog2("SENT_TAKE_RIGHT");
					}
				#endif
			}
			if(msv_src == rgt_stick){
				PTD_CK(left == lft_stick);
				PTD_CK(right == mc_null);
				right = rgt_stick;

				PTD_CK(left != mc_null);
				PTD_CK(right != mc_null);
				PTD_CK(num_bites < MAX_BITES);
				num_bites++;
				PH_DBG("#BITES %d \n", num_bites);
				PTD_LOG("#BITES %d \n", num_bites);

				#ifdef PHILO_EPH_DBG
					mck_slog2("#BITES____");
					mck_ilog(num_bites);
					mck_slog2("___\n");
				#endif

				send(lft_stick, tok_drop);
				send(rgt_stick, tok_drop);
			}
		break;
		case tok_not_taken:
			PTD_CK((msv_src == lft_stick) || (msv_src == rgt_stick));
			if(msv_src == lft_stick){
				PTD_CK(left == mc_null);
				PTD_CK(right == mc_null);
				send(this, tok_eat);
			}
			if(msv_src == rgt_stick){
				PTD_CK(left == lft_stick);
				PTD_CK(right == mc_null);
				send(lft_stick, tok_drop);
			}
		break;
		case tok_droped:
			PTD_CK((msv_src == lft_stick) || (msv_src == rgt_stick));
			if(msv_src == lft_stick){
				PTD_CK(left == lft_stick);
				left = mc_null;
			}
			if(msv_src == rgt_stick){
				PTD_CK(right == rgt_stick);
				right = mc_null;
			}
			if((left == mc_null) && (right == mc_null)){
				if(num_bites == MAX_BITES){
					PH_DBG("I AM FULL \n");
					PTD_LOG("I AM FULL \n");
					mck_sprt2("I AM FULL____");
					mck_iprt(mck_get_kernel()->get_workeru_nn());
					mck_sprt2("___\n");

					PH_DBG_COD(mc_loop_set_var(dbg_all_full[nn], true);)

					send(lft_philo, tok_yes_full);
					send(rgt_philo, tok_yes_full);

					if(can_exit()){
						call_exit();
					}
				} else {
					send(this, tok_eat);
				}
			}
		break;
		case tok_yes_full:
			PTD_CK((msv_src == lft_philo) || (msv_src == rgt_philo));
			if(msv_src == lft_philo){ 
				PTD_CK(! lft_ph_full);
				lft_ph_full = true; 
			}
			if(msv_src == rgt_philo){ 
				PTD_CK(! rgt_ph_full);
				rgt_ph_full = true; 
			}
			if(can_exit()){
				call_exit();
			}
		break;
		default:
			mck_abort(1, mc_cstr("BAD_PHILO_TOK"));
		break;
	} 
	
}

void
philosopher::call_exit(){
	PH_DBG_COD(
		prt_idle();
		prt_full();
		prt_all_philo();
	)
	PTD_LOG("FINISHING \n");

	//mck_sprt2("CALLING_EXIT____");
	//mck_iprt(mck_get_kernel()->get_workeru_nn());
	//mck_sprt2("___\n");

	mck_get_kernel()->set_idle_exit();
}

#ifdef PHILO_WITH_DBG
	void prt_idle(){
		char full_str[500];
		char* pt = full_str;
		pt += sprintf(pt, "ALL_IDLE=[");
		for(int aa = 0; aa < 16; aa++){
			bool idl = dbg_all_idle_prt[aa];
			if(idl){
				pt += sprintf(pt, "%d,", aa);
			}
		}
		pt += sprintf(pt, "]\n");
		PH_DBG("%s", full_str);
	}

	void prt_full(){
		char full_str[500];
		char* pt = full_str;
		pt += sprintf(pt, "ALL_FULL=[");
		for(int aa = 0; aa < 16; aa++){
			bool fll = dbg_all_full[aa];
			if(fll){
				pt += sprintf(pt, "%d,", aa);
			}
		}
		pt += sprintf(pt, "]\n");
		PH_DBG("%s", full_str);
	}

	void prt_ph(int aa, philosopher* ph){
		char full_str[500];
		char* pt = full_str;
		bool is_fll = (ph->num_bites == MAX_BITES);
		bool to_exit = ph->can_exit();
		pt += sprintf(pt, "PHILO (%p) %d=[", ph, aa);
		pt += sprintf(pt, "F%d ", is_fll);
		pt += sprintf(pt, "X%d ", to_exit);
		pt += sprintf(pt, "L%d ", (ph->left != mc_null)?(1):(0));
		pt += sprintf(pt, "R%d ", (ph->right != mc_null)?(1):(0));
		pt += sprintf(pt, "N%d ", ph->num_bites);
		pt += sprintf(pt, "LF%d ", ph->lft_ph_full);
		pt += sprintf(pt, "RF%d ", ph->rgt_ph_full);
		pt += sprintf(pt, "Lsnt=%s ", tok_to_str(ph->last_sent_lft));
		pt += sprintf(pt, "Lrcv=%s ", tok_to_str(ph->last_recv_lft));
		pt += sprintf(pt, "Rsnt=%s ", tok_to_str(ph->last_sent_rgt));
		pt += sprintf(pt, "Rrcv=%s ", tok_to_str(ph->last_recv_rgt));
		pt += sprintf(pt, "]\n");
		PH_DBG("%s", full_str);
	}

	void prt_ch(int aa, chopstick* ch){
		char full_str[500];
		char* pt = full_str;
		pt += sprintf(pt, "STICK (%p) %d=[", ch, aa);
		pt += sprintf(pt, "lst_sent=%s ", tok_to_str(ch->last_sent));
		pt += sprintf(pt, "lst_recv=%s ", tok_to_str(ch->last_recv));
		pt += sprintf(pt, "lst_src=%p ", ch->last_src);
		pt += sprintf(pt, "]\n");
		PH_DBG(full_str);
	}

	void prt_pc(int aa, philo_workeru* pc){
		char full_str[500];
		char* pt = full_str;
		pt += sprintf(pt, "WORKERU %d=[", aa);
		PTD_64_CODE(
			pt += sprintf(pt, "out_work_sz=%ld ", pc->out_work_sz);
			pt += sprintf(pt, "sent_work_sz=%ld ", pc->sent_work_sz);
		)
		PTD_32_CODE(
			pt += sprintf(pt, "out_work_sz=%d ", pc->out_work_sz);
			pt += sprintf(pt, "sent_work_sz=%d ", pc->sent_work_sz);
		)
		EPH_CODE(
			pt += sprintf(pt, "out_work_sz=%d ", pc->out_work_sz);
			pt += sprintf(pt, "sent_work_sz=%d ", pc->sent_work_sz);
		)
		pt += sprintf(pt, "]\n");
		PH_DBG(full_str);
	}

	void prt_all_philo(){
		for(int aa = 0; aa < 16; aa++){
			philo_workeru* phl = dbg_all_philo[aa];
			if(phl != mc_null){
				prt_ch(aa, &(phl->stick));
				prt_ph(aa, &(phl->philo));
				prt_pc(aa, phl);
			}
		}
	}

	void ker_func(){
		kernel* ker = mck_get_kernel();
		mc_workeru_nn_t nn = ker->get_workeru_nn();
		if(! ker->did_work && ! dbg_all_idle_prt[nn]){
			mc_loop_set_var(dbg_all_idle_prt[nn], true);
		}
		if(ker->did_work && dbg_all_idle_prt[nn]){
			mc_loop_set_var(dbg_all_idle_prt[nn], false);
		}
		philo_workeru* phl = dbg_all_philo[nn];

		phl->to_manageru_work_sz = ker->to_manageru_work.calc_size();
		phl->in_work_sz = ker->in_work.calc_size();
		phl->local_work_sz = ker->local_work.calc_size();
		phl->out_work_sz = ker->out_work.calc_size();
		phl->sent_work_sz = ker->sent_work.calc_size();
		phl->cls_available_cell_sz = ker->cls_available_cell.calc_size();
		phl->cls_available_missive_sz = ker->cls_available_missive.calc_size();
		phl->cls_available_agent_ref_sz = ker->cls_available_agent_ref.calc_size();
		phl->cls_available_agent_grp_sz = ker->cls_available_agent_grp.calc_size();
	}
#endif // END_OF_PHILO_WITH_DBG

void mc_workerus_main() {
	kernel::init_sys();
	kernel* ker = mck_get_kernel();
	MC_MARK_USED(ker);

	mc_workeru_nn_t nn = ker->get_workeru_nn();

	PH_DBG_COD(
		mc_loop_set_var(dbg_all_idle_prt[nn], false);
		mc_loop_set_var(dbg_all_full[nn], false);
	)

	philo_workeru* workeru_dat = philo_workeru::acquire_alloc();
	if(workeru_dat == mc_null){
		mck_abort(1, mc_cstr("CAN NOT INIT GLB WORKERU DATA"));
	}

	ker->user_data = workeru_dat;

	PH_DBG_COD(
		ker->user_func = ker_func;
		dbg_all_philo[nn] = workeru_dat;
	);

	glb_philo->lft_stk_id = mc_nn_to_id(left_chp_nn(nn));
	glb_philo->rgt_stk_id = mc_nn_to_id(right_chp_nn(nn));
	glb_philo->lft_stick = get_stick(glb_philo->lft_stk_id);
	glb_philo->rgt_stick = get_stick(glb_philo->rgt_stk_id);
	glb_philo->lft_phi_id = mc_nn_to_id(left_phl_nn(nn));
	glb_philo->rgt_phi_id = mc_nn_to_id(right_phl_nn(nn));
	glb_philo->lft_philo = get_philo(glb_philo->lft_phi_id);
	glb_philo->rgt_philo = get_philo(glb_philo->rgt_phi_id);

	philo_init_handlers();

	missive::separate(mc_out_num_workerus);
	agent_ref::separate(mc_out_num_workerus);
	agent_grp::separate(mc_out_num_workerus);

	//chopstick::separate(1);
	//philosopher::separate(1);

	PH_DBG("started\n");

	glb_philo->send(glb_philo, tok_eat);
	kernel::run_sys();

	PH_DBG("finished\n");
	PTD_LOG("PHILOSOPHER %d FINISHED !!\n", nn);
	mck_slog2("PHILOSOPHERS FINISHED !!\n");	

	kernel::finish_sys();
}

