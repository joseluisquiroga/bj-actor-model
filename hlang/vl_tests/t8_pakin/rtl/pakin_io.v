
`include "hglobal.v"

`default_nettype	none

`define NS_DBG_NXT_ADDR(adr)  ((adr >= MAX_ADDR)?(MIN_ADDR):(adr + 1))

`define NS_DBG_SRC_ADDR 3

`define NS_DBG_INIT_CK 14

`define NS_DBG_INIT_DAT 5

`define NS_DBG_INIT_RED 15

`define NS_DBG_MAX_SRC_CASE 4

module pakin_io
#(parameter 
	MIN_ADDR=1, 
	MAX_ADDR=1, 
	PSZ=`NS_PACKET_SIZE, 
	FSZ=`NS_PACKIN_FSZ, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	input wire src_clk,
	input wire snk_clk,
	input wire reset,
	
	// SRC_0
	`NS_DECLARE_PAKOUT_CHNL(o0),
	// SNK_0
	`NS_DECLARE_IN_CHNL(i0),

	`NS_DECLARE_DBG_CHNL(dbg)
);
	parameter RCV_REQ_CKS = `NS_REQ_CKS;
	parameter SND_ACK_CKS = `NS_ACK_CKS;
	
	`NS_DEBOUNCER_ACK(src_clk, reset, o0)
	`NS_DEBOUNCER_REQ(snk_clk, reset, i0)

	localparam TOT_PKS = ((`NS_FULL_MSG_SZ / PSZ) + 1);
	localparam FIFO_IDX_WIDTH = ((($clog2(FSZ)-1) >= 0)?($clog2(FSZ)-1):(0));
	localparam PACKETS_IDX_WIDTH = ((($clog2(TOT_PKS)-1) >= 0)?($clog2(TOT_PKS)-1):(0));
 
	reg [0:0] rg_rdy = `NS_OFF;
	
	reg [3:0] cnt_0 = `NS_DBG_INIT_DAT;

	// SRC regs
	reg [0:0] ro0_has_dst = `NS_OFF;	
	reg [0:0] ro0_has_dat = `NS_OFF;	
	reg [0:0] ro0_has_red = `NS_OFF;	
	reg [ASZ-1:0] ro0_src = `NS_DBG_SRC_ADDR;
	reg [ASZ-1:0] ro0_dst = MIN_ADDR;
	reg [DSZ-1:0] ro0_dat = `NS_DBG_INIT_DAT;
	reg [RSZ-1:0] ro0_red = `NS_DBG_INIT_RED;
	//reg [0:0] ro0_req = `NS_OFF;

	wire [RSZ-1:0] ro0_redun;
	calc_redun #(.ASZ(ASZ), .DSZ(DSZ), .RSZ(RSZ)) 
		r1 (ro0_src, ro0_dst, ro0_dat, ro0_redun);
	
	
	`NS_DECLARE_FIFO(bf0)
	reg [0:0] added_hd = `NS_OFF;
	
	`NS_DECLARE_REG_PAKOUT(rgo0)
	reg [0:0] rgo0_req = `NS_OFF;

	// SNK
	reg [0:0] has_inp0 = `NS_OFF;
	reg [0:0] inp0_has_redun = `NS_OFF;
	reg [0:0] inp0_done_cks = `NS_OFF;
	`NS_DECLARE_REG_MSG(inp0)
	wire [RSZ-1:0] inp0_calc_redun;
	reg [RSZ-1:0] inp0_redun = 0;
	calc_redun #(.ASZ(ASZ), .DSZ(DSZ), .RSZ(RSZ)) 
		md_calc_red0 (inp0_src, inp0_dst, inp0_dat, inp0_calc_redun);
	
	
	reg [0:0] inp0_ack = `NS_OFF;
	reg [DSZ-1:0] inp0_back_dat = 15;
	//reg [ASZ-1:0] inp0_back_dst = 0;
	reg [0:0] inp0_err = `NS_OFF;
	
	reg [0:0] inp0_err_0 = `NS_OFF;
	reg [0:0] inp0_err_1 = `NS_OFF;
	reg [0:0] inp0_err_2 = `NS_OFF;
	reg [0:0] inp0_err_3 = `NS_OFF;
	
	
	`NS_DECLARE_REG_DBG(rg_dbg)

	//SRC_0
	always @(posedge src_clk)
	begin
		if(reset) begin
			rg_rdy <= `NS_OFF;
		end
		if(! reset && ! rg_rdy) begin
			rg_rdy <= `NS_ON;
			
			`NS_PACKOUT_INIT(rgo0)
			rgo0_req <= `NS_OFF;
			
			`NS_FIFO_INIT(bf0);
			
			added_hd <= `NS_OFF;
		end
		if(! reset && rg_rdy) begin
			
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
				`NS_FIFO_TRY_ADD_HEAD(bf0, ro0, added_hd);
			end
			if(added_hd) begin
				ro0_has_dst <= `NS_OFF;
				ro0_has_dat <= `NS_OFF;
				ro0_has_red <= `NS_OFF;
				added_hd <= `NS_OFF;
			end
			`NS_PACKOUT_TRY_INC(rgo0, bf0, o0_ckd_ack, rgo0_req)
		end
	end
		
	//SNK_0
	always @(posedge snk_clk)
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
					if(inp0_src != `NS_DBG_SRC_ADDR) begin
						inp0_err <= `NS_ON;
						inp0_err_0 <= `NS_ON;
					end
					else
					if(inp0_red != inp0_redun) begin
						inp0_err <= `NS_ON;
						inp0_err_1 <= `NS_ON;
					end
					//else
					if((inp0_back_dat <= 14) && ((inp0_back_dat + 1) != inp0_dat)) begin
						inp0_err <= `NS_ON;
						inp0_err_2 <= `NS_ON;
						//rg_dbg_disp0 <= inp0_back_dat[3:0];
						//rg_dbg_disp1 <= inp0_dat[3:0];
					end 
					else 
					begin 
						inp0_back_dat <= inp0_dat;
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
	assign o0_pakio = rgo0_pakio;
	assign o0_req_out = rgo0_req;

	//SNK_0
	assign i0_ack_out = inp0_ack;

	assign dbg_leds[0:0] = inp0_err_0;
	assign dbg_leds[1:1] = inp0_err_1;
	assign dbg_leds[2:2] = inp0_err_2;
	assign dbg_leds[3:3] = inp0_err;
	assign dbg_disp0 = rg_dbg_disp0;
	assign dbg_disp1 = rg_dbg_disp1;
	
endmodule


