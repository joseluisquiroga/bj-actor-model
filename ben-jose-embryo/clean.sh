#!/bin/bash

echo "Clean ben-jose_embryo" 

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $SCRIPT_DIR
bj_maker/clean.sh
cd $CURR_DIR

