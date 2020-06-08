
`include "hglobal.v"

`default_nettype	none

module debouncer 
(	
	`NS_DECLARE_GLB_CHNL(gch),
	input bouncing,
	output steady
);
 
	parameter TOT_CKS = 1;  //old val 250000

	reg [0:0] rg_dbn_rdy = 0;
	reg [$clog2(TOT_CKS):0] cnt_cks = 0;
	reg rg_stdy = 1'b0;
 
	always @(posedge gch_clk)
	begin
		if(gch_reset) begin
			rg_dbn_rdy <= 0;
		end
		if(! gch_reset && ! rg_dbn_rdy) begin
			rg_dbn_rdy <= ! rg_dbn_rdy;
			
			cnt_cks <= 0;
			rg_stdy <= 1'b0;
		end
		if(! gch_reset && rg_dbn_rdy) begin
			// Switch input is different than internal switch value, so an input is
			// changing.  Increase the counter until it is stable for enough time.  
			if (bouncing !== rg_stdy && cnt_cks < TOT_CKS) begin
				cnt_cks <= cnt_cks + 1;
			end
			// End of counter reached, switch is stable, register it, restart counter
			else if (cnt_cks == TOT_CKS) 
			begin
				rg_stdy <= bouncing;
				cnt_cks <= 0;
			end 
			// Switches are the same state, restart the counter
			else 
			begin
				cnt_cks <= 0;
			end
		end
	end
 
	assign gch_ready = rg_dbn_rdy;
	// Assign internal register to output (debounced!)
	assign steady = rg_stdy;
 
endmodule

