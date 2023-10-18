`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 17:06:16
// Design Name: 
// Module Name: mux3
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


module mux3(
    input [31:0]alu_result,
    input [31:0]PC_add,
    input clk,
    input mux3_s,//cu指令 选择写回的信号种类
    output reg [31:0]npc_in
    );
    always@(negedge clk)
    begin
        if(mux3_s==1)npc_in = alu_result;
        else npc_in <= PC_add;
    end

endmodule
