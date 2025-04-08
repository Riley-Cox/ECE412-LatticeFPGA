# SPI Module Independent Verification

## Environment Setup

* Confirm that your Open Source FPGA toolchain is installed (Yosys, nextpnr-ice40, icestorm).
* Ensure your environment variables are correctly set.

## Module Build and Simulation

* Navigate to the SPI module directory:

``` bash
    cd /SPI/Verification
```

* Clean previous builds

``` bash
    make clean
```

* Compile the SPI module using Yosys

``` bash
    yosys
```

* Run simulation to verify that the synthesized design matches expected SPI behavior.

## Hardware Verification

* Program the FPGA with the SPI module bitstream (using iceprog or your preferred flasher):

```bash
    sudo iceprog spi_module.bin
```

* Connect the FPGA board to the test setup and use a logic analyzer or oscilloscope to check SPI signal integrity (MOSI, SCLK, CS).
* Verify that data transmission and reception conform to specifications.

