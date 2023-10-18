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
    //指令拆分
    input [31:0]IR,
    input [5:0]op,//高六位
    input [10:0]func,//低十位
    //四个mux的输出信号
    output reg mux1_s,
    output reg mux2_s,
    output reg mux3_s,
    output reg mux4_s,
    //alu操作码
    output reg [5:0]alu_op,
    output reg stall
    );
//    always@(posedge clk)begin//复位mux（清零）
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
    always@(*)begin//判断变化 有变化就开始运行
        case(op)//判断高六位操作码值 来确定指令种类
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
