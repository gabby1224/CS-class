`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 15:28:18
// Design Name: 
// Module Name: EXTEND
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


module EXTEND(
    input clk,
    input rstn,
    input [15:0]short,
    input [25:0]long,
    input [5:0]op,
    output reg [31:0]ex_out
    );
always@(short,long,op)begin
        case(op)
            6'b000010:  ex_out<={{6{long[25]}},long[25:0]};
            default:   ex_out<={{16{short[15]}},short[15:0]};
        endcase
    
end
endmodule
