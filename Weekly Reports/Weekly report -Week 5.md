# **Lattice FPGA Project Weekly Progress Report \- Week 5**

Report date: 2025/2/6

* **Last Week (time in hours)**  
  * Team Review  
    * Icicle is verified functional on V2.1 boards  
    * Nathan can finally stop fighting his Linux environment  
    * Icicle supports UART natively, so debug messages can go out  
  * Nathaniel Fraly  
    * Debian system setup and portable (2)  
    * Work with Rahul and team on PDS (4)  
    * Start project documentation requested by Rahul (1)  
  * Riley Cox  
    * Flashed simple blinking LED bitstream to FPGA (1)  
    * Migrated Icicle from UPduino V1 to V2 (3)  
    * Flashed Icicle to UPduino using open source tools (1)  
    * Worked on USB tunneling for WSL (1)  
    * Started documentation for project setup (1)  
  * Mohammad Alshaiji  
    * Finished V2.1 and V3.1 board comparison for Rahul (1)  
    * Assisted with PDS (1)  
    * Finished preliminary SPI controller Core research (2)  
    * Radiant toolchain installed but setup not completed (2)  
  * Xiang Li  
    * Revise PDS (2)  
    * Collect and learn SPI modules (5)  
  * Haoyang Han  
    * Complete power monitor research and purchase (3)  
    * Radiant toolchain installed and setup（2）  
* **Next week**  
  * Team Plan  
    * Focus on SPI implementation  
    * Provide Rahul with requested additional documentation  
    * Work on additional future modules as able  
  * Nathaniel Fraly  
    * Help walk Xiang and Mohammad through Icicle and how to read SV code  
    * Assist with Radiant migration  
    * Start on PWM module testing  
  * Riley Cox  
    * Figure out how to flash Icicle onto FPGA using Radiant  
    * Finish WSL setup and be able to work with board  
    * Finish documentation for setting up project from scratch  
  * Mohammad Alshaiji  
    * Finish Setting up radiant toolchain  
    * Begin Basic SPI implementation to control LCD  
    * Research and determine ideal method for interfacing with Icicle core  
    * Finish Icestorm Setup  
  * Xiang Li  
    * Insert SPI module into icicle  
    * Develop code for SPI module  
  * Haoyang Han  
    * Get Power Monitor  
    * Debugging Power Monitors  
        
* **Blocked**  
  * Team Blocks  
    * No teamwide blocks  
  * Nathaniel Fraly  
    * None  
  * Riley Cox  
    * Icicle currently uses a Makefile with yosys and icestorm commands along with compiling the C code that the RISC-V core is running,  I need to figure out how I can make that work with Radiant. This will involve getting used to Radiant and understanding the Makefile better.  
  * Mohammad Alshaiji  
    * Waiting on LCD order that will be beneficial for testing output from SPI  
    * Need to understand SystemVerilog better  
  * Xiang Li  
    * Not sure about how to insert modules into icicle  
    * Confused about the working principle of icicle  
  * Haoyang Han  
    * How to choose a more suitable microprocessor controlled power monitor