`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 17:16:33
// Design Name: 
// Module Name: DMEM
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


module data_mem(
    input clk,
    input data_en,
    input [31:0]F,
    input [31:0]wdata,
    output [31:0]data_rdata
    );
    reg [31:0]data[255:0];
    initial begin
		$readmemh("*\\additional_data_data1", data);
    end
wire [31:0]data_addr;
assign data_addr=F/4;
assign data_rdata={data[data_addr]};

    always@(posedge clk) begin
        if(data_en==1)begin
            data[data_addr] <= wdata;
         end
        end

    


endmodule
