

module riscv_pll_ipgen_lscc_pll #(
    parameter DIVR = "0",
    parameter DIVF = "4",
    parameter DIVQ = "4",
    parameter FILTER_RANGE = "1",
    parameter FREQUENCY_PIN_REFERENCECLK = "12.000000",
    parameter FEEDBACK_PATH = "DELAY",
    parameter DELAY_ADJUSTMENT_MODE_FEEDBACK = "FIXED",
    parameter FDA_FEEDBACK = "0",
    parameter DELAY_ADJUSTMENT_MODE_RELATIVE = "FIXED",
    parameter FDA_RELATIVE = "0",
    parameter SHIFTREG_DIV_MODE = "0",
    parameter PLLOUT_SELECT_PORTA = "GENCLK",
    parameter PLLOUT_SELECT_PORTB = "GENCLK",
    parameter EXTERNAL_DIVIDE_FACTOR = "NONE",
    parameter DELAY_PORT_WIDTH = 4,
    parameter ENABLE_ICEGATE_PORTA = "0",
    parameter ENABLE_ICEGATE_PORTB = "0"
)(
    input  wire ref_clk_i,
    input  wire rst_n_i,
    input  wire feedback_i,
    input  wire [DELAY_PORT_WIDTH-1:0] dynamic_delay_i,
    input  wire bypass_i,
    input  wire latch_i,
    output wire lock_o,
    output wire outcore_o,
    output wire outglobal_o,
    output wire outcoreb_o,
    output wire outglobalb_o,
    input  wire sclk_i,
    input  wire sdi_i,
    output wire sdo_o
);

    wire [7:0] dynamic_delay_w;
    wire feedback_w;
    wire intfbout_w;

    generate
        if (EXTERNAL_DIVIDE_FACTOR != "NONE") begin
            assign feedback_w = feedback_i;
        end else begin
            assign feedback_w = intfbout_w;
        end
    endgenerate

    generate
        if ((DELAY_ADJUSTMENT_MODE_FEEDBACK == "FIXED") && (DELAY_ADJUSTMENT_MODE_RELATIVE == "FIXED")) begin
            assign dynamic_delay_w = 8'b0;
        end else if ((DELAY_ADJUSTMENT_MODE_FEEDBACK == "DYNAMIC") && (DELAY_ADJUSTMENT_MODE_RELATIVE == "DYNAMIC")) begin
            assign dynamic_delay_w = dynamic_delay_i;
        end else if ((DELAY_ADJUSTMENT_MODE_FEEDBACK == "FIXED") && (DELAY_ADJUSTMENT_MODE_RELATIVE == "DYNAMIC")) begin
            assign dynamic_delay_w = {dynamic_delay_i[3:0], 4'b0};
        end else begin
            assign dynamic_delay_w = {4'b0, dynamic_delay_i[3:0]};
        end
    endgenerate

    PLL_B #(
        .DIVR(DIVR),
        .DIVF(DIVF),
        .DIVQ(DIVQ),
        .FEEDBACK_PATH(FEEDBACK_PATH),
        .FILTER_RANGE(FILTER_RANGE),
        .DELAY_ADJUSTMENT_MODE_FEEDBACK(DELAY_ADJUSTMENT_MODE_FEEDBACK),
        .FDA_FEEDBACK(FDA_FEEDBACK),
        .DELAY_ADJUSTMENT_MODE_RELATIVE(DELAY_ADJUSTMENT_MODE_RELATIVE),
        .FDA_RELATIVE(FDA_RELATIVE),
        .SHIFTREG_DIV_MODE(SHIFTREG_DIV_MODE),
        .PLLOUT_SELECT_PORTA(PLLOUT_SELECT_PORTA),
        .PLLOUT_SELECT_PORTB(PLLOUT_SELECT_PORTB),
        .EXTERNAL_DIVIDE_FACTOR(EXTERNAL_DIVIDE_FACTOR),
        .ENABLE_ICEGATE_PORTA(ENABLE_ICEGATE_PORTA),
        .ENABLE_ICEGATE_PORTB(ENABLE_ICEGATE_PORTB),
        .FREQUENCY_PIN_REFERENCECLK(FREQUENCY_PIN_REFERENCECLK)
    ) u_PLL_B (
        .REFERENCECLK(ref_clk_i),
        .RESET_N(rst_n_i),
        .FEEDBACK(feedback_w),
        .DYNAMICDELAY7(dynamic_delay_w[7]),
        .DYNAMICDELAY6(dynamic_delay_w[6]),
        .DYNAMICDELAY5(dynamic_delay_w[5]),
        .DYNAMICDELAY4(dynamic_delay_w[4]),
        .DYNAMICDELAY3(dynamic_delay_w[3]),
        .DYNAMICDELAY2(dynamic_delay_w[2]),
        .DYNAMICDELAY1(dynamic_delay_w[1]),
        .DYNAMICDELAY0(dynamic_delay_w[0]),
        .INTFBOUT(intfbout_w),
        .BYPASS(bypass_i),
        .LATCH(latch_i),
        .OUTCORE(outcore_o),
        .OUTGLOBAL(outglobal_o),
        .OUTCOREB(outcoreb_o),
        .OUTGLOBALB(outglobalb_o),
        .LOCK(lock_o),
        .SCLK(sclk_i),
        .SDI(sdi_i),
        .SDO(sdo_o)
    );

endmodule
