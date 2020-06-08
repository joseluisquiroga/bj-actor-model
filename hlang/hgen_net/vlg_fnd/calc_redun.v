
`include "hglobal.v"

`default_nettype	none


module calc_redun
#(parameter ASZ=`NS_ADDRESS_SIZE, DSZ=`NS_DATA_SIZE, RSZ=`NS_REDUN_SIZE)
(
	input wire [ASZ-1:0] mg_src,
	input wire [ASZ-1:0] mg_dst,
	input wire [DSZ-1:0] mg_dat,
	output wire [RSZ-1:0] redun
);

	parameter MSZ = (ASZ + ASZ + DSZ);
	parameter PART_SZ = (MSZ / RSZ);
	parameter REST_SZ = (MSZ % RSZ) + PART_SZ;
	
	wire [MSZ-1:0] full_msg; 

	assign full_msg = {mg_src, mg_dst, mg_dat};

	genvar ii;
	
	for(ii = 0; ii < RSZ-1; ii = ii+1) begin
		localparam hi_lim = (((ii+1)*PART_SZ)-1);
		localparam low_lim = (ii*PART_SZ);
		tree_nand #(.WIDTH(PART_SZ)) 
			t1 (full_msg[hi_lim:low_lim], redun[ii]);
	end
	
	tree_nand #(.WIDTH(REST_SZ)) 
		t1 (full_msg[MSZ-1:((RSZ-1)*PART_SZ)], redun[RSZ-1]);
	
	
endmodule

