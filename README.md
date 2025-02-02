# ECE412-LatticeFPGA
## Migrating Icicle from UPduino V1 to V2.1
- Remove SPI_FLASH define from the upduino-defines.sv file located in `/icicle/boards`
- Change PROGMEM value from flash to ram in the upduino.mk file located in `/icicle/boards`
- Change UART RX/TX pins to 15 and 14 respectively in the upduino.pcf file located in `/icicle/boards`
- Create a flash_csn output in icicle.sv and assign it 1 (`assign flash_csn = 1`) 
