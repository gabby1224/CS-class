`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/09/05 19:53:58
// Design Name: 
// Module Name: CPU
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


module mycpu(
    input    clk,
    input    rstn,  // active low

    // debug signals
    output [31:0]    debug_wb_pc     ,  // 当前正在执行指令的PC
    output           debug_wb_rf_wen ,  // 当前通用寄存器组的写使能信号
    output [4 :0]    debug_wb_rf_addr,  // 当前通用寄存器组写回的寄存器编号
    output [31:0]    debug_wb_rf_wdata  // 当前指令需要写回的数据
    );
     //下面是一些必要的连线
    wire [31:0]PC_i;
    wire [31:0]PC_o;
    //npc
    wire [31:0]NPC_i;
    wire [31:0]NPC_o;
    //npc_2
    wire [31:0]npc_2_o;
    //imem
    wire [31:0]ir;
    wire [31:0]IR;
    //reg
    wire [4:0]rs;
    wire [4:0]rt;
    wire [4:0]rd;
    wire we;
    wire [31:0]wdata;
    wire [31:0]rdata1;
    wire [31:0]rdata2;
    //A
    wire [31:0]A;
    wire [31:0]B;
    //extend
    wire [15:0]short;
    wire [25:0]long;
    wire [5:0]in_op;
    wire [31:0]ex_out;
    wire [31:0]imm;
    //mux
    wire [31:0]mux1;
    wire [31:0]mux2;
    wire [31:0]mux3;
    wire [31:0]mux4;
    wire stall0,stall1,stall2;
    //alu
    wire [5:0]alu_op;
    wire [5:0]alu_op_2;
    wire [4:0]sa;
    wire neql;
    wire [31:0]alu_result;

    wire data_en;
    wire [31:0]data1;
    wire [31:0]data2;
    wire [31:0]data2_o;
    wire [31:0]t_wdata;
    //wb
    wire [10:0]func;
    wire [31:0]ddata;
    wire [31:0]ddata_2;
    wire [4:0]waddr;
    //ALU_output
    wire [31:0]alu_output;
    wire [31:0]alu_output2;
    wire we_o,dmem_en_o;
    wire we_2,dmem_en_2;
    //IR_2
    wire [31:0]IR_2;
    wire [4:0]rt_2;
    wire [4:0]rs_2;
    wire [4:0]sa_2;
    
    wire [31:0]IR_3;
    
    wire [31:0]IR_4;
    wire [4:0]rd_4;
    wire [4:0]rt_4;
    wire [5:0]in_op_4;
    //for redirection
    wire redirmux1_1,redirmux1_2,redirmux1_12,redirmux1_22;
    wire redirmux2_1,redirmux2_2,redirmux2_12,redirmux2_22;
    wire redir_dm,redir_dm2,redir_dm4daddr,redir_dm4daddr2,redir_dm4daddr3;
    //signal2
    wire mux1_s,mux2_s,mux3_s,mux4_s;
    wire mux1_s_2,mux2_s_2,mux3_s_2,mux4_s_2;
    wire mux1_s_3,mux2_s_3,mux3_s_3,mux4_s_3;
    wire mux1_s_4,mux2_s_4,mux3_s_4,mux4_s_4;

    
    
    //负责把pc的值带到4个周期之后
    wire [31:0]pc_in_2;
    wire [31:0]pc_in_3;
    wire [31:0]pc_in_4;
    wire [31:0]pc_in_5;
    wire [31:0]pc_add;
    PC_for_test PC_for_test_tb(
    .clk(clk),
    .rstn(rstn),
    .stall1(stall1),
    .pc_in(PC_o),
    .pc_in_2(pc_in_2)
    );
    PC_for_test2 PC_for_test2_tb(
    .clk(clk),
    .rstn(rstn),
    .stall2(stall2),
    .pc_in_2(pc_in_2),
    .pc_in_3(pc_in_3)
    );
    PC_for_test3 PC_for_test3_tb(
    .clk(clk),
    .rstn(rstn),
    .pc_in_4(pc_in_4),
    .pc_in_3(pc_in_3)
    );
    PC_for_test4 PC_for_test4_tb(
    .clk(clk),
    .rstn(rstn),
    .pc_in_4(pc_in_4),
    .pc_in_5(pc_in_5)
    );
    
    assign debug_wb_pc = pc_in_5;
    assign debug_wb_rf_wen = we_2;
    assign debug_wb_rf_addr = waddr;
    assign debug_wb_rf_wdata = wdata;
    
    
     PC PC_tb(
        .pc_in(NPC_i),
        .pc_out(PC_o),
        .pc_en(pc_en),
        .clk(clk),
        .rstn(rstn),
        .stall(stall0)
     );
           
      pc_add pc_add_tb(
        .pc_out(PC_o),
        .pc_add(pc_add),
        .clk(clk)
      );
      
      register register_tb(
        .clk(clk),
        .raddr1(rs),
        .raddr2(rt),
        .waddr(waddr),
        .wdata(wdata),
        .we(we_2),
        .rdata1(rdata1),
        .rdata2(rdata2),
        .rstn(rstn)
      );
      
     inst_mem instmem_tb(
        .pc_out(PC_o),
        .ir(ir),
        .clk(clk)
      );

      
      EXTEND EXTEND_tb(
        .clk(clk),
        .rstn(rstn),
        .short(short),
        .long(long),
        .op(in_op),
        .ex_out(ex_out)
      );
      /*新增一个模块，用于判断定向
      */
      hazard_dect hazard_dect_tb(
      .IR(IR),
      .IR_2(IR_2),
      .stall0(stall0),
      .stall1(stall1),
      .stall2(stall2)
      );
      
      Redir Redir_tb (
        .IR (IR_2),
        .IR_2 (IR_3),
        .IR_3 (IR_4),
        .redir_mux1 (forwarding_mux_select_1),
        .redir_mux2 (forwarding_mux_select_2)
      );
      Dm2dm DM2DM_tb(
      .IR(IR),
      .IR_2(IR_2),
      .redir_dm_2(redir_dm)
//      .redir_dm_1(redir_dm4daddr)
      );
      
      mux1 mux1_tb(
          .mux1_s(mux1_s_2),
          .clk(clk),
          .rdata1(A),
          .npc_out(NPC_o),
          .data1(data1)
      );

      
      mux2 mux2_tb(
        .rdata2(B),
        .ex_out(imm),
        .clk(clk),
        .mux2_s(mux2_s_2),
        .data2(data2)
      );
      
      mux3 mux3_tb(
        .clk(clk),
        .alu_result(alu_output),
        .PC_add(pc_add),
        .mux3_s(mux3_s_3),
        .npc_in(NPC_i)
      );
      
      mux4 mux4_tb(
        .clk(clk),
        .mux4_s(mux4_s_4),
        .ddata(ddata_2),
        .wdata(wdata),
        .alu_result(alu_output2)
      );
      wire [31:0]forwarding_mux1_output;
      wire [31:0]forwarding_mux2_output;
      wire [1:0]forwarding_mux_select_1;
      wire [1:0]forwarding_mux_select_2;
      mux_41 forwarding_mux_1_tb (
        .A (data1),
        .B (alu_output),
        .C (wdata),
        .D (),
        .select (forwarding_mux_select_1),
        .result (forwarding_mux1_output)
    );

      mux_41 forwarding_mux_2_tb (
        .A (data2),
        .B (alu_output),
        .C (wdata),
        .D (),
        .select (forwarding_mux_select_2),
        .result (forwarding_mux2_output)
    );

      
      eql eql_tb(
        .rdata1(A),
        .rdata2(B),
        .neql(neql)
      );
 //
       ALU ALU_tb(
         .data1(forwarding_mux1_output),
         .data2(forwarding_mux2_output),
         .rt(rt_2),
         .rs(rs_2),
         .alu_op(alu_op_2),
         .sa(sa_2),
         .neql(neql),
    
         .alu_result(alu_result),
         .we(we),
         .dmem_en(data_en)
      );
      
      CU CU_tb(
        .clk(clk),
        .rstn(rstn),
        .op(in_op),
        .func(func),
        .mux1_s(mux1_s),
        .mux2_s(mux2_s),
        .mux3_s(mux3_s),
        .mux4_s(mux4_s),
        .alu_op(alu_op),
        .IR(IR),
        .stall(stall)
      );
      
      mux_wdata mux_wdata_tb(
      .redir_dm(redir_dm3),
      .data_rdata(ddata_2),
      .data2_o(data2_o),
      .t_wdata(t_wdata)
      );
            
      data_mem data_mem_tb(
        .clk(clk),
        .data_en(dmem_en_o),
        .F(alu_output),//就是地址
        .wdata(t_wdata),
        .data_rdata(ddata)
      );
      
      writeback writeback_tb(
        .rt(rt_4),
        .rd(rd_4),
        .waddr(waddr),
        .in_op(in_op_4)
      );
      
      //IF/ID的流水线寄存器
      NPC NPC_tb(
        .clk(clk),
        .stall1(stall1),
        .rstn(rstn),
        .npc_in(NPC_i),
        .npc_out(NPC_o)
      );
      IR IR_tb(
        .clk(clk),
        .rstn(rstn),
        .stall1(stall1),
        .ir(ir),
        .IR(IR),
        .rs(rs),
        .rt(rt),
        .rd(rd),
        .sa(sa),
        .in_op(in_op),
        .func(func),
        .short(short),
        .long(long)
      );
      //ID,EX流水线寄存器
      wire [31:0]alu_output_t;
      wire [31:0]alu_output2_t;
      wire [31:0]c_alu_output;
      wire [31:0]c_alu_output2;

      A A_tb(
      .clk(clk),
      .stall2(stall2),
      .rdata1(rdata1),
      .A(A)
      ); 
      B B_tb(
      .clk(clk),
      .stall2(stall2),
      .rdata2(rdata2),
      .B(B)
      );
      IMM IMM_tb(
      .clk(clk),
      .stall2(stall2),
      .ex_out(ex_out),
      .imm(imm)
      );
      IR_2 IR_2_tb(
      .clk(clk),
      .stall2(stall2),
      .IR(IR),
      .IR_2(IR_2),
      .sa(sa_2),
      .rs(rs_2),
      .rt(rt_2)
      );
      NPC_2 NPC_2_tb(
      .clk(clk),
      .stall2(stall2),
      .npc_o(NPC_o),
      .npc_2_o(npc_2_o)
      );
      
      signal_2 signal_2_tb(
      .clk(clk),
      .stall2(stall2),
      .alu_op(alu_op),
      .alu_op_2(alu_op_2),
      .mux1_s(mux1_s),
      .mux2_s(mux2_s),
      .mux3_s(mux3_s),
      .mux4_s(mux4_s),
      .mux1_s_2(mux1_s_2),
      .mux2_s_2(mux2_s_2),
      .mux3_s_2(mux3_s_2),
      .mux4_s_2(mux4_s_2)
      );
      
      redir_sig redir_sig_tb(
      .clk(clk),
      .stall2(stall2),
      .redir_dm2(redir_dm2),
      .redir_dm(redir_dm)
      );
      
      //EX/mem
      IR_3 IR_3_tb(
      .clk(clk),
      .stall(stall),
      .IR_2(IR_2),
      .IR_3(IR_3)
      );
//      neql neql_tb(
//      .clk(clk),
//      .neql(neql),
//      .neql_o()
//      );

      
      ALU_output ALU_op_tb(
      .clk(clk),
      .stall(stall),
      .alu_result(alu_result),
      .alu_output(alu_output),
      .we(we),
      .dmem_en(data_en),
      .we_o(we_o),
      .dmem_en_o(dmem_en_o)
      );
      signal_3 signal_3_tb(
      .clk(clk),
      .stall(stall),
      .mux1_s_2(mux1_s_2),
      .mux2_s_2(mux2_s_2),
      .mux3_s_2(mux3_s_2),
      .mux4_s_2(mux4_s_2),
      .mux1_s_3(mux1_s_3),
      .mux2_s_3(mux2_s_3),
      .mux3_s_3(mux3_s_3),
      .mux4_s_3(mux4_s_3)
      );
      B_2 B_2_tb(
      .clk(clk),
      .data2(forwarding_mux2_output),
      .data2_o(data2_o)
      );
      wire stall_s;
      redir_dm_save redir_dm_s_tb(
      .clk(clk),
      .stall2(stall2),
      .redir_dm2(redir_dm2),
      .redir_dm3(redir_dm3),
      .stall(stall_s)
      );
      
      
      //MEM/WB流水寄存器
      IR_4 IR_4_tb(
      .clk(clk),
      .stall(stall),
      .IR_3(IR_3),
      .IR_4(IR_4),
      .rt(rt_4),
      .rd(rd_4),
      .in_op(in_op_4)
      );
      LMD LMD_tb(
      .clk(clk),
      .stall(stall),
      .rdata(ddata),
      .rdata2(ddata_2)
      );
      signal_4 signal_4_tb(
      .clk(clk),
      .stall(stall),
      .mux1_s_4(mux1_s_4),
      .mux2_s_4(mux2_s_4),
      .mux3_s_4(mux3_s_4),
      .mux4_s_4(mux4_s_4),
      .mux1_s_3(mux1_s_3),
      .mux2_s_3(mux2_s_3),
      .mux3_s_3(mux3_s_3),
      .mux4_s_3(mux4_s_3)
      );
      WB_S WB_S_tb(
      .clk(clk),
      .alu_output(alu_output),
      .alu_output2(alu_output2),
      .we_o(we_o),
      .dmem_en_o(dmem_en_o),
      .dmem_en_2(dmem_en_2),
      .we_2(we_2)
      );
      
endmodule
