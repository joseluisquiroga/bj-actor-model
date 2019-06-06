
SEND_MSG_BIN_DIR := ${HTESTS_BIN_DIR}/hsend_msg

TARGET := ${SEND_MSG_BIN_DIR}/hl_send_msg.elf

TGT_LDFLAGS := ${HTESTS_LDFLAGS} -rdynamic
TGT_LDLIBS  := ${HTESTS_LDLIBS} -lstdc++ 
TGT_PREREQS := ${HLANG_LIB_DIR}/libhlang.a

define POST_OPERS
	printf "====================================\nFinished building "$(TARGET)"\n\n\n" 
	cd $(TARGET_DIR)/${HTESTS_BIN_DIR}/hsend_msg; ./hl_send_msg.elf
	$(MAKE) ${TARGET_DIR}/${HTESTS_BIN_DIR}/hsend_msg/mc_send_msg_ptd.elf
	printf "====================================\nFinished hl_send_msg.elf \n\n\n" 
endef

TGT_POSTMAKE := ${POST_OPERS}

SRC_CFLAGS := ${HTESTS_BMETAL_CFLAGS} ${HTESTS_DBG_FLAG} 
SRC_CXXFLAGS := ${HTESTS_BMETAL_CXXFLAGS} ${HTESTS_DBG_FLAG} 

SRC_INCDIRS := ${HTESTS_LIB_INCLUDES}

SOURCES := \
	${HTESTS_BASE_DIR}/hsend_msg/hl_code/hl_send_test.cpp \




