module PipelinedCPU (
    input clk,
    input reset
);

// ---------------------
// 1. IF STAGE
// ---------------------
reg [15:0] PC;
wire [15:0] instruction;
reg [15:0] IF_ID_Instruction;

reg [15:0] instruction_mem [0:255]; // Simple instruction memory
assign instruction = instruction_mem[PC[15:1]];

always @(posedge clk) begin
    if (reset) PC <= 0;
    else PC <= PC + 2;
    IF_ID_Instruction <= instruction;
end

// ---------------------
// 2. ID STAGE
// ---------------------
reg [15:0] ID_EX_Instruction;
wire [2:0] rs1 = IF_ID_Instruction[11:9];
wire [2:0] rs2 = IF_ID_Instruction[8:6];
wire [2:0] rd  = IF_ID_Instruction[14:12];
wire [15:0] reg_data1, reg_data2;

RegisterFile reg_file (
    .clk(clk),
    .we(WB_we),
    .ra1(rs1),
    .ra2(rs2),
    .wa(WB_rd),
    .wd(WB_data),
    .rd1(reg_data1),
    .rd2(reg_data2)
);

reg [15:0] ID_EX_data1, ID_EX_data2;
reg [2:0] ID_EX_rs1, ID_EX_rs2, ID_EX_rd;

always @(posedge clk) begin
    ID_EX_Instruction <= IF_ID_Instruction;
    ID_EX_data1 <= reg_data1;
    ID_EX_data2 <= reg_data2;
    ID_EX_rs1 <= rs1;
    ID_EX_rs2 <= rs2;
    ID_EX_rd <= rd;
end

// ---------------------
// 3. EX STAGE
// ---------------------
reg [15:0] EX_WB_result;
reg [2:0] EX_WB_rd;
reg EX_WB_we;

always @(posedge clk) begin
    case (ID_EX_Instruction[15:12]) // opcode
        4'b0001: EX_WB_result <= ID_EX_data1 + ID_EX_data2; // ADD
        4'b0010: EX_WB_result <= ID_EX_data1 - ID_EX_data2; // SUB
        4'b0011: EX_WB_result <= instruction_mem[ID_EX_data2]; // LOAD
    endcase
    EX_WB_rd <= ID_EX_rd;
    EX_WB_we <= 1'b1;
end

// ---------------------
// 4. WB STAGE
// ---------------------
wire WB_we = EX_WB_we;
wire [2:0] WB_rd = EX_WB_rd;
wire [15:0] WB_data = EX_WB_result;

endmodule# Pipeline-processor-design
