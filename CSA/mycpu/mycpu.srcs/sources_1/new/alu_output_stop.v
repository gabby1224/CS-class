`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/12 09:23:49
// Design Name: 
// Module Name: alu_output_stop
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


module alu_output_stop(
input clk,
input stall2,
input [31:0]alu_output,
input [31:0]alu_output2,
output reg [31:0]alu_output_t,reg [31:0]alu_output2_t
    );
    always@(posedge clk) begin
        begin
                alu_output_t <= alu_output;
                alu_output2_t <= alu_output2;
        end
    end
    
    
    
endmodule
