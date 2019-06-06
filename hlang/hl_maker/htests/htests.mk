
HLANG_BASE_DIR := ${HL_CURR_DIR}/../hl_src
HTESTS_BASE_DIR := ${HLANG_BASE_DIR}/hl_tests
MCELLS_DIR := ../../cells
MCELLS_PTD_LIB_DIR := cell_libs

HTESTS_GENERATED_BASE_DIR := ${HL_CURR_DIR}/hl_install/htests/bin/

HTESTS_BMETAL_CFLAGS := ${BMETAL_CFLAGS} 
HTESTS_BMETAL_CXXFLAGS := ${BMETAL_CXXFLAGS} 

HTESTS_DBG_FLAG := -DFULL_DEBUG

#HTESTS_LD_IN_SECTIONS := --gc-sections
HTESTS_LD_IN_SECTIONS := 

MC_ESDK=${EPIPHANY_HOME}
MC_ETOOLS=${MC_ESDK}/tools
MC_STD_INCDIRS := ${MCELLS_DIR} ${MCELLS_DIR}/manageru ${MC_ETOOLS}/host/include 

MC_STD_PTD_LDFLAGS := -L${MCELLS_PTD_LIB_DIR} -L${TARGET_DIR} 

MC_STD_PTD_LDLIBS := -lmc-ptd-cell -lmc-ptd-tak-mak -lmc-ptd-dlmalloc -lpthread

SRC_HTESTS_LIB_DIR := hlang
HTESTS_LIB_DIR := htests/lib/
HTESTS_BIN_DIR := htests/bin/

HLANG_LIB_DIR := hlang/lib/

#	${HLANG_BASE_DIR}/hl_test 

HTESTS_LIB_INCLUDES := \
	${HLANG_BASE_DIR}/ \
	${HLANG_BASE_DIR}/hl_util \
	
	


HTESTS_LDFLAGS := -L${TARGET_DIR}/${HLANG_LIB_DIR}
HTESTS_LDLIBS := -lhlang 


SUBMAKEFILES := \
	./hpru1/hl_pru1.mk \
	./hsend_val/hl_send_val.mk \
	./hsend_val/mc_send_val.mk \
	./hphilo/hl_philo.mk \
	./hphilo/mc_philo.mk \
	./hget_set/hl_get_set.mk \
	./hget_set/mc_get_set.mk \
	./hsend_msg/hl_send_msg.mk \
	./hsend_msg/mc_send_msg.mk \
	./hmem_ops/hl_mem_ops.mk \
	./hmem_ops/mc_mem_ops.mk \
	
	
	

	



