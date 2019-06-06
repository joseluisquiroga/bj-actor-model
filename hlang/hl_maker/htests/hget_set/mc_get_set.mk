

MC_GET_SET_BASE_DIR := ${HTESTS_GENERATED_BASE_DIR}/hget_set/hl_generated_output_cpp

MC_GET_SET_BIN_DIR := ${HTESTS_BIN_DIR}/hget_set

# =======================================

TARGET := ${MC_GET_SET_BIN_DIR}/mc_get_set_ptd.elf

TGT_LDFLAGS := ${MC_STD_PTD_LDFLAGS} ${HTESTS_LD_IN_SECTIONS} -rdynamic
TGT_LDLIBS  := ${MC_STD_PTD_LDLIBS}
TGT_PREREQS := ${HTESTS_BIN_DIR}/hget_set/hl_get_set.elf

SRC_CFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CFLAGS} ${HTESTS_DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CXXFLAGS} ${HTESTS_DBG_FLAG} 

SRC_INCDIRS := ${MC_STD_INCDIRS} ${HTESTS_LIB_INCLUDES} ${MC_GET_SET_BASE_DIR}

SOURCES := \
	${HTESTS_BASE_DIR}/hget_set/mc_code/mc_get_set_mgru.cpp \
	${HTESTS_BASE_DIR}/hget_set/mc_code/mc_get_set_wrku.cpp \
	${MC_GET_SET_BASE_DIR}/hl_generated_output_glbs.cpp \
	${MC_GET_SET_BASE_DIR}/cls_gs_data.cpp \
	
	




