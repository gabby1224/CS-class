`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 17:06:01
// Design Name: 
// Module Name: mux2
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


module mux2(
    input clk,
    input mux2_s,
    input[31:0]rdata2,
    input[31:0]ex_out,//À©Õ¹Êý¾Ý
    input [31:0]alu_output,
    input [31:0]alu_output2,
    input [31:0]data_rdata,
    input redirmux2_1,redirmux2_2,
    output[31:0]data2
    );

    assign data2=(mux2_s==1)?ex_out:rdata2;
//    wire A,B,C,D,en;
//        assign A = (mux2_s==1);
//        assign B = (mux2_s==0);
//        assign en = (redirmux2_1 | redirmux2_2);
//        assign C = (redirmux2_1==1);
//        assign D = (redirmux2_2==1);
////        assign data2 = {32{A}} & ex_out | {32{B}}& rdata2 | {32{C}} & alu_output | {32{D}}&alu_output2;
//        assign data2 = (en==1)? ({32{C}} & alu_output | {32{D}}&alu_output2) : ({32{A}} & ex_out | {32{B}}& rdata2);

endmodule
