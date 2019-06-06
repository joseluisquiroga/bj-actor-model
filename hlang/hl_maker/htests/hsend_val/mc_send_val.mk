

MC_SEND_VAL_BASE_DIR := ${HTESTS_GENERATED_BASE_DIR}/hsend_val/hl_generated_output_cpp

MC_SEND_VAL_BIN_DIR := ${HTESTS_BIN_DIR}/hsend_val

# =======================================

TARGET := ${MC_SEND_VAL_BIN_DIR}/mc_send_val_ptd.elf

TGT_LDFLAGS := ${MC_STD_PTD_LDFLAGS} ${HTESTS_LD_IN_SECTIONS} -rdynamic
TGT_LDLIBS  := ${MC_STD_PTD_LDLIBS}
TGT_PREREQS := ${HTESTS_BIN_DIR}/hsend_val/hl_send_val.elf

SRC_CFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CFLAGS} ${HTESTS_DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CXXFLAGS} ${HTESTS_DBG_FLAG} 

SRC_INCDIRS := ${MC_STD_INCDIRS} ${HTESTS_LIB_INCLUDES} ${MC_SEND_VAL_BASE_DIR}

SOURCES := \
	${HTESTS_BASE_DIR}/hsend_val/mc_code/mc_send_val_mgru.cpp \
	${HTESTS_BASE_DIR}/hsend_val/mc_code/mc_send_val_wrku.cpp \
	${MC_SEND_VAL_BASE_DIR}/hl_generated_output_glbs.cpp \
	${MC_SEND_VAL_BASE_DIR}/cls_snd_val.cpp \
	
	




