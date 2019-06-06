

#ifndef HL_PHILO_H
#define HL_PHILO_H

#include "hlang.hh"

class chopstick;
class philosopher;

hdeclare_const(MAX_BITES);
hdeclare_const(TOT_WAIT_FULL);

hdeclare_token(pr_tok_invalid);
hdeclare_token(pr_tok_eat);
hdeclare_token(pr_tok_take);
hdeclare_token(pr_tok_taken);
hdeclare_token(pr_tok_not_taken);
hdeclare_token(pr_tok_drop);
hdeclare_token(pr_tok_droped);
hdeclare_token(pr_tok_not_droped);
hdeclare_token(pr_tok_yes_full);

#define hl_philo_tok(nam) hlong(nam)

class chopstick : public hcell {
public:
	hcell_class(chopstick);
	
	hreference(philosopher, owner);
	hreference(philosopher, last_src);

	hl_philo_tok(last_sent);
	hl_philo_tok(last_recv);

	//chopstick(){}
	//~chopstick(){}

	hnucleus(chopstick_nucl);

	hmethod(init_chopstick);
	
};

class philosopher : public hcell {
public:
	hcell_class(philosopher);

	hint(tot_philo);
	hint(num_full);
	hreference(philosopher, manager);
	
	hreference(chopstick, left);
	hreference(chopstick, right);

	hint(num_bites);

	hreference(chopstick, lft_stick);
	hreference(chopstick, rgt_stick);

	hreference(philosopher, lft_philo);
	hreference(philosopher, rgt_philo);

	hl_philo_tok(last_sent);
	hl_philo_tok(last_sent_lft);
	hl_philo_tok(last_sent_rgt);
	hl_philo_tok(last_recv);
	hl_philo_tok(last_recv_lft);
	hl_philo_tok(last_recv_rgt);
	
	//philosopher(){}
	//~philosopher(){}

	hnucleus(philosopher_nucl);

	hmethod(init_philosopher);
	
};


void hl_philo_main(int argc, char *argv[]);

#endif		// HL_PHILO_H

