# Define the primary clock on the actual net name

#create_clock -name clk -period 83.33 [get_net clk]
create_clock -name clk_inferred_clock -period 166.6667 [get_nets {clk}]

#create_clock -name clk -period 166.67 [get_ports {test_out}]
