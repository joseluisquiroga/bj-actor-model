
TARGET := modules/libmod_1.a

TGT_PREREQS := libcommon.a

define POST_OPERS
	modules/mod_make_redef_list.sh $(TARGET_DIR)/$(TARGET) m1
	modules/mod_repl_common_syms.sh $(TARGET_DIR)/$(TARGET) m1 module1_code module1_data
	printf "====================================\nFinished building "$(TARGET)"\n\n\n" 
endef

TGT_POSTMAKE := ${POST_OPERS}

TGT_CC := e-gcc
TGT_CXX := e-g++

SRC_CFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS}
SRC_CXXFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS}

SRC_INCDIRS := ${MC_STD_INCDIRS}

SOURCES := common.cpp pru_1.cpp


