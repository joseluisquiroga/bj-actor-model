
`include "hglobal.v"

`default_nettype	none


module hnull_source
#(parameter 
	MY_LOCAL_ADDR=0, 
	PRB_DST_ADDR=0, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE)
(
	`NS_DECLARE_GLB_CHNL(gch),
	`NS_DECLARE_OUT_CHNL(snd0)
);
	reg [0:0] rg_rdy = `NS_OFF;
	
	// out0 regs
	`NS_DECLARE_REG_MSG(rgo0)
	reg [0:0] rgo0_req = `NS_OFF;
	reg [0:0] rgo0_busy = `NS_OFF;

	always @(posedge gch_clk)
	begin
		if(gch_reset) begin
			rg_rdy <= `NS_OFF;
		end
		if(! gch_reset && ! rg_rdy) begin
			rg_rdy <= ! rg_rdy;
			
			`NS_REG_MSG_INIT(rgo0)
			rgo0_req <= `NS_OFF;
			rgo0_busy <= `NS_OFF;
		end
	end

	assign gch_ready = rg_rdy;
	
	//out1
	`NS_ASSIGN_MSG(snd0, rgo0)
	assign snd0_req_out = rgo0_req;

endmodule

