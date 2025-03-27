`ifndef RV32_MEM
`define RV32_MEM

`include "rv32_branch.sv"
`include "rv32_csrs.sv"

`define RV32_MEM_WIDTH_WORD 2'b00
`define RV32_MEM_WIDTH_HALF 2'b01
`define RV32_MEM_WIDTH_BYTE 2'b10

module rv32_mem (
    input clk,
    input reset,

    // Formal/debug interface (unchanged)
`ifdef RISCV_FORMAL
    input intr_in,
    input [4:0] rs1_in,
    input [4:0] rs2_in,
    input [31:0] next_pc_in,
    input [31:0] instr_in,
    output logic intr_out,
    output logic trap_out,
    output logic [4:0] rs1_out,
    output logic [4:0] rs2_out,
    output logic [3:0] read_mask_out,
    output logic [3:0] write_mask_out,
    output logic [31:0] pc_out,
    output logic [31:0] next_pc_out,
    output logic [31:0] instr_out,
    output logic [31:0] rs1_value_out,
    output logic [31:0] rs2_value_out,
    output logic [31:0] address_out,
    output logic [31:0] read_value_out,
    output logic [31:0] write_value_out,
`endif

    input stall_in,
    input flush_in,
    input writeback_flush_in,

    input branch_predicted_taken_in,
    input branch_misaligned_in,
    input valid_in,
    input exception_in,
    input [3:0] exception_cause_in,
    input read_in,
    input write_in,
    input [1:0] width_in,
    input zero_extend_in,
    input csr_read_in,
    input csr_write_in,
    input [1:0] csr_write_op_in,
    input csr_src_in,
    input [1:0] branch_op_in,
    input ecall_in,
    input ebreak_in,
    input mret_in,
    input [4:0] rd_in,
    input rd_write_in,

    input data_fault_in,

    input [31:0] pc_in,
    input [31:0] result_in,
    input [31:0] rs1_value_in,
    input [31:0] rs2_value_in,
    input [31:0] imm_value_in,
    input [31:0] branch_pc_in,
    input [11:0] csr_in,

    input [31:0] data_read_value_in,

    output logic valid_out,
    output logic trap_unreg_out,
    output logic branch_mispredicted_out,
    output logic [4:0] rd_out,
    output logic rd_write_out,

    output logic data_read_out,
    output logic data_write_out,
    output logic [3:0] data_write_mask_out,

    output logic [31:0] rd_value_out,
    output logic [31:0] trap_pc_out,
    output logic [31:0] branch_pc_out,

    output logic [31:0] data_address_out,
    output logic [31:0] data_write_value_out,

    output logic [63:0] cycle_out
);

    // SPI register base addresses
    localparam [31:0] SPI_BASE_ADDR   = 32'h80000000;
    localparam [31:0] SPI_DATA_ADDR   = SPI_BASE_ADDR;
    localparam [31:0] SPI_CTRL_ADDR   = SPI_BASE_ADDR + 4;
    localparam [31:0] SPI_STATUS_ADDR = SPI_BASE_ADDR + 8;
    localparam [31:0] SPI_DC_ADDR     = SPI_BASE_ADDR + 12;
	
    logic branch_mispredicted;
    logic branch_taken;

    rv32_branch_unit branch_unit (
        .predicted_taken_in(branch_predicted_taken_in),
        .op_in(branch_op_in),
        .result_in(result_in),
        .taken_out(branch_taken),
        .mispredicted_out(branch_mispredicted)
    );

    assign branch_pc_out = branch_pc_in;
    assign branch_mispredicted_out = branch_mispredicted && !flush_in;

    logic mem_misaligned;
    logic [31:0] read_value;
    logic [3:0] read_mask;

    assign data_address_out = {result_in[31:2], 2'b0};
    assign data_read_out = read_in && !mem_misaligned;
    assign data_write_out = write_in && !mem_misaligned;
    assign data_address_out = {result_in[31:2], 2'b0};
	
	logic spi_selected;
    assign spi_selected = result_in >= SPI_BASE_ADDR && result_in < (SPI_BASE_ADDR + 16);

    always_comb begin
        case (width_in)
            `RV32_MEM_WIDTH_WORD: mem_misaligned = result_in[1:0] != 0;
            `RV32_MEM_WIDTH_HALF: mem_misaligned = result_in[0] != 0;
            `RV32_MEM_WIDTH_BYTE: mem_misaligned = 0;
            default:              mem_misaligned = 1'bx;
        endcase

       
        /* write port */
        if (write_in && !spi_selected) begin
            case (width_in)
                `RV32_MEM_WIDTH_WORD: begin
                    data_write_value_out = rs2_value_in;
                    data_write_mask_out = 4'b1111;
                end
                `RV32_MEM_WIDTH_HALF: begin
                    case (result_in[1])
                        1'b0: begin
                            data_write_value_out = {16'bx, rs2_value_in[15:0]};
                            data_write_mask_out = 4'b0011;
                        end
                        1'b1: begin
                            data_write_value_out = {rs2_value_in[15:0], 16'bx};
                            data_write_mask_out = 4'b1100;
                        end
                    endcase
                end
                `RV32_MEM_WIDTH_BYTE: begin
                    case (result_in[1:0])
                        2'b00: data_write_value_out = {24'bx, rs2_value_in[7:0]};
                        2'b01: data_write_value_out = {16'bx, rs2_value_in[7:0], 8'bx};
                        2'b10: data_write_value_out = {8'bx, rs2_value_in[7:0], 16'bx};
                        2'b11: data_write_value_out = {rs2_value_in[7:0], 24'bx};
                    endcase

                    case (result_in[1:0])
                        2'b00: data_write_mask_out = 4'b0001;
                        2'b01: data_write_mask_out = 4'b0010;
                        2'b10: data_write_mask_out = 4'b0100;
                        2'b11: data_write_mask_out = 4'b1000;
                    endcase
                end
                default: begin
                    data_write_value_out = 32'bx;
                    data_write_mask_out = 4'bx;
                end
            endcase
        end else begin
            data_write_value_out = 32'bx;
            data_write_mask_out = 4'b0;
        end

        if (read_in) begin
            if (result_in == SPI_STATUS_ADDR) begin
                read_value = {30'b0, spi_done, spi_busy};
                read_mask = 4'b1111;
            end else begin
                case (width_in)
                    `RV32_MEM_WIDTH_WORD: begin
                        read_value = data_read_value_in;
                        read_mask = 4'b1111;
                    end
                    `RV32_MEM_WIDTH_HALF: begin
                        case (result_in[1])
                            1'b0: begin
                                read_value = {{16{zero_extend_in ? 1'b0 : data_read_value_in[15]}}, data_read_value_in[15:0]};
                                read_mask = 4'b0011;
                            end
                            1'b1: begin
                                read_value = {{16{zero_extend_in ? 1'b0 : data_read_value_in[31]}}, data_read_value_in[31:16]};
                                read_mask = 4'b1100;
                            end
                        endcase
                    end
                    `RV32_MEM_WIDTH_BYTE: begin
                        case (result_in[1:0])
                            2'b00: begin
                                read_value = {{24{zero_extend_in ? 1'b0 : data_read_value_in[7]}},  data_read_value_in[7:0]};
                                read_mask = 4'b0001;
                            end
                            2'b01: begin
                                read_value = {{24{zero_extend_in ? 1'b0 : data_read_value_in[15]}}, data_read_value_in[15:8]};
                                read_mask = 4'b0010;
                            end
                            2'b10: begin
                                read_value = {{24{zero_extend_in ? 1'b0 : data_read_value_in[23]}}, data_read_value_in[23:16]};
                                read_mask = 4'b0100;
                            end
                            2'b11: begin
                                read_value = {{24{zero_extend_in ? 1'b0 : data_read_value_in[31]}}, data_read_value_in[31:24]};
                                read_mask = 4'b1000;
                            end
                        endcase
                    end
                    default: begin
                        read_value = 32'bx;
                        read_mask = 4'bx;
                    end
                endcase
            end

        end else begin
            read_value = 32'bx;
            read_mask = 4'b0;
        end
    end

    always_ff @(posedge clk) begin
        if (!stall_in && valid_in && !flush_in) begin
            if (write_in && spi_selected) begin
                case (result_in)
                    SPI_DATA_ADDR:   spi_data_out <= rs2_value_in[7:0];
                    SPI_CTRL_ADDR:   spi_start    <= 1'b1;
                    SPI_DC_ADDR:     spi_dc       <= rs2_value_in[0];
                    default:;
                endcase
            end else begin
                spi_start <= 1'b0;
            end
        end else begin
            spi_start <= 1'b0;
        end
    end

    /* traps */
    logic exception;
    logic [3:0] exception_cause;
    logic mem_exception;

    always_comb begin
        exception = 0;
        exception_cause = 4'bx;
        mem_exception = 0;

        if (exception_in) begin
            exception = 1;
            exception_cause = exception_cause_in;
        end else if (branch_taken && branch_misaligned_in) begin
            exception = 1;
            exception_cause = `RV32_MCAUSE_INSTR_MISALIGNED_EXCEPTION;
            mem_exception = 1;
        end else if (read_in && mem_misaligned) begin
            exception = 1;
            exception_cause = `RV32_MCAUSE_LOAD_MISALIGNED_EXCEPTION;
            mem_exception = 1;
        end else if (write_in && mem_misaligned) begin
            exception = 1;
            exception_cause = `RV32_MCAUSE_STORE_MISALIGNED_EXCEPTION;
            mem_exception = 1;
        end else if (read_in && data_fault_in) begin
            exception = 1;
            exception_cause = `RV32_MCAUSE_LOAD_FAULT_EXCEPTION;
            mem_exception = 1;
        end else if (write_in && data_fault_in) begin
            exception = 1;
            exception_cause = `RV32_MCAUSE_STORE_FAULT_EXCEPTION;
            mem_exception = 1;
        end else if (ecall_in) begin
            exception = 1;
            exception_cause = `RV32_MCAUSE_MACHINE_ECALL_EXCEPTION;
        end else if (ebreak_in) begin
            exception = 1;
            exception_cause = `RV32_MCAUSE_BREAKPOINT_EXCEPTION;
        end
    end

    logic [31:0] csr_read_value;
    logic [63:0] cycle;

    rv32_csrs csrs (
        .clk(clk),
        .reset(reset),
        .stall_in(stall_in),
        .flush_in(flush_in),
        .writeback_flush_in(writeback_flush_in),
        .exception_in(exception),
        .exception_cause_in(exception_cause),
        .read_in(csr_read_in),
        .write_in(csr_write_in),
        .write_op_in(csr_write_op_in),
        .src_in(csr_src_in),
        .mret_in(mret_in),
        .instr_retired_in(valid_out),
        .pc_in(pc_in),
        .rs1_value_in(rs1_value_in),
        .imm_value_in(imm_value_in),
        .csr_in(csr_in),
        .trap_out(trap_unreg_out),
        .read_value_out(csr_read_value),
        .trap_pc_out(trap_pc_out),
        .cycle_out(cycle_out)
    );

    logic [31:0] next_pc;

`ifdef RISCV_FORMAL
    always_comb begin
        if (trap_unreg_out)
            next_pc = trap_pc_out;
        else if (branch_mispredicted)
            next_pc = branch_pc_in;
        else
            next_pc = next_pc_in;
    end
`endif

    always_ff @(posedge clk) begin
        if (!stall_in) begin
`ifdef RISCV_FORMAL
            intr_out <= intr_in;
            pc_out <= pc_in;
            next_pc_out <= next_pc;
            trap_out <= trap_unreg_out;
            rs1_out <= rs1_in;
            rs2_out <= rs2_in;
            instr_out <= instr_in;
            rs1_value_out <= rs1_value_in;
            rs2_value_out <= rs2_value_in;
            address_out <= data_address_out;
            read_mask_out <= read_mask;
            write_mask_out <= data_write_mask_out;
            read_value_out <= data_read_value_in;
            write_value_out <= data_write_value_out;
`endif

            valid_out <= valid_in;
            rd_out <= rd_in;
            rd_write_out <= rd_write_in;

            if (read_in)
                rd_value_out <= read_value;
            else if (csr_read_in)
                rd_value_out <= csr_read_value;
            else
                rd_value_out <= result_in;

`ifdef RISCV_FORMAL
            if (flush_in)
                trap_out <= 0;
`endif

            if (flush_in || mem_exception) begin
                valid_out <= 0;
                rd_write_out <= 0;
            end
        end

        if (reset) begin
`ifdef RISCV_FORMAL
            trap_out <= 0;
`endif
            valid_out <= 0;
            rd_out <= 0;
            rd_write_out <= 0;
            rd_value_out <= 0;
        end
    end
endmodule

`endif
