`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 09:28:21
// Design Name: 
// Module Name: IMM
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


module IMM(
input [31:0]ex_out,
input clk,
input stall2,
output reg [31:0]imm
    );
    always@(posedge clk)
    begin
        if(stall2) imm <= 32'h0;
        else begin
            imm <= ex_out;
        end
        
    end
endmodule
