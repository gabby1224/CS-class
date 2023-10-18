`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/10 14:50:10
// Design Name: 
// Module Name: PC_for_test3
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


//负责把pc的值带到4个周期之后
module PC_for_test3(
input clk,
input rstn,
input [31:0]pc_in_3,
output reg [31:0]pc_in_4
    );
    always@(posedge clk)
    begin
    if(rstn)
        pc_in_4 <= pc_in_3;
    end
    
    
    
endmodule

