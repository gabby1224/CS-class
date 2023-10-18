`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/10 14:49:12
// Design Name: 
// Module Name: PC_for_test2
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
module PC_for_test2(
input clk,
input rstn,
input stall2,
input [31:0]pc_in_2,
output reg [31:0]pc_in_3
    );
    always@(posedge clk)
    begin
        if(rstn)
        if(stall2) pc_in_3 <= 32'h0;
        else begin
            pc_in_3 <= pc_in_2;
        end
    end
    
    
    
endmodule
