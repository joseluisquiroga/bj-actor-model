
PRU1_BIN_DIR := ${HTESTS_BIN_DIR}/hpru1

TARGET := ${PRU1_BIN_DIR}/hl_pru1.elf

TGT_LDFLAGS := ${HTESTS_LDFLAGS} -rdynamic
TGT_LDLIBS  := ${HTESTS_LDLIBS} -lstdc++ 
TGT_PREREQS := ${HLANG_LIB_DIR}/libhlang.a

define POST_OPERS
	printf "====================================\nFinished building "$(TARGET)"\n\n\n" 
	pwd
	cd $(TARGET_DIR)/${HTESTS_BIN_DIR}/hpru1; ./hl_pru1.elf
	pwd
	printf "====================================\nFinished hl_pru1.elf \n\n\n" 
endef

TGT_POSTMAKE := ${POST_OPERS}

SRC_CFLAGS := ${HTESTS_BMETAL_CFLAGS} ${HTESTS_DBG_FLAG} 
SRC_CXXFLAGS := ${HTESTS_BMETAL_CXXFLAGS} ${HTESTS_DBG_FLAG} 

SRC_INCDIRS := ${HTESTS_LIB_INCLUDES}

SOURCES := \
	${HTESTS_BASE_DIR}/hpru1/hl_pru1.cpp \




