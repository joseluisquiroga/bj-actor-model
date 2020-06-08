
`include "hglobal.v"

`default_nettype	none

module hprb_source
#(parameter 
	MY_LOCAL_ADDR=0, 
	PRB_DST_ADDR=0, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	`NS_DECLARE_GLB_CHNL(gch),
	`NS_DECLARE_OUT_CHNL(snd0)
);

	parameter SND_ACK_CKS = `NS_ACK_CKS;
	
	`NS_DEBOUNCER_ACK(gch_clk, gch_reset, snd0)

	reg [0:0] rg_rdy = `NS_OFF;

	reg [DSZ-1:0] cnt_0 = 0;

	// SRC regs
	reg [0:0] ro0_has_dst = `NS_OFF;	
	reg [0:0] ro0_has_dat = `NS_OFF;	
	reg [0:0] ro0_has_red = `NS_OFF;	
	reg [0:0] ro0_busy = `NS_OFF;	
	reg [ASZ-1:0] ro0_src = MY_LOCAL_ADDR;
	reg [DSZ-1:0] ro0_dat = 0;
	reg [ASZ-1:0] ro0_dst = PRB_DST_ADDR;
	reg [RSZ-1:0] ro0_red = 0;
	reg [0:0] ro0_req = `NS_OFF;
	reg [0:0] ro0_err = `NS_OFF;
	
	wire [RSZ-1:0] ro0_redun;

	calc_redun #(.ASZ(ASZ), .DSZ(DSZ), .RSZ(RSZ)) 
		r1 (ro0_src, ro0_dst, ro0_dat, ro0_redun);
	
	//SRC
	always @(posedge gch_clk)
	begin
		if(gch_reset) begin
			rg_rdy <= `NS_OFF;
		end
		if(! gch_reset && ! rg_rdy) begin
			rg_rdy <= ! rg_rdy;
			
			cnt_0 <= 0;
			
			ro0_has_dst <= `NS_OFF;	
			ro0_has_dat <= `NS_OFF;	
			ro0_has_red <= `NS_OFF;	
			ro0_busy <= `NS_OFF;	
			ro0_src <= MY_LOCAL_ADDR;
			ro0_dat <= 0;
			ro0_dst <= PRB_DST_ADDR;
			ro0_red <= 0;
			ro0_req <= `NS_OFF;
			ro0_err <= `NS_OFF;
	
		end
		if(! gch_reset && rg_rdy) begin
			if((! ro0_req) && (! snd0_ckd_ack)) begin
				if(! ro0_has_dst) begin
					ro0_has_dst <= `NS_ON;
					ro0_dst <= PRB_DST_ADDR;
				end
				else
				if(! ro0_has_dat) begin
					ro0_has_dat <= `NS_ON;

					ro0_dat <= cnt_0;
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
			if(ro0_req && snd0_ckd_ack) begin
				ro0_has_dst <= `NS_OFF;
				ro0_has_dat <= `NS_OFF;
				ro0_has_red <= `NS_OFF;
				ro0_req <= `NS_OFF;
			end
		end
	end
		
	assign gch_ready = rg_rdy && snd0_rdy;
	
	//SRC
	`NS_ASSIGN_MSG(snd0, ro0)
	assign snd0_req_out = ro0_req;

endmodule
