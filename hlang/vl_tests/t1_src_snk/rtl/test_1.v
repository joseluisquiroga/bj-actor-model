
`include "hglobal.v"

`default_nettype	none

`define NS_NUM_TEST 4'b0001
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

	wire w_Switch_1;
	reg  r_Switch_1 = `NS_OFF;

	localparam TOT_DEBOUNCE_CLICK = 250000;  // 10 ms at 25 MHz
	
	reg [0:0] reset = 0;
	
	`NS_DECLARE_GLB_LINK(gch0)
	assign gch0_clk = i_clk;
	assign gch0_reset = reset;
	
	`NS_DEBOUNCER_DBG_BUT(gch0, 1)
	
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
	
	//reg r_LED_1 = `NS_OFF;
	reg r_LED_2 = `NS_OFF;
	reg r_LED_3 = `NS_OFF;
	reg r_LED_4 = `NS_OFF;

	// LNK_0
	`NS_DECLARE_LINK(lnk_0)
	wire [DSZ-1:0] lnk_0_ck_dat;
	
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
	
	wire ready0;
	hprb_source #(.MY_LOCAL_ADDR(0), .PRB_DST_ADDR(1))
	it_src (
		`NS_INSTA_GLB_CHNL_VALS(gch, clk_0, reset, ready0),
		`NS_INSTA_SND_CHNL(snd0, lnk_0)
	);

	`NS_DECLARE_ERR_LINK(gerr1) 
	
	wire ready1;
	hprb_sink #(.MY_LOCAL_ADDR(1), .PRB_SRC_ADDR(0))
	io_dst (
		`NS_INSTA_GLB_CHNL_VALS(gch, clk_0, reset, ready1),
		`NS_INSTA_RCV_CHNL(rcv0, lnk_0),
		`NS_INSTA_ERR_CHNL(err0, gerr1)
	);
	
	wire the_all_ready = ready0 && ready1;

	wire sw1_ON = ((w_Switch_1 == `NS_ON) && (r_Switch_1 == `NS_OFF));
	wire sw1_OFF = ((w_Switch_1 == `NS_OFF) && (r_Switch_1 == `NS_ON));
	
	reg [3:0] rg_disp0 = 0;
	
	always @(posedge i_clk)
	begin
		r_Switch_1 <= w_Switch_1;
		
		if(sw1_ON)
		begin
			rg_disp0 <= lnk_0_dat[2:0];
		end
	end

	
	bin_to_disp disp_1(
	.i_Clk(i_clk),
	.i_Binary_Num(`NS_NUM_TEST),
	//.i_Binary_Num((gerr1_error != 0)?(dbg0_disp0):(`NS_NUM_TEST)),
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
	.i_Binary_Num(rg_disp0),
	//.i_Binary_Num((gerr1_error != 0)?(dbg0_disp1):(`NS_NUM_TEST)),
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

	assign o_LED_1 = gerr1_error;
	assign o_LED_2 = 0;
	assign o_LED_3 = 0;
	assign o_LED_4 = 0;

endmodule
