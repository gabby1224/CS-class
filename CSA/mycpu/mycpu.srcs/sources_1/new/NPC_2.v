`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 10:02:54
// Design Name: 
// Module Name: NPC_2
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


module NPC_2(
input [31:0]npc_o,
input clk,
input stall2,
output reg [31:0]npc_2_o
    );
    always@(posedge clk)
    begin
        if(stall2) npc_2_o <= 32'h0;
        else begin
            npc_2_o <= npc_o;
        end
    end
endmodule
