`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/12 20:55:29
// Design Name: 
// Module Name: hazard_dect
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


module hazard_dect(
    input [31:0] IR,
    input [31:0] IR_2,
    output reg stall0,
    output reg stall1,
    output reg stall2
    );
    
    reg [4:0] rs1, rs2, rd;
    
        always @(*) begin
            stall0 <= 1'b0;
            stall1 <= 1'b0;
            stall2 <= 1'b0;
        
            rs1 <= IR[25:21];
            rs2 <= IR[20:16];
    
            if (IR_2[31:26] == 6'b000000) begin
                rd <= IR_2[15:11];
            end else if (IR_2[31:26] == 6'b100011) begin
                rd <= IR_2[20:16];
            end
            if((IR[31:26]!=6'b101011)|| (rs2 != rd)) begin
                if (IR_2[31:26] == 6'b100011 && ((rs1 == rd) || (rs2 == rd)) && (rd != 0)) begin
                    stall0 <= 1'b1;
                    stall1 <= 1'b1;
                    stall2 <= 1'b1;
                end
        end
        
    end
endmodule

