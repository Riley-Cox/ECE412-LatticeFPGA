`ifndef RV32_REGS
`define RV32_REGS

module rv32_regs (
    input  logic        clk,
    input  logic        stall_in,
    input  logic        writeback_flush_in,

    /* control in */
    input  logic [4:0]  rs1_in,
    input  logic [4:0]  rs2_in,
    input  logic [4:0]  rd_in,
    input  logic        rd_write_in,

    /* data in */
    input  logic [31:0] rd_value_in,

    /* data out */
    output logic [31:0] rs1_value_out,
    output logic [31:0] rs2_value_out
);
    // 32-entry register file with initial zero content
    logic [31:0] regs [31:0] = '{default:32'h0};
    logic [4:0]  rs1;
    logic [4:0]  rs2;

    // Read ports (registered addresses)
    assign rs1_value_out = regs[rs1];
    assign rs2_value_out = regs[rs2];

    // Address pipelining and write-back
    always_ff @(posedge clk) begin
        if (!stall_in) begin
            rs1 <= rs1_in;
            rs2 <= rs2_in;
        end
        if (!writeback_flush_in && rd_write_in && |rd_in) begin
            regs[rd_in] <= rd_value_in;
        end
    end
endmodule

`endif
