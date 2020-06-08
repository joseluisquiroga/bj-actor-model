yosys -p "read_verilog tree_nand.v; read_verilog calc_redun.v; prep -top calc_redun -flatten; aigmap; write_json calc_redun.json"
netlistsvg calc_redun.json -o calc_redun.svg
