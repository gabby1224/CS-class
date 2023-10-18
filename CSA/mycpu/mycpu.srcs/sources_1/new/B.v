`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 08:37:54
// Design Name: 
// Module Name: B
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


module B(
input [31:0]rdata2,
input clk,
input stall2,
output reg [31:0]B

    );
    always@(posedge clk)
    begin
        if(stall2) begin
        B <= {32{0}};
        end
        else begin
            B <= rdata2;
        end        
    end
endmodule
