/************************************************************
HBE-EMP2CYC Address Decoder
*************************************************************/

// Network Packet Transmit/Receiver
module AddrDecoder (
		// EMPOS II Connector <-> Cyclone
		// Input Data pin
		CX_A,							
		nPX_CS5,					
		CX_D,						
		
		//  Ignore data pin
		nPX_PWE,
		nRESET,	
		
		// EMPOS II FPGA MODULE Application

		// 16 x 2 Text LCD Port
		LCD_RS,
		LCD_RW,
		LCD_E,	
		LCD_DATA,

		// 6-Digit 7-Segment Port
		SEG_DATA,
		SEG_COM	,

		// 4 x 4 Push Switch Port
		PUSH_SCAN,
		PUSH_DATA,

		// 8-Bits LED Port
		LED	,				

		//  Dip switch Port
		DIP_sw_a,	

		// 7 x 5 Dot Matrix Port
		DOT_ADDRESS,
		DOT_DATA,

		// Step Motor Port
		A, 
		B ,
		Abar,		
		Bbar,	
		
		// USB Chip Select
		USB_CS,
		
		// External User Port to Conn.		
		
		// Fixed Data
		nRAM_CS,				
		nRAM_OE,				
		nRAM_WE,			
		// ETC
		nCX_WE			
);
	input [23:20] CX_A; 
	input nPX_CS5;
	inout [15:0] CX_D; 

	input nPX_PWE; 
	input nRESET; 	

	output reg LCD_RS;
	output reg LCD_RW;
	output reg LCD_E; 
	output reg [7:0] LCD_DATA;

	output reg [7:0] SEG_DATA;
	output reg [5:0] SEG_COM;

	output reg [3:0] PUSH_SCAN;
	input [3:0] PUSH_DATA;

	output reg [7:0] LED;

	input [7:0] DIP_sw_a;

	output reg [9:0] DOT_ADDRESS;
	output reg [6:0] DOT_DATA;

	output reg A; 
	output reg B;
	output reg Abar;
	output reg Bbar;

	output USB_CS; 

	output nRAM_CS;
	output nRAM_OE;
	output nRAM_WE;

	output nCX_WE; 

	/*********************************
	Regiter
	*********************************/	
	wire [15:0] CS;

	/*********************************
	Parameter
	*********************************/	
	parameter  USB			= 16'hFFFE;	
	parameter  SRAM_1		= 16'hFFFD;	
	parameter  SRAM_2		= 16'hFFFB;	
	parameter  SEG			= 16'hFFF7;	
	parameter  DIPSW		= 16'hFFEF;	
	parameter  KeyPada		= 16'hFFDF;	
	parameter  KeyPadb		= 16'hFFBF;	
	parameter  LED_T		= 16'hFF7F;	
	parameter  LCD			= 16'hFEFF;	
	parameter  DOT_D		= 16'hFDFF;
	parameter  DOT_C		= 16'hFBFF;
	parameter  STEP_MOTOR	= 16'hF7FF;
	parameter  USER_CS1		= 16'hEFFF;	
	parameter  USER_CS2		= 16'hDFFF;	
	parameter  USER_CS3		= 16'hBFFF;	
	parameter  USER_CS4		= 16'h7FFF;	

	/*********************************
	Assign
	*********************************/	
	assign		 nRAM_CS	= 1'b0;
	assign		 nRAM_OE	= 1'b0;	
	assign		 nRAM_WE	= 1'b0;
	assign		 nCX_WE		= nPX_PWE;
	assign		 USB_CS     = CS[0];

	assign		CS = ( (CX_A[23:20] == 4'b0000) && ( nPX_CS5 == 1'b0 ) )? USB:
 					 ( (CX_A[23:20] == 4'b0001) && ( nPX_CS5 == 1'b0 ) )? SRAM_1:
 					 ( (CX_A[23:20] == 4'b0010) && ( nPX_CS5 == 1'b0 ) )? SRAM_2:
					 ( (CX_A[23:20] == 4'b0011) && ( nPX_CS5 == 1'b0 ) )? SEG:
 					 ( (CX_A[23:20] == 4'b0100) && ( nPX_CS5 == 1'b0 ) )? DIPSW:
 					 ( (CX_A[23:20] == 4'b0101) && ( nPX_CS5 == 1'b0 ) )? KeyPada:
 					 ( (CX_A[23:20] == 4'b0110) && ( nPX_CS5 == 1'b0 ) )? KeyPadb:
 					 ( (CX_A[23:20] == 4'b0111) && ( nPX_CS5 == 1'b0 ) )? LED_T:
 					 ( (CX_A[23:20] == 4'b1000) && ( nPX_CS5 == 1'b0 ) )? LCD:
 					 ( (CX_A[23:20] == 4'b1001) && ( nPX_CS5 == 1'b0 ) )? DOT_D:
 					 ( (CX_A[23:20] == 4'b1010) && ( nPX_CS5 == 1'b0 ) )? DOT_C:
 					 ( (CX_A[23:20] == 4'b1011) && ( nPX_CS5 == 1'b0 ) )? STEP_MOTOR:
 					 ( (CX_A[23:20] == 4'b1100) && ( nPX_CS5 == 1'b0 ) )? USER_CS1:
 					 ( (CX_A[23:20] == 4'b1101) && ( nPX_CS5 == 1'b0 ) )? USER_CS2:
 					 ( (CX_A[23:20] == 4'b1110) && ( nPX_CS5 == 1'b0 ) )? USER_CS3:
 					 ( (CX_A[23:20] == 4'b1111) && ( nPX_CS5 == 1'b0 ) )? USER_CS4:
					 16'hFFFF;

	always @( posedge nPX_PWE )	
	begin
		if(nPX_PWE == 1'b1)
		begin
			case ( CS )
				USB:
				begin
				end
				SRAM_1:
				begin
				end
				SRAM_2:
				begin
				end
				SEG:
				begin
					SEG_DATA[7:0] <= CX_D[7:0];
					SEG_COM[5:0]  <= CX_D[13:8];
				end
				DIPSW:
				begin
				end

				KeyPada:
				begin
					PUSH_SCAN[3:0] <= CX_D[3:0];
				end

				KeyPadb:
				begin
				end

				LED_T:
				begin
					LED <= CX_D[7:0];
				end

				LCD:
				begin
					//
					LCD_E	 <= CX_D[10];
					LCD_RW	 <= CX_D[9];
					LCD_RS	 <= CX_D[8];
					LCD_DATA <= CX_D[7:0];
				end

				DOT_D:
				begin
					DOT_DATA <= {CX_D[0], CX_D[1], CX_D[2],CX_D[3],CX_D[4],CX_D[5],CX_D[6]};
				end
				DOT_C:
				begin
					DOT_ADDRESS <= CX_D[9:0];
				end
				
				STEP_MOTOR:
				begin
					A		<= CX_D[3];
					B		<= CX_D[2];
					Abar	<= CX_D[1];
					Bbar	<= CX_D[0];
				end
				USER_CS1:
				begin
				end
				USER_CS2:
				begin
				end
				USER_CS3:
				begin
				end
				USER_CS4:
				begin
				end
				default : 
				begin
				end
			endcase
		end
	end

	assign CX_D = (CS == SRAM_1)?16'hz:
				  (CS == SRAM_2)?16'hz:
				  (CS == USB)?16'hz:
				  (CS == DIPSW)? {8'h00, DIP_sw_a}:
				  (CS == KeyPadb)?{12'h000, !PUSH_DATA}:
				  (CS == USER_CS1)?16'hz:
				  (CS == USER_CS2)?16'hz:
				  (CS == USER_CS3)?16'hz:
				  (CS == USER_CS4)?16'hz:
				  16'hz;

endmodule