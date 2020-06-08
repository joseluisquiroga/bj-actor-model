#!/bin/bash

CURR_DIR=$(pwd)
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Getting files" $(basename $SCRIPT_DIR)

cd $SCRIPT_DIR

rm ./*.v

cp ../../vl_tests/common/* .
cp ../../vl_tests/t5_nd_1to2/rtl/nd_1to2.v .
cp ../../vl_tests/t6_nd_2to1/rtl/nd_2to1.v .
cp ../../vl_tests/t7_pakout/rtl/pakout.v .
cp ../../vl_tests/t8_pakin/rtl/pakin.v .


cd $CURR_DIR
