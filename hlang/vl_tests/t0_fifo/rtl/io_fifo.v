
`include "hglobal.v"

`default_nettype	none

`define NS_DBG_NXT_ADDR(adr)  ((adr >= MAX_ADDR)?(MIN_ADDR):(adr + 1))


module io_fifo
#(parameter 
	MIN_ADDR=1, 
	MAX_ADDR=1, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	input wire src_clk,
	input wire snk_clk,
	input wire reset,
	
	// SRC_0
	`NS_DECLARE_OUT_CHNL(o0),
	
	// SNK_0
	`NS_DECLARE_IN_CHNL(i0),

	`NS_DECLARE_DBG_CHNL(dbg)
	
);
	parameter RCV_REQ_CKS = `NS_REQ_CKS;
	parameter SND_ACK_CKS = `NS_ACK_CKS;
	
	`NS_DEBOUNCER_ACK(src_clk, reset, o0)
	`NS_DEBOUNCER_REQ(snk_clk, reset, i0)

	`NS_DECLARE_REG_DBG(rg_dbg)
 
	reg [3:0] cnt_0 = 0;
	reg [3:0] cnt_1 = 0;

	// SRC regs
	reg [0:0] ro0_busy = `NS_OFF;	
	reg [ASZ-1:0] ro0_src = 0;
	reg [ASZ-1:0] ro0_dst = MIN_ADDR;
	reg [DSZ-1:0] ro0_dat = 0;
	reg [RSZ-1:0] ro0_red = 0;
	reg [0:0] ro0_req = `NS_OFF;
	reg [0:0] ro0_err = `NS_OFF;
	wire [RSZ-1:0] ro0_redun;
	
	calc_redun #(.ASZ(ASZ), .DSZ(DSZ), .RSZ(RSZ)) 
		ro0_c_red (ro0_src, ro0_dst, ro0_dat, ro0_redun);
	
	
	// SNK_0 regs
	reg [0:0] ri0_cks_done = `NS_OFF;
	reg [0:0] ri0_ack = `NS_OFF;
	reg [DSZ-1:0] ri0_ck_dat = 0;
	
	// CHECK regs
	reg [DSZ-1:0] r_0_ck_dat = 15;
	reg [DSZ-1:0] r_1_ck_dat = 15;
	
	wire [RSZ-1:0] i0_redun;
	calc_redun #(.ASZ(ASZ), .DSZ(DSZ), .RSZ(RSZ)) 
		ri0_c_red (i0_src, i0_dst, i0_dat, i0_redun);
	
	//SRC_0
	always @(posedge src_clk)
	begin
		if(! ro0_busy) begin
			ro0_busy <= `NS_ON;

			if(ro0_dat > 15) begin
				ro0_err <= `NS_ON;
			end
			/*if(ro0_dat < 0) begin
				ro0_err <= `NS_ON;
			end*/
			ro0_dat[3:0] <= cnt_0;
			cnt_0 <= cnt_0 + 1;
		end
		if(ro0_busy) begin
			if((! ro0_req) && (! o0_ckd_ack)) begin
				ro0_red <= ro0_redun;
				ro0_req <= `NS_ON;
			end 
			if(ro0_req && o0_ckd_ack) begin
				ro0_dst <= `NS_DBG_NXT_ADDR(ro0_dst);
				
				ro0_busy <= `NS_OFF;
				ro0_req <= `NS_OFF;
			end
		end
	end
		
	//SNK_0
	always @(posedge snk_clk)
	begin
		//rg_dbg_leds[3:3] <= 1;
		if(! ri0_cks_done && i0_ckd_req && (! ri0_ack)) begin
			if(! rg_dbg_leds[2:2]) begin
				if(i0_dat > 15) begin
					rg_dbg_leds[2:2] <= `NS_ON;
				end
				/*if(i0_dat < 0) begin
					rg_dbg_leds[2:2] <= `NS_ON;
				end*/
			end
			if(! rg_dbg_leds[0:0] && (i0_src == 0)) begin
				if(i0_red != i0_redun) begin
					rg_dbg_leds[0:0] <= `NS_ON;
				end
				else
				if((r_0_ck_dat <= 14) && ((r_0_ck_dat + 1) != i0_dat)) begin
					rg_dbg_leds[0:0] <= `NS_ON;
					rg_dbg_disp0 <= i0_dat[3:0];
					rg_dbg_disp1 <= r_0_ck_dat[3:0];
				end else begin 
					r_0_ck_dat <= i0_dat;
				end
			end
			if(! rg_dbg_leds[1:1] && (i0_src == 1)) begin
				if(i0_red != i0_redun) begin
					rg_dbg_leds[1:1] <= `NS_ON;
				end
				else
				if((r_1_ck_dat <= 14) && ((r_1_ck_dat + 1) != i0_dat)) begin
					rg_dbg_leds[1:1] <= `NS_ON;
					rg_dbg_disp0 <= i0_dat[3:0];
					rg_dbg_disp1 <= r_1_ck_dat[3:0];
				end else begin 
					r_1_ck_dat <= i0_dat;
				end
			end
			ri0_cks_done <= `NS_ON;
			ri0_ck_dat <= i0_dat;
		end
		if(ri0_cks_done && i0_ckd_req && (! ri0_ack)) begin
			ri0_cks_done <= `NS_OFF;
			ri0_ack <= `NS_ON;
		end
		if((! i0_ckd_req) && ri0_ack) begin
			ri0_ack <= `NS_OFF;
		end
	end
	
	//SRC_0
	`NS_ASSIGN_MSG(o0, ro0)
	assign o0_req_out = ro0_req;

	//SNK_0
	assign i0_ack_out = ri0_ack;

	`NS_ASSIGN_OUT_DBG(dbg, rg_dbg)
	
endmodule
