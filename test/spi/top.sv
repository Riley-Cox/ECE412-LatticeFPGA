`timescale 1ns/1ps

module top;

  logic clk;
  logic reset_n;

  
  logic [31:0] address_in;
  logic sel_in;
  logic read_in;
  logic [3:0] write_mask_in;
  logic [31:0] write_value_in;
  logic [31:0] read_value_out;
  logic ready_out;

  int clk_cnt, shift_cnt;
  
  logic spi_clk;
  logic spi_mosi;
  logic spi_cs_n;
  logic lcd_dc;
  logic [7:0] mosi = '0;

  
  spi_controller dut (
    .clk(clk),
    .reset_n(reset_n),
    .address_in(address_in),
    .sel_in(sel_in),
    .read_in(read_in),
    .write_mask_in(write_mask_in),
    .write_value_in(write_value_in),
    .read_value_out(read_value_out),
    .ready_out(ready_out),
    .spi_clk(spi_clk),
    .spi_mosi(spi_mosi),
    .spi_cs_n(spi_cs_n),
    .lcd_dc(lcd_dc)
  );

  
  always #5 clk = ~clk; // 100 MHz clock
  

  
  task mem_write(input [31:0] addr, input [31:0] data);
    begin
      address_in = addr;
      write_value_in = data;
      write_mask_in = 4'b1111;
      sel_in = 1;
      read_in = 0;
      @(posedge clk);
      sel_in = 0;
      $display("[WRITE] Addr: %h, Data: %h", addr, data);
    end
  endtask

  
  task mem_read(input [31:0] addr);
    begin
      address_in = addr;
      sel_in = 1;
      read_in = 1;
      @(posedge clk);
      sel_in = 0;
      read_in = 0;
      $display("[READ] Addr: %h, Data: %h, mosi: %h", addr, read_value_out, spi_mosi);
    end
  endtask

  
  initial begin
    $display("Starting SPI Controller Testbench");

    
    clk = 0;
    reset_n = 0;
    address_in = 0;
    sel_in = 0;
    read_in = 0;
    write_mask_in = 0;
    write_value_in = 0;

    
    #20;
    reset_n = 1;
    $display("[RESET RELEASED]");

    
    mem_write(32'h00000000, 32'hA5A5A5A5); // Write to SPI_DATA_ADDR
    mem_write(32'h00000004, 32'h00000001); // Write to SPI_CTRL_ADDR to start

    repeat (8) @(posedge spi_clk) begin
      mem_read(32'h00000008); // Read SPI_STATUS_ADDR
        mosi = mosi | spi_mosi;
        if (shift_cnt < 7)begin
        mosi = mosi << 1;
        shift_cnt++;
        end

    end

    
    mem_write(32'h0000000C, 32'h00000001); // Set lcd_dc

    #100;
    $display("Test complete");
    $display("MOSI: %h", mosi);
    $finish;
  end

endmodule
