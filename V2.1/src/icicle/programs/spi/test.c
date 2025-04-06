/**
 * Combined ST7735 Display Driver Test
 * Based on the files created by Xiang
 */

#include <stdint.h>
#include <stdbool.h>

/* ============================== */
/*     FUNCTION PROTOTYPES        */
/* ============================== */

void st7735_delay_ms(uint32_t ms);
void spi_wait_done(void);
void st7735_write_command(uint8_t cmd);
void st7735_write_data(uint8_t data);
void st7735_write_data_buffer(const uint8_t* buffer, uint16_t size);
void st7735_hardware_reset(void);
void st7735_init(void);
void st7735_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void st7735_fill_screen(uint16_t color);
void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

/* ============================== */
/*     ST7735_HW.H CONTENTS       */
/* ============================== */

// Memory-mapped register definitions for SPI controller
#define SPI_BASE_ADDR     0x00041000

#define SPI_DATA_OFFSET   0x00
#define SPI_CTRL_OFFSET   0x04
#define SPI_STATUS_OFFSET 0x08
#define SPI_CONFIG_OFFSET 0x0C

#define SPI_DATA_REG     (*(volatile uint8_t*)(SPI_BASE_ADDR + SPI_DATA_OFFSET))
#define SPI_CTRL_REG     (*(volatile uint8_t*)(SPI_BASE_ADDR + SPI_CTRL_OFFSET))
#define SPI_STATUS_REG   (*(volatile uint8_t*)(SPI_BASE_ADDR + SPI_STATUS_OFFSET))
#define SPI_CONFIG_REG   (*(volatile uint8_t*)(SPI_BASE_ADDR + SPI_CONFIG_OFFSET))

#define SPI_CTRL_START   0x01
#define SPI_CTRL_DC      0x02
#define SPI_CTRL_RESET   0x04

#define SPI_STATUS_BUSY  0x01
#define SPI_STATUS_DONE  0x02

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 128

// Command constants
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

#define ST7735_BLACK     0x0000
#define ST7735_BLUE      0x001F
#define ST7735_RED       0xF800
#define ST7735_GREEN     0x07E0
#define ST7735_CYAN      0x07FF
#define ST7735_MAGENTA   0xF81F
#define ST7735_YELLOW    0xFFE0
#define ST7735_WHITE     0xFFFF

/* ============================== */
/*     ST7735_HW.C CONTENTS       */
/* ============================== */

void spi_wait_done(void) {
    while (!(SPI_STATUS_REG & SPI_STATUS_DONE));
}

void st7735_write_command(uint8_t cmd) {
    while (SPI_STATUS_REG & SPI_STATUS_BUSY);
    SPI_DATA_REG = cmd;
    SPI_CTRL_REG = SPI_CTRL_START;
    spi_wait_done();
}

void st7735_write_data(uint8_t data) {
    while (SPI_STATUS_REG & SPI_STATUS_BUSY);
    SPI_DATA_REG = data;
    SPI_CTRL_REG = SPI_CTRL_START | SPI_CTRL_DC;
    spi_wait_done();
}

void st7735_write_data_buffer(const uint8_t* buffer, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        st7735_write_data(buffer[i]);
    }
}

void st7735_hardware_reset(void) {
    SPI_CTRL_REG &= ~SPI_CTRL_RESET;
    st7735_delay_ms(10);
    SPI_CTRL_REG |= SPI_CTRL_RESET;
    st7735_delay_ms(120);
}

void st7735_delay_ms(uint32_t ms) {
    volatile uint32_t cycles = ms * 12000;
    while (cycles--);
}

/* ============================== */
/*       ST7735.C CONTENTS        */
/* ============================== */

void st7735_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    st7735_write_command(ST7735_CASET);
    st7735_write_data(0x00); st7735_write_data(x0);
    st7735_write_data(0x00); st7735_write_data(x1);

    st7735_write_command(ST7735_RASET);
    st7735_write_data(0x00); st7735_write_data(y0);
    st7735_write_data(0x00); st7735_write_data(y1);

    st7735_write_command(ST7735_RAMWR);
}

void st7735_fill_screen(uint16_t color) {
    st7735_set_addr_window(0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);
    uint32_t total_pixels = (uint32_t)ST7735_TFTWIDTH * ST7735_TFTHEIGHT;
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    for (uint32_t i = 0; i < total_pixels; i++) {
        st7735_write_data(hi);
        st7735_write_data(lo);
    }
}

void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if ((x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT)) return;
    st7735_set_addr_window(x, y, x, y);
    st7735_write_data(color >> 8);
    st7735_write_data(color & 0xFF);
}

void st7735_init(void) {
    st7735_hardware_reset();
    st7735_write_command(ST7735_SWRESET); 
    st7735_delay_ms(150);
    st7735_write_command(ST7735_SLPOUT);
    st7735_delay_ms(500);
    st7735_write_command(ST7735_FRMCTR1);
    st7735_write_data(0x01); 
    st7735_write_data(0x2C);   
    st7735_write_data(0x2D);
    st7735_write_command(ST7735_FRMCTR2); 
    st7735_write_data(0x01); 
    st7735_write_data(0x2C);  
    st7735_write_data(0x2D);
    st7735_write_command(ST7735_FRMCTR3); 
    st7735_write_data(0x01); 
    st7735_write_data(0x2C); 
    st7735_write_data(0x2D);
    st7735_write_data(0x01);
    st7735_write_data(0x2C);
    st7735_write_data(0x2D);
    st7735_write_command(ST7735_INVCTR); 
    st7735_write_data(0x07);
    st7735_write_command(ST7735_PWCTR1); 
    st7735_write_data(0xA2);
    st7735_write_data(0x02);
    st7735_write_data(0x84);
    st7735_write_command(ST7735_PWCTR2); 
    st7735_write_data(0xC5);
    st7735_write_command(ST7735_PWCTR3); 
    st7735_write_data(0x0A); 
    st7735_write_data(0x00);
    st7735_write_command(ST7735_PWCTR4); 
    st7735_write_data(0x8A); 
    st7735_write_data(0x2A);
    st7735_write_command(ST7735_PWCTR5); 
    st7735_write_data(0x8A);
    st7735_write_data(0xEE);
    st7735_write_command(ST7735_VMCTR1); 
    st7735_write_data(0x0E);
    st7735_write_command(ST7735_INVOFF);
    st7735_write_command(ST7735_MADCTL);
    st7735_write_data(0xC8);
    st7735_write_command(ST7735_COLMOD); 
    st7735_write_data(0x05);

    st7735_write_command(ST7735_CASET);
    st7735_write_data(0x00);
    st7735_write_data(0x00);  
    st7735_write_data(0x00); 
    st7735_write_data(0x7F);
    st7735_write_command(ST7735_RASET);
    st7735_write_data(0x00);
    st7735_write_data(0x00); 
    st7735_write_data(0x00); 
    st7735_write_data(0x7F);

    uint8_t pos_gamma[16] = {0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10};
    st7735_write_command(ST7735_GMCTRP1);
    st7735_write_data_buffer(pos_gamma, 16);

    uint8_t neg_gamma[16] = {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10};
    st7735_write_command(ST7735_GMCTRN1);
    st7735_write_data_buffer(neg_gamma, 16);

    st7735_write_command(ST7735_NORON); 
    st7735_delay_ms(10);
    st7735_write_command(ST7735_DISPON); 
    st7735_delay_ms(100);
}

/* ============================== */
/*         MAIN FUNCTION          */
/* ============================== */

int main(void) {
    st7735_init();
    
    st7735_fill_screen(ST7735_RED);
    st7735_delay_ms(1000);

    st7735_fill_screen(ST7735_GREEN);
    st7735_delay_ms(1000);

    st7735_fill_screen(ST7735_BLUE);
    st7735_delay_ms(1000);

    for (int i = 0; i < ST7735_TFTHEIGHT; i += 10) {
        for (int j = 0; j < ST7735_TFTWIDTH; j++) {
            st7735_draw_pixel(j, i, ST7735_WHITE);
        }
    }

    while (1) {
        // Idle loop
    }

    return 0;
}
