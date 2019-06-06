
# =======================================

SRC_CFLAGS := -DMC_IS_ZNQ_CODE ${STD_EPH_CFLAGS} ${DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_ZNQ_CODE ${STD_EPH_CXXFLAGS} ${DBG_FLAG} 

SRC_INCDIRS := $(SRC_MANAGER_DIR) $(SRC_CELLS_DIR) $(SRC_PLLA_MANAGER_DIR) $(H_INC_1)

SOURCES := \
	$(SRC_PLLA_MGR_CELLS_DIR)/log_znq.c \
	$(SRC_PLLA_MGR_CELLS_DIR)/global_znq.cpp \
	$(SRC_PLLA_MGR_CELLS_DIR)/loader_znq.cpp \
	$(SRC_PLLA_MGR_CELLS_DIR)/cell_znq.cpp \
	$(SRC_CELLS_DIR)/global.c \
	$(SRC_CELLS_DIR)/binder.cpp \
	$(SRC_CELLS_DIR)/cell.cpp \


