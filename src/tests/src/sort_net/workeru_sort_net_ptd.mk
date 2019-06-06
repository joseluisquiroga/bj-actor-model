
SRC_CFLAGS := -DMC_IS_PTD_CODE ${MC_STD_EPH_CFLAGS} ${MC_DBG_FLAG} 
SRC_CXXFLAGS := -DMC_IS_PTD_CODE ${MC_STD_EPH_CXXFLAGS} ${MC_DBG_FLAG} 

SRC_INCDIRS := ${MC_STD_INCDIRS}

#	sornet.cpp \
#	load_sornet.cpp \
#	solver.cpp \


SOURCES := manageru_preload.cpp workerus_sort_net.cpp




