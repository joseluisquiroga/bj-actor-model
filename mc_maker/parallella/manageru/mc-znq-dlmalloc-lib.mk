
SRC_DLMALLOC_DIR := $(SRC_DIR)/cells/manageru/dlmalloc

# =======================================

DEF_FLAGS_1 := -DUSE_TSD_DATA_HACK -D_REENTRANT -DTHREAD_STATS=1 
DEF_FLAGS_2 := -DONLY_MSPACES -DUSE_LOCKS=0 -DHAVE_MMAP=0 -DHAVE_MREMAP=0 

TARGET := ${MC_PLLA_LIB_DIR}/libmc-znq-dlmalloc.a

TGT_CC := $(CROSS_COMPILE)gcc
TGT_CXX := $(CROSS_COMPILE)g++
TGT_LINKER := $(CROSS_COMPILE)gcc

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := -Wall -std=gnu11 -O2 -Wstrict-prototypes $(DEF_FLAGS_1) $(DEF_FLAGS_2)

SOURCES := $(SRC_DLMALLOC_DIR)/dlmalloc.c 


