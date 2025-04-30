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
 (*PULLUP*)    input bit brightPush,
 (*PULLUP*)    input bit colorPush,
    output logic screenPower,

    //Test signal
    output bit test_out

);

	
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
        .uart_tx(uart_tx),
		
		/* SPI Controller */
		.spi_clk(spi_clk),
		.spi_mosi(spi_mosi),
		.spi_cs_n(spi_cs_n),
		.lcd_dc(lcd_dc),
		//.changeOut(changeOut),
		//.color_start(color_start),
	/* Button */
	.brightPush(~brightPush),
	.colorPush(~colorPush),
	.screenPower(screenPower),
	.test_out(test_out)

    );

endmodule
