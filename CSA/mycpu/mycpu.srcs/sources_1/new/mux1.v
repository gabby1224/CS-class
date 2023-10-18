`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 17:05:25
// Design Name: 
// Module Name: mux1
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


module mux1(
    input clk,
    input mux1_s,
    input [31:0]rdata1,
    input [31:0]npc_out,
    output [31:0]data1
    );

        assign data1 = (mux1_s==1)?npc_out:rdata1;
 
endmodule
