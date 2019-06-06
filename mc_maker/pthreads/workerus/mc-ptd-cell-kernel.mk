
SRC_MANAGERU_DIR := $(SRC_DIR)/cells/manageru
SRC_CELLS_DIR := $(SRC_DIR)/cells
SRC_PTD_CELLS_DIR := $(SRC_DIR)/pthreads/cells

# =======================================

SRC_CFLAGS := -DMC_IS_PTD_CODE ${STD_EPH_CFLAGS} ${DBG_FLAG} ${g_FLAG} -pthread 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${STD_EPH_CXXFLAGS} ${DBG_FLAG} ${g_FLAG} -pthread 

SRC_INCDIRS := $(SRC_PTHREADS_MANAGER) $(SRC_MANAGERU_DIR) $(SRC_CELLS_DIR) 

SOURCES := \
	$(SRC_PTD_CELLS_DIR)/umm_malloc_ptd.cpp \
	$(SRC_PTD_CELLS_DIR)/shared_ptd.cpp \
	$(SRC_PTD_CELLS_DIR)/global_ptd.cpp \
	$(SRC_PTD_CELLS_DIR)/loader_ptd.cpp \
	$(SRC_PTD_CELLS_DIR)/trace_ptd.cpp \
	$(SRC_PTD_CELLS_DIR)/dyn_mem_ptd.cpp \
	$(SRC_PTD_CELLS_DIR)/cell_ptd.cpp \
	$(SRC_CELLS_DIR)/umm_malloc.c \
	$(SRC_CELLS_DIR)/log.c \
	$(SRC_CELLS_DIR)/shared.c \
	$(SRC_CELLS_DIR)/global.c \
	$(SRC_CELLS_DIR)/broadcast_maps.c \
	$(SRC_CELLS_DIR)/rr_array.c \
	$(SRC_CELLS_DIR)/dyn_mem.c \
	$(SRC_CELLS_DIR)/binder.cpp \
	$(SRC_CELLS_DIR)/cell.cpp \


