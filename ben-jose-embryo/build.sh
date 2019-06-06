#!/bin/bash

echo "Build ben-jose_embryo" 

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $SCRIPT_DIR
cd bj_maker
make
cd ..
cd $CURR_DIR


