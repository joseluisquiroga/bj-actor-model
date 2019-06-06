
H_DIR=${ESDK}/tools/host

H_LIB_1=${ETOOLS}/host/lib
H_INC_1=${ETOOLS}/host/include

SRC_MANAGER_DIR := $(SRC_DIR)/cells/manageru
SRC_PARALLELLA_DIR := $(SRC_DIR)/parallella
SRC_PLLA_MANAGER_DIR := $(SRC_PARALLELLA_DIR)/manageru
SRC_PLLA_MGR_CELLS_DIR := $(SRC_PLLA_MANAGER_DIR)/cells
SRC_CELLS_DIR := $(SRC_DIR)/cells

# =======================================

TARGET := ${MC_PLLA_LIB_DIR}/libmc-znq-cell.a

TGT_PREREQS := ${MC_PLLA_LIB_DIR}/libmc-znq-dlmalloc.a

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

TGT_CC := $(CROSS_COMPILE)gcc
TGT_CXX := $(CROSS_COMPILE)g++

TGT_POSTMAKE := printf "====================================\nFinished building "$(TARGET)"\n\n\n"

SUBMAKEFILES := mc-znq-manageru.mk mc-znq-cell-kernel.mk

