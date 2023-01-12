`timescale 1ps / 1ps
`include "utils.sv"

module DataMemory( fd, reset, clock, pcValue, address, writeEnabled, writeInput, ctrl, readResult);
    input integer fd;
    input wire reset, clock, writeEnabled;
    input wire [31:0] pcValue, address, writeInput;
    input Controller ctrl;
    output wire[31:0] readResult;
    reg [31:0] data[2047:0], res;
    integer i;
    assign readResult = res;
    
    always_comb begin
        if(ctrl.memread == 1) begin
            case(ctrl.mem_bit) 
                2'b00: begin
                    if (address[1:0] != 2'b00) begin
                        $display("@%h: *%h SignalException AddressError", pcValue, address);
                        $finish;
                    end
                    res = data[address[12:2]];
                end
                2'b01: res = {{24{ctrl.memsign & data[address[12:2]][8*address[1:0]+7 +: 1]}}, data[address[12:2]][8*address[1:0] +: 8]};
                2'b10: begin
                    if (address[0] != 1'b0) begin
                        $display("@%h: *%h SignalException AddressError", pcValue, address);
                        $finish;
                    end
                    res = {{16{ctrl.memsign & data[address[12:2]][16*address[1]+15 +: 1]}}, data[address[12:2]][16*address[1] +: 16]};
                end
            endcase
        end
    end
    
    always_ff @(posedge clock ) begin
        if(reset == 1) begin 
            for (i = 0; i< 2048; i = i + 1)
                data[i] <= 0;
        end 
        else if (writeEnabled) begin
                case(ctrl.mem_bit) 
                    2'b00: begin
                        if (address[1:0] != 2'b00) begin
                            $display("@%h: *%h SignalException AddressError", pcValue, address);
                            $finish;
                        end
                        data[address[12:2]] = writeInput;
                    end
                    2'b01: data[address[12:2]][8*address[1:0] +: 8] = writeInput[7:0];
                    2'b10: begin
                        if (address[0] != 1'b0) begin
                            $display("@%h: *%h SignalException AddressError", pcValue, address);
                            $finish;
                        end
                        data[address[12:2]][16*address[1] +: 16] = writeInput[15:0];
                    end
                endcase
                $display("@%h: *%h <= %h", pcValue, {address[31:2], 2'b00}, data[address[12:2]]);
                $fdisplay(fd, "@%h: *%h <= %h", pcValue, {address[31:2], 2'b00}, data[address[12:2]]);
            end
    end
    
endmodule
