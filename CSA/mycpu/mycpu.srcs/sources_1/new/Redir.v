`timescale 1ns / 1ps
`define LW 6'b100011
`define OPERATION 6'b000000
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/10 22:01:57
// Design Name: 
// Module Name: Redir
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


module Redir(
    input [31:0] IR,
    input [31:0] IR_2,
    input [31:0] IR_3,
    output reg [1:0] redir_mux1,
    output reg [1:0] redir_mux2
    );
    reg mem_reg_write, wb_reg_write;
    reg [4:0] rs1, rs2, mem_rd, wb_rd;

    always @(*) begin
        redir_mux1 <= 2'b00;
        redir_mux2 <= 2'b00;
        // �����жϵ�����һ���Լ��������Ƿ��п����ṩ����
        mem_reg_write <= (IR_2[31:26] == `LW) ||
                         ((IR_2[31:26] == `OPERATION));
        wb_reg_write <= (IR_3[31:26] == `LW) ||
                        ((IR_3[31:26] == `OPERATION));

        // ����Ҫ������������
        if ((IR[31:26] != `OPERATION) || (mem_reg_write == 1'b0) || (wb_reg_write == 1'b0)) begin
            redir_mux1 <= 2'b00;
            redir_mux2 <= 2'b00;
        end

        rs1 <= IR[25:21];
        rs2 <= IR[20:16];
        if (IR_2[31:26] == `OPERATION) begin
            mem_rd <= IR_2[15:11];
        end else if (IR_2[31:26] == `LW) begin
            mem_rd <= IR_2[20:16];
        end

        if (IR_3[31:26] == `OPERATION) begin
            wb_rd <= IR_3[15:11];
        end else if (IR_3[31:26] == `LW) begin
            wb_rd <= IR_3[20:16];
        end
        
        //IR��IR2������أ�ȷ���Ǵ�alu_output��
        //�����sw��rt������أ���alu_output������rt��ֵ
        if (mem_reg_write && (mem_rd != 5'h0) && (mem_rd == rs1)) begin
            redir_mux1 <= 2'b01;
        end
        if (mem_reg_write && (mem_rd != 5'h0) && (mem_rd == rs2)) begin
            redir_mux2 <= 2'b01;
        end
        //���������SW��rt������أ�ʵ������dm2dm������Ҫ2alu������ѡ��Ļ���exout
        if(IR[31:26]==6'b101011&&(mem_reg_write && (mem_rd != 5'h0) && (mem_rd == rs2))) begin
            redir_mux2 <= 2'b00;//�ڸ��ź�
        end

        //���IR2���ݲ���أ����IR3��IR������أ�����wb�ε�д��
        if (wb_reg_write && (wb_rd != 5'h0) && (wb_rd == rs1) && !(mem_reg_write && (mem_rd != 5'h0) && (mem_rd == rs1))) begin
            redir_mux1 <= 2'b10;
        end
        if (wb_reg_write && (wb_rd != 5'h0) && (wb_rd == rs2) && !(mem_reg_write && (mem_rd != 5'h0) && (mem_rd == rs2))) begin
            redir_mux2 <= 2'b10;
        end


    end
endmodule
