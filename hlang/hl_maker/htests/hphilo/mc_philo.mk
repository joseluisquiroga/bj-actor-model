

MC_PHILO_BASE_DIR := ${HTESTS_GENERATED_BASE_DIR}/hphilo/hl_generated_output_cpp

MC_PHILO_BIN_DIR := ${HTESTS_BIN_DIR}/hphilo

# =======================================

TARGET := ${MC_PHILO_BIN_DIR}/mc_philo_ptd.elf

TGT_LDFLAGS := ${MC_STD_PTD_LDFLAGS} ${HTESTS_LD_IN_SECTIONS} -rdynamic
TGT_LDLIBS  := ${MC_STD_PTD_LDLIBS}
TGT_PREREQS := ${HTESTS_BIN_DIR}/hphilo/hl_philo.elf

SRC_CFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CFLAGS} ${HTESTS_DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${HTESTS_BMETAL_CXXFLAGS} ${HTESTS_DBG_FLAG} 

SRC_INCDIRS := ${MC_STD_INCDIRS} ${HTESTS_LIB_INCLUDES} ${MC_PHILO_BASE_DIR}

SOURCES := \
	${HTESTS_BASE_DIR}/hphilo/mc_code/mc_philo_mgru.cpp \
	${HTESTS_BASE_DIR}/hphilo/mc_code/mc_philo_wrku.cpp \
	${MC_PHILO_BASE_DIR}/hl_generated_output_glbs.cpp \
	${MC_PHILO_BASE_DIR}/chopstick.cpp \
	${MC_PHILO_BASE_DIR}/philosopher.cpp \
	
	




