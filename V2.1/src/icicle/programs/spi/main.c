#include <stdint.h>

#define LEDS		(*(volatile uint32_t*)) 0x00010000)
#define SPI_DATA 	(*(volatile uint32_t*) 0x80000000)
#define SPI_CTRL	(*(volatile uint32_t*) 0x80000004)
#define SPI_STATUS  (*(volatile uint32_t*) 0x80000008)
#define SPI_DC		(*(volatile uint32_t*) 0x8000000C)

//macros to access status bits
#define SPI_BUSY     (SPI_STATUS & 0x01)

void spi_send(uint8_t byte, uint8_t dc_flag) {
    SPI_DC = dc_flag;         // Set data/command
    SPI_DATA = byte;          // Write data byte
    SPI_CTRL = 1;             // Trigger send

    // Wait for SPI to finish
    while (SPI_BUSY);
}

int main() {
    int i = 10;
while (i > 0)
{
    // Send a command byte (DC = 0)
    spi_send(0xAE, 0); 

    // Send a data byte (DC = 1)
    spi_send(0x3F, 1); 



	i += -1;
}

}
