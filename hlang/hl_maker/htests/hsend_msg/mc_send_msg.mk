

MC_SEND_MSG_BASE_DIR := ${HTESTS_GENERATED_BASE_DIR}/hsend_msg/hl_generated_output_cpp

MC_SEND_MSG_BIN_DIR := ${HTESTS_BIN_DIR}/hsend_msg

# =======================================

TARGET := ${MC_SEND_MSG_BIN_DIR}/mc_send_msg_ptd.elf

TGT_LDFLAGS := ${MC_STD_PTD_LDFLAGS} ${HTESTS_LD_IN_SECTIONS} -rdynamic
TGT_LDLIBS  := ${MC_STD_PTD_LDLIBS}
TGT_PREREQS := ${HTESTS_BIN_DIR}/hsend_msg/hl_send_msg.elf

SRC_CFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CFLAGS} ${HTESTS_DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CXXFLAGS} ${HTESTS_DBG_FLAG} 

SRC_INCDIRS := ${MC_STD_INCDIRS} ${HTESTS_LIB_INCLUDES} ${MC_SEND_MSG_BASE_DIR}

SOURCES := \
	${HTESTS_BASE_DIR}/hsend_msg/mc_code/mc_send_msg_mgru.cpp \
	${HTESTS_BASE_DIR}/hsend_msg/mc_code/mc_send_msg_wrku.cpp \
	${MC_SEND_MSG_BASE_DIR}/hl_generated_output_glbs.cpp \
	${MC_SEND_MSG_BASE_DIR}/msv_1.cpp \
	${MC_SEND_MSG_BASE_DIR}/cls_snd.cpp \
	
	




