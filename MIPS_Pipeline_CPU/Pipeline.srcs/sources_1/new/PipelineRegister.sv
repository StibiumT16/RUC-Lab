`timescale 1ps / 1ps
`include "utils.sv"
 
module PipelineRegister( reset, clock, in, out );
    input wire reset, clock;
    input PipelineReg in;
    output PipelineReg out;
    reg [$bits(PipelineReg)-1:0] tmp;
    assign out = tmp;
    
    always_ff @ (posedge clock) begin
        if (reset == 1) tmp <= 0;
        else tmp <= in;
    end
endmodule
