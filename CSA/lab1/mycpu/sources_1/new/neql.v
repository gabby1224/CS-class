`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/08 22:57:02
// Design Name: 
// Module Name: neql
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


module neql(
input clk,
input neql,
output reg neql_o
    );
    always@(posedge clk)
    begin
        neql_o <= neql;
    end
endmodule
