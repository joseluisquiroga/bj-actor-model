
`include "hglobal.v"

`default_nettype	none


module nd_fifo
#(parameter 
	FSZ=1, // 1, 2 or 4
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE)
(
	input wire i_clk,
	input wire reset,
	output wire ready,
	
	`NS_DECLARE_OUT_CHNL(snd0)
	`NS_DECLARE_IN_CHNL(rcv0)
	
);
 
	reg [0:0] rg_rdy = `NS_OFF;
	
	// out0 regs
	`NS_DECLARE_REG_MSG(rgo0)
	reg [0:0] rgo0_req = `NS_OFF;
	reg [0:0] rgo0_busy = `NS_OFF;

	// inp0 regs
	reg [0:0] rgi0_ack = `NS_OFF;

	// fifos
	`NS_DECLARE_FIFO(bf0)
	
	always @(posedge i_clk)
	begin
		if(reset) begin
			rg_rdy <= `NS_OFF;
		end
		if(! reset && ! rg_rdy) begin
			rg_rdy <= `NS_ON;
			
			`NS_REG_MSG_INIT(rgo0)
			rgo0_req <= `NS_OFF;
			
			rgi0_ack <= `NS_OFF;
			
			`NS_FIFO_INIT(bf0);
		end
		if(! reset && rg_rdy) begin
			if(rcv0_req && (! rgi0_ack)) begin
				`NS_FIFO_TRY_INC_HEAD(bf0, rcv0, rgi0_ack);
			end
			
			`NS_FIFO_TRY_SET_OUT(bf0, rgo0, snd0_ack, rgo0_req, rgo0_busy);
			
			if((! rcv0_req) && rgi0_ack) begin
				rgi0_ack <= `NS_OFF;
			end
		end
	end

	assign ready = rg_rdy;
	
	//out1
	`NS_ASSIGN_OUT_MSG(snd0, rgo0)
	assign snd0_req = rgo0_req;

	//inp0
	assign rcv0_ack = rgi0_ack;
	
endmodule

