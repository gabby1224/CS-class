`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/10 14:44:00
// Design Name: 
// Module Name: PC_for_test
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
module PC_for_test(
input clk,
input stall1,
input rstn,
input [31:0]pc_in,
output reg [31:0]pc_in_2
    );
    always@(posedge clk)
    begin
        if(rstn)
        if(stall1) ;
        else begin
            pc_in_2 <= pc_in;
        end
    end
    
    
    
endmodule
