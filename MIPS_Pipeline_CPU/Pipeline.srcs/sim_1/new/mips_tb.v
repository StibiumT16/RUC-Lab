module mips_tb;

reg reset;
reg clock;

integer k;

TopLevel mips(.reset(reset), .clock(clock));

initial begin
    reset = 1;

    clock = 0;
    #1;
    clock = 1;
    #1;
    clock = 0;
    #1;
    
    reset = 0;
    #1;
    
    #1;

    for (k = 0; ; k = k + 1) begin
        clock = 1;
        #5;
        clock = 0;
        #5;
	$display("%%clock: %d", k);
    end

    $finish;
end
    
endmodule
