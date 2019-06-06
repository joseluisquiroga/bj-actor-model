
#LD_IN_SECTIONS := --gc-sections
LD_IN_SECTIONS := 

BJ_LDF := bj_parallella/bj-lk-script.ldf

TARGET := ${BJ_PLLA_BIN_DIR}/ben-jose-embryo.elf

TGT_LDFLAGS := -T ${BJ_LDF} ${MC_EPH_LDFLAGS_2} ${LD_IN_SECTIONS} --no-check-sections -L${TARGET_DIR}
TGT_LDLIBS  := -ldbg_only -lload_cnf -lnervenet -lpropag -lstabi -lsornet -lpgroup ${MC_STD_EPH_LDLIBS}

TGT_PREREQS := \
	${MC_EPH_LIBS} \
	${BJ_PLLA_LIB_DIR}/libdbg_only.a \
	${BJ_PLLA_LIB_DIR}/libnervenet.a \
	${BJ_PLLA_LIB_DIR}/libload_cnf.a \
	${BJ_PLLA_LIB_DIR}/libpropag.a \
	${BJ_PLLA_LIB_DIR}/libstabi.a \
	${BJ_PLLA_LIB_DIR}/libsornet.a \
	${BJ_PLLA_LIB_DIR}/libpgroup.a \


CNFS_PLLA_LNK_DIR := $(TARGET_DIR)/${BJ_PLLA_BIN_DIR}
CNFS_PLLA_LNK_DIR := $(call CANONICAL_PATH,${CNFS_PLLA_LNK_DIR})

CNFS_PLLA_LNK := ${CNFS_PLLA_LNK_DIR}/cnfs
CNFS_PLLA_LNK := $(call CANONICAL_PATH,${CNFS_PLLA_LNK})

define POST_OPERS
	if [ ! -e $(CNFS_PLLA_LNK) ]; then ln -s ${CNFS_DIR} ${CNFS_PLLA_LNK_DIR}; fi;
	e-objdump -h $(TARGET_DIR)/$(TARGET) > $(TARGET_DIR)/$(TARGET)_sizes.txt
	e-objdump -D $(TARGET_DIR)/$(TARGET) > $(TARGET_DIR)/$(TARGET).s
	printf "====================================\nFinished building "$(TARGET)"\n\n\n" 
endef

TGT_POSTMAKE := ${POST_OPERS}

TGT_CC := e-gcc
TGT_CXX := e-g++
TGT_LINKER := e-ld

SRC_CFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS}
SRC_CXXFLAGS := -DMC_IS_EPH_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} ${SRC_IN_SECTIONS}

SRC_INCDIRS := ${BJ_WORKERUS_INCLUDES} 

SOURCES := ${SRC_BJ_WORKERU_DIR}/solver.cpp

