

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


// all_regs.h

#ifndef MC_ALL_REGS_H
#define MC_ALL_REGS_H

#include "attribute.h"

#ifdef __cplusplus
mc_c_decl {
#endif

// eCore General Purpose Registers
typedef enum {
	MC_CORE_GP_REG_BASE     = 0xf0000,
	MC_REG_R0               = MC_CORE_GP_REG_BASE + 0x0000,
	MC_REG_R1               = MC_CORE_GP_REG_BASE + 0x0004,
	MC_REG_R2               = MC_CORE_GP_REG_BASE + 0x0008,
	MC_REG_R3               = MC_CORE_GP_REG_BASE + 0x000c,
	MC_REG_R4               = MC_CORE_GP_REG_BASE + 0x0010,
	MC_REG_R5               = MC_CORE_GP_REG_BASE + 0x0014,
	MC_REG_R6               = MC_CORE_GP_REG_BASE + 0x0018,
	MC_REG_R7               = MC_CORE_GP_REG_BASE + 0x001c,
	MC_REG_R8               = MC_CORE_GP_REG_BASE + 0x0020,
	MC_REG_R9               = MC_CORE_GP_REG_BASE + 0x0024,
	MC_REG_R10              = MC_CORE_GP_REG_BASE + 0x0028,
	MC_REG_R11              = MC_CORE_GP_REG_BASE + 0x002c,
	MC_REG_R12              = MC_CORE_GP_REG_BASE + 0x0030,
	MC_REG_R13              = MC_CORE_GP_REG_BASE + 0x0034,
	MC_REG_R14              = MC_CORE_GP_REG_BASE + 0x0038,
	MC_REG_R15              = MC_CORE_GP_REG_BASE + 0x003c,
	MC_REG_R16              = MC_CORE_GP_REG_BASE + 0x0040,
	MC_REG_R17              = MC_CORE_GP_REG_BASE + 0x0044,
	MC_REG_R18              = MC_CORE_GP_REG_BASE + 0x0048,
	MC_REG_R19              = MC_CORE_GP_REG_BASE + 0x004c,
	MC_REG_R20              = MC_CORE_GP_REG_BASE + 0x0050,
	MC_REG_R21              = MC_CORE_GP_REG_BASE + 0x0054,
	MC_REG_R22              = MC_CORE_GP_REG_BASE + 0x0058,
	MC_REG_R23              = MC_CORE_GP_REG_BASE + 0x005c,
	MC_REG_R24              = MC_CORE_GP_REG_BASE + 0x0060,
	MC_REG_R25              = MC_CORE_GP_REG_BASE + 0x0064,
	MC_REG_R26              = MC_CORE_GP_REG_BASE + 0x0068,
	MC_REG_R27              = MC_CORE_GP_REG_BASE + 0x006c,
	MC_REG_R28              = MC_CORE_GP_REG_BASE + 0x0070,
	MC_REG_R29              = MC_CORE_GP_REG_BASE + 0x0074,
	MC_REG_R30              = MC_CORE_GP_REG_BASE + 0x0078,
	MC_REG_R31              = MC_CORE_GP_REG_BASE + 0x007c,
	MC_REG_R32              = MC_CORE_GP_REG_BASE + 0x0080,
	MC_REG_R33              = MC_CORE_GP_REG_BASE + 0x0084,
	MC_REG_R34              = MC_CORE_GP_REG_BASE + 0x0088,
	MC_REG_R35              = MC_CORE_GP_REG_BASE + 0x008c,
	MC_REG_R36              = MC_CORE_GP_REG_BASE + 0x0090,
	MC_REG_R37              = MC_CORE_GP_REG_BASE + 0x0094,
	MC_REG_R38              = MC_CORE_GP_REG_BASE + 0x0098,
	MC_REG_R39              = MC_CORE_GP_REG_BASE + 0x009c,
	MC_REG_R40              = MC_CORE_GP_REG_BASE + 0x00a0,
	MC_REG_R41              = MC_CORE_GP_REG_BASE + 0x00a4,
	MC_REG_R42              = MC_CORE_GP_REG_BASE + 0x00a8,
	MC_REG_R43              = MC_CORE_GP_REG_BASE + 0x00ac,
	MC_REG_R44              = MC_CORE_GP_REG_BASE + 0x00b0,
	MC_REG_R45              = MC_CORE_GP_REG_BASE + 0x00b4,
	MC_REG_R46              = MC_CORE_GP_REG_BASE + 0x00b8,
	MC_REG_R47              = MC_CORE_GP_REG_BASE + 0x00bc,
	MC_REG_R48              = MC_CORE_GP_REG_BASE + 0x00c0,
	MC_REG_R49              = MC_CORE_GP_REG_BASE + 0x00c4,
	MC_REG_R50              = MC_CORE_GP_REG_BASE + 0x00c8,
	MC_REG_R51              = MC_CORE_GP_REG_BASE + 0x00cc,
	MC_REG_R52              = MC_CORE_GP_REG_BASE + 0x00d0,
	MC_REG_R53              = MC_CORE_GP_REG_BASE + 0x00d4,
	MC_REG_R54              = MC_CORE_GP_REG_BASE + 0x00d8,
	MC_REG_R55              = MC_CORE_GP_REG_BASE + 0x00dc,
	MC_REG_R56              = MC_CORE_GP_REG_BASE + 0x00e0,
	MC_REG_R57              = MC_CORE_GP_REG_BASE + 0x00e4,
	MC_REG_R58              = MC_CORE_GP_REG_BASE + 0x00e8,
	MC_REG_R59              = MC_CORE_GP_REG_BASE + 0x00ec,
	MC_REG_R60              = MC_CORE_GP_REG_BASE + 0x00f0,
	MC_REG_R61              = MC_CORE_GP_REG_BASE + 0x00f4,
	MC_REG_R62              = MC_CORE_GP_REG_BASE + 0x00f8,
	MC_REG_R63              = MC_CORE_GP_REG_BASE + 0x00fc,
} mc_gp_reg_id_t;

// eCore Special Registers
typedef enum {
	MC_CORE_SP_REG_BASE     = 0xf0000,
	//MC_CORE_SP_REG_BASE     = 0x0,
	// Control registers
	MC_REG_CONFIG           = MC_CORE_SP_REG_BASE + 0x0400,
	MC_REG_STATUS           = MC_CORE_SP_REG_BASE + 0x0404,
	MC_REG_PC               = MC_CORE_SP_REG_BASE + 0x0408,
	MC_REG_DEBUGSTATUS      = MC_CORE_SP_REG_BASE + 0x040c,
	MC_REG_LC               = MC_CORE_SP_REG_BASE + 0x0414,
	MC_REG_LS               = MC_CORE_SP_REG_BASE + 0x0418,
	MC_REG_LE               = MC_CORE_SP_REG_BASE + 0x041c,
	MC_REG_IRET             = MC_CORE_SP_REG_BASE + 0x0420,
	MC_REG_IMASK            = MC_CORE_SP_REG_BASE + 0x0424,
	MC_REG_ILAT             = MC_CORE_SP_REG_BASE + 0x0428,
	MC_REG_ILATST           = MC_CORE_SP_REG_BASE + 0x042C,
	MC_REG_ILATCL           = MC_CORE_SP_REG_BASE + 0x0430,
	MC_REG_IPEND            = MC_CORE_SP_REG_BASE + 0x0434,
	MC_REG_CTIMER0          = MC_CORE_SP_REG_BASE + 0x0438,
	MC_REG_CTIMER1          = MC_CORE_SP_REG_BASE + 0x043C,
	MC_REG_FSTATUS          = MC_CORE_SP_REG_BASE + 0x0440,
	MC_REG_DEBUGCMD         = MC_CORE_SP_REG_BASE + 0x0448,

	// DMA Registers
	MC_REG_DMA0CONFIG       = MC_CORE_SP_REG_BASE + 0x0500,
	MC_REG_DMA0STRIDE       = MC_CORE_SP_REG_BASE + 0x0504,
	MC_REG_DMA0COUNT        = MC_CORE_SP_REG_BASE + 0x0508,
	MC_REG_DMA0SRCADDR      = MC_CORE_SP_REG_BASE + 0x050C,
	MC_REG_DMA0DSTADDR      = MC_CORE_SP_REG_BASE + 0x0510,
	MC_REG_DMA0AUTODMA0     = MC_CORE_SP_REG_BASE + 0x0514,
	MC_REG_DMA0AUTODMA1     = MC_CORE_SP_REG_BASE + 0x0518,
	MC_REG_DMA0STATUS       = MC_CORE_SP_REG_BASE + 0x051C,
	MC_REG_DMA1CONFIG       = MC_CORE_SP_REG_BASE + 0x0520,
	MC_REG_DMA1STRIDE       = MC_CORE_SP_REG_BASE + 0x0524,
	MC_REG_DMA1COUNT        = MC_CORE_SP_REG_BASE + 0x0528,
	MC_REG_DMA1SRCADDR      = MC_CORE_SP_REG_BASE + 0x052C,
	MC_REG_DMA1DSTADDR      = MC_CORE_SP_REG_BASE + 0x0530,
	MC_REG_DMA1AUTODMA0     = MC_CORE_SP_REG_BASE + 0x0534,
	MC_REG_DMA1AUTODMA1     = MC_CORE_SP_REG_BASE + 0x0538,
	MC_REG_DMA1STATUS       = MC_CORE_SP_REG_BASE + 0x053C,

	// Memory Protection Registers
	MC_REG_MEMSTATUS        = MC_CORE_SP_REG_BASE + 0x0604,
	MC_REG_MEMPROTECT       = MC_CORE_SP_REG_BASE + 0x0608,

	// Node Registers
	MC_REG_MESHCFG          = MC_CORE_SP_REG_BASE + 0x0700,
	MC_REG_COREID           = MC_CORE_SP_REG_BASE + 0x0704,
	MC_REG_CORE_RESET       = MC_CORE_SP_REG_BASE + 0x070c,
} mc_workeru_reg_id_t;

// Chip registers
typedef enum {
	MC_CHIP_REG_BASE        = 0xf0000,
	MC_REG_IO_LINK_MODE_CFG = MC_CHIP_REG_BASE + 0x0300,
	MC_REG_IO_LINK_TX_CFG   = MC_CHIP_REG_BASE + 0x0304,
	MC_REG_IO_LINK_RX_CFG   = MC_CHIP_REG_BASE + 0x0308,
	MC_REG_IO_GPIO_CFG      = MC_CHIP_REG_BASE + 0x030c,
	MC_REG_IO_FLAG_CFG      = MC_CHIP_REG_BASE + 0x0318,
	MC_REG_IO_SYNC_CFG      = MC_CHIP_REG_BASE + 0x031c,
	MC_REG_IO_HALT_CFG      = MC_CHIP_REG_BASE + 0x0320,
	MC_REG_IO_RESET         = MC_CHIP_REG_BASE + 0x0324,
	MC_REG_IO_LINK_DEBUG    = MC_CHIP_REG_BASE + 0x0328,
} mc_chip_reg_id_t;


#ifdef __cplusplus
}
#endif

#endif // MC_ALL_REGS_H


