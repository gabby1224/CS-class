`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 09:42:06
// Design Name: 
// Module Name: IR_4
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


module IR_4(
input [31:0]IR_3,
input clk,
input stall,
output reg [31:0]IR_4,
output reg [5:0]in_op,
output reg [4:0]rd,
output reg [4:0]rt
    );
//    assign IR_4 = IR_3;
//    assign in_op=IR_3[31:26];
//    assign rd=IR_3[15:11];
//    assign rt=IR_3[20:16];
    always@(posedge clk) begin
        if(stall) ;
        else begin
            IR_4 <= IR_3;
            in_op <= IR_3[31:26];
            rd <= IR_3[15:11];
            rt <= IR_3[20:16];
        end
    end
endmodule
