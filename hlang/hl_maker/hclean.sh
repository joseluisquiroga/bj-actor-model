#!/bin/bash

echo "hlang clean" 

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# BASE_DIR=$SCRIPT_DIR/../..

cd $SCRIPT_DIR
rm -rf ./hl_tmp_build
rm -rf ./hl_install
cd $CURR_DIR

