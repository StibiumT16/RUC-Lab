`timescale 1ps / 1ps

module ProgramCounter( reset, clock, jumpInput, pcValue );
    input wire reset, clock;
    input wire [31:0] jumpInput;
    output wire[31:0] pcValue;
    reg [31:0] pc;
    assign pcValue = pc;
    
    always_ff @( posedge clock ) begin
        if (reset) pc <= 'h00003000;
        else pc <= jumpInput;
    end
endmodule
