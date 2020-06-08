
`include "hglobal.v"

`default_nettype	none

`define NS_NUM_TEST 4'b1010
`define NS_TEST_MIN_ADDR 0
`define NS_TEST_MAX_ADDR 55
`define NS_TEST_REF_ADDR 23

module test_top 
#(parameter ASZ=`NS_ADDRESS_SIZE, DSZ=`NS_DATA_SIZE, RSZ=`NS_REDUN_SIZE)
(
	input  i_clk,      // Main Clock (25 MHz)
	input  i_Switch_1, 
	input  i_Switch_2, 
	
	output o_Segment1_A,
	output o_Segment1_B,
	output o_Segment1_C,
	output o_Segment1_D,
	output o_Segment1_E,
	output o_Segment1_F,
	output o_Segment1_G,
	
	output o_Segment2_A,
	output o_Segment2_B,
	output o_Segment2_C,
	output o_Segment2_D,
	output o_Segment2_E,
	output o_Segment2_F,
	output o_Segment2_G,
	output o_LED_1,
	output o_LED_2,
	output o_LED_3,
	output o_LED_4
	);

	reg [0:0] the_reset = 0;
	wire the_all_ready;
	
	wire w_Switch_1;
	reg  r_Switch_1 = `NS_OFF;

	reg [2:0] cnt_clk0 = 0;
	reg [3:0] cnt_clk1 = 0;
	reg [5:0] cnt_clk2 = 0;
	reg [7:0] cnt_clk3 = 0;
	reg [11:0] cnt_clk4 = 0;
	
	reg clk_0 = `NS_OFF;
	reg clk_1 = `NS_OFF;
	reg clk_2 = `NS_OFF;
	reg clk_3 = `NS_OFF;
	reg clk_4 = `NS_OFF;

	`NS_DECLARE_DBG_LINK(dbg0)
	`NS_DECLARE_DBG_LINK(dbg1)
	
	reg [DSZ-1:0] disp_i_data = `NS_NUM_TEST;
	reg [DSZ-1:0] disp_o_data = `NS_NUM_TEST;
	
	//reg r_LED_1 = `NS_OFF;
	reg r_LED_2 = `NS_OFF;
	reg r_LED_3 = `NS_OFF;
	reg r_LED_4 = `NS_OFF;

	// LNK_0
	`NS_DECLARE_LINK(lnk_0)
	wire [DSZ-1:0] lnk_0_ck_dat;
	
	// LNK_1_
	`NS_DECLARE_LINK(lnk_1)
	wire [DSZ-1:0] lnk_1_ck_dat;
  
	wire w_Segment1_A;
	wire w_Segment1_B;
	wire w_Segment1_C;
	wire w_Segment1_D;
	wire w_Segment1_E;
	wire w_Segment1_F;
	wire w_Segment1_G;
	
	wire w_Segment2_A;
	wire w_Segment2_B;
	wire w_Segment2_C;
	wire w_Segment2_D;
	wire w_Segment2_E;
	wire w_Segment2_F;
	wire w_Segment2_G;

	always @(posedge i_clk)
	begin
		// clk_0
		if(cnt_clk0 == 0) begin
			cnt_clk0 <= 1;
			`ns_bit_toggle(clk_0);
		end
		else  begin
			cnt_clk0 <= (cnt_clk0 << 1);
		end
		// clk_1
		if(cnt_clk1 == 0) begin
			cnt_clk1 <= 1;
			`ns_bit_toggle(clk_1);
		end
		else  begin
			cnt_clk1 <= (cnt_clk1 << 1);
		end
		// clk_2
		if(cnt_clk2 == 0) begin
			cnt_clk2 <= 1;
			`ns_bit_toggle(clk_2);
		end
		else  begin
			cnt_clk2 <= (cnt_clk2 << 1);
		end
		// clk_3
		if(cnt_clk3 == 0) begin
			cnt_clk3 <= 1;
			`ns_bit_toggle(clk_3);
		end
		else  begin
			cnt_clk3 <= (cnt_clk3 << 1);
		end
		// clk_4
		if(cnt_clk4 == 0) begin
			cnt_clk4 <= 1;
			`ns_bit_toggle(clk_4);
		end
		else  begin
			cnt_clk4 <= (cnt_clk4 << 1);
		end
	end
	
	nd_fifo
	gtfifo (
		`NS_INSTA_GLB_CHNL_VALS(gch, clk_4, the_reset, the_all_ready),
		
		//.i_clk(i_clk),
		// out0
		`NS_INSTA_SND_CHNL(snd0, lnk_0),
		// in0
		`NS_INSTA_RCV_CHNL(rcv0, lnk_1)
	);

	io_fifo #(.MIN_ADDR(`NS_TEST_MIN_ADDR), .MAX_ADDR(`NS_TEST_MAX_ADDR))
	io_t0 (
		.src_clk(clk_2),
		.snk_clk(clk_2),
		.reset(the_reset),
		//i_clk, clk_0, clk_1
		
		// SRC0
		`NS_INSTA_SND_CHNL(o0, lnk_1),
		// SNK0
		`NS_INSTA_RCV_CHNL(i0, lnk_0),

		`NS_INSTA_DBG_CHNL(dbg, dbg0, i_clk)
	);
	
	bin_to_disp disp_1(
	.i_Clk(i_clk),
	.i_Binary_Num((dbg0_leds != 0)?(dbg0_disp0):(`NS_NUM_TEST)),
	.o_Segment_A(w_Segment1_A),
	.o_Segment_B(w_Segment1_B),
	.o_Segment_C(w_Segment1_C),
	.o_Segment_D(w_Segment1_D),
	.o_Segment_E(w_Segment1_E),
	.o_Segment_F(w_Segment1_F),
	.o_Segment_G(w_Segment1_G)
	);
	
	// Instantiate Binary to 7-Segment Converter
	bin_to_disp disp2(
	.i_Clk(i_clk),
	.i_Binary_Num((dbg0_leds != 0)?(dbg0_disp1):(`NS_NUM_TEST)),
	.o_Segment_A(w_Segment2_A),
	.o_Segment_B(w_Segment2_B),
	.o_Segment_C(w_Segment2_C),
	.o_Segment_D(w_Segment2_D),
	.o_Segment_E(w_Segment2_E),
	.o_Segment_F(w_Segment2_F),
	.o_Segment_G(w_Segment2_G)
	);

	assign o_Segment1_A = ~w_Segment1_A;
	assign o_Segment1_B = ~w_Segment1_B;
	assign o_Segment1_C = ~w_Segment1_C;
	assign o_Segment1_D = ~w_Segment1_D;
	assign o_Segment1_E = ~w_Segment1_E;
	assign o_Segment1_F = ~w_Segment1_F;
	assign o_Segment1_G = ~w_Segment1_G;
	
	assign o_Segment2_A = ~w_Segment2_A;
	assign o_Segment2_B = ~w_Segment2_B;
	assign o_Segment2_C = ~w_Segment2_C;
	assign o_Segment2_D = ~w_Segment2_D;
	assign o_Segment2_E = ~w_Segment2_E;
	assign o_Segment2_F = ~w_Segment2_F;
	assign o_Segment2_G = ~w_Segment2_G;

	assign o_LED_1 = dbg0_leds[0:0];
	assign o_LED_2 = dbg0_leds[1:1];
	assign o_LED_3 = dbg0_leds[2:2];
	assign o_LED_4 = dbg0_leds[3:3];

endmodule
