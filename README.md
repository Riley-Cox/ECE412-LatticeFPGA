# ECE412-LatticeFPGA
## WSL Setup
- Install the Terminal app on the Mircosoft Store
- Install a Linux Distribution from the Microsoft Store, Debian was used for this project
- Install usbipd through PowerShell by using the following command
  
  ```
  winget install --interactive --exact dorssel.usbipd-win
  ```
- To ensure that it is installed run `usbipd list` , you should see your usb device with its BusID 
- A USB bind is needed. Open Powershell as Admin, then run `usbipd bind --busid <busid>`
- To attach a USB to WSL run `usbipd attach --wsl --busid <busid>`
- In WSL, the device should show up when `lsusb` is ran
- To disconnect the device run `usbipd detach --busid <busid>`

## Dependencies
### Ubuntu/Debian
```
sudo apt-get install autoconf automake autotools-dev curl python3 python3-pip python3-tomli libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev xxd make
```
### Fedora
```
sudo yum install autoconf automake python3 libmpc-devel mpfr-devel gmp-devel gawk  bison flex texinfo patchutils gcc gcc-c++ zlib-devel expat-devel libslirp-devel xxd make
```
### Arch
```
sudo pacman -Syu autoconf automake curl python3 libmpc mpfr gmp gawk base-devel bison flex texinfo gperf libtool patchutils bc zlib expat libslirp xxd make
```
## RISC-V GNU Toolchain
- From your home directory, clone the RISC-V GNU toolchain from the following repo 

  ```
  git clone https://github.com/riscv/riscv-gnu-toolchain
  ```
  
- `cd` into the directory, run
  ```
  ./configure --prefix=/opt/riscv
  ```
  then run `sudo make` (This will take a long time)
- From your home directory, open your shell rc file to edit (`vim .zshrc` or `vim .bashrc`) and add
  ```
  export PATH="/opt/riscv/bin:$PATH"
  ```
- Then run `source .zshrc` if using zsh or `source .bashrc` if using bash
  
  
## Open Source Toolchain Setup
### Ubuntu/Debian
```
sudo apt-get install yosys fpga-icestorm nextpnr-ice40-qt
```
### Arch
```
yay -S yosys-git icestorm-git nextpnr-git
```
### Fedora
```
sudo dnf install yosys icestorm nextpnr
```


## Building on the UPduino board
- Navigate to `/icicle`
- Run `make clean`
- Run `make BOARD=upduino`
- Run `sudo iceprog top.bin`


## Migrating Icicle from UPduino V1 to V2.1
- Removed SPI_FLASH define from the upduino-defines.sv file located in `/icicle/boards`
- Changed PROGMEM value from flash to ram in the upduino.mk file located in `/icicle/boards`
- Changed UART RX/TX pins to 15 and 14 respectively in the upduino.pcf file located in `/icicle/boards`
- Created a flash_csn output in icicle.sv and assign it 1 (`assign flash_csn = 1`)
