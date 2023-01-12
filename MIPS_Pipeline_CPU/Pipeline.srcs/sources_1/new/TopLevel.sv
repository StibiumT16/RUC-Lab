`timescale 1ps / 1ps
`include "utils.sv"

module TopLevel( input wire reset, input wire clock );
    reg[31:0] insmem[1023:0];
    integer fd;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\0dE.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\0dE.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\0eC.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\0eC.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\0eL.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\0eL.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\0hJ.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\0hJ.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\0vM.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\0vM.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\02H.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\02H.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\08H.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\08H.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\22H.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\22H.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\28H.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\28H.asm.txt", insmem); end;
    // initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\82H.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\82H.asm.txt", insmem); end;
    initial begin fd = $fopen("E:\\Vivado\\TEST_code\\my_solution_50\\88H.txt", "w"); $readmemh("E:\\Vivado\\TEST_code\\pipeline50_test\\code\\88H.asm.txt", insmem); end;
    
    PipelineReg IF_ID, ID_EX, EX_MEM, MEM_WB, next_IF_ID, next_ID_EX, next_EX_MEM, next_MEM_WB;
    reg [31:0] nextpc;
    wire flush, Mdubusy, Mduflush;
    wire [1:0] rs_val_in_EX, rt_val_in_EX, rs_val_in_ID, rt_val_in_ID;
    HazardDetectionUnit HDU(.Mdubusy(Mdubusy), .IF_ID(IF_ID), .ID_EX(ID_EX), .EX_MEM(EX_MEM), .MEM_WB(MEM_WB), .next_ID_EX(next_ID_EX), .flush(flush), .Mduflush(Mduflush));
    PipelineRegister IFID(.reset(reset), .clock(clock), .in(next_IF_ID), .out(IF_ID) );
    PipelineRegister IDEX(.reset(reset | flush), .clock(clock), .in(Mduflush ? ID_EX : next_ID_EX), .out(ID_EX) );
    PipelineRegister EXMEM(.reset(reset | Mduflush), .clock(clock), .in(next_EX_MEM), .out(EX_MEM) );
    PipelineRegister MEMWB(.reset(reset), .clock(clock), .in(next_MEM_WB), .out(MEM_WB));
    ForwardingUnit FU(.IF_ID(IF_ID), .ID_EX(ID_EX), .EX_MEM(EX_MEM), .MEM_WB(MEM_WB), .next_ID_EX(next_ID_EX), .rs_val_in_EX(rs_val_in_EX), .rt_val_in_EX(rt_val_in_EX), .rs_val_in_ID(rs_val_in_ID), .rt_val_in_ID(rt_val_in_ID));
    
    //IF-------------------------------------------------------------------------------------------------------
    wire [31:0] pc;
    Instruction  instr;
    assign instr = insmem[next_IF_ID.pc[11:2]];
    ProgramCounter PC( .reset(reset), .clock(clock), .jumpInput(nextpc), .pcValue(pc) );
    always_comb begin
        next_IF_ID = 0;
        next_IF_ID.instr = instr;
        if (flush | Mduflush) next_IF_ID.pc = IF_ID.pc;//    STALL
        else next_IF_ID.pc = pc;
    end
    
    //ID-------------------------------------------------------------------------------------------------------
    Controller ctrl;
    wire[31:0] rsval, rtval, pc_4;
    wire[3:0] ALU_sig;
    wire gobranch;
    assign pc_4 =  IF_ID.pc + 4;
    ControllerUnit CU(.instr(IF_ID.instr), .ctrl(ctrl));
    GeneralPurposeRegisters GPR( .fd(fd), .reset(reset), .clock(clock), .pcValue(MEM_WB.pc), .rs(next_ID_EX.rs), .rt(next_ID_EX.rt), .rd(MEM_WB.rd), .writeEnabled(MEM_WB.ctrl.regwrite), .writeInput(MEM_WB.rd_val), .regval1(rsval), .regval2(rtval));
    ALUControl ALUC(.ALU_op(next_ID_EX.ctrl.ALU_op), .func(IF_ID.instr.R.func), .ctrl(ALU_sig));    
    BranchUnit BU( .in1(next_ID_EX.rs_val), .in2(next_ID_EX.rt_val), .ALU_op(next_ID_EX.ctrl.ALU_op), .out(gobranch) ); 
    always_comb begin
        next_ID_EX = IF_ID;
        next_ID_EX.ctrl = ctrl;
        next_ID_EX.imm_val = {{16{IF_ID.instr.I.imm[15] & next_ID_EX.ctrl.immsign}}, IF_ID.instr.I.imm};
        next_ID_EX.ALU_sig = ALU_sig;
        if(next_ID_EX.ctrl.writera == 1) begin
            if(next_ID_EX.ctrl.jumpreg == 1) begin
                next_ID_EX.rs = IF_ID.instr.R.rs;
                next_ID_EX.rt = 0;
                next_ID_EX.rd = IF_ID.instr.R.rd;
            end 
            else begin
                next_ID_EX.rs = 0;
                next_ID_EX.rt = 0;
                next_ID_EX.rd = 5'b11111;
            end 
        end
        else if(next_ID_EX.ctrl.shift == 1) begin
            next_ID_EX.rs = IF_ID.instr.R.rt;
            next_ID_EX.rt = IF_ID.instr.R.rs;
            next_ID_EX.rd = IF_ID.instr.R.rd;
        end
        else if(next_ID_EX.ctrl.regdst == 1 || next_ID_EX.ctrl.memwrite == 1 || next_ID_EX.ctrl.branch || next_ID_EX.ctrl.Mdu_start) begin
            next_ID_EX.rs = IF_ID.instr.R.rs;
            next_ID_EX.rt = IF_ID.instr.R.rt;
            next_ID_EX.rd = IF_ID.instr.R.rd;
        end
        else begin
            next_ID_EX.rs = IF_ID.instr.R.rs;
            next_ID_EX.rt = 0;
            next_ID_EX.rd = IF_ID.instr.R.rt;
        end
        
        case (rs_val_in_ID)
            2'b00: next_ID_EX.rs_val = rsval;
            2'b01: next_ID_EX.rs_val = MEM_WB.rd_val;
            2'b10: next_ID_EX.rs_val = EX_MEM.rd_val; 
            2'b11: next_ID_EX.rs_val = ID_EX.rd_val; 
        endcase
        case (rt_val_in_ID)
            2'b00: next_ID_EX.rt_val = rtval;
            2'b01: next_ID_EX.rt_val = MEM_WB.rd_val;
            2'b10: next_ID_EX.rt_val = EX_MEM.rd_val; 
            2'b11: next_ID_EX.rt_val = ID_EX.rd_val; 
        endcase
        
        if(flush | Mduflush) nextpc = next_IF_ID.pc + 4;
        else if(next_ID_EX.ctrl.jump) begin
            if(next_ID_EX.ctrl.jumpreg) nextpc = next_ID_EX.rs_val;
            else nextpc = {pc_4[31:28], IF_ID.instr.J.addr[25:0], 2'b00};
        end
        else if (next_ID_EX.ctrl.branch & gobranch) nextpc = IF_ID.pc + 4 + {next_ID_EX.imm_val[29:0],2'b00};
        else nextpc = next_IF_ID.pc + 4;
    end
    
    //EX-------------------------------------------------------------------------------------------------------
    wire [31:0] ALU_res, Mdu_res;
    wire ALU_zero;
    ALU ALU( .pc(ID_EX.pc), .A(next_EX_MEM.rs_val), .B(next_EX_MEM.ALU_input), .overflow(ID_EX.ctrl.overflow), .ctrl(ID_EX.ALU_sig), .C(ALU_res), .zero(ALU_zero) );
    MultiplicationDivisionUnit MDU(.reset(reset), .clock(clock), .operand1(next_EX_MEM.rs_val), .operand2(next_EX_MEM.ALU_input), .operation(ID_EX.ctrl.Mdu_op), .start(ID_EX.ctrl.Mdu_start & (ID_EX.ctrl.Mdu_op > MDU_WRITE_LO)), .busy(Mdubusy), .dataRead(Mdu_res));
    always_comb begin
        next_EX_MEM = ID_EX;
        next_EX_MEM.ALU_zero = ALU_zero;
        case (rs_val_in_EX)
            2'b00: next_EX_MEM.rs_val = ID_EX.rs_val;
            2'b01: next_EX_MEM.rs_val = MEM_WB.rd_val;
            2'b10: next_EX_MEM.rs_val = EX_MEM.rd_val; 
        endcase
        case (rt_val_in_EX)
            2'b00: next_EX_MEM.rt_val = ID_EX.rt_val;
            2'b01: next_EX_MEM.rt_val = MEM_WB.rd_val;
            2'b10: next_EX_MEM.rt_val = EX_MEM.rd_val; 
        endcase
        
        if(ID_EX.ctrl.ALU_src) begin
            if(ID_EX.ctrl.imm2reg) next_EX_MEM.ALU_input = {ID_EX.instr.I.imm, 16'b0};
            else next_EX_MEM.ALU_input = ID_EX.imm_val;
        end
        else if(ID_EX.ctrl.shift) begin
            if(ID_EX.ctrl.shift_sa) next_EX_MEM.ALU_input = ID_EX.instr.R.padding;
            else next_EX_MEM.ALU_input = {{27{(1'b0)}}, next_EX_MEM.rt_val[4:0]};
        end
        else if(ID_EX.ctrl.writera) next_EX_MEM.ALU_input = ID_EX.pc + 8 - next_EX_MEM.rs_val;
        else next_EX_MEM.ALU_input = next_EX_MEM.rt_val;
        
        if(ID_EX.ctrl.Mdu_start & (ID_EX.ctrl.Mdu_op < MDU_WRITE_HI)) next_EX_MEM.rd_val = Mdu_res;
        else  next_EX_MEM.rd_val = ALU_res;
    end
    
    //MEM------------------------------------------------------------------------------------------------------
    wire [31:0] readResult;
    DataMemory DM(.fd(fd), .reset(reset), .clock(clock), .pcValue(EX_MEM.pc), .address(EX_MEM.rd_val), .writeEnabled(EX_MEM.ctrl.memwrite), .writeInput(EX_MEM.rt_val), .ctrl(EX_MEM.ctrl), .readResult(readResult));
    always_comb begin
        next_MEM_WB = EX_MEM;
        if(EX_MEM.ctrl.mem2reg) next_MEM_WB.rd_val = readResult;
    end
    
    //WB-------------------------------------------------------------------------------------------------------
    always @(posedge clock) begin
        if(MEM_WB.instr.R.op == 6'b000000 && MEM_WB.instr.R.func == 6'b001100) begin
            $fclose(fd);
            $finish; //syscall
        end
    end
endmodule
