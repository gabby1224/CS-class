`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/10 15:02:37
// Design Name: 
// Module Name: PC_for_test4
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


module PC_for_test4(
input clk,
input rstn,
input [31:0]pc_in_4,
output reg [31:0]pc_in_5
    );
    always@(posedge clk)
    begin
    if(rstn)
        pc_in_5 <= pc_in_4;
    end
    
    
    
endmodule