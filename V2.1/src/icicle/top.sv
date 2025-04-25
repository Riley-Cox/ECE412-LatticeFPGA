`include "defines.sv"
//`include "icicle.sv"
//`include "pll.sv"
`include "sync.sv"

module top (

    /* LEDs */
    output logic [7:0] leds,

    /* UART */
    input uart_rx,
    output logic uart_tx
	
);

    logic clk;

    HSOSC #(.CLKHF_DIV ("0b01")) inthosc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(clk));


   (* keep *) logic pll_clk;
    logic pll_locked_async;
/**
    pll pll (
`ifdef ECP5
        .clki(clk),
        .clko(pll_clk),
`else
        .clock_in(clk),
        .clock_out(pll_clk),
`endif
        .locked(pll_locked_async)
    );
**/
logic unused_clk;
/**
pll u_pll (
    .ref_clk_i(clk),     // 12 MHz input from HFOSC
    .rst_n_i(1'b1),            // Active-low reset; hold high for normal operation
    .lock_o(pll_locked_async),       // Indicates when PLL is stable
    .outcore_o(unused_clk),       
	.outglobal_o(pll_clk)
);
**/


    riscv_pll_ipgen_lscc_pll lscc_pll_inst (
        .ref_clk_i(clk),
        .rst_n_i(1'b1),
        .feedback_i(1'b0),
        .dynamic_delay_i(4'b0000),
        .bypass_i(1'b0),
        .latch_i(1'b0),
        .lock_o(pll_locked_async),
        .outcore_o(),
        .outglobal_o(pll_clk),
        .outcoreb_o(),
        .outglobalb_o(),
        .sclk_i(),
        .sdi_i(),
        .sdo_o()
    );

    logic pll_locked;
    logic reset;

    logic [3:0] reset_count = '0;


    always_ff @(posedge pll_clk) begin
        if (&reset_count) begin
            if (pll_locked) begin
                reset <= 0;
            end else begin
                reset <= 1;
                reset_count <= 0;
            end
        end else begin
            reset <= 1;
            reset_count <= reset_count + pll_locked;
        end
    end

    sync sync (
        .clk(pll_clk),
        .in(pll_locked_async),
        .out(pll_locked)
    );

    icicle icicle (
        .clk(pll_clk),
        .reset(reset),

        /* LEDs */
        .leds(leds),

        /* UART */
        .uart_rx(uart_rx),
        .uart_tx(uart_tx)
    );
endmodule
