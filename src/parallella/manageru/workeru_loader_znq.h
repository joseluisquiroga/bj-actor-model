

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

------------------------------------------------------------

File: workeru_loader_znq.h
Base on: e-loader.h

This file is part of the Epiphany Software Development Kit.

Copyright (C) 2013 Adapteva, Inc.
See AUTHORS for list of contributors.
Support e-mail: <support@adapteva.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL)
as published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
and the GNU Lesser General Public License along with this program,
see the files COPYING and COPYING.LESSER.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------*/

#ifndef MC_CODE_LOADER_H
#define MC_CODE_LOADER_H

#include <elf.h>
#include "e-hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "shared.h"

#ifndef MCL_MARK_USED
#define MCL_MARK_USED(X)  ((void)(&(X)))
#endif

extern uint8_t* MCH_EXTERNAL_RAM_BASE_PT;

extern e_mem_t mch_glb_emem;
extern e_epiphany_t mch_glb_dev;

#define mch_min_shd_eph_addr ((mc_addr_t) mch_glb_emem.ephy_base)
#define mch_max_shd_eph_addr ((mc_addr_t) (mch_glb_emem.ephy_base + MC_EXTERNAL_RAM_LOAD_DATA.extnl_ram_size))
#define mch_min_shd_znq_addr ((mc_addr_t) mch_glb_emem.base)
#define mch_max_shd_znq_addr ((mc_addr_t) (mch_glb_emem.base + MC_EXTERNAL_RAM_LOAD_DATA.extnl_ram_size))
#define mch_znq_addr_in_shd_ram(znq_addr) ((mch_min_shd_znq_addr <= znq_addr) && (znq_addr < mch_max_shd_znq_addr))

#define mch_disp_to_pt(disp) (MCH_EXTERNAL_RAM_BASE_PT + disp)

typedef enum {
	L_D0 = 0,
	L_D1 = 1,
	L_D2 = 2,
	L_D3 = 3,
	L_D4 = 40,
} mcl_loader_diag_t;

extern mcl_loader_diag_t mcl_load_verbose;

#define mcl_diag(vN)   if (mcl_load_verbose >= vN)

struct load_info_st {    
	char *executable;
	e_epiphany_t *dev;

	mc_workeru_nn_t root_nn;

	mc_workeru_co_t row;
	mc_workeru_co_t col;
	mc_workeru_co_t rows;
	mc_workeru_co_t cols;

	char**	all_module_names;
	void**	all_module_addrs;	// as seen from ephiphany side

	void *file;
	int   fd;
	size_t f_sz;
	e_mem_t *emem;
};
typedef struct load_info_st load_info_t;


int mc_load_group(load_info_t *ld_dat);

int mcl_load_root(load_info_t *ld_dat);

#define DBGH_CODE_SHD_SZ 20

extern uint16_t DBGH_CODE_SHD_1[DBGH_CODE_SHD_SZ];
extern mc_addr_t DBGH_FUNC_ADDR;

#define MCL_MAX_TOT_MODULES 1000

extern char* mcl_module_names[MCL_MAX_TOT_MODULES];
extern int mcl_module_names_sz;

bool 
mcl_is_epiphany_exec_elf(Elf32_Ehdr *ehdr);

void
ck_dbg_shd_code(char* msg, uint16_t* base);

mc_addr_t
mc_manageru_eph_loc_addr_to_znq_addr(int row, int col, mc_addr_t ld_addr);

void
mc_start_all_workerus();

mc_addr_t
mc_znq_addr_to_eph_addr(mc_addr_t znq_addr);

mc_addr_t
mc_eph_addr_to_znq_addr(mc_addr_t eph_addr);


#ifdef __cplusplus
}
#endif

#endif // MC_CODE_LOADER_H
