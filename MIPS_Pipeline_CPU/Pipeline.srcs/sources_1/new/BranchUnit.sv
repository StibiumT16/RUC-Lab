`timescale 1ps / 1ps

module BranchUnit( in1, in2, ALU_op, out );
    input wire[31:0] in1;
    input wire[31:0] in2;
    input wire[3:0] ALU_op;
    output wire out;
    reg tmp;
    assign out = tmp;
    always_comb begin
        case(ALU_op)
            4'b0001: tmp = (in1 == in2);
            4'b0010: tmp = (in1 != in2);
            4'b0011: tmp = ($signed(in1) <= 0);
            4'b0100: tmp = ($signed(in1) > 0);
            4'b0101: tmp = ($signed(in1) >= 0);
            4'b0110: tmp = ($signed(in1) < 0);
        endcase
    end
endmodule
