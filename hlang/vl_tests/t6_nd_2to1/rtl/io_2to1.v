
`include "hglobal.v"

`default_nettype	none

`define NS_DBG_NXT_ADDR(adr)  ((adr >= MAX_ADDR)?(MIN_ADDR):(adr + 1))


module io_2to1
#(parameter 
	MIN_ADDR=1, 
	MAX_ADDR=1, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	input wire src0_clk,
	input wire src1_clk,
	input wire snk0_clk,
	input wire reset,
	
	// SRC_0
	`NS_DECLARE_OUT_CHNL(o0),
	
	// SRC_1
	`NS_DECLARE_OUT_CHNL(o1),
	
	// SNK_0
	`NS_DECLARE_IN_CHNL(i0),
	
	`NS_DECLARE_DBG_CHNL(dbg)
);

	parameter RCV_REQ_CKS = `NS_REQ_CKS;
	parameter SND_ACK_CKS = `NS_ACK_CKS;
	
	`NS_DEBOUNCER_ACK(src0_clk, reset, o0)
	`NS_DEBOUNCER_ACK(src1_clk, reset, o1)
	`NS_DEBOUNCER_REQ(snk0_clk, reset, i0)
 
	`NS_DECLARE_REG_DBG(rg_dbg)

	reg [3:0] cnt_0 = 0;
	reg [3:0] cnt_1 = 0;

	// SRC regs
	reg [0:0] ro0_has_dst = `NS_OFF;	
	reg [0:0] ro0_has_dat = `NS_OFF;	
	reg [0:0] ro0_has_red = `NS_OFF;	
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
	
	
	// SRC regs
	reg [0:0] ro1_has_dst = `NS_OFF;	
	reg [0:0] ro1_has_dat = `NS_OFF;	
	reg [0:0] ro1_has_red = `NS_OFF;	
	reg [0:0] ro1_busy = `NS_OFF;	
	reg [ASZ-1:0] ro1_src = 1;
	reg [ASZ-1:0] ro1_dst = MIN_ADDR;
	reg [DSZ-1:0] ro1_dat = 0;
	reg [RSZ-1:0] ro1_red = 0;
	reg [0:0] ro1_req = `NS_OFF;
	reg [0:0] ro1_err = `NS_OFF;
	wire [RSZ-1:0] ro1_redun;

	calc_redun #(.ASZ(ASZ), .DSZ(DSZ), .RSZ(RSZ)) 
		ro1_c_red (ro1_src, ro1_dst, ro1_dat, ro1_redun);
	
	
	// SNK_0 regs
	reg [0:0] has_inp0 = `NS_OFF;
	reg [0:0] inp0_has_redun = `NS_OFF;
	reg [0:0] inp0_done_cks = `NS_OFF;
	`NS_DECLARE_REG_MSG(inp0)
	wire [RSZ-1:0] inp0_calc_redun;
	reg [RSZ-1:0] inp0_redun = 0;
	calc_redun #(.ASZ(ASZ), .DSZ(DSZ), .RSZ(RSZ)) 
		md_calc_red0 (inp0_src, inp0_dst, inp0_dat, inp0_calc_redun);
	
	reg [0:0] inp0_ack = `NS_OFF;
	reg [0:0] inp0_err = `NS_OFF;
	
	reg [DSZ-1:0] r_0_ck_dat = 15;
	reg [DSZ-1:0] r_1_ck_dat = 15;
	reg [0:0] r_0_err = `NS_OFF;
	reg [0:0] r_1_err = `NS_OFF;
	reg [0:0] r_2_err = `NS_OFF;
	
	//SRC_0
	always @(posedge src0_clk)
	begin
		if((! ro0_req) && (! o0_ckd_ack)) begin
			if(! ro0_has_dst) begin
				ro0_has_dst <= `NS_ON;
				ro0_dst <= `NS_DBG_NXT_ADDR(ro0_dst);
			end
			else
			if(! ro0_has_dat) begin
				ro0_has_dat <= `NS_ON;

				ro0_dat[3:0] <= cnt_0;
				cnt_0 <= cnt_0 + 1;
			end
			else
			if(! ro0_has_red) begin
				ro0_has_red <= `NS_ON;
				ro0_red <= ro0_redun;
			end
			if(ro0_has_red) begin
				ro0_req <= `NS_ON;
			end
		end
		if(ro0_req && o0_ckd_ack) begin
			ro0_has_dst <= `NS_OFF;
			ro0_has_dat <= `NS_OFF;
			ro0_has_red <= `NS_OFF;
			ro0_req <= `NS_OFF;
		end
	end
		
	//SRC_1
	always @(posedge src1_clk)
	begin
		if((! ro1_req) && (! o1_ckd_ack)) begin
			if(! ro1_has_dst) begin
				ro1_has_dst <= `NS_ON;
				ro1_dst <= `NS_DBG_NXT_ADDR(ro1_dst);
			end
			else
			if(! ro1_has_dat) begin
				ro1_has_dat <= `NS_ON;

				ro1_dat[3:0] <= cnt_1;
				cnt_1 <= cnt_1 + 1;
			end
			else
			if(! ro1_has_red) begin
				ro1_has_red <= `NS_ON;
				ro1_red <= ro1_redun;
			end
			if(ro1_has_red) begin
				ro1_req <= `NS_ON;
			end
		end
		if(ro1_req && o1_ckd_ack) begin
			ro1_has_dst <= `NS_OFF;
			ro1_has_dat <= `NS_OFF;
			ro1_has_red <= `NS_OFF;
			ro1_req <= `NS_OFF;
		end
	end
	
	//SNK_0
	always @(posedge snk0_clk)
	begin
		if(i0_ckd_req && (! inp0_ack)) begin
			if(! has_inp0) begin
				has_inp0 <= `NS_ON;
				`NS_MOV_REG_MSG(inp0, i0)
			end
			else
			if(! inp0_has_redun) begin
				inp0_has_redun <= `NS_ON;
				inp0_redun <= inp0_calc_redun;
			end
			else
			if(! inp0_done_cks) begin
				inp0_done_cks <= `NS_ON;
				if(! inp0_err) begin
					if(! r_0_err && (inp0_src == 0)) begin
						if(inp0_red != inp0_redun) begin
							inp0_err <= `NS_ON;
							r_0_err <= `NS_ON;
						end
						else
						if((r_0_ck_dat <= 14) && ((r_0_ck_dat + 1) != inp0_dat)) begin
							r_0_err <= `NS_ON;
						end else begin 
							r_0_ck_dat <= inp0_dat;
						end
					end
					if(! r_1_err && (inp0_src == 1)) begin
						if(inp0_red != inp0_redun) begin
							inp0_err <= `NS_ON;
							r_1_err <= `NS_ON;
						end
						else
						if((r_1_ck_dat <= 14) && ((r_1_ck_dat + 1) != inp0_dat)) begin
							r_1_err <= `NS_ON;
						end else begin 
							r_1_ck_dat <= inp0_dat;
						end
					end
				end
			end
			if(inp0_done_cks) begin
				rg_dbg_disp0 <= inp0_dat[3:0];
				
				has_inp0 <= `NS_OFF;
				inp0_has_redun <= `NS_OFF;
				inp0_done_cks <= `NS_OFF;
				
				inp0_ack <= `NS_ON;
			end
		end
		else
		if((! i0_ckd_req) && inp0_ack) begin
			inp0_ack <= `NS_OFF;
		end
	end
	
	//SRC_0
	`NS_ASSIGN_MSG(o0, ro0)
	assign o0_req_out = ro0_req;

	//SRC_1
	`NS_ASSIGN_MSG(o1, ro1)
	assign o1_req_out = ro1_req;

	//SNK_0
	assign i0_ack_out = inp0_ack;
	
	
	assign dbg_leds[0:0] = r_0_err;
	assign dbg_leds[1:1] = r_1_err;
	assign dbg_leds[2:2] = (ro0_err || ro1_err || r_2_err);
	assign dbg_leds[3:3] = 0;
	assign dbg_disp0 = rg_dbg_disp0;
	assign dbg_disp1 = rg_dbg_disp1;
	
	
endmodule
