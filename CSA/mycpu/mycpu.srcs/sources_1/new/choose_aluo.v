`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/12 14:04:56
// Design Name: 
// Module Name: choose_aluo
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


module choose_aluo(
input stall2,
input [31:0]alu_output, [31:0]alu_output2,
input [31:0]alu_output_t,[31:0]alu_output2_t,
output [31:0]c_alu_output,[31:0]c_alu_output2
    );
    
    assign c_alu_output = (stall2==1)? alu_output_t : alu_output;
    assign c_alu_output2 = (stall2==1)?alu_output2_t : alu_output2;
       
endmodule
