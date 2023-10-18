`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 08:32:25
// Design Name: 
// Module Name: A
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


module A(
input [31:0]rdata1,
input clk,
input stall2,
output reg [31:0]A
    );
    always@(posedge clk)
    begin
        if(stall2) A <= {32{0}};
        else begin
            A <= rdata1;
        end
        
    end
endmodule
