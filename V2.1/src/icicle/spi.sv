module spi_controller #(
  // Number of system clocks for one half period of SPI clock.
  parameter integer SPI_DIV = 4,
  parameter integer DIV_WIDTH = 2
)(
  input  logic         clk,         // system clock
  input  logic         reset_n,     // active-low reset

  // Processor interface:
  input  logic         spi_start,   // Pulse to start a transaction
  input  logic [7:0]   spi_data_in, // Data byte to send (command or data)
  input  logic         spi_dc,      // Data/command flag (0 = command, 1 = data)
  output logic         spi_busy,    // High while a transfer is in progress
  output logic         spi_done,    // One cycle pulse when transfer completes

  // SPI pins:
  output logic         spi_clk,     // SPI clock (mode 0: idle low)
  output logic         spi_mosi,    // SPI Master-Out Slave-In
  output logic         spi_cs_n,    // Active low chip select

  // Additional LCD control signal (e.g. Data/Command)
  output logic         lcd_dc       
);

  localparam  IDLE         = 2'b00,
              TRANSFER_LOW = 2'b01,
              TRANSFER_HIGH= 2'b10,
              FINISH       = 2'b11;

  logic [1:0] state;
  logic [DIV_WIDTH-1:0] counter;
  logic [3:0]           bit_cnt;
  logic [7:0]           shift_reg;
  logic                 lcd_dc_reg;
  logic                 start_latched;

  assign lcd_dc = lcd_dc_reg;

  // Latch the spi_start pulse
  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      start_latched <= 1'b0;
    end else begin
      if (state == IDLE && spi_start && !spi_busy)
        start_latched <= 1'b1;
      else if (state != IDLE)
        start_latched <= 1'b0;
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state       <= IDLE;
      spi_cs_n    <= 1'b1;
      spi_clk     <= 1'b0;
      spi_busy    <= 1'b0;
      spi_done    <= 1'b0;
      spi_mosi    <= 1'b0;
      counter     <= '0;
      bit_cnt     <= 4'd0;
      shift_reg   <= 8'd0;
      lcd_dc_reg  <= 1'b0;
    end else begin
      case (state)
        IDLE: begin
          spi_clk  <= 1'b0;
          spi_cs_n <= 1'b1;
          spi_done <= 1'b0;
          spi_busy <= 1'b0;

          if (start_latched && !spi_busy) begin
            shift_reg   <= spi_data_in;
            bit_cnt     <= 4'd8;
            lcd_dc_reg  <= spi_dc;
            spi_cs_n    <= 1'b0;
            spi_busy    <= 1'b1;
            counter     <= SPI_DIV - 1;
            state       <= TRANSFER_LOW;
          end
        end

        TRANSFER_LOW: begin
          spi_clk  <= 1'b0;
          spi_mosi <= shift_reg[7];
          if (counter == 0) begin
            spi_clk <= 1'b1;
            counter <= SPI_DIV - 1;
            state   <= TRANSFER_HIGH;
          end else begin
            counter <= counter - 1;
          end
        end

        TRANSFER_HIGH: begin
          spi_clk <= 1'b1;
          if (counter == 0) begin
            if (bit_cnt == 1) begin
              bit_cnt <= 4'd0;
              state   <= FINISH;
            end else begin
              bit_cnt   <= bit_cnt - 1;
              shift_reg <= shift_reg << 1;
              counter   <= SPI_DIV - 1;
              state     <= TRANSFER_LOW;
            end
          end else begin
            counter <= counter - 1;
          end
        end

        FINISH: begin
          spi_cs_n <= 1'b1;
          spi_clk  <= 1'b0;
          spi_busy <= 1'b0;
          spi_done <= 1'b1;
          state    <= IDLE;
        end

        default: state <= IDLE;
      endcase
    end
  end

endmodule
