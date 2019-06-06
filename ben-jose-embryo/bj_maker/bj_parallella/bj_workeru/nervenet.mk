
TARGET := ${BJ_PLLA_LIB_DIR}/libnervenet.a

TGT_PREREQS := ${MC_EPH_LIBS}

define POST_OPERS
	bj_parallella/mod_get_common_syms.sh $(TARGET_DIR)/$(TARGET)
	printf "====================================\nFinished building "$(TARGET)"\n\n\n" 
endef

TGT_POSTMAKE := ${POST_OPERS}

TGT_CC := e-gcc
TGT_CXX := e-g++

SRC_CFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS}
SRC_CXXFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS}

SRC_INCDIRS := ${BJ_WORKERUS_INCLUDES} 

SOURCES := ${SRC_BJ_WORKERU_DIR}/nervenet.cpp 

