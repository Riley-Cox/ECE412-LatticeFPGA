module button (input logic press, output logic action);

always_comb begin
	if (press)
		action = '1;
	else
		action = '0;
end

endmodule

module colorChange (input bit clock, reset, input logic change, output logic [3:0] color);
	
	always_ff @(posedge clock) begin
		if (reset) begin
			color <= '0;
			end
		else if (change) begin
			color <= color + 1;
			end
		else begin
			color <= color;
			end
		end
			
	endmodule
	
module brightness (input bit clock, reset, input logic change, output logic screenPower);
	
	logic [2:0] divider, count;
	
	always_ff@(posedge clock) begin
		if (reset) begin
			divider <= 0;
			count <= 0;
			end
		else if (change) begin
			divider <= divider + 1;
			end
		else begin
			count <= count +1;
			if (count == divider) begin
				screenPower <= '1;
				count <= '0;
				end
			else 
				screenPower <= '0;
			end
		end
endmodule