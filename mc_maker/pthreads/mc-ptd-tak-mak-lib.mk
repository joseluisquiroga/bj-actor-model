
SRC_CELLS_DIR := $(SRC_DIR)/cells

# =======================================

DEF_FLAGS_1 := -DUSE_TSD_DATA_HACK -D_REENTRANT -DTHREAD_STATS=1 
DEF_FLAGS_2 := -DONLY_MSPACES -DUSE_LOCKS=0 -DHAVE_MMAP=0 -DHAVE_MREMAP=0 

TARGET := ${MC_PTD_LIB_DIR}/libmcm-tak-mak.a

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := -Wall -std=gnu11 -O2 -Wstrict-prototypes $(DEF_FLAGS_1) $(DEF_FLAGS_2)

SOURCES := $(SRC_CELLS_DIR)/tak_mak.cpp 


