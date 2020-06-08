
`include "hglobal.v"

`default_nettype	none


module tree_nand
#(parameter WIDTH = 32)
(
	input [WIDTH-1:0] in_nand,
	output            out_nand
);

generate
	if(WIDTH == 1) begin
		assign out_nand = in_nand;
	end
	else if(WIDTH == 2) begin
		assign out_nand = ~(in_nand[0] & in_nand[1]);
	end
	else begin
		wire out_nand_low;
		wire out_nand_high;
		tree_nand #(.WIDTH (WIDTH/2))
			tree_nand_low(.in_nand (in_nand[WIDTH/2-1:0]), .out_nand (out_nand_low));

		tree_nand #(.WIDTH (WIDTH - WIDTH/2))
			tree_nand_high(.in_nand (in_nand[WIDTH-1:WIDTH/2]), .out_nand (out_nand_high));

		assign out_nand = ~(out_nand_low & out_nand_high);
	end
endgenerate

endmodule

