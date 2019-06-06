

TARGET := sort_net_ptd.elf

TGT_LDFLAGS := ${MC_STD_PTD_LDFLAGS} ${LD_IN_SECTIONS} -rdynamic
TGT_LDLIBS  := ${MC_STD_PTD_LDLIBS}

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := -DMC_IS_PTD_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS} -g3 -pthread
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS} -g3 -pthread

SRC_INCDIRS := ${MC_STD_INCDIRS}

SUBMAKEFILES := ./workeru_sort_net_ptd.mk 

SOURCES := manageru_sort_net.cpp







