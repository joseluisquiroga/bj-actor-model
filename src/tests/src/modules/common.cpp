

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
/*! \file common.cpp
\ingroup docgrp_modules

\brief Common file for all modules of the \ref docgrp_modules example.

\details 

\include common.cpp

------------------------------------------------------------*/

#include "cell.hh"
#include "common.h"

long some_glb_val mc_comm_dat = 6;


// THE FOLLOWING COMMENTED LINE generates code of all 
// 		*static_initialization_and_destruction* functions
// 		*_GLOBAL__sub_I_main* functions (wrappers to the first)
//
// NEVER USE GLOBAL VARIABLES (PTD WILL NOT WORK)

//comm_cla some_comm_obj mc_comm_dat;  

// INSTED USE GLOBAL POINTER AND EXPLICITLY INITIALIZE A START UP TIME

comm_cla* some_comm_obj mc_comm_dat = mc_null;

void comm_f1_workeru(){
	//comm_cla oo;
	prts("comm_f1_workeru \n");
	comm_f2_workeru();
}

void comm_f2_workeru(){
	//cell a;
	prts("comm_f2_workeru \n");
}

void comm_prt(char* val){
}

void
comm_cla::method1(){
}

comm_cla::comm_cla(){
	init_comm_cla();
}

comm_cla::~comm_cla(){
}

void
comm_cla::init_comm_cla(){
	aa = 7;
	bb = 33;
}

