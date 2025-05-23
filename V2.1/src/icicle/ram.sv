module ram #(
    parameter SIZE = 2048
) (
    input clk,
    input reset,

    // memory bus
    input [31:0] address_in,
    input sel_in,
    output logic [31:0] read_value_out,
    input [3:0] write_mask_in,
    input [31:0] write_value_in,
    output logic ready_out
);

    wire        wr_en    = sel_in && (|write_mask_in);
    wire        clk_en   = sel_in;
    wire [10:0] addr     = address_in[31:2];

    progmem_ram u_ram (
        .wr_clk_i      (clk),
        .rd_clk_i      (clk),
        .rst_i         (reset),
        .wr_clk_en_i   (clk_en),
        .rd_clk_en_i   (clk_en),
        .wr_en_i       (wr_en),
        .wr_data_i     (write_value_in),
        .wr_addr_i     (addr),
        .rd_addr_i     (addr),
        .rd_en_i       (sel_in),
        .rd_data_o     (read_value_out)
    );

    assign ready_out = sel_in;

endmodule
