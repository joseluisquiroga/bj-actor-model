
BJ_MC_LIBS_PLLA := ../../mc_maker/mc_install/parallella/lib
BJ_MC_LIBS_PLLA := $(call CANONICAL_PATH,${BJ_MC_LIBS_PLLA})
# $(warning "BJ_MC_LIBS_PLLA is "${BJ_MC_LIBS_PLLA})

BJ_PLLA_LIB_DIR := parallella/lib/
BJ_PLLA_BIN_DIR := parallella/bin/
BJ_PLLA_LIB_FLG_DIR := ${TARGET_DIR}/${BJ_PLLA_LIB_DIR}

# =======================================

define SET_CROSS_COMPILE
ifeq "${CROSS_COMPILE+xxx}" ""
	ifeq "$(findstring arm,$(shell uname -p))" "arm"
		CROSS_COMPILE=
	else
		CROSS_COMPILE=arm-linux-gnueabihf-
	endif	
endif	
endef

$(eval $(SET_CROSS_COMPILE))

MC_ESDK=${EPIPHANY_HOME}
MC_ETOOLS=${MC_ESDK}/tools

MC_EPH_LDFLAGS_1 := -L${MC_ETOOLS}/e-gnu/epiphany-elf/lib -L${MC_ETOOLS}/e-gnu/lib/gcc/epiphany-elf/5.4.0/
MC_EPH_LDFLAGS_2 := -L${BJ_MC_LIBS_PLLA} -L${TARGET_DIR} -L${BJ_PLLA_LIB_FLG_DIR} ${MC_EPH_LDFLAGS_1} --strip-debug -static 

MC_ZNQ_LDFLAGS_1 := -L${MC_ETOOLS}/host/lib 
MC_STD_ZNQ_LDFLAGS := -L${BJ_MC_LIBS_PLLA} -L${TARGET_DIR} -L${BJ_PLLA_LIB_FLG_DIR} ${MC_ZNQ_LDFLAGS_1} 

MC_STD_C_LDLIBS := -lc -lepiphany -lgcc -lg 
MC_STD_ZNQ_LDLIBS := -lmc-znq-cell -lmc-znq-tak-mak -lmc-znq-dlmalloc -le-hal -lm 
MC_STD_EPH_LDLIBS := -lmc-eph-cell -lmc-eph-tak-mak 

MC_ZNQ_LIBS := 
MC_EPH_LIBS := 


SUBMAKEFILES := \
	./bj_manageru/bj_manageru_znq.mk \
	./bj_workeru/dbg_only.mk \
	./bj_workeru/nervenet.mk \
	./bj_workeru/load_cnf.mk \
	./bj_workeru/pgroup.mk \
	./bj_workeru/sornet.mk \
	./bj_workeru/propag.mk \
	./bj_workeru/stabi.mk \
	./bj_workeru/solver.mk \


