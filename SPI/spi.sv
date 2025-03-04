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

  //-------------------------------------------------------------------------
  // State machine declaration
  //-------------------------------------------------------------------------
localparam 	IDLE         = 2'b00,
			TRANSFER_LOW = 2'b01, // SPI clock low phase: output current bit on MOSI
			TRANSFER_HIGH= 2'b10, // SPI clock high phase: wait half period
			FINISH       = 2'b11;  // End of transaction
  
 logic [1:0] state;

  //-------------------------------------------------------------------------
  // Internal registers
  //-------------------------------------------------------------------------
  logic [DIV_WIDTH-1:0] counter;   // Counts down to generate SPI halfâ€“period
  logic [3:0]           bit_cnt;   // Bit counter (8 bit transfer)
  logic [7:0]           shift_reg; // Holds the byte to be shifted out
  logic                 lcd_dc_reg;// Latches the spi_dc flag

  // Drive the LCD DC signal from our internal register.
  assign lcd_dc = lcd_dc_reg;

  //-------------------------------------------------------------------------
  // SPI Master State Machine (Transmit Only)
  //-------------------------------------------------------------------------
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
    end
    else begin
      case (state)
        //===============================================================
        // IDLE: Wait for a start pulse to begin transmission.
        //===============================================================
        IDLE: begin
          spi_done <= 1'b0;      // Clear done flag
          spi_busy <= 1'b0;
          spi_cs_n <= 1'b1;      // Chip select inactive
          spi_clk  <= 1'b0;      // SPI clock idle low
          spi_mosi <= 1'b0;
          if (spi_start) begin
            // Latch input data and control signals.
            shift_reg   <= spi_data_in;
            bit_cnt     <= 4'd8;       // 8 bits to transfer
            lcd_dc_reg  <= spi_dc;       // Latch the DC flag for LCD control
            spi_busy    <= 1'b1;
            spi_cs_n    <= 1'b0;       // Assert chip select (active low)
            counter     <= SPI_DIV - 1;
            state       <= TRANSFER_LOW;
          end
        end

        //===============================================================
        // TRANSFER_LOW: With the SPI clock low, output the MSB on MOSI.
        //===============================================================
        TRANSFER_LOW: begin
          spi_clk  <= 1'b0;
          spi_mosi <= shift_reg[7];
          if (counter == 0) begin
            // After the halfâ€“period, raise the clock.
            spi_clk <= 1'b1;
            counter <= SPI_DIV - 1;
            state   <= TRANSFER_HIGH;
          end
          else begin
            counter <= counter - 1;
          end
        end

        //===============================================================
        // TRANSFER_HIGH: SPI clock remains high for a half period.
        // After the period, update bit counter and shift register.
        //===============================================================
        TRANSFER_HIGH: begin
          spi_clk <= 1'b1;
          if (counter == 0) begin
            if (bit_cnt == 1) begin
              // Last bit has been transmitted.
              bit_cnt <= 4'd0;
              state   <= FINISH;
            end
            else begin
              // Shift the data left to bring next bit to MSB.
              bit_cnt   <= bit_cnt - 1;
              shift_reg <= shift_reg << 1'b1;
              counter   <= SPI_DIV - 1;
              state     <= TRANSFER_LOW;
            end
          end
          else begin
            counter <= counter - 1;
          end
        end

        //===============================================================
        // FINISH: Transaction complete. Deassert CS and generate done pulse.
        //===============================================================
        FINISH: begin
          spi_cs_n <= 1'b1;
          spi_clk  <= 1'b0;
          spi_busy <= 1'b0;
          spi_done <= 1'b1; // One cycle done pulse
          state    <= IDLE;
        end

        default: state <= IDLE;
      endcase
    end
  end

endmodule
