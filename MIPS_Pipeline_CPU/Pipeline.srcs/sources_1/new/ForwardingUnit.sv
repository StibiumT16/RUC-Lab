`timescale 1ps / 1ps
`include "utils.sv"

module ForwardingUnit( IF_ID, ID_EX, EX_MEM, MEM_WB, next_ID_EX, rs_val_in_EX, rt_val_in_EX, rs_val_in_ID, rt_val_in_ID );
    input PipelineReg IF_ID, ID_EX, EX_MEM, MEM_WB, next_ID_EX;
    output reg[1:0] rs_val_in_EX, rt_val_in_EX, rs_val_in_ID, rt_val_in_ID;
    always_comb begin
    //EX
        if(EX_MEM.ctrl.regwrite == 1 && EX_MEM.rd !=0 && EX_MEM.rd == ID_EX.rs)  rs_val_in_EX = 2'b10;
        else if(MEM_WB.ctrl.regwrite == 1 && MEM_WB.rd != 0 && MEM_WB.rd == ID_EX.rs) rs_val_in_EX = 2'b01;
        else rs_val_in_EX = 2'b00;
        if(EX_MEM.ctrl.regwrite == 1 && EX_MEM.rd !=0 && EX_MEM.rd == ID_EX.rt)  rt_val_in_EX = 2'b10;
        else if(MEM_WB.ctrl.regwrite == 1 && MEM_WB.rd != 0 && MEM_WB.rd == ID_EX.rt) rt_val_in_EX = 2'b01;
        else rt_val_in_EX = 2'b00;
    //ID
        if(ID_EX.ctrl.regwrite == 1 && ID_EX.rd !=0 && ID_EX.rd == next_ID_EX.rs)  rs_val_in_ID = 2'b11;
        else if(EX_MEM.ctrl.regwrite == 1 && EX_MEM.rd != 0 && EX_MEM.rd == next_ID_EX.rs) rs_val_in_ID = 2'b10;
        else if(MEM_WB.ctrl.regwrite == 1 && MEM_WB.rd != 0 && MEM_WB.rd == next_ID_EX.rs) rs_val_in_ID = 2'b01;
        else rs_val_in_ID = 2'b00;
        if(ID_EX.ctrl.regwrite == 1 && ID_EX.rd !=0 && ID_EX.rd == next_ID_EX.rt)  rt_val_in_ID = 2'b11;
        else if(EX_MEM.ctrl.regwrite == 1 && EX_MEM.rd != 0 && EX_MEM.rd == next_ID_EX.rt) rt_val_in_ID = 2'b10;
        else if(MEM_WB.ctrl.regwrite == 1 && MEM_WB.rd != 0 && MEM_WB.rd == next_ID_EX.rt) rt_val_in_ID = 2'b01;
        else rt_val_in_ID = 2'b00;
    end
endmodule
