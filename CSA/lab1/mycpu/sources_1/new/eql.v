`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 17:30:52
// Design Name: 
// Module Name: eql
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


module eql(
    input [31:0]rdata1,
    input [31:0]rdata2,
    output neql
    );

        assign neql=(rdata1==rdata2)?0:1;

endmodule
