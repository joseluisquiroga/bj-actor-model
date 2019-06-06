

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


SRC_TEST_DIR := ./
SRC_MANAGERU_DIR := ../../host
SRC_CELLS_DIR := ../../cells

DBG_FLAG := -DFULL_DEBUG

# =======================================

TARGET := znq_dlmalloc_test.elf

TGT_LDFLAGS := -L${TARGET_DIR} 
TGT_LDLIBS  := -lmc-znq-tak-mak -lmc-znq-dlmalloc
TGT_PREREQS := libmc-znq-tak-mak.a libmc-znq-dlmalloc.a

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

TGT_CC := $(CROSS_COMPILE)gcc
TGT_CXX := $(CROSS_COMPILE)g++
TGT_LINKER := $(CROSS_COMPILE)gcc

C_FLAGS_1 := -Wall -std=gnu11 -pthread
SRC_CFLAGS := -DMC_IS_ZNQ_CODE ${DBG_FLAG} ${C_FLAGS_1}

CXX_FLAGS_1 := -Wall -std=c++17 -pthread
SRC_CXXFLAGS := -DMC_IS_ZNQ_CODE ${DBG_FLAG} ${CXX_FLAGS_1}

SRC_INCDIRS := $(SRC_MANAGERU_DIR) ${SRC_CELLS_DIR}

SOURCES := $(SRC_TEST_DIR)/dbg_dlmalloc.cpp



