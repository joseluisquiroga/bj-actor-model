
BUILD_DIR := ./gh_build
TARGET_DIR := ./

HL_CURR_DIR := $(shell pwd)

BMETAL_CFLAGS := -Wall -std=gnu11 -ffreestanding -nostdlib -nostartfiles -fno-default-inline 

BMETAL_CXXFLAGS_P1 := -Wall -std=c++17 -nostdlib -fno-exceptions -fno-unwind-tables -fno-extern-tls-init
BMETAL_CXXFLAGS_P2 := -fno-rtti -fno-default-inline -fno-threadsafe-statics -fno-elide-constructors
BMETAL_CXXFLAGS := ${BMETAL_CXXFLAGS_P1} ${BMETAL_CXXFLAGS_P2}

HG_DBG_FLAG := -DFULL_DEBUG

HG_BASE_DIR := .

# -------------------------------------------------------------------------------------------


TARGET := hgen_lognet

TGT_LDFLAGS := -rdynamic -pthread

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SRC_CFLAGS := ${BMETAL_CFLAGS} ${HG_DBG_FLAG} -pthread
SRC_CXXFLAGS := ${BMETAL_CXXFLAGS} ${HG_DBG_FLAG} -pthread

SRC_INCDIRS := ${HG_BASE_DIR}/gh_util

SOURCES := \
	${HG_BASE_DIR}/gh_util/tak_mak.cpp \
	${HG_BASE_DIR}/gh_util/gh_dbg_util.cpp \
	${HG_BASE_DIR}/gh_util/gh_file_utils.cpp \
	${HG_BASE_DIR}/hgen_main.cpp \
	${HG_BASE_DIR}/hgen_net.cpp \
	${HG_BASE_DIR}/simu_hnet.cpp \
	${HG_BASE_DIR}/vlg_hnet.cpp \


