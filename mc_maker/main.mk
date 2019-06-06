
DBG_FLAG := -DFULL_DEBUG

#SRC_IN_SECTIONS := -fdata-sections -ffunction-sections
SRC_IN_SECTIONS := 

MC_MAKER_DIR := $(shell pwd)

SRC_DIR := ${MC_MAKER_DIR}/../src

BUILD_DIR := ./mc_tmp_build
TARGET_DIR := ./mc_install

STD_EPH_CFLAGS := -Wall -std=gnu11 -ffreestanding -nostdlib -nostartfiles -fno-default-inline 

CXXFLAGS_1 := -Wall -std=c++17 -nostdlib -fno-exceptions -fno-unwind-tables 
#CXXFLAGS_1 := -Wall -std=c++17 -nostdlib -fno-unwind-tables 
CXXFLAGS_2 := -fno-rtti -fno-default-inline -fno-threadsafe-statics -fno-elide-constructors
STD_EPH_CXXFLAGS := ${CXXFLAGS_1} ${CXXFLAGS_2}

SUBMAKEFILES := \
	pthreads/mc_ptd_all_libs.mk \
	parallella/mc_plla_all_libs.mk \


default: all
	bash mc_prep_includes/prepare_include_dir_sh ; echo "FINISHED_BUILDING_MESSAGING_CELLS"
	
install: 
	@echo "Copy the files in ../bin to the desired install directories."

help: 
	@echo "See documentation in <base_dir>/docs."
