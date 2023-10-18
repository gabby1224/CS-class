`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/10 23:21:30
// Design Name: 
// Module Name: mux_wdata
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


module mux_wdata(
input redir_dm,
input [31:0]data_rdata,
input [31:0]data2_o,

output [31:0]t_wdata
    );
    assign t_wdata = (redir_dm==1)?data_rdata:data2_o;   
    
endmodule
