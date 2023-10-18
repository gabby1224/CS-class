`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 09:40:46
// Design Name: 
// Module Name: IR_3
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


module IR_3(
input [31:0]IR_2,
input clk,
input stall,
output reg [31:0]IR_3
    );
    always@(posedge clk)
    begin
    if(stall);
    else begin
        IR_3 <= IR_2;
    end
    end
endmodule
