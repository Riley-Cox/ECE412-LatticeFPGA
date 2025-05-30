module rv32_regs (
    input  logic        clk,
    input  logic        stall_in,
    input  logic        writeback_flush_in,

    input  logic [4:0]  rs1_in,
    input  logic [4:0]  rs2_in,
    input  logic [4:0]  rd_in,
    input  logic        rd_write_in,
    input  logic [31:0] rd_value_in,

    output logic [31:0] rs1_value_out,
    output logic [31:0] rs2_value_out
);

    logic [4:0] rs1, rs2;

    // Address capture
    always_ff @(posedge clk) begin
        if (!stall_in) begin
            rs1 <= rs1_in;
            rs2 <= rs2_in;
        end
    end

    logic ram_write_en;
    assign ram_write_en = (!writeback_flush_in && rd_write_in && (rd_in != 5'd0));

    // Internal wires for raw RAM outputs
    logic [31:0] rs1_raw;
    logic [31:0] rs2_raw;

    // Instantiate RAM primitive for read port 1
    ram_32x regsA (
        .wr_clk_i(clk),
        .rd_clk_i(clk),
        .rst_i(1'b0),
        .wr_clk_en_i(1'b1),
        .rd_en_i(1'b1),
        .rd_clk_en_i(1'b1),
        .wr_en_i(ram_write_en),
        .wr_data_i(rd_value_in),
        .wr_addr_i(rd_in),
        .rd_addr_i(rs1),
        .rd_data_o(rs1_raw)
    );

    // Instantiate RAM primitive for read port 2
    ram_32x regsB (
        .wr_clk_i(clk),
        .rd_clk_i(clk),
        .rst_i(1'b0),
        .wr_clk_en_i(1'b1),
        .rd_en_i(1'b1),
        .rd_clk_en_i(1'b1),
        .wr_en_i(ram_write_en),
        .wr_data_i(rd_value_in),
        .wr_addr_i(rd_in),
        .rd_addr_i(rs2),
        .rd_data_o(rs2_raw)
    );

    // Final output logic: x0 is always zero
    assign rs1_value_out = (rs1 == 5'd0) ? 32'b0 : rs1_raw;
    assign rs2_value_out = (rs2 == 5'd0) ? 32'b0 : rs2_raw;

endmodule
