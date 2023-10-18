`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 17:23:28
// Design Name: 
// Module Name: mux4
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


module mux4(
    input clk,
    input [31:0]ddata,
    input [31:0]alu_result,
    input mux4_s,//cu给的选择信号
    output [31:0]wdata
    );

    assign wdata=(mux4_s==1)?ddata:alu_result;

endmodule
