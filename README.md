# ECE412-LatticeFPGA
## WSL Setup
- Install the Terminal app on the Mircosoft Store
- Install a Linux Distrobution from the Microsoft Store, Debian was used for this project
- Install usbipd through PowerShell by using the following command `winget install --interactive --exact dorssel.usbipd-win`
- To ensure that it is installed run `usbipd list`
- A USB bind is needed, run `usbipd bind --busid <busid>`
- To attach a USB to WSL run `usbipd attach --wsl --busid <busid>`
- In WSL, the device should show up when `lsusb` is ran
- To disconnect the device run `usbipd detach --busid <busid>`

## Dependencies
- Install RISC-V GNU toolchain `sudo apt-get install gcc-riscv-unknown-elf`
- Install Make `sudo apt-get install make`
  
## Open Source Toolchain Setup
### Ubuntu/Debian
`sudo apt-get install yosys fpga-icestorm nextpnr-ice40-qt`
### Arch
`yay -S yosys-git icestorm-git nextpnr-git`
### Fedora
`sudo dnf install yosys icestorm nextpnr`

## Migrating Icicle from UPduino V1 to V2.1
- Remove SPI_FLASH define from the upduino-defines.sv file located in `/icicle/boards`
- Change PROGMEM value from flash to ram in the upduino.mk file located in `/icicle/boards`
- Change UART RX/TX pins to 15 and 14 respectively in the upduino.pcf file located in `/icicle/boards`
- Create a flash_csn output in icicle.sv and assign it 1 (`assign flash_csn = 1`) 
