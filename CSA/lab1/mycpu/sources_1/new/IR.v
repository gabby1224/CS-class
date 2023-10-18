`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/06 22:14:42
// Design Name: 
// Module Name: IR
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
 

module IR(
input clk,
input rstn,
input stall1,
input [31:0]ir,
output reg [31:0]IR,
output reg [5:0]in_op,
output reg [4:0]rs,
output reg [4:0]rt,
output reg [4:0]rd,
output reg [4:0]sa,
output reg [10:0]func,
output reg [15:0]short,
output reg [25:0]long
    );
    
//        assign IR = ir;
//        assign in_op=ir[31:26];
//        assign rs=ir[25:21];
//        assign rt=ir[20:16];
//        assign rd=ir[15:11];
//        assign sa=ir[10:6];
//        assign func=ir[10:0];
//        assign short=ir[15:0];
//        assign long=ir[25:0];

    always @(posedge clk) begin
        if(stall1) begin
            
        end    
        else begin
            IR <= ir;
            in_op <= ir[31:26];
            rs <= ir[25:21];
            rt <= ir[20:16];
            rd <= ir[15:11];
            sa <= ir[10:6];
            func <= ir[10:0];
            short <= ir[15:0];
            long <= ir[25:0];
        end
    end
endmodule
