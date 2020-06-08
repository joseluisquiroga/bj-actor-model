#!/bin/bash

echo "FULL_CLEAN" 

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $SCRIPT_DIR
mc_maker/clean.sh
src/tests/clean.sh
ben-jose-embryo/bj_maker/clean.sh
hlang/clean.sh
cd $CURR_DIR

