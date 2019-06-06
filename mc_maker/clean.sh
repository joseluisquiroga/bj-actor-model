#!/bin/bash

echo "Messaging Cells clean" 

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $SCRIPT_DIR
rm -rf ./mc_tmp_build
rm -rf ./mc_install
cd $CURR_DIR

