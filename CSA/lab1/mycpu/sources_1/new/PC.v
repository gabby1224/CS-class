`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 14:32:07
// Design Name: 
// Module Name: PC
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


module PC(
    //input 
    input [31:0]pc_in,
    input stall,
    input clk,//时钟
    input rstn,//复位
    input pc_en,//写使能
    output reg [31:0]pc_out
    );
    always@(posedge clk)begin
        if(!rstn)begin
            pc_out<=(0-4);//复位信号清零
            end
        else if(stall==1) ;
        else pc_out<=pc_in;
        end
endmodule
