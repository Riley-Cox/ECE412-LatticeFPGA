`include "defines.sv"
`include "icicle.sv"
`include "pll.sv"
`include "sync.sv"

module top (

    /* LEDs */
    output logic [7:0] leds,

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

    //Test signal
    output bit test_out

);


logic reset, greset, ssr, q, qBar;

always_ff @(posedge pll_clk) begin
	q <= r;
end
assign qBar = ~q;
assign ssr = qBar & r;	
	

assign greset = reset | ssr;




	
`ifdef INTERNAL_OSC
    logic clk;

    SB_HFOSC inthosc (
	.CLKHFPU(1'b1),
	.CLKHFEN(1'b1),
	.CLKHF(clk)
    );
`endif

    logic pll_clk;
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

pll_risc u_pll (
    .ref_clk_i(clk),       // 12 MHz input clock
    .out_clk(pll_clk),     // 192 MHz output
    .locked(pll_locked_async)    // PLL lock status
);

    logic pll_locked;

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
        .reset(greset),

        /* LEDs */
        .leds(leds),

        /* UART */
        .uart_rx(uart_rx),
        .uart_tx(uart_tx),
		
	/* SPI Controller */
	.spi_clk(spi_clk),
	.spi_mosi(spi_mosi),
	.spi_cs_n(spi_cs_n),
	.lcd_dc(lcd_dc),

	/* Button */
	.brightPush(~brightPush),
	.colorPush(~colorPush),
	.screenPower(screenPower),
	.test_out(test_out)

    );

endmodule
