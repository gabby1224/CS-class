`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 11:03:22
// Design Name: 
// Module Name: mux_1_new
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


module mux_1_new(
input [31:0]alu_output,rdata1,npc_out,
output [31:0]data1,
input mux1_s,mux1_redir,
input clk
    );
//    wire [31:0]result1;
//    wire [31:0]result2;
//    wire [31:0]result3;
//    wire [31:0]fei;
    
//    assign result1 = npc_out & {32{signal[0]}};
//    assign result2 = rdata1 & ~{32{signal[0]}};
//    assign result3 = {32{signal[1]}} & alu_output;
//    assign fei = ~{32{signal[0]}};
    
    assign data1 = ~{32{mux1_redir}} & ((npc_out & {32{mux1_s}})|(rdata1 & ~{32{mux1_s}})) | {32{mux1_redir}} & alu_output;
    
    
endmodule
