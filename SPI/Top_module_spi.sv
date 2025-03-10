`timescale 1ns/100ps

`ifndef SB_GB_V
`define SB_GB_V

module SB_GB (
  input  USER_SIGNAL_TO_GLOBAL_BUFFER,
  output GLOBAL_BUFFER_OUTPUT
);
  // For simulation, simply pass the input to the output.
  assign GLOBAL_BUFFER_OUTPUT = USER_SIGNAL_TO_GLOBAL_BUFFER;
endmodule

`endif

module Top_module_spi (
  //input  clk,       // External clock (e.g. 16 MHz)
  input  reset_n,   // Active low reset
  // SPI and LCD interface outputs 
  output spi_clk,
  output spi_mosi,
  output spi_cs_n,
  output lcd_dc,
  output test_out
);

wire clk_buf;
wire clk;

HSOSC
#(
.CLKHF_DIV ("0b11")
// 0b00=48MHz(default), 0b01=24MHz, 0b10=12MHz, 0b11=6MHz
) u_HSOSC (
.CLKHFPU (1'b1), // I: power up, active high
.CLKHFEN (1'b1), // I: output enable, active high
.CLKHF (clk_buf) // O: high speed clock output
);

SB_GB clk_buf_inst (
  .USER_SIGNAL_TO_GLOBAL_BUFFER(clk_buf),
  .GLOBAL_BUFFER_OUTPUT(clk)
);

assign test_out = clk;
//logic reset_n;
/**
initial begin
  reset_n = 1'b0;  // assert reset
  #500;            // hold reset for 100 time units
  reset_n = 1'b1;  // deassert reset
end
**/

  // Internal control signals for the SPI controller
   (* syn_preserve = "1" *) reg        spi_start_reg;
   (* syn_preserve = "1" *) reg [7:0]  spi_data_in_reg;
   (* syn_preserve = "1" *) reg        spi_dc_reg;
  wire       spi_busy;
  wire       spi_done;

  // Instantiate the SPI controller (DUT)
  spi_controller #(
    .SPI_DIV(4)
  ) dut (
    .clk(clk),
    .reset_n(reset_n),
    .spi_start(spi_start_reg),
    .spi_data_in(spi_data_in_reg),
    .spi_dc(spi_dc_reg),
    .spi_busy(spi_busy),
    .spi_done(spi_done),
    .spi_clk(spi_clk),
    .spi_mosi(spi_mosi),
    .spi_cs_n(spi_cs_n),
    .lcd_dc(lcd_dc)
  );

  // FSM state definitions for the test transactions
  localparam STATE_IDLE   = 3'd0,
             STATE_DELAY  = 3'd1,
             STATE_TRANS1 = 3'd2,
             STATE_WAIT1  = 3'd3,
             STATE_TRANS2 = 3'd4,
             STATE_WAIT2  = 3'd5,
             STATE_DONE   = 3'd6;

  (* syn_preserve = "1" *) reg [2:0] state;
  (* syn_preserve = "1" *) reg [15:0] delay_counter;  // Adjust width and max count as needed

  // FSM: drives two transactions in sequence.
  // Transaction 1 sends 8'hA5 with spi_dc = 1, and Transaction 2 sends 8'h3C with spi_dc = 0.

  always @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state           <= STATE_IDLE;
      delay_counter   <= 16'd0;
      spi_start_reg   <= 1'b0;
      spi_data_in_reg <= 8'h00;
      spi_dc_reg      <= 1'b0;
    end else begin
      case (state)
        STATE_IDLE: begin
          // Wait a few clock cycles after reset deassertion
          delay_counter <= 16'd0;
          state <= STATE_DELAY;
        end

        STATE_DELAY: begin
          // Simple delay counter 
          if (delay_counter < 16'd10) begin
            delay_counter <= delay_counter + 16'd1;
          end else begin
            // Start Transaction 1: send 8'hA5 with dc = 1
            spi_data_in_reg <= 8'hA5;
            spi_dc_reg      <= 1'b1;
            spi_start_reg   <= 1'b1;  // Assert start for one clock cycle
            state <= STATE_TRANS1;
          end
        end

        STATE_TRANS1: begin
          // Deassert spi_start (one-cycle pulse)
          spi_start_reg <= 1'b0;
          state <= STATE_WAIT1;
        end

        STATE_WAIT1: begin
          // Wait here until the SPI controller asserts done for Transaction 1
          if (spi_done)
            state <= STATE_TRANS2;
        end

        STATE_TRANS2: begin
          // Begin Transaction 2: send 8'h3C with dc = 0
          spi_data_in_reg <= 8'h3C;
          spi_dc_reg      <= 1'b0;
          spi_start_reg   <= 1'b1;
          state <= STATE_WAIT2;
        end

        STATE_WAIT2: begin
          // Deassert spi_start and wait until Transaction 2 completes
          spi_start_reg <= 1'b0;
          if (spi_done)
            state <= STATE_DONE;
        end

        STATE_DONE: begin
          // Remain in DONE state
          state <= STATE_DONE;
        end

        default: state <= STATE_DONE;
      endcase
    end
  end

endmodule
