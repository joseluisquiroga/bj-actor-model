
DIR_LIB=$(dirname $1)

awk -v pref=$2 '{ print $1" "pref"_"$1 }' $DIR_LIB/"comm_nms.txt" > $DIR_LIB/$2"_comm_nms.txt"

