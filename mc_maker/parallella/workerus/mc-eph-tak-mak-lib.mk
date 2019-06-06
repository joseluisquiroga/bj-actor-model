
SRC_CELLS_DIR := $(SRC_DIR)/cells

# =======================================

TARGET := ${MC_PLLA_LIB_DIR}/libmc-eph-tak-mak.a

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

TGT_CC := e-gcc
TGT_CXX := e-g++

SRC_CFLAGS := -DMC_IS_EPH_CODE ${STD_EPH_CFLAGS} ${DBG_FLAG} ${SRC_IN_SECTIONS}
SRC_CXXFLAGS := -DMC_IS_EPH_CODE ${STD_EPH_CXXFLAGS} ${DBG_FLAG} ${SRC_IN_SECTIONS}

SRC_INCDIRS := $(SRC_CELLS_DIR) 

SOURCES := $(SRC_CELLS_DIR)/tak_mak.cpp 

