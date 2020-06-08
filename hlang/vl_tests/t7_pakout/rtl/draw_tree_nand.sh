yosys -p "read_verilog tree_nand.v; prep -top tree_nand -flatten; aigmap; write_json tree_nand.json"
netlistsvg tree_nand.json -o tree_nand.svg
