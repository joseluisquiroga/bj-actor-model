#!/bin/bash 

cd /home/jose/devel/messaging-cells

for number in {1..1000}
do
	./ptd_bj.sh $*
	if [ $? -ne 0 ]
	then
		echo "SCRIPT_FAILED_"$number
		echo "SCRIPT_FAILED_"$number
		echo "SCRIPT_FAILED_"$number
		echo "SCRIPT_FAILED_"$number
		exit 0
	else
		echo "FINISHED_"$number
		echo "FINISHED_"$number
		echo "FINISHED_"$number > current_iter_mil.txt
	fi
done

exit 0

