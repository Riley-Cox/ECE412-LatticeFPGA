# **Lattice FPGA Project Weekly Progress Report \- Week 4**

Report date: 2025/01/31

* **Last Week (time in hours)**  
  * Team Review  
    * Xiang and Mohammed worked on PDS to send to Roy and Rahul. Implementing feedback now  
  * Nathaniel Fraly  
    * Yosys, Icestorm, and NextPNR tool setup (2)  
    * Debian Linux environment setup (10)  
  * Riley Cox  
    * Yosys, Icestorm, and NextPNR tool setup and working with bitstream generation (4)  
    * RISC-V core research (3)  
    * Icicle RISC-V core build and verification (2)  
  * Mohammad Alshaiji  
    * Successfully completed synthesis using Yosys on WSL Debian (3)  
    * Researched SPI implementation and identified some IP cores for further research (3)  
    * Read further through IEEE papers (2)  
  * Xiang Li  
    * Revise PDS(3)  
    * Set up Yosys (1)  
    * Research on SPI cores (3)  
  * Haoyang Han  
    * Research power monitor (3)  
    * Choose the right power monitor (1)  
    * Set up Yosys (1)  
    * Research on SPI cores (3)  
* **Next week**  
  * Team Plan  
    * Familiarize with Icicle RISC-V core, get SPI module implementation started  
  * Nathaniel Fraly  
    * Get OpenSource Toolchain verified functional  
    * Verify Icicle default persistence (is program stored in volatile or non-volatile memory)  
    * Get Icicle core on FPGA board (Dual w/ Riley)  
    * Assist w/ PDS as needed to meet the deadline.  
  * Riley Cox  
    * Need to get Lattice Radiant setup and working  
    * Need to modify current version of Icicle to work for UPduino V2.1  
  * Mohammad Alshaiji  
    * Get Icestorm setup and read further through the documentation  
    * Get upduino V2.1 and V3.1 comparison to Rahul before Tuesday  
    * Assist with PDS  
    * Implement SPI IP on upduino board  
    * Get Radiant toolchain installed and setup  
  * Xiang Li  
    * Finish PDS  
    * Figure out how to implement SPI inside a core  
    * Learn about SPI core code  
  * Haoyang Han  
    * Get power monitor and start debugging  
    * Figure out how to implement SPI inside a core  
    * Learn about SPI core code  
* **Blocked**  
  * Team Blocks  
    * Differences between UPDuino v2.1 and v3.0 need to be documented so implementation can move forward. Weekly progress reports being done last-minute, Nathan to implement team policy to prevent reoccurence.   
  * Nathaniel Fraly  
    * Issues with Linux environment not working on personal hardware. Need to fix bootloader issues and SecureBoot issues, or move to a different environment solution. Perfect environment is useless if it doesn’t start working until April.  
  * Riley Cox  
    * The Icicle RISC-V core for UPduino is meant for V1.0 boards, the board I have is a V2.1 so there are changes that need to be made to get it working to be able to see output on the serial monitor.   
    * Need to get USB tunneling working for WSL on desktop, currently using a laptop with Arch to program board.  
  * Mohammad Alshaiji  
    * Need to get a test bitstream using Icestorm and Yosys and ensure functioning with arduino board.   
  * Xiang Li  
    * WSL can’t directly visit USB device  
    * ST7735R datasheet hard to read  
  * Haoyang Han  
    * There are so many different types of power monitors, it can be hard to find the right one