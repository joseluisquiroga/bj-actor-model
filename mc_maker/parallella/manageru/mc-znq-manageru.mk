

# =======================================

SRC_CFLAGS := -DMC_IS_ZNQ_CODE -Wall -std=gnu11 ${DBG_FLAG}
SRC_CXXFLAGS := -DMC_IS_ZNQ_CODE -Wall -std=c++17 ${DBG_FLAG}

SRC_INCDIRS := $(SRC_MANAGER_DIR) $(SRC_PLLA_MANAGER_DIR) $(SRC_CELLS_DIR) $(H_INC_1) 

SOURCES := \
	$(SRC_MANAGER_DIR)/booter.c \
	$(SRC_CELLS_DIR)/rr_array.c \
	$(SRC_CELLS_DIR)/shared.c \
	$(SRC_PLLA_MGR_CELLS_DIR)/shared_znq.cpp \
	$(SRC_PLLA_MANAGER_DIR)/workeru_loader_znq.c \
	$(SRC_PLLA_MANAGER_DIR)/prt_eph_call_stack_znq.c \
	$(SRC_PLLA_MANAGER_DIR)/read_eph_elf_link_syms_tab_znq.c \
	$(SRC_PLLA_MANAGER_DIR)/booter_znq.cpp \


