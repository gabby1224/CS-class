`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/09 08:48:35
// Design Name: 
// Module Name: signal_4
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


module signal_4(
input clk,
input stall,
input mux1_s_3,mux2_s_3,mux3_s_3,mux4_s_3,
output reg mux1_s_4,mux2_s_4,mux3_s_4,mux4_s_4
    );
    always@(posedge clk)
    begin
        if(stall);
        else begin
            mux1_s_4 <= mux1_s_3;
            mux2_s_4 <= mux2_s_3;
            mux3_s_4 <= mux3_s_3;
            mux4_s_4 <= mux4_s_3;
        end
    end
    
endmodule
