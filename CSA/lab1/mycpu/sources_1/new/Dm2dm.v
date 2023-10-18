`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/13 00:04:27
// Design Name: 
// Module Name: Dm2dm
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


module Dm2dm(
input [31:0]IR,
input [31:0]IR_2,
output reg redir_dm_2,redir_dm_1
    );
    reg [4:0]rs;
    reg [4:0]rd;
    always@(*)
    begin
        redir_dm_1 <= 0;
        redir_dm_2 <= 0;
        if(IR_2[31:26]==6'b100011 && IR[31:26]==6'b101011) begin
            rs <= IR[20:16];
            rd <= IR_2[20:16];
            if(rs == rd)
            begin
                redir_dm_2 <= 1;
            end
        end
    
    end
    
    
    
endmodule
