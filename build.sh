#!/bin/bash

# set -e

CURR_DIR=`pwd`

cd mc_maker
make
cd $CURR_DIR

cd src/tests/src
make
cd $CURR_DIR

cd ben-jose-embryo/bj_maker
make
cd $CURR_DIR

cd hlang/hl_maker
make
cd $CURR_DIR

