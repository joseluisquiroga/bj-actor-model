

TARGET := ${BJ_PLLA_BIN_DIR}/bj_manageru_znq.elf

TGT_LDFLAGS := ${MC_STD_ZNQ_LDFLAGS}
TGT_LDLIBS  := ${MC_STD_ZNQ_LDLIBS} -static-libstdc++ -static-libgcc -ldimacs 
TGT_PREREQS := ${MC_ZNQ_LIBS} ${BJ_PLLA_LIB_DIR}/libdimacs.a 

TGT_CC := $(CROSS_COMPILE)gcc
TGT_CXX := $(CROSS_COMPILE)g++
TGT_LINKER := $(CROSS_COMPILE)g++

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := -DMC_IS_ZNQ_CODE -Wall -std=gnu11 ${MC_DBG_FLAG}
SRC_CXXFLAGS := -DMC_IS_ZNQ_CODE -Wall -std=c++17 ${MC_DBG_FLAG}

SRC_INCDIRS := ${BJ_MANAGERU_INCLUDES}

SUBMAKEFILES := ./lib-dimacs-znq.mk ./preload_znq.mk 

SOURCES := \
	${SRC_BJ_MANAGERU_DIR}/bj_manageru_code.cpp \



