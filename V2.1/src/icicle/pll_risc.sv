module pll_risc (
    input  wire ref_clk_i,
    output wire out_clk,
    output wire locked
);

SB_PLL40_CORE #(
    .FEEDBACK_PATH("SIMPLE"),
    .DIVR(4'd0),        // Divide by 1
    .DIVF(7'd19),       // Multiply by 64
    .DIVQ(3'd2),        // Divide by 4 â†’ 12 MHz * 64 / 4 = 192 MHz
    .FILTER_RANGE(3'd1)
) pll_inst (
    .REFERENCECLK(ref_clk_i),
    .PLLOUTCORE(out_clk),
    .LOCK(locked),
    .BYPASS(1'b0),
    .RESETB(1'b1)
);

endmodule
