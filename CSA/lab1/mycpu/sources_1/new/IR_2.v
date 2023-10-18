`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 09:38:15
// Design Name: 
// Module Name: IR_2
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


module IR_2(
input [31:0]IR,
input clk,
input stall2,
input redir_1,redir_2,
output reg [31:0]IR_2,
output reg [4:0]rs,
output reg [4:0]rt,
output reg [4:0]rd,
output reg [4:0]sa
    );
//    assign IR_2 = IR;
//    assign rs=IR[25:21];
//    assign rt=IR[20:16];
//    assign rd=IR[15:11];
//    assign sa = IR[10:6];
    always@(posedge clk)
    begin
        if(stall2) begin
            IR_2 <= 32'h0;
            rs <= 5'h0;
            rt <= 5'h0;
            rd <= 5'h0;
            sa <= 5'h0;
        end
        else begin
            IR_2 <= IR;
            rs <= IR[25:21];
            rt <= IR[20:16];
            rd <= IR[15:11];
            sa <= IR[10:6];
        end
        
    end
endmodule
