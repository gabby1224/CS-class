`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 15:06:27
// Design Name: 
// Module Name: register
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


module register(
    input clk,
    input rstn,
    input [4:0]raddr1,raddr2,waddr,//����ַ1��2��д
    input[31:0]wdata,//д������
    input we,//дʹ��
    output [31:0]rdata1,rdata2//������1��2
);
reg [31:0]rf[31:0];

integer i ;
//������� ���ڳ�ʼ���Ĵ����ռ�
always@(*)begin
    if(!rstn)begin
        for(i=5'b00000;i<5'b11111;i=i+1)
            rf[i]<=32'h00000000;
    end
    if(we==1&&waddr!=0)
        rf[waddr]=wdata;//��дʹ����1��д������
    end

assign rdata1 = rf[raddr1];
assign rdata2 = rf[raddr2];

endmodule