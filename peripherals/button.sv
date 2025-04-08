module button (input logic press, output logic action);

always_comb begin
	if (press)
		action = '1;
	else
		action = '0;
end

endmodule

module colorChange (input bit clock, reset, input logic change, output logic [15:0] color);

	logic colorArray [3:0][15:0];
	logic index [3:0];
	assign colorArray [0] = 16'hF800;
	assign colorArray [1] = 16'h07E0;
	assign colorArray [2] = 16'h001F;
	assign colorArray [3] = 16'hFFFF;
	assign colorArray [4] = 16'hF000;
	assign colorArray [5] = 16'h0F00;
	assign colorArray [6] = 16'h00F0;
	assign colorArray [7] = 16'h000F;
	assign colorArray [8] = 16'hFF00;
	assign colorArray [9] = 16'h0FF0;
	assign colorArray [10] = 16'h00FF;
	assign colorArray [11] = 16'hF00F;
	assign colorArray [12] = 16'hF807;
	assign colorArray [13] = 16'hFC03;
	assign colorArray [14] = 16'hFE01;
	assign colorArray [15] = 16'hA5A5;

	
	always_ff @(posedge clock) begin
		if (reset) begin
			index <= '0;
			color <= colorArray[0];
			end
		else if (change) begin
			color <= colorArray[index+1];
			index <= index+1;
			end
		else begin
			color <= colorArray[index];
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
