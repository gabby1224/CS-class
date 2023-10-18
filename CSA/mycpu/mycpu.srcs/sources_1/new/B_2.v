`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/09 23:14:49
// Design Name: 
// Module Name: B_2
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


module B_2(
input clk,
input [31:0]data2,
output reg [31:0]data2_o
    );
    always@(posedge clk)
    begin
        data2_o <= data2;
    end
    
    
endmodule
