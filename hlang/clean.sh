#!/bin/bash

echo "Clean hlang" 

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd $SCRIPT_DIR
hl_maker/hclean.sh
cd $CURR_DIR

