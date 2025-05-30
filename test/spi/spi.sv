//`include "peripheral.sv"

module spi_controller #(
  parameter logic [5:0] SPI_DIV = 4,
  parameter integer DIV_WIDTH = 6
)(
  input  logic         clk,
  input  logic         reset_n,
  input  logic         change,

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
  output logic         lcd_dc,
  //output logic 		color_start,
  output logic 		changeOut
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
  logic [7:0]           shift_reg;
  logic                 lcd_dc_reg;
  logic                 start_latched;
  logic                 spi_busy;
  logic                 spi_done;
  logic                 spi_done_ack;
  logic [3:0]           bitCounter;

  logic [7:0] data_reg;
  logic [31:0] offset;
  assign offset = address_in & 32'h0000000F;

  assign lcd_dc = lcd_dc_reg;

  //Change color module and logic
  
  logic [15:0] color;
  logic color_start;
  logic color_hold;
  logic color_again; 
  logic color_done;

  colorChange colorChange (clk, reset_n, change,/*changeOut,*/ color, color_start);
  //assign color_start = '0;

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
      last_write_sel <= write_sel || (color_hold || color_again && !color_done);
       if (state != IDLE)
        start_latched <= 1'b0;
      else if (write_sel && !last_write_sel && !spi_busy && !spi_done)
        start_latched <= 1'b1;
	  else if ((color_start || color_again) && !last_write_sel && !spi_busy && !spi_done)
		  start_latched <= 1'b1;
    end
  end

  //Next State logic
  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      state <= IDLE;
    end
    else begin
      state <= nextState;
    end
  end

  //Transaction length counter
  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n) begin
      counter <= SPI_DIV - 1;
    end
    else if (counter == 0) begin
      counter <= SPI_DIV - 1;

    end
    else if (state == TRANSFER_LOW || state == TRANSFER_HIGH) begin
      counter <= counter - 1;
    end
  end

  //SPI output register logic
  always_ff @(posedge clk or negedge reset_n) begin
    if (!reset_n)begin
      shift_reg <= '0;
    end
    else if(start_latched) begin
      shift_reg <= data_reg;
      bitCounter <= 4'd8;
    end
    else if(state == TRANSFER_HIGH) begin
      if (counter == 0) begin
        if (bitCounter > 1) begin
          shift_reg <= shift_reg << 1;
          bitCounter <= bitCounter - 1;
        end
        else begin
          shift_reg <= shift_reg;
        end
      end
      else begin
        shift_reg <= shift_reg;
      end
    end
  end

  //Color change control logic
  always_ff @(posedge clk or negedge reset_n) begin
	if (!reset_n) begin
		color_hold <= '0;
		color_again <= '0;
		color_done <= '0;
	end
	else if(state == IDLE && color_start) begin
		color_hold <= '1;
		color_again <= color_again;
		color_done <= color_done;
	end
	else if(state == FINISH) begin
		color_again <= color_hold;
		color_hold <= '0;
		color_done <= (color_hold | color_again);
	end
	else begin
		color_hold <= color_hold;
		color_again <= color_again;
		color_done <= 0;
	end
  end	

		

  //Control logic
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
          spi_cs_n = 1'b1;
          spi_busy = 1'b0;
        end
        else begin
          spi_done = 1'b0;
          spi_clk = '0;
          spi_mosi = '0;
          spi_busy = 1'b1;
          spi_cs_n = 1'b1;
        end
      end
      TRANSFER_LOW: begin
        spi_clk = 1'b0;
        spi_mosi = shift_reg[7];
        spi_done = 1'b0;
        spi_busy = 1'b1;
        spi_cs_n = 1'b0;
      end
      TRANSFER_HIGH: begin
        spi_clk = 1'b1;
        spi_done = 1'b0;
        spi_busy = 1'b1;
        spi_cs_n = 1'b0;
        spi_mosi = shift_reg[7];
      end
      FINISH: begin
          spi_cs_n = 1'b1;
          spi_clk  = 1'b0;
          spi_busy = 1'b0;
          spi_done = 1'b1;
          spi_mosi = 1'b0;
      end
      default: begin
        spi_cs_n = 1'b1;
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
            if (bitCounter > 1) begin
              nextState = TRANSFER_LOW;
            end else begin
              nextState     = FINISH;
            end
        end
        else begin
          nextState = TRANSFER_HIGH;
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
    end 
    else if (sel_in && |write_mask_in) begin
      case (offset)
        SPI_DATA_ADDR:   data_reg <= write_value_in[7:0];
        SPI_DC_ADDR:     lcd_dc_reg <= write_value_in[0];
        default: ;
      endcase
    end
	else if (color_start && !start_latched) begin
	    data_reg <= color[7:0];
    end
    else if (color_again && !start_latched) begin
	    data_reg <= color[15:8];
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
