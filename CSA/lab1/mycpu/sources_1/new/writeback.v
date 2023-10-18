`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 21:24:25
// Design Name: 
// Module Name: writeback
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


module writeback(
    input [4:0]rt,
    input [4:0]rd,
    output[4:0]waddr,
    input [5:0]in_op
    );
assign waddr=(in_op==6'b100011)?rt:rd;
endmodule
