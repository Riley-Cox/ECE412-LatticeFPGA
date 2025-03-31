/**
 * ST7735 Hardware Interface Implementation
 * 
 * This file implements the low-level hardware communication functions
 * for controlling an ST7735 display from a RISC-V processor using
 * a memory-mapped SPI controller.
 */

#include "st7735_hw.h"

/**
 * Wait for SPI transfer to complete
 * This function polls the status register until the DONE bit is set.
 */
void spi_wait_done(void) {
    // Poll until the SPI_STATUS_DONE bit is set
    while (!(SPI_STATUS_REG & SPI_STATUS_DONE));
}

/**
 * Send a command byte to the ST7735 display
 * Sets DC=0 to indicate command mode.
 * 
 * @param cmd Command byte to send
 */
void st7735_write_command(uint8_t cmd) {
    // Wait if a previous transfer is still in progress
    while (SPI_STATUS_REG & SPI_STATUS_BUSY);
    
    // Place data in the data register
    SPI_DATA_REG = cmd;
    
    // Set control register with DC=0 (command) and START=1
    SPI_CTRL_REG = SPI_CTRL_START;  // DC bit is 0 for command
    
    // Wait for transfer to complete
    spi_wait_done();
}

/**
 * Send a data byte to the ST7735 display
 * Sets DC=1 to indicate data mode.
 * 
 * @param data Data byte to send
 */
void st7735_write_data(uint8_t data) {
    // Wait if a previous transfer is still in progress
    while (SPI_STATUS_REG & SPI_STATUS_BUSY);
    
    // Place data in the data register
    SPI_DATA_REG = data;
    
    // Set control register with DC=1 (data) and START=1
    SPI_CTRL_REG = SPI_CTRL_START | SPI_CTRL_DC;
    
    // Wait for transfer to complete
    spi_wait_done();
}

/**
 * Send multiple data bytes to the ST7735 display
 * 
 * @param buffer Pointer to data buffer to send
 * @param size Number of bytes to send
 */
void st7735_write_data_buffer(const uint8_t* buffer, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        st7735_write_data(buffer[i]);
    }
}

/**
 * Perform hardware reset of the ST7735 display
 * This toggles the reset line according to the datasheet timing requirements.
 * Uses normall format, will need to change based on implementation
 */
void st7735_hardware_reset(void) {
    // Assert reset (active low)
    SPI_CTRL_REG &= ~SPI_CTRL_RESET;  // Clear reset bit
    st7735_delay_ms(10);              // Hold in reset for 10ms
    
    // De-assert reset
    SPI_CTRL_REG |= SPI_CTRL_RESET;   // Set reset bit
    st7735_delay_ms(120);             // Wait for display to initialize (datasheet requirement)
}

/**
 * Delay for specified number of milliseconds
 * This is a simple busy-wait delay function.
 * 
 * @param ms Number of milliseconds to delay
 */
void st7735_delay_ms(uint32_t ms) {
    // This is a simple busy-wait delay
    // For a 12MHz system clock, approximately 12,000 cycles per millisecond
    // Adjust the multiplier based on system clock frequency
    volatile uint32_t cycles = ms * 12000;
    while (cycles--);
}