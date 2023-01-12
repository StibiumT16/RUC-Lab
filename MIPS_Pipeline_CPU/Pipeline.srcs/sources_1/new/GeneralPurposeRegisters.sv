`timescale 1ps / 1ps

module GeneralPurposeRegisters( fd, reset, clock, pcValue, rs, rt, rd, writeEnabled, writeInput, regval1, regval2 );
    input integer fd;
    input reset, clock, writeEnabled;
    input [4:0] rs,rt,rd;
    input [31:0] pcValue, writeInput;
    output [31:0] regval1, regval2;
    reg[31:0] regdata[31:0];
    assign regval1 = regdata[rs];
    assign regval2 = regdata[rt];
    integer i;
    
    always_ff @ (posedge clock) begin
        if (reset) begin
            for ( i = 0; i < 32; i = i + 1 )
                regdata[i] <= 0;
        end
        if (writeEnabled) begin
            if (rd != 0) regdata[rd] <= writeInput; 
            $display("@%h: $%d <= %h", pcValue, rd, writeInput);
            $fdisplay(fd, "@%h: $%d <= %h", pcValue, rd, writeInput);
        end
    end
    
endmodule
