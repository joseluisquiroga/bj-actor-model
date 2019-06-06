#!/bin/bash

echo "hlang clean" 

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# BASE_DIR=$SCRIPT_DIR/../..

cd $SCRIPT_DIR
rm -rf ./bj_tmp_build/hlang
rm -rf ./bj_install/hlang
cd $CURR_DIR

