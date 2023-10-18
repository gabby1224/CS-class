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
    input [4:0]raddr1,raddr2,waddr,//读地址1、2；写
    input[31:0]wdata,//写入数据
    input we,//写使能
    output [31:0]rdata1,rdata2//读数据1、2
);
reg [31:0]rf[31:0];

integer i ;
//清零操作 用于初始化寄存器空间
always@(*)begin
    if(!rstn)begin
        for(i=5'b00000;i<5'b11111;i=i+1)
            rf[i]<=32'h00000000;
    end
    if(we==1&&waddr!=0)
        rf[waddr]=wdata;//若写使能是1则写入数据
    end

assign rdata1 = rf[raddr1];
assign rdata2 = rf[raddr2];

endmodule