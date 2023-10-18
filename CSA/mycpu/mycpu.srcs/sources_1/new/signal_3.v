`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/09 08:45:11
// Design Name: 
// Module Name: signal_3
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


module signal_3(
input clk,
input stall,
input mux1_s_2,mux2_s_2,mux3_s_2,mux4_s_2,
output reg mux1_s_3,mux2_s_3,mux3_s_3,mux4_s_3
    );
    always@(posedge clk)
    begin
    if(stall);
    else begin
        mux1_s_3 <= mux1_s_2;
        mux2_s_3 <= mux2_s_2;
        mux3_s_3 <= mux3_s_2;
        mux4_s_3 <= mux4_s_2;
    end
    end
endmodule
