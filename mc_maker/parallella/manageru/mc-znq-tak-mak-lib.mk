
SRC_CELLS_DIR := $(SRC_DIR)/cells

# =======================================

TARGET := ${MC_PLLA_LIB_DIR}/libmc-znq-tak-mak.a

TGT_CC := $(CROSS_COMPILE)gcc
TGT_CXX := $(CROSS_COMPILE)g++
TGT_LINKER := $(CROSS_COMPILE)gcc

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := -Wall -std=gnu11 -O2 -Wstrict-prototypes 

SOURCES := $(SRC_CELLS_DIR)/tak_mak.cpp 


