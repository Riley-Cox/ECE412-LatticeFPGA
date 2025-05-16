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
    // infer a single-port SPRAM on UP5K
    (* ramstyle = "block", syn_ramstyle = "block" *)
    logic [31:0] regs [0:31];

    logic [4:0] rs1, rs2;

    // two-port read (registered address)
    assign rs1_value_out = regs[rs1];
    assign rs2_value_out = regs[rs2];

    always_ff @(posedge clk) begin
        if (!stall_in) begin
            rs1 <= rs1_in;
            rs2 <= rs2_in;
        end

        // write-back on MEM→WB
        if (!writeback_flush_in && rd_write_in && |rd_in)
            regs[rd_in] <= rd_value_in;
    end
endmodule

`endif
