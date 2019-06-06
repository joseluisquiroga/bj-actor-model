
HLANG_BASE_DIR := ${HL_CURR_DIR}/../hl_src

HLANG_LIB_DIR := hlang/lib/

HLANG_BMETAL_CFLAGS := ${BMETAL_CFLAGS} 
HLANG_BMETAL_CXXFLAGS := ${BMETAL_CXXFLAGS} 

HLANG_DBG_FLAG := -DFULL_DEBUG

HLANG_LIB_INCLUDES := \
	${HLANG_BASE_DIR}/ \
	${HLANG_BASE_DIR}/hl_util \
	${HLANG_BASE_DIR}/hl_test \


# -------------------------------------------------------------------------------------------

TARGET := ${HLANG_LIB_DIR}/libhlang.a

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := ${HLANG_BMETAL_CFLAGS} ${HLANG_DBG_FLAG} 
SRC_CXXFLAGS := ${HLANG_BMETAL_CXXFLAGS} ${HLANG_DBG_FLAG} 

SRC_INCDIRS := ${HLANG_LIB_INCLUDES}

SOURCES := \
	${HLANG_BASE_DIR}/hl_util/dbg_util.cpp \
	${HLANG_BASE_DIR}/hl_util/sha2.cpp \
	${HLANG_BASE_DIR}/hlang.cpp \




