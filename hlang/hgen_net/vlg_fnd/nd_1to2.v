
`include "hglobal.v"

`default_nettype	none

module nd_1to2
#(parameter 
	OPER_1=`NS_GT_OP, 
	REF_VAL_1=0, 
	IS_RANGE=`NS_FALSE, 
	OPER_2=`NS_GT_OP, 
	REF_VAL_2=0, 
	
	FSZ=`NS_1to2_FSZ, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	`NS_DECLARE_GLB_CHNL(gch),
	`NS_DECLARE_OUT_CHNL(snd0),
	`NS_DECLARE_OUT_CHNL(snd1),
	`NS_DECLARE_IN_CHNL(rcv0)
	
);

	parameter RCV_REQ_CKS = `NS_REQ_CKS;
	parameter SND_ACK_CKS = `NS_ACK_CKS;
	
	`NS_DEBOUNCER_ACK(gch_clk, gch_reset, snd0)
	`NS_DEBOUNCER_ACK(gch_clk, gch_reset, snd1)
	`NS_DEBOUNCER_REQ(gch_clk, gch_reset, rcv0)
	
	localparam FIFO_IDX_WIDTH = ((($clog2(FSZ)-1) >= 0)?($clog2(FSZ)-1):(0));
 
	reg [0:0] rg_rdy = `NS_OFF;
	
	// out1 regs
	`NS_DECLARE_REG_MSG(rgo0)
	reg [0:0] rgo0_req = `NS_OFF;
	reg [0:0] rgo0_busy = `NS_OFF;
	reg [0:0] rgo0_added_hd = `NS_OFF;

	// out2 regs
	`NS_DECLARE_REG_MSG(rgo1)
	reg [0:0] rgo1_req = `NS_OFF;
	reg [0:0] rgo1_busy = `NS_OFF;
	reg [0:0] rgo1_added_hd = `NS_OFF;

	// inp regs
	reg [0:0] rgi0_ack = `NS_OFF;

	// fifos
	`NS_DECLARE_FIFO(bf0)
	`NS_DECLARE_FIFO(bf1)

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
			rgo0_added_hd <= `NS_OFF;
			
			`NS_REG_MSG_INIT(rgo1)
			rgo1_req <= `NS_OFF;
			rgo1_busy <= `NS_OFF;
			rgo1_added_hd <= `NS_OFF;
	
			rgi0_ack <= `NS_OFF;
			
			`NS_FIFO_INIT(bf0)
			`NS_FIFO_INIT(bf1)
		end
		if(! gch_reset && rg_rdy) begin
			if(rcv0_ckd_req && (! rgi0_ack)) begin
				if(`NS_RANGE_CMP_OP(IS_RANGE, OPER_1, REF_VAL_1, rcv0_dst, OPER_2, REF_VAL_2, rcv0_dst)) begin
					`NS_FIFO_TRY_ADD_HEAD(bf0, rcv0, rgo0_added_hd);
				end else begin
					`NS_FIFO_TRY_ADD_HEAD(bf1, rcv0, rgo1_added_hd);
				end
			end
			`NS_FIFO_ACK_ADDED_HEAD(bf0, rgi0_ack, rgo0_added_hd)
			`NS_FIFO_ACK_ADDED_HEAD(bf1, rgi0_ack, rgo1_added_hd)
			
			`NS_FIFO_TRY_SET_OUT(bf0, rgo0, snd0_ckd_ack, rgo0_req, rgo0_busy);
			`NS_FIFO_TRY_SET_OUT(bf1, rgo1, snd1_ckd_ack, rgo1_req, rgo1_busy);
			
			if((! rcv0_ckd_req) && rgi0_ack) begin
				rgi0_ack <= `NS_OFF;
			end
		end
	end

	assign gch_ready = rg_rdy && snd0_rdy && snd1_rdy && rcv0_rdy;
	
	//out1
	`NS_ASSIGN_MSG(snd0, rgo0)
	assign snd0_req_out = rgo0_req;

	//out2
	`NS_ASSIGN_MSG(snd1, rgo1)
	assign snd1_req_out = rgo1_req;

	//inp0
	assign rcv0_ack_out = rgi0_ack;
	
endmodule

