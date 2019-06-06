

#*************************************************************
#
#This file is part of messaging-cells.
#
#messaging-cells is free software: you can redistribute it and/or modify
#it under the terms of the version 3 of the GNU General Public 
#License as published by the Free Software Foundation.
#
#messaging-cells is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with messaging-cells.  If not, see <http://www.gnu.org/licenses/>.
#
#------------------------------------------------------------
#
#Copyright (C) 2017-2018. QUIROGA BELTRAN, Jose Luis.
#Id (cedula): 79523732 de Bogota - Colombia.
#See https://messaging-cells.github.io/
#
#messaging-cells is free software thanks to The Glory of Our Lord 
#	Yashua Melej Hamashiaj.
#Our Resurrected and Living, both in Body and Spirit, 
#	Prince of Peace.
#
#------------------------------------------------------------


#SRC_IN_SECTIONS := -fdata-sections -ffunction-sections
SRC_IN_SECTIONS := 

#LD_IN_SECTIONS := --gc-sections
LD_IN_SECTIONS := 

# =======================================

TARGET := test_dlmalloc_ptd.elf

TGT_LDFLAGS := ${MC_STD_PTD_LDFLAGS} ${LD_IN_SECTIONS} 
TGT_LDLIBS  := ${MC_STD_PTD_LDLIBS}

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := -DMC_IS_PTD_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS} -pthread
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS} -pthread

SRC_INCDIRS := ${MC_STD_INCDIRS}

SOURCES := test_dlmalloc.cpp 


