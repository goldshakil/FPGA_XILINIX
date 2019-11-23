`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: Dahab Shakil
// 
// Create Date:    15:05:47 01/22/2017 
// Design Name: 
// Module Name:    test_code 
// Project Name: XILINIX and sorting
// Target Devices: Spartan 6
// Tool versions: 
// Description: 
//
// Dependencies: Starup.ucf
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////



// Dahab's notes:
// 1) module is a like a building block/class which is used for port interface (connecting it with hardware. any variable not initialized here can't be attached to hardware port
//2) in verilog variables have no data type (just like python)
// 3) to write bit sequence -> follow https://cseweb.ucsd.edu/classes/sp09/cse141L/Slides/01-Verilog1.pdf 
// 4)  always@ blocks are used to describe events that should happen under certain conditions. 
// 5) posedge and negative define the rising or the falling edge of the frequency

module test_code(
//this is port list (which is connected to hardware ports using the UCF file (describes the input output termnials (I/O)ports

	input	clk_50MHz, //clock_speed_frequency: the number of pulses per second generate by an oscillator that sets the tempo of the prcoessor
	input reset,  //reset button on the FPGA
	input	[4:0] key,  // 0-4  = 5 keys
	
	//output reg [15:0] led, //route them in ucf file
	//reg: They represent data storage elements in Verilog/SystemVerilog. They retain their value till next value is assigned to them (not through assign statement)
	output reg [3:0] digit,
	output reg [7:0] fnd,
	
	output reg [4:0] keyLed //5 leds next to 5 kyes
    );
	 
	 /*-------------------------------------------------------END OF PORT LIST--------------------------------------------------*/
	 
	 //some variables
	reg [24:0]counter;
	reg [3:0] number=4'b0;
	reg [1:0] digit_sel=2'b0;
	reg [8:0] alert_num;
	reg [2:0] state;
	integer a=1;
	  reg [15:0] dat1, dat2, dat3, dat4, dat5;
	   reg  [15:0] out1, out2, out3, out4, out5;


	
	
	
	
	//Counter
	always @(posedge clk_50MHz or negedge reset)
		if(!reset)
			counter<=0;
		else
			counter<=counter+1;
/*
	//Led[]  16 ones under the display
	always @(posedge clk_50MHz or negedge reset)
	begin
	if (!reset)
		begin
		led <= 16'b0;    //size_in_bits ' baseformat representation
		end
	else
		begin
		led<= 16'b1111_1111_1111_1111;
		end
	end
	*/
	
	
	
	//Key & KeyLed
	always @(posedge clk_50MHz or negedge reset)
		begin
		if(!reset)
			begin
			keyLed <=5'b11111;
			end
		else
			begin
			keyLed  <=~key;
			end
		end
		 
		
		
	always @(posedge clk_50MHz)
  begin
      dat1 <= 7;
      dat2 <= 6;
      dat3 <= 1;
      dat4 <= 2;
      dat5 <= 0;
  end
    integer i, j;
    reg [15:0] temp;
    reg [15:0] array [1:5];
    always @*
    begin
  array[1] = dat1;
  array[2] = dat2;
  array[3] = dat3;
  array[4] = dat4;
  array[5] = dat5;
  for (i = 5; i > 0; i = i - 1) begin
  for (j = 1 ; j < i; j = j + 1) begin
          if (array[j] < array[j + 1])
          begin
            temp = array[j];
            array[j] = array[j + 1];
            array[j + 1] = temp;
  end end
  end end
    always @(posedge clk_50MHz)
    begin
      out1 <= array[1];
      out2 <= array[2];
      out3 <= array[3];
      out4 <= array[4];
      out5 <= array[5];
  end
		
		
		//<= non blocking assignment
	//Fnd number
	always @(posedge counter[10] or negedge reset)
		begin 
			if(!reset)
				begin 
					digit_sel<=0;
					digit<=4'b0000;
					number<=0;
				end
			else
				begin
					digit_sel<=digit_sel+1'b1;
					if(digit_sel==2'b11)
						digit_sel<=0;
						case(digit_sel)
						0: begin digit<=4'b1110; number<=out1; end
						1: begin digit<=4'b1101; number<=out2; end
						2: begin digit<=4'b1011; number<=out3; end
						3: begin digit<=4'b0111; number<=out4; end	
						endcase
					end
				end
	
	//number harfward initilization
	//Fnd number
	always @(*)
		case (number)
		4'h0 : fnd=8'b00000011;
		4'h1 : fnd=8'b10011111;
		4'h2 : fnd=8'b00100101;
		4'h3 : fnd=8'b00001101;
		4'h4 : fnd=8'b10011001;
		4'h5 : fnd=8'b01001001;
		4'h6 : fnd=8'b01000001;
		4'h7 : fnd=8'b00011011;
		4'h8 : fnd=8'b00000001;
		4'h9 : fnd=8'b00011001;
		4'ha : fnd=8'b10001001;
		4'hb : fnd=8'b10000011;
		4'hc : fnd=8'b01100011;
		4'hd : fnd=8'b10000101;
		4'he : fnd=8'b01100001;
		4'hf : fnd=8'b01110001;
		default : fnd=8'b00000000;
		endcase

		
	
endmodule
/*-------------------------------------------------------END OF MODULE DEFINITION-----------------------------------------------------------*/
