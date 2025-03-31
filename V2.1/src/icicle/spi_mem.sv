module spi_mem (
    input  logic        clk,
    input  logic        reset,

    // Memory bus interface
    input  logic [31:0] address_in,
    input  logic        sel_in,
    input  logic        read_in,
    input  logic [3:0]  write_mask_in,
    input  logic [31:0] write_value_in,
    output logic [31:0] read_value_out,
    output logic        ready_out,

    // SPI controller outputs
    output logic        spi_clk,
    output logic        spi_mosi,
    output logic        spi_cs_n,
    output logic        lcd_dc
);

    // SPI register addresses (offsets)
    localparam SPI_DATA_ADDR   = 32'h00000000;
    localparam SPI_CTRL_ADDR   = 32'h00000004;
    localparam SPI_STATUS_ADDR = 32'h00000008;
    localparam SPI_DC_ADDR     = 32'h0000000C;

    // SPI controller signals
    logic        spi_start;
    logic [7:0]  spi_data_in;
    logic        spi_dc;
    logic        spi_busy;
    logic        spi_done;

    // Internal registers
    logic [7:0]  data_reg;
    logic        last_write_sel;
    logic        write_sel;

    // Address decode
    logic [31:0] offset;
    assign offset = address_in & 32'h0000000F;

    // One-cycle pulse generation for spi_start
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            spi_start <= 1'b0;
            last_write_sel <= 1'b0;
        end else begin
            write_sel <= (sel_in && |write_mask_in && offset == SPI_CTRL_ADDR);
            spi_start <= write_sel && !last_write_sel;
            last_write_sel <= write_sel;
        end
    end

    // Write logic
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            data_reg <= 8'h00;
            spi_dc   <= 1'b0;
        end else begin
            if (sel_in && |write_mask_in) begin
                case (offset)
                    SPI_DATA_ADDR:   data_reg <= write_value_in[7:0];
                    SPI_DC_ADDR:     spi_dc   <= write_value_in[0];
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
                default:         read_value_out = 32'h00000000;
            endcase
        end
    end

    assign ready_out = sel_in;

    // SPI controller instance
    spi_controller spi_core (
        .clk(clk),
        .reset_n(~reset),
        .spi_start(spi_start),
        .spi_data_in(data_reg),
        .spi_dc(spi_dc),
        .spi_busy(spi_busy),
        .spi_done(spi_done),
        .spi_clk(spi_clk),
        .spi_mosi(spi_mosi),
        .spi_cs_n(spi_cs_n),
        .lcd_dc(lcd_dc)
    );

endmodule
