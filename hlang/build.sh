#!/bin/bash

echo "Build hlang" 

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $SCRIPT_DIR
cd hl_maker
make
cd ..
cd $CURR_DIR


