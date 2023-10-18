`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 15:13:27
// Design Name: 
// Module Name: ALU
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


module ALU(
    input [31:0]data1,//mux1����
    input [31:0]data2,//mux2����
    input [4:0]rt,rs,
    input [5:0]alu_op,//cu�����ź�
    input [4:0]sa,//cu���Ŀ����ź�
    input neql,
    
    output reg [31:0]alu_result,
    output reg we,//��Ĵ�����д��ʹ�ܣ����ڱ���Ӽ����Ȳ����Ľ��
    output reg dmem_en//�����ݴ洢����д��ʹ�� ��ȡ������
    );
always@(*)begin
    case(alu_op)
        6'b100000: begin alu_result<=data1+data2; we<=1'b1; dmem_en<=0;end
        6'b100010: begin alu_result<=data1-data2; we<=1'b1; dmem_en<=0;end
        6'b100100: begin alu_result<=data1&data2; we<=1'b1; dmem_en<=0;end
        6'b100101: begin alu_result<=data1|data2; we<=1'b1; dmem_en<=0;end
        6'b100110: begin alu_result<=data1^data2; we<=1'b1; dmem_en<=0;end
        6'b101010: begin alu_result<=(data1<data2)?1:0; we<=1'b1; dmem_en<=0;end
        6'b001010: begin alu_result<=data1; we<=(data2==0)?1:0; dmem_en<=0;end//�����ƶ�data2���������ƶ�
        6'b000000: begin alu_result<=data2<<sa; we<=1; dmem_en<=0;end//��λ 
        6'b101011: begin alu_result<=data1+data2; dmem_en<=1; we<=0;end//����ָ�� 
        6'b100011: begin alu_result<=data1+data2; we<=1; dmem_en<=0;end//ȡ��ָ��
        6'b000101: begin alu_result<=(neql==1)?(data1+(data2<<2)):data1;end//����ת��
        6'b000010: begin alu_result<={(data1[31:28]),(data2 << 2)};end//������ת��
    endcase
    end
endmodule
