
`include "hglobal.v"

`default_nettype	none

`define NS_NUM_TEST 12

`define NS_SEL_HALFBYTE(ii) ii*4 +: 4

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
	output o_LED_4,
	
	output io_PMOD_1,
	output io_PMOD_2,
	output io_PMOD_3,
	output io_PMOD_4,
	
	output io_PMOD_7,
	output io_PMOD_8,
	output io_PMOD_9,
	output io_PMOD_10
	
	);

	wire w_Switch_1;
	reg  r_Switch_1 = `NS_OFF;
	wire w_Switch_2;
	reg  r_Switch_2 = `NS_OFF;
	wire w_Switch_3;
	reg  r_Switch_3 = `NS_OFF;
	wire w_Switch_4;
	reg  r_Switch_4 = `NS_OFF;

	localparam TOT_DEBOUNCE_CLICK = 250000;  // 10 ms at 25 MHz

	reg [0:0] reset = 0;
	wire ready;
	
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

	localparam TOT_BITS_CNTR = 64;

	reg [0:0] mul_clock = 0;
	reg [TOT_BITS_CNTR - 1:0] tik_count = 0;
	reg [TOT_BITS_CNTR - 1:0] lim_tiks_up = 64'h00_00_00_00_00_00_00_FF;
	reg [TOT_BITS_CNTR - 1:0] lim_tiks_down = 64'h00_00_00_00_00_00_00_FF;
	
	always @(posedge i_clk)
	begin
		if(mul_clock) begin
			if(tik_count == lim_tiks_up) begin
				tik_count <= 0;
				mul_clock <= 0;
			end
			else  begin
				tik_count <= tik_count + 1;
			end 
		end 
		else 
		begin
			if(tik_count == lim_tiks_down) begin
				tik_count <= 0;
				mul_clock <= 1;
			end
			else  begin
				tik_count <= tik_count + 1;
			end 
		end
	end
	
	wire sw1_ON = ((w_Switch_1 == `NS_ON) && (r_Switch_1 == `NS_OFF));
	wire sw1_OFF = ((w_Switch_1 == `NS_OFF) && (r_Switch_1 == `NS_ON));
	wire sw2_ON = ((w_Switch_2 == `NS_ON) && (r_Switch_2 == `NS_OFF));
	wire sw2_OFF = ((w_Switch_2 == `NS_OFF) && (r_Switch_2 == `NS_ON));
	wire sw3_ON = ((w_Switch_3 == `NS_ON) && (r_Switch_3 == `NS_OFF));
	wire sw3_OFF = ((w_Switch_3 == `NS_OFF) && (r_Switch_3 == `NS_ON));
	wire sw4_ON = ((w_Switch_4 == `NS_ON) && (r_Switch_4 == `NS_OFF));
	wire sw4_OFF = ((w_Switch_4 == `NS_OFF) && (r_Switch_4 == `NS_ON));
	
	reg [0:0] changing = 0;
	reg [0:0] cicles_up = 0;
	
	reg [3:0] up_byte_to_disp = 0;
	reg [3:0] down_byte_to_disp = 0;

	wire [3:0] byte_to_disp = (cicles_up)?(up_byte_to_disp):(down_byte_to_disp);
	wire [3:0] byte_val_to_disp = (cicles_up)?(lim_tiks_up[`NS_SEL_HALFBYTE(byte_to_disp)]):(lim_tiks_down[`NS_SEL_HALFBYTE(byte_to_disp)]);
	
	always @(posedge i_clk)
	begin
		r_Switch_1 <= w_Switch_1;
		r_Switch_2 <= w_Switch_2;
		r_Switch_3 <= w_Switch_3;
		r_Switch_4 <= w_Switch_4;
		
		// cnt_clk <= (cnt_clk << 1);
		// lim_tiks_up <= lim_tiks_up + 1;
		
		if(sw1_ON)
		begin
			if(! changing) begin
				if(cicles_up) begin
					up_byte_to_disp <= up_byte_to_disp + 1;
				end
				else
				begin
					down_byte_to_disp <= down_byte_to_disp + 1;
				end
			end
			else
			begin
				if(cicles_up) begin
					lim_tiks_up[`NS_SEL_HALFBYTE(up_byte_to_disp)] <= lim_tiks_up[`NS_SEL_HALFBYTE(up_byte_to_disp)] + 1;
				end
				else
				begin
					lim_tiks_down[`NS_SEL_HALFBYTE(down_byte_to_disp)] <= lim_tiks_down[`NS_SEL_HALFBYTE(down_byte_to_disp)] + 1;
				end
			end
		end
		else 
		if(sw2_ON) begin
			if(! changing) begin
				if(cicles_up) begin
					up_byte_to_disp <= up_byte_to_disp - 1;
				end
				else
				begin
					down_byte_to_disp <= down_byte_to_disp - 1;
				end
			end
			else
			begin
				if(cicles_up) begin
					lim_tiks_up[`NS_SEL_HALFBYTE(up_byte_to_disp)] <= lim_tiks_up[`NS_SEL_HALFBYTE(up_byte_to_disp)] - 1;
				end
				else
				begin
					lim_tiks_down[`NS_SEL_HALFBYTE(down_byte_to_disp)] <= lim_tiks_down[`NS_SEL_HALFBYTE(down_byte_to_disp)] - 1;
				end
			end
		end
		else if(sw3_ON) begin
			`ns_bit_toggle(cicles_up);
		end
		else if(sw4_ON) begin
			`ns_bit_toggle(changing);
		end
	end

	bin_to_disp disp_0(
	.i_Clk(i_clk),
	.i_Binary_Num(byte_to_disp),
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
	.i_Binary_Num(byte_val_to_disp),
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
	
	assign o_LED_1 = cicles_up;	 // showing (#tiks up) VS showing (#tiks down)
	assign o_LED_2 = changing;	 // changing current 'byte_val_to_disp' VS changing 'byte_to_disp'
	assign o_LED_3 = 0;
	assign o_LED_4 = 0;

	assign io_PMOD_1 = mul_clock;
	assign io_PMOD_2 = mul_clock;
	assign io_PMOD_3 = mul_clock;
	assign io_PMOD_4 = mul_clock;
	
	assign io_PMOD_7 = 0;
	assign io_PMOD_8 = 0;
	assign io_PMOD_9 = 0;
	assign io_PMOD_10 = 0;
	
	
endmodule


/*

PMOD pins looking from the front:

V3.2 GND p4  p3 p2 p1
V3.2 GND p10 p9 p8 p7

*/
