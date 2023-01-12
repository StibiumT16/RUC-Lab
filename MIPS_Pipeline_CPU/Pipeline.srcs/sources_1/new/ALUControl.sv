`timescale 1ps / 1ps

module ALUControl( ALU_op, func, ctrl );
    input wire[3:0] ALU_op;
    input wire[5:0] func;
    output wire[3:0] ctrl;
    reg [3:0] res;
    assign ctrl = res;
    
    always_comb begin
        if(ALU_op == 0) res = 4'b0010; //add in alu
        else if (ALU_op == 2'b01) res = 4'b0110; //sub in alu
        else if (ALU_op == 2'b10) begin
            case(func)
                6'b000000: res = 4'b1000; //SLL
                6'b000010: res = 4'b1001; //SRL
                6'b000011: res = 4'b1011; //SRA
                6'b000100: res = 4'b1000; //SLLV
                6'b000110: res = 4'b1001; //SRLV
                6'b000111: res = 4'b1011; //SRAV
                6'b100000: res = 4'b0010; //ADD
                6'b100001: res = 4'b0010; //ADDU
                6'b100010: res = 4'b0110; //SUB
                6'b100011: res = 4'b0110; //SUBU
                6'b100100: res = 4'b0000; //AND
                6'b100101: res = 4'b0001; //OR
                6'b100110: res = 4'b0011; //XOR
                6'b100111: res = 4'b0100; //NOR
                6'b101010: res = 4'b0111; //SLT
                6'b101011: res = 4'b0101; //SLTU
            endcase
        end
        else if (ALU_op & 4'b1000) begin
            case (ALU_op[2:0])
                3'b001: res = 4'b0000; //ANDI
                3'b010: res = 4'b0001; //ORI
                3'b011: res = 4'b0011; //XORI
                3'b100: res = 4'b0111; //STLI
                3'b101: res = 4'b0101; //STLIU
            endcase
        end           
    end 
endmodule
