

MC_MEM_OPS_BASE_DIR := ${HTESTS_GENERATED_BASE_DIR}/hmem_ops/hl_generated_output_cpp

MC_MEM_OPS_BIN_DIR := ${HTESTS_BIN_DIR}/hmem_ops

# =======================================

TARGET := ${MC_MEM_OPS_BIN_DIR}/mc_mem_ops_ptd.elf

TGT_LDFLAGS := ${MC_STD_PTD_LDFLAGS} ${HTESTS_LD_IN_SECTIONS} -rdynamic
TGT_LDLIBS  := ${MC_STD_PTD_LDLIBS}
TGT_PREREQS := ${HTESTS_BIN_DIR}/hmem_ops/hl_mem_ops.elf

SRC_CFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CFLAGS} ${HTESTS_DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CXXFLAGS} ${HTESTS_DBG_FLAG} 

SRC_INCDIRS := ${MC_STD_INCDIRS} ${HTESTS_LIB_INCLUDES} ${MC_MEM_OPS_BASE_DIR}

SOURCES := \
	${HTESTS_BASE_DIR}/hmem_ops/mc_code/mc_mem_ops_mgru.cpp \
	${HTESTS_BASE_DIR}/hmem_ops/mc_code/mc_mem_ops_wrku.cpp \
	${MC_MEM_OPS_BASE_DIR}/hl_generated_output_glbs.cpp \
	${MC_MEM_OPS_BASE_DIR}/msv_1.cpp \
	${MC_MEM_OPS_BASE_DIR}/cls_mem_ops.cpp \
	
	




