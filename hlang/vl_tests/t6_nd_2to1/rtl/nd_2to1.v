
`include "hglobal.v"

`default_nettype	none


module nd_2to1
#(parameter 
	FSZ=`NS_2to1_FSZ, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE)
(
	`NS_DECLARE_GLB_CHNL(gch),
	`NS_DECLARE_OUT_CHNL(snd0),
	`NS_DECLARE_IN_CHNL(rcv0),
	`NS_DECLARE_IN_CHNL(rcv1)
	
);
	parameter RCV_REQ_CKS = `NS_REQ_CKS;
	parameter SND_ACK_CKS = `NS_ACK_CKS;
	
	`NS_DEBOUNCER_ACK(gch_clk, gch_reset, snd0)
	`NS_DEBOUNCER_REQ(gch_clk, gch_reset, rcv0)
	`NS_DEBOUNCER_REQ(gch_clk, gch_reset, rcv1)
	
	localparam FIFO_IDX_WIDTH = ((($clog2(FSZ)-1) >= 0)?($clog2(FSZ)-1):(0));
 
	reg [0:0] rg_rdy = `NS_OFF;
	
	// out0 regs
	`NS_DECLARE_REG_MSG(rgo0)
	reg [0:0] rgo0_req = `NS_OFF;
	reg [0:0] rgo0_busy = `NS_OFF;

	// inp0 regs
	reg [0:0] rgi0_ack = `NS_OFF;
	reg [0:0] rgi0_added_hd = `NS_OFF;

	// inp1 regs
	reg [0:0] rgi1_ack = `NS_OFF;
	reg [0:0] rgi1_added_hd = `NS_OFF;

	// fifos
	`NS_DECLARE_FIFO(bf0)
	
	reg [0:0] choose_0 = `NS_FALSE;
	
	wire in0_rq = (rcv0_ckd_req && (! rgi0_ack));
	wire in1_rq = (rcv1_ckd_req && (! rgi1_ack));

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
			
			rgi0_ack <= `NS_OFF;
			rgi0_added_hd <= `NS_OFF;
			
			rgi1_ack <= `NS_OFF;
			rgi1_added_hd <= `NS_OFF;
			
			`NS_FIFO_INIT(bf0);
			
			choose_0 <= `NS_TRUE;
		end
		if(! gch_reset && rg_rdy) begin
			if(in0_rq && ((! in1_rq) || (in1_rq && choose_0))) begin
				`NS_FIFO_TRY_ADD_HEAD(bf0, rcv0, rgi0_added_hd);
				if(in1_rq) begin
					choose_0 <= `NS_FALSE;
				end
			end
			`NS_FIFO_ACK_ADDED_HEAD(bf0, rgi0_ack, rgi0_added_hd)
			
			if(in1_rq && ((! in0_rq) || (in0_rq && (! choose_0)))) begin
				`NS_FIFO_TRY_ADD_HEAD(bf0, rcv1, rgi1_added_hd);
				if(in0_rq) begin
					choose_0 <= `NS_TRUE;
				end
			end
			`NS_FIFO_ACK_ADDED_HEAD(bf0, rgi1_ack, rgi1_added_hd)

			`NS_FIFO_TRY_SET_OUT(bf0, rgo0, snd0_ckd_ack, rgo0_req, rgo0_busy);
			
			if((! rcv0_ckd_req) && rgi0_ack) begin
				rgi0_ack <= `NS_OFF;
			end
			if((! rcv1_ckd_req) && rgi1_ack) begin
				rgi1_ack <= `NS_OFF;
			end
		end
	end

	assign gch_ready = rg_rdy && snd0_rdy && rcv0_rdy && rcv1_rdy;
	
	//out1
	`NS_ASSIGN_MSG(snd0, rgo0)
	assign snd0_req_out = rgo0_req;

	//inp0
	assign rcv0_ack_out = rgi0_ack;
	
	//inp1
	assign rcv1_ack_out = rgi1_ack;
	
endmodule

