
# =======================================

TARGET := libsornet.a

TGT_PREREQS := ${MC_EPH_LIBS} \
	libnervenet.a

TGT_CC := e-gcc
TGT_CXX := e-g++

define POST_OPERS
	e-objdump -D $(TARGET_DIR)/$(TARGET) > $(TARGET_DIR)/$(TARGET).s
	printf "====================================\nFinished building "$(TARGET)"\n\n\n" 
endef

TGT_POSTMAKE := ${POST_OPERS}

SRC_CFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} 

SRC_INCDIRS := ${BJ_WORKERUS_INCLUDES} 

SOURCES := ${SRC_BJ_WORKERU_DIR}/sornet.cpp

