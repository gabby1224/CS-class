`timescale 1ns / 1ps

module cpu_tb(

    );

//-----Clock and reset signal simulation-----
//signals
reg clk;
reg resetn;
//simulation
initial begin
    clk    = 1'b0;
    resetn = 1'b0;
//    #5;
//    clk = 1;
//    #5;
//    clk = 0;
//    #5;
//    clk = 1;
//    #5;
//    clk = 0;
//    #5;
//    clk = 1;
//    #5;
//    clk = 0;
//    #5;
//    clk = 1;
//    #5;
//    clk = 0;
    #20;
    resetn = 1'b1;
//    #5;
//    clk = 1;
//    #5;
//    clk = 0;
//    #5;
//    clk = 1;
//    #5;
//    clk = 0;
end

always #5 clk = ~clk;


cpu_top U_cpu_top(
        .clk     (clk     ),
        .resetn   (resetn  )
    );

//-----monitor test-----
initial
begin
    $timeformat(-9,0," ns",10);
    while(!resetn) #5;
    $display("==============================================================");
    $display("Test begin!");
    #10000;
end

endmodule
