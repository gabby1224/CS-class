`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/10/07 11:06:56
// Design Name: 
// Module Name: mux_1_n_tb
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


module mux_1_n_tb(

    );
    reg clk;
    reg [1:0]signal;
    reg [31:0]a,b,c;
    wire [31:0]out;
    wire [31:0]data2;
    
    mux_1_new tb_1(
    .clk(clk),
    .alu_output(c),
    .npc_out(a),
    .rdata1(b),
    .signal(signal),
    .data1(out)
    );
    
    alu_new alu_tb(
    .data1(out),
    .data2(data2),
    .clk(clk)
    );
    
    initial begin
        signal = 0;
        clk = 0;
        a = 1;
        b = 2;
        c = 3;    
    
    end
    
    always #5 clk = ~clk;
    
    always #30 signal = signal + 1;
    
endmodule
