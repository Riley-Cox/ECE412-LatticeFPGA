`ifndef SYNC
`define SYNC

module sync #(
    parameter BITS = 1
) (
    input logic clk,
    input [BITS-1:0] in,
    output logic [BITS-1:0] out
);
    logic [BITS-1:0] metastable = '0;

    always_ff @(posedge clk) begin
        metastable <= in;
        out <= metastable;
    end
endmodule

`endif
