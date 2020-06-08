# verilog_defaults -add -I./cell_src_snk/
yosys -import
read_verilog bin_to_disp.v; 
read_verilog debouncer.v; 
read_verilog pulses.v;
synth_ice40 -top test_top -json ../$::env(BUILD_DIR)/pulses.json;
