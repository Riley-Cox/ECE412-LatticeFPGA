module spi_controller #(
  // Number of system clocks for one half period of SPI clock.
  parameter integer SPI_DIV = 4,
  parameter integer DIV_WIDTH = 2
)(
  input  logic         clk,         // system clock
  input  logic         reset_n,     // active-low reset

  // Memory bus interface
  input  logic [31:0]  address_in,
  input  logic         sel_in,
  input  logic         read_in,
  input  logic [3:0]   write_mask_in,
  input  logic [31:0]  write_value_in,
  output logic [31:0]  read_value_out,
  output logic         ready_out,

  // SPI pins
  output logic         spi_clk,
  output logic         spi_mosi,
  output logic         spi_cs_n,
  output logic         lcd_dc
);

  // SPI register addresses (offsets)
  localparam SPI_DATA_ADDR   = 32'h00000000;
  localparam SPI_CTRL_ADDR   = 32'h00000004;
  localparam SPI_STATUS_ADDR = 32'h00000008;
  localparam SPI_DC_ADDR     = 32'h0000000C;

  localparam  IDLE         = 2'b00,
              TRANSFER_LOW = 2'b01,
              TRANSFER_HIGH= 2'b10,
              FINISH       = 2'b11;

  logic [1:0] state;
  logic [DIV_WIDTH-1:0] counter;
  logic [3:0]           bit_cnt;
  logic [7:0]           shift_reg;
  logic [7:0]           data_reg;
  logic                 lcd_dc_reg;
  logic                 start_latched;
  logic                 spi_start;
  logic                 spi_busy;
  logic                 spi_done;

  assign lcd_dc = lcd_dc_reg;

  // Reset delay logic
  logic [3:0] reset_counter = 0;
  logic internal_reset_n;

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n)
      reset_counter <= 0;
    else if (!&reset_counter)
      reset_counter <= reset_counter + 1;
  end

  assign internal_reset_n = &reset_counter;

  // Pulse generation for spi_start
  logic last_write_sel;
  logic write_sel;
  logic [31:0] offset;
  assign offset = address_in & 32'h0000000F;

  always_ff @(posedge clk or negedge internal_reset_n) begin
    if (!internal_reset_n) begin
      spi_start <= 1'b0;
      last_write_sel <= 1'b0;
    end else begin
      write_sel <= (sel_in && |write_mask_in && offset == SPI_CTRL_ADDR);
      spi_start <= write_sel && !last_write_sel;
      last_write_sel <= write_sel;
    end
  end

  // Write logic
  always_ff @(posedge clk or negedge internal_reset_n) begin
    if (!internal_reset_n) begin
      data_reg <= 8'h00;
      lcd_dc_reg <= 1'b0;
    end else begin
      if (sel_in && |write_mask_in) begin
        case (offset)
          SPI_DATA_ADDR: data_reg <= write_value_in[7:0];
          SPI_DC_ADDR:   lcd_dc_reg <= write_value_in[0];
          default: ;
        endcase
      end
    end
  end

  // Read logic
  always_comb begin
    read_value_out = 32'h00000000;
    if (sel_in && read_in) begin
      case (offset)
        SPI_STATUS_ADDR: read_value_out = {30'b0, spi_done, spi_busy};
        default:          read_value_out = 32'h00000000;
      endcase
    end
  end

  assign ready_out = sel_in;

  // Latch the spi_start pulse
  always_ff @(posedge clk or negedge internal_reset_n) begin
    if (!internal_reset_n) begin
      start_latched <= 1'b0;
    end else begin
      if (state == IDLE && spi_start && !spi_busy)
        start_latched <= 1'b1;
      else if (state != IDLE)
        start_latched <= 1'b0;
    end
  end

  // SPI FSM
  always_ff @(posedge clk or negedge internal_reset_n) begin
    if (!internal_reset_n) begin
      state       <= IDLE;
      spi_cs_n    <= 1'b1;
      spi_clk     <= 1'b0;
      spi_busy    <= 1'b0;
      spi_done    <= 1'b0;
      spi_mosi    <= 1'b0;
      counter     <= '0;
      bit_cnt     <= 4'd0;
      shift_reg   <= 8'd0;
    end else begin
      case (state)
        IDLE: begin
          spi_clk  <= 1'b0;
          spi_cs_n <= 1'b1;
          spi_done <= 1'b0;
          spi_busy <= 1'b0;

          if (start_latched && !spi_busy) begin
            shift_reg   <= data_reg;
            bit_cnt     <= 4'd8;
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
