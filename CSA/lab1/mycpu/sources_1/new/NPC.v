`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 14:47:24
// Design Name: 
// Module Name: NPC
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

//NPC存多一个节拍的PC
module NPC(
    input clk,
    input rstn,
    input stall1,
    input [31:0]npc_in,
    output reg [31:0]npc_out
    );

//    assign npc_out=npc_in;
always @(posedge clk)begin
    if(!rstn) npc_out = {32{0}};
    else if(stall1==1)
    begin
    end
    else begin
        npc_out <= npc_in;    
    end
end


endmodule
