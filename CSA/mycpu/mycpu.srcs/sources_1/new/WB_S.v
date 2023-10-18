`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/09 21:46:53
// Design Name: 
// Module Name: WB_S
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


module WB_S(
input clk,
input we_o,dmem_en_o,
input [31:0]alu_output,
output reg we_2,dmem_en_2,
output reg [31:0]alu_output2
    );
    always@(posedge clk)
    begin
        we_2 <= we_o;
        dmem_en_2 <= dmem_en_o;
        alu_output2 <= alu_output;
    end
    
    
    
endmodule
