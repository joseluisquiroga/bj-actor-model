
#SRC_IN_SECTIONS := -fdata-sections -ffunction-sections
SRC_IN_SECTIONS := 

#LD_IN_SECTIONS := --gc-sections
LD_IN_SECTIONS := 

# =======================================

TARGET := philo_emu.elf

TGT_LDFLAGS := ${MC_STD_EMU_LDFLAGS} ${LD_IN_SECTIONS} -rdynamic
TGT_LDLIBS  := ${MC_STD_EMU_LDLIBS}
TGT_PREREQS := ${MC_EMU_LIBS}

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := -DMC_IS_EMU_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS} -g3 -pthread
SRC_CXXFLAGS := -DMC_IS_EMU_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS} -g3 -pthread

SRC_INCDIRS := ${MC_STD_INCDIRS}

SOURCES := philo.cpp ../nil_host/nil_host.cpp


