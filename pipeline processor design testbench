module tb;

reg clk = 0;
reg reset = 1;

PipelinedCPU cpu (.clk(clk), .reset(reset));

// Clock
always #5 clk = ~clk;

initial begin
    // Reset the CPU
    #10 reset = 0;

    // Initialize instruction memory
    // Encoding: [15:12] opcode, [14:12] rd, [11:9] rs1, [8:6] rs2
    // ADD R1, R2, R3 ==> 0001_001_010_011
    cpu.instruction_mem[0] = 16'b0001001010011;

    // SUB R4, R1, R2 ==> 0010_100_001_010
    cpu.instruction_mem[1] = 16'b0010100001010;

    // LOAD R5, addr ==> 0011_101_000_000 (addr in rs2 field)
    cpu.instruction_mem[2] = 16'b0011101000000;

    // Additional test instructions...

    // Let the clock run
    #100 $finish;
end

endmodule
