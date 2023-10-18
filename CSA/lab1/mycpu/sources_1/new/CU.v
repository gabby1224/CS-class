`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 15:52:50
// Design Name: 
// Module Name: CU
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


module CU(
    input clk,
    input rstn,
    //ָ����
    input [31:0]IR,
    input [5:0]op,//����λ
    input [10:0]func,//��ʮλ
    //�ĸ�mux������ź�
    output reg mux1_s,
    output reg mux2_s,
    output reg mux3_s,
    output reg mux4_s,
    //alu������
    output reg [5:0]alu_op,
    output reg stall
    );
//    always@(posedge clk)begin//��λmux�����㣩
//        mux1_s<=(rstn==0)?0:0;
//        mux2_s<=(rstn==0)?0:0;
//        mux3_s<=(rstn==0)?0:0;
//        mux4_s<=(rstn==0)?0:0;
//        stall <=(rstn==0)?0:0;
//    end
//    initial begin
//        mux1_s<=(rstn==0)?0:1;
//        mux2_s<=(rstn==0)?0:1;
//        mux3_s<=(rstn==0)?0:1;
//        mux4_s<=(rstn==0)?0:1;
//        stall <=(rstn==0)?0:1;
//    end
    always@(*)begin//�жϱ仯 �б仯�Ϳ�ʼ����
        case(op)//�жϸ���λ������ֵ ��ȷ��ָ������
            6'b000000: begin
                alu_op<=func[5:0];
                mux1_s <= 0;
                mux2_s <= 0;
                mux3_s <= 0;
                mux4_s <= 0;
            end
            6'b101011: begin
                alu_op<=6'b101011;
                mux1_s <= 0;
                mux2_s <= 1;
                mux3_s <= 0;
                mux4_s <= 0;
            end
            6'b100011: begin
                alu_op<=6'b100011;
                mux1_s <= 0;
                mux2_s <= 1;
                mux3_s <= 0;
                mux4_s <= 1;
            end
            6'b000101: begin
                alu_op<=6'b000101;
                mux1_s <= 1;
                mux2_s <= 1;
                mux3_s <= 1;
                mux4_s <= 0;
            end
            6'b000010: begin
                alu_op<=6'b000010;
                mux1_s <= 1;
                mux2_s <= 1;
                mux3_s <= 1;
                mux4_s <= 0;
            end
            6'b100011: begin alu_op<=6'b100011; mux1_s <= 1; mux2_s <= 1; mux3_s <= 1; mux4_s <= 1; end
            6'b000101: begin alu_op<=6'b000101;mux1_s <= 1;mux2_s <= 1;mux3_s <= 1;mux4_s <= 1; end
            6'b000010: begin alu_op<=6'b000010;mux1_s <= 1;mux2_s <= 1;mux3_s <= 1;mux4_s <= 1; end
            endcase
     end
endmodule
