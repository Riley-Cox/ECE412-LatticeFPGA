module top;

bit clk, reset, brightPush, colorPush;
bit bright, color;

logic [15:0] colorOut;
logic brightOut;

initial forever
#5 clk = ~clk;

button pwmButton(brightPush, bright);
button colorButton(colorPush, color);

brightness screenBright(clk, reset, bright, brightOut);
colorChange screenColor(clk, reset, color, colorOut);


initial begin
$monitor("Time: %t Brightness Button:%b | Color Button:%b | BrightOut:%b | ColorOut:%4X",$time, brightPush, colorPush, brightOut, colorOut); 
	
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
	repeat (20) @(negedge clk);
	brightPush = 1;
	#10 brightPush = 0;
	colorPush = 1;
	#10 colorPush = 0;
	repeat(30) @(negedge clk);
	$stop();
	end
endmodule
