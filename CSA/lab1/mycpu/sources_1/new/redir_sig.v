`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/10 23:28:30
// Design Name: 
// Module Name: redir_sig
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


module redir_sig(
input clk,stall2,
input redir_dm,
output reg redir_dm2
    );
    always@(posedge clk)
    begin
        if(stall2==1) redir_dm2 <= 0;
        else begin
            redir_dm2 = redir_dm;
        end
    end
    
    
endmodule
