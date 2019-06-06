
BUILD_DIR := ./hl_tmp_build
TARGET_DIR := ./hl_install

HL_CURR_DIR := $(shell pwd)

BMETAL_CFLAGS := -Wall -std=gnu11 -ffreestanding -nostdlib -nostartfiles -fno-default-inline 

BMETAL_CXXFLAGS_P1 := -Wall -std=c++17 -nostdlib -fno-exceptions -fno-unwind-tables -fno-extern-tls-init
BMETAL_CXXFLAGS_P2 := -fno-rtti -fno-default-inline -fno-threadsafe-statics -fno-elide-constructors
BMETAL_CXXFLAGS := ${BMETAL_CXXFLAGS_P1} ${BMETAL_CXXFLAGS_P2}

SUBMAKEFILES := \
	./hlang/hlang.mk \
	./htests/htests.mk \


default: all
	@echo "Finished building hlang"

install: 
	@echo "Copy the files in ../bin to the desired install directories."

help: 
	@echo "See documentation in <base_dir>/docs."
