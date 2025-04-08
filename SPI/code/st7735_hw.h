// Hardware interface for ST7735 display on RISC-V
// This header file defines the memory-mapped registers and control bits for the SPI controller
// Used a general format, will need to be changed for specific RISC-V implementations(icicle)

#ifndef ST7735_HW_H
#define ST7735_HW_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Memory-mapped register definitions for SPI controller
 * Base address: 0x10010000 (customize for your specific RISC-V implementation)
 */

// Base address for the SPI controller
// CHANGE based on implementation
#define SPI_BASE_ADDR     0x10010000

// Register offsets
// All offsets will need to be changed based on implementation, this is just a general format
#define SPI_DATA_OFFSET   0x00  // Data register (write: send data, read: receive data)
#define SPI_CTRL_OFFSET   0x04  // Control register (start bit, DC bit, etc.)
#define SPI_STATUS_OFFSET 0x08  // Status register (busy flag, done flag)
#define SPI_CONFIG_OFFSET 0x0C  // Optional configuration register

// Memory-mapped register addresses
// Volatile keyword ensures that the compiler does not optimize these accesses
// Useful for hardware registers
#define SPI_DATA_REG     (*(volatile uint8_t*)(SPI_BASE_ADDR + SPI_DATA_OFFSET))
#define SPI_CTRL_REG     (*(volatile uint8_t*)(SPI_BASE_ADDR + SPI_CTRL_OFFSET))
#define SPI_STATUS_REG   (*(volatile uint8_t*)(SPI_BASE_ADDR + SPI_STATUS_OFFSET))
#define SPI_CONFIG_REG   (*(volatile uint8_t*)(SPI_BASE_ADDR + SPI_CONFIG_OFFSET))

/**
 * Control register bit definitions
 * Just example, delete or change if needed
 */
#define SPI_CTRL_START   0x01  // Bit 0: Start transfer when set to 1
#define SPI_CTRL_DC      0x02  // Bit 1: Data/Command selection (0=command, 1=data)
#define SPI_CTRL_RESET   0x04  // Bit 2: LCD Reset control (0=reset active, 1=normal operation)

/**
 * Status register bit definitions
 */
#define SPI_STATUS_BUSY  0x01  // Bit 0: SPI transfer in progress when set to 1
#define SPI_STATUS_DONE  0x02  // Bit 1: SPI transfer complete when set to 1


/**
 * ST7735R display specific constants
 */
// Display dimensions
// For 1.44" display should be 128x128
// For 1.8" display should be 128x160
// For mini display should be 80x160
#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 128

// ST7735R commands from Adafruit library
#define ST7735_NOP       0x00
#define ST7735_SWRESET   0x01
#define ST7735_RDDID     0x04
#define ST7735_RDDST     0x09
#define ST7735_SLPIN     0x10
#define ST7735_SLPOUT    0x11
#define ST7735_PTLON     0x12
#define ST7735_NORON     0x13
#define ST7735_INVOFF    0x20
#define ST7735_INVON     0x21
#define ST7735_DISPOFF   0x28
#define ST7735_DISPON    0x29
#define ST7735_CASET     0x2A
#define ST7735_RASET     0x2B
#define ST7735_RAMWR     0x2C
#define ST7735_RAMRD     0x2E
#define ST7735_PTLAR     0x30
#define ST7735_COLMOD    0x3A
#define ST7735_MADCTL    0x36
#define ST7735_FRMCTR1   0xB1
#define ST7735_FRMCTR2   0xB2
#define ST7735_FRMCTR3   0xB3
#define ST7735_INVCTR    0xB4
#define ST7735_DISSET5   0xB6
#define ST7735_PWCTR1    0xC0
#define ST7735_PWCTR2    0xC1
#define ST7735_PWCTR3    0xC2
#define ST7735_PWCTR4    0xC3
#define ST7735_PWCTR5    0xC4
#define ST7735_VMCTR1    0xC5
#define ST7735_RDID1     0xDA
#define ST7735_RDID2     0xDB
#define ST7735_RDID3     0xDC
#define ST7735_RDID4     0xDD
#define ST7735_GMCTRP1   0xE0
#define ST7735_GMCTRN1   0xE1

// Color definitions in 5-6-5 RGB format (16-bit)
// This is some already defined colors
#define ST7735_BLACK     0x0000
#define ST7735_BLUE      0x001F
#define ST7735_RED       0xF800
#define ST7735_GREEN     0x07E0
#define ST7735_CYAN      0x07FF
#define ST7735_MAGENTA   0xF81F
#define ST7735_YELLOW    0xFFE0
#define ST7735_WHITE     0xFFFF

/**
 * Hardware interface function prototypes
 */
// Wait until SPI transfer completes
void spi_wait_done(void);

// Send a command byte to the ST7735 display
void st7735_write_command(uint8_t cmd);

// Send a data byte to the ST7735 display
void st7735_write_data(uint8_t data);

// Send multiple data bytes to the ST7735 display
void st7735_write_data_buffer(const uint8_t* buffer, uint16_t size);

// Perform hardware reset of the ST7735 display
void st7735_hardware_reset(void);

// Delay for specified number of milliseconds
void st7735_delay_ms(uint32_t ms);

#endif // ST7735_HW_H