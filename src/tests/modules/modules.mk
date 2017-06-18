
MODULES_DIR := modules

MODULES_LDF=modules/lds_modules.ldf

TARGET := ${MODULES_DIR}/modules.elf

TGT_LDFLAGS := -T ${MODULES_LDF} ${MC_EPH_LDFLAGS_2} --no-check-sections -L${TARGET_DIR}/${MODULES_DIR}
TGT_LDLIBS  := -lmod_1 -lmod_2 -lmod_3 ${MC_STD_EPH_LDLIBS}


TGT_PREREQS := \
	${MC_EPH_LIBS} \
	${MODULES_DIR}/libcommon.a \
	${MODULES_DIR}/libmod_1.a \
	${MODULES_DIR}/libmod_2.a \
	${MODULES_DIR}/libmod_3.a \


define POST_OPERS
	e-objdump -D $(TARGET_DIR)/$(TARGET) > $(TARGET_DIR)/$(TARGET).s
	printf "====================================\nFinished building "$(TARGET)"\n\n\n" 
endef

TGT_POSTMAKE := ${POST_OPERS}

TGT_CC := e-gcc
TGT_CXX := e-g++
TGT_LINKER := e-ld

SRC_CFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS}
SRC_CXXFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS}

SRC_INCDIRS := ${MC_STD_INCDIRS}

SUBMAKEFILES := \
	common.mk \
	mod1.mk \
	mod2.mk \
	mod3.mk \

SOURCES := mods_test.cpp


