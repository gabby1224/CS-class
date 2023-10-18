`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/10 23:33:26
// Design Name: 
// Module Name: redir_dm_save
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


module redir_dm_save(
input clk,
input redir_dm2,
input stall2,
output reg redir_dm3,stall
    );
    always@(posedge clk)
    begin
        redir_dm3 = redir_dm2;
        stall <= stall2;
    end
    
    
endmodule
