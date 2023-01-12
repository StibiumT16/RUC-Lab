`ifndef MYCPU
`define MYCPU
`include "MultiplicationDivisionUnit.sv"

typedef struct packed{
    logic regdst;
    logic jump;
    logic branch;
    logic memread;
    logic mem2reg;
    logic [3:0] ALU_op;
    logic imm2reg;
    logic jumpreg;
    logic writera;
    logic memwrite;
    logic ALU_src;
    logic regwrite;
    logic immsign;
    logic shift_sa;
    logic shift;
    mdu_operation_t Mdu_op;
    logic Mdu_start;
    logic [1:0] mem_bit;
    logic memsign;
    logic overflow;
}Controller;

typedef struct packed {
    logic [5:0] op;
    logic [4:0] rs;
    logic [4:0] rt;
    logic [4:0] rd;
    logic [4:0] padding;
    logic [5:0] func;
}R_form;

typedef struct packed {
    logic [5:0] op;
    logic [4:0] rs;
    logic [4:0] rt;
    logic [15:0] imm;
}I_form;

typedef struct packed {
    logic [5:0] op;
    logic [25:0] addr;
}J_form;

typedef union packed {
    R_form R;
    I_form I;
    J_form J;
}Instruction;

typedef struct packed {
    Instruction instr;
    Controller ctrl;
    logic [3:0] ALU_sig;
    logic ALU_zero;
    logic [31:0] pc;
    logic [31:0] rs_val, rt_val, rd_val;
    logic [31:0] imm_val;
    logic [31:0] ALU_input;
    logic [4:0] rs, rt, rd;
}PipelineReg;

`endif