module spi_controller #(
  parameter logic [5:0] SPI_DIV = 32,
  parameter integer DIV_WIDTH = 6
)(
  input  logic         clk,
  input  logic         reset_n,

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

  localparam SPI_DATA_ADDR   = 32'h00000000;
  localparam SPI_CTRL_ADDR   = 32'h00000004;
  localparam SPI_STATUS_ADDR = 32'h00000008;
  localparam SPI_DC_ADDR     = 32'h0000000C;

  localparam IDLE         = 2'b00,
             TRANSFER_LOW = 2'b01,
             TRANSFER_HIGH= 2'b10,
             FINISH       = 2'b11;

  logic [1:0] state, nextState;
  logic [DIV_WIDTH-1:0] counter;
  logic [3:0]           bit_cnt;
  logic [7:0]           shift_reg;
  logic                 lcd_dc_reg;
  logic                 start_latched;
  logic                 spi_busy;
  logic                 spi_done;
  logic                 spi_done_ack;

  logic [7:0] data_reg;
  logic [31:0] offset;
  assign offset = address_in & 32'h0000000F;

  assign lcd_dc = lcd_dc_reg;

  // Write detection logic
  logic write_sel;
  logic last_write_sel;
  assign write_sel = (sel_in && |write_mask_in && offset == SPI_CTRL_ADDR);

  // Rising edge detection and start gating
  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      last_write_sel <= 1'b0;
      start_latched  <= 1'b0;
    end else begin
      last_write_sel <= write_sel;
      if (write_sel && !last_write_sel && !spi_busy && !spi_done)
        start_latched <= 1'b1;
      else if (state != IDLE)
        start_latched <= 1'b0;
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state <= IDLE;
    end
    else begin
      state <= nextState;
    end
  end

  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      counter <= SPI_DIV - 1;
    end
    else if (counter == 0) begin
      counter <= SPI_DIV - 1;

    end
    else begin
      counter <= counter - 1;
    end
  end


  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n)begin
      shift_reg <= '0;
    end
    else if(start_latched)
      shift_reg <= data_reg;
    else if(state == TRANSFER_HIGH) begin
      if (counter == 0)
        if ((shift_reg << 1) != 0)
          shift_reg <= shift_reg << 1;
    end
  end


  always_comb begin
    unique case (state)
      IDLE: begin
        if (start_latched) begin
          spi_cs_n = 1'b0;
          spi_busy = 1'b1;
          spi_done = 1'b0;
          spi_clk = 1'b0;
          spi_mosi = '0;
        end        
        else if(!spi_done_ack) begin
          spi_done = 1'b0;
          spi_clk = '0;
          spi_mosi = '0;
          spi_cs_n = 1'b0;
          spi_busy = 1'b1;
        end
        else begin
          spi_done = 1'b0;
          spi_clk = '0;
          spi_mosi = '0;
          spi_busy = 1'b1;
          spi_cs_n = 1'b0;
        end
      end
      TRANSFER_LOW: begin
        spi_mosi = data_reg[7];
        spi_done = 1'b0;
        spi_busy = 1'b1;
        spi_cs_n = 1'b0;
        spi_clk = 1'b0;
        if(counter == 0)
          spi_clk = 1'b1;
      end
      TRANSFER_HIGH: begin
        spi_clk = 1'b1;
        spi_mosi = '0;
        spi_done = 1'b0;
        spi_busy = 1'b1;
        spi_cs_n = 1'b0;
      end
      FINISH: begin
          spi_cs_n = 1'b1;
          spi_clk  = 1'b0;
          spi_busy = 1'b0;
          spi_done = 1'b1;
          spi_mosi = 1'b0;
      end
      default: begin
        spi_cs_n = 1'b0;
        spi_clk = 1'b0;
        spi_mosi = 1'b0;
        spi_done = 1'b0;
        spi_busy = 1'b1;
      end
    endcase
  end

  // Transaction FSM
  always_comb begin
    unique case (state)
        IDLE: begin
          if (start_latched) begin
            nextState   = TRANSFER_LOW;
          end else begin
            nextState = IDLE; 
          end
        end
        TRANSFER_LOW: begin
          if (counter == 0) begin
            nextState  = TRANSFER_HIGH;
          end
          else begin
            nextState = TRANSFER_LOW;
          end
        end
        TRANSFER_HIGH: begin
          if (counter == 0) begin
            if ((data_reg << 1) == 0) begin
              nextState = FINISH;
            end else begin
              nextState     = TRANSFER_LOW;
            end
        end
        else begin
          nextState = state;
        end
      end
        FINISH: begin
          nextState    = IDLE;
        end
        default: nextState = IDLE;
      endcase
    end

  // Register interface
  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      data_reg <= 8'h00;
      lcd_dc_reg <= 1'b0;
    end else if (sel_in && |write_mask_in) begin
      case (offset)
        SPI_DATA_ADDR:   data_reg <= write_value_in[7:0];
        SPI_DC_ADDR:     lcd_dc_reg <= write_value_in[0];
        default: ;
      endcase
    end
  end

  // Acknowledge done signal when CPU reads SPI_STATUS
  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n)
      spi_done_ack <= 1'b0;
    else
      spi_done_ack <= (sel_in && read_in && offset == SPI_STATUS_ADDR);
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

endmodule
