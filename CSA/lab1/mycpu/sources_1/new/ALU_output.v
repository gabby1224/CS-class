`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/06 21:48:50
// Design Name: 
// Module Name: ALU_output
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


module ALU_output(
input clk,
input stall,
input [31:0]alu_result,
input dmem_en,we,
output reg [31:0]alu_output,
output reg dmem_en_o,we_o
    );
    always@ (posedge clk) begin
        if(stall) ;
        else begin
            alu_output <= alu_result;
            dmem_en_o <= dmem_en;
            we_o <= we;
        end
    end
    
    
endmodule
