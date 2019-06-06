
SRC_PLLA_WORKERS_DIR := $(SRC_DIR)/parallella/workerus
SRC_CELLS_DIR := $(SRC_DIR)/cells

# =======================================

TARGET := ${MC_PLLA_LIB_DIR}/libmc-eph-cell.a

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

TGT_CC := e-gcc
TGT_CXX := e-g++

SRC_CFLAGS := -DMC_IS_EPH_CODE ${STD_EPH_CFLAGS} ${DBG_FLAG} ${SRC_IN_SECTIONS}
SRC_CXXFLAGS := -DMC_IS_EPH_CODE ${STD_EPH_CXXFLAGS} ${DBG_FLAG} ${SRC_IN_SECTIONS}

SRC_INCDIRS := $(SRC_CELLS_DIR) $(SRC_PLLA_MANAGER_DIR)


SOURCES := \
	$(SRC_PLLA_WORKERS_DIR)/interruptions_eph.c \
	$(SRC_PLLA_WORKERS_DIR)/umm_malloc_eph.c \
	$(SRC_PLLA_WORKERS_DIR)/shared_eph.c \
	$(SRC_PLLA_WORKERS_DIR)/loader_eph.c \
	$(SRC_PLLA_WORKERS_DIR)/global_eph.c \
	$(SRC_PLLA_WORKERS_DIR)/call_workerus_main_eph.c \
	$(SRC_PLLA_WORKERS_DIR)/cell_eph.cpp \
	$(SRC_PLLA_WORKERS_DIR)/trace_eph.c \
	$(SRC_PLLA_WORKERS_DIR)/e_start.s \
	$(SRC_CELLS_DIR)/umm_malloc.c \
	$(SRC_CELLS_DIR)/log.c \
	$(SRC_CELLS_DIR)/shared.c \
	$(SRC_CELLS_DIR)/broadcast_maps.c \
	$(SRC_CELLS_DIR)/global.c \
	$(SRC_CELLS_DIR)/rr_array.c \
	$(SRC_CELLS_DIR)/dyn_mem.c \
	$(SRC_CELLS_DIR)/binder.cpp \
	$(SRC_CELLS_DIR)/cell.cpp \


