
SRC_MANAGERU_DIR := $(SRC_DIR)/cells/manageru
SRC_CELLS_DIR := $(SRC_DIR)/cells

# =======================================

C_FLAGS_1 := -Wall -std=gnu11 ${g_FLAG} -pthread 
SRC_CFLAGS := -DMC_IS_PTD_CODE ${DBG_FLAG} ${C_FLAGS_1} 

CXX_FLAGS_1 := -Wall -std=c++17 ${g_FLAG} -pthread 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${DBG_FLAG} ${CXX_FLAGS_1}

SRC_INCDIRS := $(SRC_PTHREADS_MANAGER) $(SRC_MANAGERU_DIR) ${SRC_CELLS_DIR}

SOURCES := \
	$(SRC_MANAGERU_DIR)/booter.c \
	$(SRC_PTHREADS_MANAGER)/booter_ptd.cpp \
	$(SRC_PTHREADS_MANAGER)/thread_ptd.cpp



