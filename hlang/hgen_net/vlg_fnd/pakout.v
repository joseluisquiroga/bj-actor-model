
`include "hglobal.v"


`default_nettype	none


module pakout
#(parameter 
	PSZ=`NS_PACKET_SIZE, 
	FSZ=`NS_PACKOUT_FSZ, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	`NS_DECLARE_GLB_CHNL(gch),
	`NS_DECLARE_PAKOUT_CHNL(snd0),
	`NS_DECLARE_IN_CHNL(rcv0)
);
	parameter RCV_REQ_CKS = `NS_REQ_CKS;
	parameter SND_ACK_CKS = `NS_ACK_CKS;
	
	`NS_DEBOUNCER_ACK(gch_clk, gch_reset, snd0)
	`NS_DEBOUNCER_REQ(gch_clk, gch_reset, rcv0)

	localparam TOT_PKS = ((`NS_FULL_MSG_SZ / PSZ) + 1);
	localparam FIFO_IDX_WIDTH = ((($clog2(FSZ)-1) >= 0)?($clog2(FSZ)-1):(0));
	localparam PACKETS_IDX_WIDTH = ((($clog2(TOT_PKS)-1) >= 0)?($clog2(TOT_PKS)-1):(0));
	
	reg [0:0] rg_rdy = `NS_OFF;
	
	// out0 regs
	`NS_DECLARE_REG_PAKOUT(rgo0)
	reg [0:0] rgo0_req = `NS_OFF;

	// inp0 regs
	reg [0:0] rgi0_ack = `NS_OFF;

	// fifos
	`NS_DECLARE_FIFO(bf0)
	
	reg [0:0] added_hd = `NS_OFF;


	always @(posedge gch_clk)
	begin
		if(gch_reset) begin
			rg_rdy <= `NS_OFF;
		end
		if(! gch_reset && ! rg_rdy) begin
			rg_rdy <= ! rg_rdy;
			
			`NS_PACKOUT_INIT(rgo0)
			rgo0_req <= `NS_OFF;
			
			rgi0_ack <= `NS_OFF;
			
			`NS_FIFO_INIT(bf0);
			
			added_hd <= `NS_OFF;
		end
		if(! gch_reset && rg_rdy) begin
			if(rcv0_ckd_req && (! rgi0_ack)) begin
				`NS_FIFO_TRY_ADD_HEAD(bf0, rcv0, added_hd);
			end
			`NS_FIFO_ACK_ADDED_HEAD(bf0, rgi0_ack, added_hd)
			
			if((! rcv0_ckd_req) && rgi0_ack) begin
				rgi0_ack <= `NS_OFF;
			end

			`NS_PACKOUT_TRY_INC(rgo0, bf0, snd0_ckd_ack, rgo0_req)
		end
	end

	assign gch_ready = rg_rdy && snd0_rdy && rcv0_rdy;
	
	//out1
	assign snd0_pakio = rgo0_pakio;
	assign snd0_req_out = rgo0_req;

	//inp0
	assign rcv0_ack_out = rgi0_ack;

endmodule

