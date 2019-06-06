#!/bin/bash 

cd /home/jose/devel/messaging-cells

for number in {1..100}
do
	./ptd_bj.sh $*
	if [ $? -ne 0 ]
	then
		echo "SCRIPT_FAILED_"$number
		echo "SCRIPT_FAILED_"$number
		echo "SCRIPT_FAILED_"$number
		echo "SCRIPT_FAILED_"$number
		exit 0
	fi
done

exit 0

