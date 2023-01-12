`timescale 1ps / 1ps
`include "utils.sv"

module HazardDetectionUnit( Mdubusy, IF_ID, ID_EX, EX_MEM, MEM_WB, next_ID_EX, flush, Mduflush );
    input wire Mdubusy;
    input PipelineReg IF_ID, ID_EX, EX_MEM, MEM_WB, next_ID_EX;
    output reg flush, Mduflush;
    
    always_comb begin
        if(ID_EX.ctrl.memread == 1 && (ID_EX.rd == next_ID_EX.rs || ID_EX.rd == next_ID_EX.rt)) flush = 1;
        else if (EX_MEM.ctrl.memread == 1 && (EX_MEM.rd == next_ID_EX.rs || EX_MEM.rd == next_ID_EX.rt) && (next_ID_EX.ctrl.branch == 1 || next_ID_EX.ctrl.jumpreg == 1)) flush = 1;
        else if(ID_EX.ctrl.regwrite == 1 && ID_EX.rd != 0 && (ID_EX.rd == next_ID_EX.rs || ID_EX.rd == next_ID_EX.rt) && (next_ID_EX.ctrl.branch == 1 || next_ID_EX.ctrl.jumpreg == 1)) flush = 1;
        else flush = 0;
        
        if ((Mdubusy &  ID_EX.ctrl.Mdu_start) == 1 ) Mduflush = 1;
        else Mduflush = 0;
    end
endmodule
