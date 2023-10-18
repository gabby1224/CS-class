`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 14:59:25
// Design Name: 
// Module Name: IMEM
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module inst_mem(
    input [31:0]pc_out,
    input clk,
    output  [31:0]ir
    );
reg [31:0]inst[255:0];

	initial begin
		$readmemh("*\\additional_inst_data1", inst);

	end


    assign ir = {inst[pc_out/4]};    
 
endmodule
