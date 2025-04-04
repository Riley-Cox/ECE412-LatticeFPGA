#include <stdint.h>

#define LEDS            (*(volatile uint32_t*)) 0x00010000)
#define SPI_DATA        (*(volatile uint32_t*) 0x00041000)
#define SPI_CTRL        (*(volatile uint32_t*) 0x00041004)
#define SPI_STATUS  (*(volatile uint32_t*) 0x00041008)
#define SPI_DC          (*(volatile uint32_t*) 0x0004100C)

#define mssg_1    (*(volatile uint32_t*)) 0x00041100)
#define mssg_2    (*(volatile uint32_t*)) 0x0004110C)

//macros to access status bits
#define SPI_BUSY     (SPI_STATUS & 0x01)

void spi_send(uint8_t byte, uint8_t dc_flag) {
    SPI_DC = dc_flag;         // Set data/command
    SPI_DATA = byte;          // Write data byte
    SPI_CTRL = 1;             // Trigger send

    // Wait for SPI to finish
    while (*(volatile uint32_t *)SPI_STATUS & 0x1);  // Wait for spi_done = 1);
}

int main() {
    // Send a command byte (DC = 0)
uint8_t try1 = 0xAA;
uint8_t try2 = 0x55;

for (volatile int i = 0; i < 10; i++) {
    spi_send(try1, 0);

    // Send a data byte (DC = 1)
    spi_send(try2, 1);


}

}
