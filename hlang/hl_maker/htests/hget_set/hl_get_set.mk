
GET_SET_BIN_DIR := ${HTESTS_BIN_DIR}/hget_set

TARGET := ${GET_SET_BIN_DIR}/hl_get_set.elf

TGT_LDFLAGS := ${HTESTS_LDFLAGS} -rdynamic
TGT_LDLIBS  := ${HTESTS_LDLIBS} -lstdc++ 
TGT_PREREQS := ${HLANG_LIB_DIR}/libhlang.a

define POST_OPERS
	printf "====================================\nFinished building "$(TARGET)"\n\n\n" 
	cd $(TARGET_DIR)/${HTESTS_BIN_DIR}/hget_set; ./hl_get_set.elf
	$(MAKE) ${TARGET_DIR}/${HTESTS_BIN_DIR}/hget_set/mc_get_set_ptd.elf
	printf "====================================\nFinished hl_get_set.elf \n\n\n" 
endef

TGT_POSTMAKE := ${POST_OPERS}

SRC_CFLAGS := ${HTESTS_BMETAL_CFLAGS} ${HTESTS_DBG_FLAG} 
SRC_CXXFLAGS := ${HTESTS_BMETAL_CXXFLAGS} ${HTESTS_DBG_FLAG} 

SRC_INCDIRS := ${HTESTS_LIB_INCLUDES}

SOURCES := \
	${HTESTS_BASE_DIR}/hget_set/hl_code/hl_get_set_test.cpp \




