
`include "hglobal.v"

`default_nettype	none

`define NS_NUM_TEST 7
`define NS_TEST_MIN_ADDR 1
`define NS_TEST_MAX_ADDR 14
`define NS_TEST_REF_ADDR 23

`define NS_NUM_DBG_CASES 12


module test_top 
#(parameter 
	PSZ=`NS_PACKET_SIZE, 
	ASZ=`NS_ADDRESS_SIZE, 
	DSZ=`NS_DATA_SIZE, 
	RSZ=`NS_REDUN_SIZE
)(
	input  i_clk,      
	input  i_Switch_1, 
	input  i_Switch_2, 
	input  i_Switch_3, 
	input  i_Switch_4, 
	
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

	reg [0:0] reset = 0;
	wire ready;
	
	wire w_Switch_1;
	reg  r_Switch_1 = `NS_OFF;
	wire w_Switch_2;
	reg  r_Switch_2 = `NS_OFF;
	wire w_Switch_3;
	reg  r_Switch_3 = `NS_OFF;
	wire w_Switch_4;
	reg  r_Switch_4 = `NS_OFF;

	localparam TOT_DEBOUNCE_CLICK = 250000;  // 10 ms at 25 MHz
	
	`NS_DECLARE_GLB_LINK(gch0)
	assign gch0_clk = i_clk;
	assign gch0_reset = reset;
	
	`NS_DEBOUNCER_DBG_BUT(gch0, 1)
	`NS_DEBOUNCER_DBG_BUT(gch0, 2)
	`NS_DEBOUNCER_DBG_BUT(gch0, 3)
	`NS_DEBOUNCER_DBG_BUT(gch0, 4)
	
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

	localparam CLK_WDH = 17;
	localparam CLK_IDX_WDH = 2;

	reg [CLK_WDH-1:0] lim_clks_arr [6:0];  // 2, 3, 5, 7, 11, 13, 17
	reg [CLK_IDX_WDH:0] lims_idxs [3:0];

	reg lims_idxs_inited = 0;
	reg clk_lims_inited = 0;
	
	`NS_DECLARE_DBG_CLK(kl0, 0, 17'b00000000000000010) // 2
	`NS_DECLARE_DBG_CLK(kl1, 1, 2) // 3
	`NS_DECLARE_DBG_CLK(kl2, 2, 2) // 5
	`NS_DECLARE_DBG_CLK(kl3, 3, 2) // 7

	reg [3:0] changing_clks = 0;
	
	always @(posedge i_clk)
	begin
		if(changing_clks == 0) begin
			`NS_INC_DBG_CLK(kl0, i_clk)
			`NS_INC_DBG_CLK(kl1, i_clk)
			`NS_INC_DBG_CLK(kl2, i_clk)
			`NS_INC_DBG_CLK(kl3, i_clk)
		end
	end
	
	always @(posedge i_clk)
	begin
		if(! clk_lims_inited) begin
			clk_lims_inited <= 1;
			lim_clks_arr[0] <= 17'b00000000000000010; // 2
			lim_clks_arr[1] <= 17'b00000000000000100; // 3
			lim_clks_arr[2] <= 17'b00000000000010000; // 5
			lim_clks_arr[3] <= 17'b00000000001000000; // 7
			lim_clks_arr[4] <= 17'b00000010000000000; // 11
			lim_clks_arr[5] <= 17'b00001000000000000; // 13
			lim_clks_arr[6] <= 17'b10000000000000000; // 17
		end
	end
	
	`NS_DECLARE_DBG_LINK(dbg0)
	
	assign dbg0_case = 0;
	assign dbg0_doit = 0;
	
	// LNK_0
	`NS_DECLARE_PAKIO_LINK(lnk_0)
	
	// LNK_1_
	`NS_DECLARE_LINK(lnk_1)
  
	// LNK_2
	`NS_DECLARE_LINK(lnk_2)

	pakout 
	gt_01 (
		`NS_INSTA_GLB_CHNL_VALS(gch, clk_kl2, reset, ready),
		
		// out0
		`NS_INSTA_SND_PAKIO_CHNL(snd0, lnk_0),
		// in0
		`NS_INSTA_RCV_CHNL(rcv0, lnk_1)
	);

	pakout_io #(.MIN_ADDR(`NS_TEST_MIN_ADDR), .MAX_ADDR(`NS_TEST_MAX_ADDR))
	gt_02 (
		.src_clk(clk_kl0),
		.snk_clk(clk_kl1),
		.reset(reset),
		
		// SRC0
		`NS_INSTA_SND_CHNL(o0, lnk_1),
		
		// SNK0
		`NS_INSTA_RCV_PAKIO_CHNL(i0, lnk_0),

		`NS_INSTA_DBG_CHNL(dbg, dbg0, i_clk)
		
	);

	localparam TOT_TM_LIMS = 250000; 
	reg [$clog2(TOT_TM_LIMS):0] cnt_inc_lims = 0;
	always @(posedge i_clk)
	begin
		if(cnt_inc_lims == TOT_TM_LIMS) begin
			cnt_inc_lims <= 1;
			`NS_INC_DBG_IDXS_ARR(lims_idxs, 6)
			`NS_SET_LIM_DBG_CLK(kl0, lim_clks_arr, lims_idxs)
			`NS_SET_LIM_DBG_CLK(kl1, lim_clks_arr, lims_idxs)
			`NS_SET_LIM_DBG_CLK(kl2, lim_clks_arr, lims_idxs)
			`NS_SET_LIM_DBG_CLK(kl3, lim_clks_arr, lims_idxs)
		end
		else  begin
			cnt_inc_lims <= cnt_inc_lims + 1;
		end
	end

	/*
	`NS_DECLARE_REG_DBG(dd1)
	
	wire sw2_ON = ((w_Switch_2 == `NS_ON) && (r_Switch_2 == `NS_OFF));
	wire sw2_OFF = ((w_Switch_2 == `NS_OFF) && (r_Switch_2 == `NS_ON));
	always @(posedge i_clk)
	begin
		r_Switch_2 <= w_Switch_2;
		
		if(sw2_ON)
		begin
			dd1_disp0 <= dbg0_disp0;
			dd1_disp1 <= dbg0_disp1;
		end
	end

	
	bin_to_disp disp_0(
	.i_Clk(i_clk),
	.i_Binary_Num(dd1_disp0),
	.o_Segment_A(w_Segment1_A),
	.o_Segment_B(w_Segment1_B),
	.o_Segment_C(w_Segment1_C),
	.o_Segment_D(w_Segment1_D),
	.o_Segment_E(w_Segment1_E),
	.o_Segment_F(w_Segment1_F),
	.o_Segment_G(w_Segment1_G)
	);
	
	bin_to_disp disp1(
	.i_Clk(i_clk),
	.i_Binary_Num(dd1_disp1),
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
	*/

	assign o_Segment1_A = ~(lims_idxs[0][0]);
	assign o_Segment1_B = ~(lims_idxs[0][1]);
	assign o_Segment1_C = ~(lims_idxs[0][2]);
	assign o_Segment1_D = ~(lims_idxs[1][0]);
	assign o_Segment1_E = ~(lims_idxs[1][1]);
	assign o_Segment1_F = ~(lims_idxs[1][2]);
	assign o_Segment1_G = 1;
	
	assign o_Segment2_A = ~(lims_idxs[2][0]);
	assign o_Segment2_B = ~(lims_idxs[2][1]);
	assign o_Segment2_C = ~(lims_idxs[2][2]);
	assign o_Segment2_D = ~(lims_idxs[3][0]);
	assign o_Segment2_E = ~(lims_idxs[3][1]);
	assign o_Segment2_F = ~(lims_idxs[3][2]);
	assign o_Segment2_G = 1;
	
	assign o_LED_1 = dbg0_leds[0:0];
	assign o_LED_2 = dbg0_leds[1:1];
	assign o_LED_3 = dbg0_leds[2:2];
	assign o_LED_4 = dbg0_leds[3:3];

endmodule
