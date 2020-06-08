
`include "hglobal.v"

`default_nettype	none

module formal_top ();
 
	wire cn_clk;
	wire cn_req;
	wire cn_ack;
	wire [`ADDRESS_SIZE-1:0] cn_addr;
	wire [`DATA_SIZE-1:0] cn_data;
	wire cn_err;
	
	wire [`DATA_SIZE-1:0] cn_o_data;

	cellnet_source cn_src_1 (
		.i_clk(cn_clk),
		.o_addr(cn_addr),
		.o_dat(cn_data),
		.o_req(cn_req),
		.i_ack(cn_ack)
	);

	cellnet_sink cn_snk_1 (
		.i_clk(cn_clk),
		.i_addr(cn_addr),
		.i_dat(cn_data),
		.i_req(cn_req),
		.o_ack(cn_ack),
		.o_dat(cn_o_data),
		.o_err(cn_err)
	);

	`ifdef	FORMAL
		initial assert(cn_req == `OFF);
		initial assert(cn_ack == `OFF);
		initial assert(cn_err == `OFF);
		initial assert(cn_data == 0);
		initial assert(cn_o_data == 0);
		
		reg	f_past_valid;
		initial	f_past_valid = `OFF;
		always @(posedge cn_clk) begin
			f_past_valid <= `ON;
		end
		
		always @(posedge cn_clk) begin
		
			if((! cn_req) && (! cn_ack)) begin
				assert(cn_data == cn_o_data);
			end
				
			if(cn_req && (! cn_ack)) begin
			end
			
			if(cn_req && cn_ack) begin
			end
			
			if((! cn_req) && cn_ack) begin
			end
			
			if(f_past_valid) begin
			
				if($past(cn_req) && cn_req) begin
					assert(cn_data == $past(cn_data));
					assert(cn_addr == $past(cn_addr));
				end
			
				if((! $past(cn_req)) && cn_req) begin
					assert(cn_o_data == $past(cn_data));
				end
				
				if((! $past(cn_ack)) && cn_ack) begin
					assert(cn_o_data == $past(cn_data));
				end
			end
			
		end
		
	`endif
	
     
endmodule
