`ifndef RAM
`define RAM

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
    // Address trimmed to word-aligned depth
    wire [10:0] addr = address_in[31:2];

    logic [31:0] ram_write_data;
    logic [31:0] ram_read_data;

    logic        wr_en;
    logic        clk_en;
    logic        sel_d;
    logic        ready;

    // emulate toggled ready signal
    always_ff @(posedge clk) begin
        if (reset)
            ready <= 0;
        else if (sel_in)
            ready <= ~ready;
        else
            ready <= 0;
    end

    assign ready_out = sel_in ? ready : 0;

    // latch select to sync read output
    always_ff @(posedge clk) begin
        sel_d <= sel_in;
    end

    assign read_value_out = sel_d ? ram_read_data : 32'b0;

    // byte-wise write emulation with inverted endian format
    always_comb begin
        ram_write_data = 32'h00000000;
        if (write_mask_in[3]) ram_write_data[7:0]   = write_value_in[31:24];
        if (write_mask_in[2]) ram_write_data[15:8]  = write_value_in[23:16];
        if (write_mask_in[1]) ram_write_data[23:16] = write_value_in[15:8];
        if (write_mask_in[0]) ram_write_data[31:24] = write_value_in[7:0];
    end

    assign wr_en  = sel_in && (|write_mask_in);
    assign clk_en = sel_in;

    progmem_ram u_ram (
        .wr_clk_i      (clk),
        .rd_clk_i      (clk),
        .rst_i         (reset),
        .wr_clk_en_i   (clk_en),
        .rd_clk_en_i   (clk_en),
        .wr_en_i       (wr_en),
        .wr_data_i     (ram_write_data),
        .wr_addr_i     (addr),
        .rd_addr_i     (addr),
        .rd_en_i       (sel_in),
        .rd_data_o     (ram_read_data)
    );

endmodule

`endif
