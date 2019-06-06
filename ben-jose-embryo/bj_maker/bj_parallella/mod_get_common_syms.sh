
DIR_LIB=$(dirname $1)

e-objdump --syms $1 | awk '{ if(($4 == "common_code")||($4 == "common_data")){print $6} }' > $DIR_LIB/"comm_nms.txt"

echo "Wrote file "$DIR_LIB/"comm_nms.txt"

