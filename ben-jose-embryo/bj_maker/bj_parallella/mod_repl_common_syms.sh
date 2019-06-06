
DIR_LIB=$(dirname $1)

e-objcopy --redefine-syms=$DIR_LIB/$2"_comm_nms.txt" --rename-section common_code=$3 --rename-section common_data=$4 $1

# e-objcopy --rename-section common_code=$3 --rename-section common_data=$4 $1

