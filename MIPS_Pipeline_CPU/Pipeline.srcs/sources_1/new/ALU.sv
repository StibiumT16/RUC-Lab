`timescale 1ps / 1ps

module ALU( pc, A, B, overflow, ctrl, C, zero );
    input wire[31:0] pc, A, B;
    input wire overflow;
    input wire[3:0] ctrl;
    output wire [31:0] C;
    output wire zero;
    reg [31:0] ans;
    reg is_zero;
    assign C = ans;
    assign zero = is_zero;
    
    always_comb begin
        case(ctrl)
            4'b1000 : ans = A << B;
            4'b1001 : ans = A >> B;
            4'b1011 : ans = ($signed(A)) >>> B;
            4'b0010: begin
                ans = A + B;
                if ( (({ans[31], ans} != ({A[31], A} + {B[31], B})) & overflow)== 1) begin
                    $display("@%h: SignalException IntegerOverflow", pc);
                    $finish;
                end
            end
            4'b0110: begin
                ans = A - B;
                if ( (({ans[31], ans} != ({A[31], A} - {B[31], B})) & overflow)== 1) begin
                    $display("@%h: Signal Exception IntegerOverflow", pc);
                    $finish;
                end
            end
            4'b0000: ans = A & B;
            4'b0001: ans = A | B;
            4'b0011: ans = A ^ B;
            4'b0100: ans = ~(A | B);
            4'b0111: ans = $signed(A) < $signed(B);
            4'b0101: ans = A < B;
        endcase 
        if(ans === 0) is_zero = 1;
        else is_zero = 0;
    end
endmodule
