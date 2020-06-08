# verilog_defaults -add -I./cell_src_snk/
yosys -import
read_verilog bin_to_disp.v; 
read_verilog debouncer.v; 
read_verilog tree_nand.v; 
read_verilog calc_redun.v; 
read_verilog nd_1to2.v; 
read_verilog io_1to2.v; 
read_verilog test_5.v;
synth_ice40 -top test_top -json ../$::env(BUILD_DIR)/test_5.json;
