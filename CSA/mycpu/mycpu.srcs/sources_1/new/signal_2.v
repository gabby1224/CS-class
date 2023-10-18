`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/08 23:37:38
// Design Name: 
// Module Name: signal_2
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


module signal_2(
    input clk,
    input mux1_s,mux2_s,mux3_s,mux4_s,
    input [5:0]alu_op,
    input stall2,
    output reg mux1_s_2,mux2_s_2,mux3_s_2,mux4_s_2, 
    output reg [5:0]alu_op_2

    );
    always@(posedge clk)
    begin
    if(stall2) alu_op_2<= 6'h0;
    else begin
        mux1_s_2 <= mux1_s;
        mux2_s_2 <= mux2_s;
        mux3_s_2 <= mux3_s;
        mux4_s_2 <= mux4_s;
        alu_op_2 <= alu_op;
    end
    end
    
    
endmodule
