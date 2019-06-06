

# =======================================

SRC_CFLAGS := -DMC_IS_PTD_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} 

SRC_INCDIRS := ${BJ_MANAGERU_INCLUDES}


SOURCES := \
	${SRC_BJ_MANAGERU_DIR}/cnf_preload/preload_cnf.cpp \
	${SRC_BJ_MANAGERU_DIR}/cnf_preload/preload_sornet.cpp \
	${SRC_BJ_MANAGERU_DIR}/cnf_preload/preload_pgroup.cpp \
	${SRC_BJ_WORKERU_DIR}/dbg_only.cpp \
	${SRC_BJ_WORKERU_DIR}/nervenet.cpp \
	${SRC_BJ_WORKERU_DIR}/sornet.cpp \
	${SRC_BJ_WORKERU_DIR}/sornet_dbg.cpp \
	${SRC_BJ_WORKERU_DIR}/load_cnf.cpp \
	${SRC_BJ_WORKERU_DIR}/load_sornet.cpp \
	${SRC_BJ_WORKERU_DIR}/pgroup.cpp \
	${SRC_BJ_WORKERU_DIR}/propag.cpp \
	${SRC_BJ_WORKERU_DIR}/mirrow.cpp \
	${SRC_BJ_WORKERU_DIR}/stabi.cpp \
	${SRC_BJ_WORKERU_DIR}/solver.cpp \




