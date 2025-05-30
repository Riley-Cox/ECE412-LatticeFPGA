`include "sync.sv"
`define INTERNAL_OSC

module top (
    /* LEDs */
    output logic [5:0] leds,

    /* UART */
    input uart_rx,
    output logic uart_tx,

    /* SPI Controller */
    output logic spi_clk,
    output logic spi_mosi,
    output logic spi_cs_n,
    output logic lcd_dc,

    /* Button */
    input bit brightPush,
    input bit colorPush,
    output logic screenPower,

    /* ssr */
    (* PULLUP *) input logic r,

    /* Debug outputs */
    output logic dbg_reset_release,
    output logic dbg_reset,
    output logic dbg_greset,
    output logic dbg_pll_locked,

    /* PC monitor */
    output logic [3:0] pc_top,
	output logic pcgen_stall_debug,
	output logic instr_ready_debug,
	output logic overwrite_pc_debug,
	output logic [3:0] next_pc_debug


);

logic pll_clk, clk;
logic pll_locked_async, pll_locked;
logic [3:0] reset_count;
logic reset_release = 0;
logic reset, greset;

`ifdef INTERNAL_OSC
    HSOSC #(.CLKHF_DIV("0b01")) inthosc (
        .CLKHFPU(1'b1),
        .CLKHFEN(1'b1),
        .CLKHF(clk)
    );
`endif

pll_gen u_pll (
    .ref_clk_i(clk),
    .rst_n_i(1'b1),
    .outcore_o(pll_clk),
    .lock_o(pll_locked_async)
);

sync sync (
    .clk(pll_clk),
    .in(pll_locked_async),
    .out(pll_locked)
);

// SSR rising edge detection for 1-cycle pulse
logic r_sync_0 = 0, r_sync_1 = 0;
always_ff @(posedge pll_clk) begin
    r_sync_0 <= r;
    r_sync_1 <= r_sync_0;
end

wire ssr_rising = r_sync_0 & ~r_sync_1;

assign greset = reset | ssr_rising;

// Reset logic
always_ff @(posedge pll_clk) begin
    if (!reset_release) begin
        if (pll_locked) begin
            reset_count <= reset_count + 1;
            if (&reset_count)
                reset_release <= 1;
        end else begin
            reset_count <= 0;
        end
    end
end

assign reset = ~reset_release;

// Debug outputs
assign dbg_reset_release = reset_release;
assign dbg_reset         = reset;
assign dbg_greset        = greset;
assign dbg_pll_locked    = pll_locked;

// Core PC tap
logic [31:0] icicle_pc;
assign pc_top = icicle_pc[31:28];

// Core instantiation
icicle icicle (
    .clk(pll_clk),
    .reset(greset),
    .leds(),
    .uart_rx(uart_rx),
    .uart_tx(uart_tx),
    .spi_clk(spi_clk),
    .spi_mosi(spi_mosi),
    .spi_cs_n(spi_cs_n),
    .lcd_dc(lcd_dc),
    .brightPush(~brightPush),
    .colorPush(~colorPush),
    .screenPower(screenPower),
    .test_out(),
    .dbg_pc(icicle_pc),
	.pcgen_stall_debug(pcgen_stall_debug),
	.instr_ready_debug(instr_ready_debug),
	.overwrite_pc_debug(overwrite_pc_debug),
    .next_pc_debug(next_pc_debug) 
);

endmodule
