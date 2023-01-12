`include "utils.sv"

module ControllerUnit( input Instruction instr, output Controller ctrl );
    always_comb begin
        ctrl = 0;
        if(instr != 0) begin
            case(instr.R.op)
                6'b000000: begin
                    if(instr.R.func == 6'b001100) begin end //syscall
                    else if(instr.R.func == 6'b001000) begin// JR
                        ctrl.jump = 1;
                        ctrl.jumpreg = 1;
                    end
                    else if(instr.R.func == 6'b001001) begin// JALR
                        ctrl.jump = 1;
                        ctrl.jumpreg = 1;
                        ctrl.writera = 1;
                        ctrl.regwrite = 1;
                    end
                    else if(instr.R.func == 6'b010000) begin// MFHI
                        ctrl.regwrite = 1;
                        ctrl.Mdu_start = 1;
                        ctrl.Mdu_op = MDU_READ_HI;
                    end
                    else if(instr.R.func == 6'b010010) begin// MFLO
                        ctrl.regwrite = 1;
                        ctrl.Mdu_start = 1;
                        ctrl.Mdu_op = MDU_READ_LO;
                    end
                    else if(instr.R.func == 6'b010001) begin// MTHI
                        ctrl.Mdu_start = 1;
                        ctrl.Mdu_op = MDU_WRITE_HI;
                    end
                    else if(instr.R.func == 6'b010011) begin// MTLO
                        ctrl.Mdu_start = 1;
                        ctrl.Mdu_op = MDU_WRITE_LO;
                    end
                    else if(instr.R.func == 6'b011000) begin// MULT
                        ctrl.Mdu_start = 1;
                        ctrl.Mdu_op = MDU_START_SIGNED_MUL;
                    end
                    else if(instr.R.func == 6'b011001) begin// MULTU
                        ctrl.Mdu_start = 1;
                        ctrl.Mdu_op = MDU_START_UNSIGNED_MUL;
                    end
                    else if(instr.R.func == 6'b011010) begin// DIV
                        ctrl.Mdu_start = 1;
                        ctrl.Mdu_op = MDU_START_SIGNED_DIV;
                    end
                    else if(instr.R.func == 6'b011011) begin// DIVU
                        ctrl.Mdu_start = 1;
                        ctrl.Mdu_op = MDU_START_UNSIGNED_DIV;
                    end
                    else begin
                        ctrl.regwrite = 1;
                        ctrl.regdst = 1;
                        ctrl.ALU_op[1] = 1;
                        if(instr.R.func == 6'b100000 || instr.R.func == 6'b100010) ctrl.overflow = 1; //ADD SUB
                        else if(instr.R.func == 6'b000000 || instr.R.func == 6'b000010 || instr.R.func == 6'b000011) begin // SLL SRL SRA
                            ctrl.shift = 1;
                            ctrl.shift_sa = 1;
                        end
                        else if(instr.R.func == 6'b000100 || instr.R.func == 6'b000110 || instr.R.func == 6'b000111) ctrl.shift = 1;// SLLV SRLV SRAV
                    end
                end
                6'b001000: begin //ADDI
                    ctrl.overflow = 1;
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.immsign = 1;
                end
                6'b001001: begin //ADDIU
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.immsign = 1;
                end
                6'b001100: begin //ANDI
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.ALU_op = 4'b1001;
                end
                6'b001101: begin //ORI
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.ALU_op = 4'b1010;
                end
                6'b001110: begin //XORI
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.ALU_op = 4'b1011;
                end
                6'b001010: begin //SLTI
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.immsign = 1;
                    ctrl.ALU_op = 4'b1100;
                end
                6'b001011: begin //SLTIU
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.immsign = 1;
                    ctrl.ALU_op = 4'b1101;
                end
                6'b000100: begin //BEQ
                    ctrl.branch = 1;
                    ctrl.ALU_op = 4'b0001;
                    ctrl.immsign = 1;
                end
                6'b000101: begin //BNE
                    ctrl.branch = 1;
                    ctrl.ALU_op = 4'b0010;
                    ctrl.immsign = 1;
                end
                6'b000110: begin //BLEZ
                    ctrl.branch = 1;
                    ctrl.ALU_op = 4'b0011;
                    ctrl.immsign = 1;
                end
                6'b000111: begin //BGTZ
                    ctrl.branch = 1;
                    ctrl.ALU_op = 4'b0100;
                    ctrl.immsign = 1;
                end
                6'b000001: begin //BGEZ BLTZ
                    ctrl.branch = 1;
                    ctrl.immsign = 1;
                    if(instr.R.rt == 5'b00001) ctrl.ALU_op = 4'b0101;
                    else ctrl.ALU_op = 4'b0110;
                end
                6'b000010: ctrl.jump = 1;//J
                6'b000011: begin //JAL
                    ctrl.jump = 1;
                    ctrl.writera = 1;
                    ctrl.regwrite = 1;
                end
                6'b001111: begin //LUI
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.imm2reg = 1;
                end
                6'b100000: begin //LB
                    ctrl.ALU_src = 1;
                    ctrl.mem2reg = 1;
                    ctrl.regwrite = 1;
                    ctrl.memread = 1;
                    ctrl.immsign = 1;
                    ctrl.memsign = 1;
                    ctrl.mem_bit[0] = 1;
                end
                6'b100100: begin //LBU
                    ctrl.ALU_src = 1;
                    ctrl.mem2reg = 1;
                    ctrl.regwrite = 1;
                    ctrl.memread = 1;
                    ctrl.immsign = 1;
                    ctrl.mem_bit[0] = 1;
                end
                6'b100001: begin //LH
                    ctrl.ALU_src = 1;
                    ctrl.mem2reg = 1;
                    ctrl.regwrite = 1;
                    ctrl.memread = 1;
                    ctrl.immsign = 1;
                    ctrl.memsign = 1;
                    ctrl.mem_bit[1] = 1;
                end
                6'b100101: begin //LHU
                    ctrl.ALU_src = 1;
                    ctrl.mem2reg = 1;
                    ctrl.regwrite = 1;
                    ctrl.memread = 1;
                    ctrl.immsign = 1;
                    ctrl.mem_bit[1] = 1;
                end
                6'b100011: begin //LW
                    ctrl.ALU_src = 1;
                    ctrl.regwrite = 1;
                    ctrl.mem2reg = 1;
                    ctrl.memread = 1;
                    ctrl.immsign = 1;
                end
                6'b101000: begin //SB
                    ctrl.ALU_src = 1;
                    ctrl.memwrite = 1;
                    ctrl.immsign = 1;
                    ctrl.mem_bit[0] = 1;
                end
                6'b101001: begin //SH
                    ctrl.ALU_src = 1;
                    ctrl.memwrite = 1;
                    ctrl.immsign = 1;
                    ctrl.mem_bit[1] = 1;
                end
                6'b101011: begin //SW
                    ctrl.ALU_src = 1;
                    ctrl.memwrite = 1;
                    ctrl.immsign = 1;
                end
                default : begin
                    $display("illegal opcode: %b", instr.R.op);
                    $finish;
                end
            endcase
        end
    end
endmodule
