`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 10:09:49
// Design Name: 
// Module Name: LMD
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


module LMD(
input [31:0]rdata,
input clk,
input stall,
output reg [31:0]rdata2
    );
//    assign rdata2 = rdata;
    always@(posedge clk)begin
        if (stall);
        else begin
            rdata2 <= rdata;
        end
    end
endmodule
