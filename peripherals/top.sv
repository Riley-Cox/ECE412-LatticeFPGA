module top;

bit clk, reset, brightPush, colorPush;
bit bright, color;

logic [3:0] colorOut;
logic brightOut;

initial forever
#5 clk = ~clk;

button pwmButton(brightPush, bright);
button colorButton(colorPush, color);

brightness screenBright(clk, reset, bright, brightOut);
colorChange screenColor(clk, reset, color, colorOut);

initial begin
	repeat (1) @(negedge clk)
		reset = 1;
	repeat (1) @(negedge clk) begin
		reset = 0;
		brightPush = 1;
		colorPush = 1;
		end
	repeat (1) @(negedge clk) begin
		brightPush = 0;
		colorPush = 0;
		end
	repeat (50) @(negedge clk);
	$stop();
	end
endmodule