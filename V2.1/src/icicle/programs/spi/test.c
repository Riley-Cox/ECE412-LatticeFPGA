#include <stdint.h>

// === Hardware Interface (custom memory-mapped) ===
#define LEDS        (*(volatile uint32_t*) 0x00010000)
#define SPI_DATA    (*(volatile uint32_t*) 0x00041000)
#define SPI_CTRL    (*(volatile uint32_t*) 0x00041004)
#define SPI_STATUS  (*(volatile uint32_t*) 0x00041008)
#define SPI_DC      (*(volatile uint32_t*) 0x0004100C)

#define SPI_BUSY    (SPI_STATUS & 0x01)

// === ST7735 Display Constants ===
#define ST7735_TFTWIDTH  160
#define ST7735_TFTHEIGHT 160

#define ST7735_SWRESET   0x01
#define ST7735_SLPOUT    0x11
#define ST7735_FRMCTR1   0xB1
#define ST7735_FRMCTR2   0xB2
#define ST7735_FRMCTR3   0xB3
#define ST7735_INVCTR    0xB4
#define ST7735_PWCTR1    0xC0
#define ST7735_PWCTR2    0xC1
#define ST7735_PWCTR3    0xC2
#define ST7735_PWCTR4    0xC3
#define ST7735_PWCTR5    0xC4
#define ST7735_VMCTR1    0xC5
#define ST7735_INVOFF    0x20
#define ST7735_MADCTL    0x36
#define ST7735_COLMOD    0x3A
#define ST7735_CASET     0x2A
#define ST7735_RASET     0x2B
#define ST7735_RAMWR     0x2C
#define ST7735_GMCTRP1   0xE0
#define ST7735_GMCTRN1   0xE1
#define ST7735_NORON     0x13
#define ST7735_DISPON    0x29

#define ST7735_RED       0xF800
#define ST7735_GREEN     0x07E0
#define ST7735_BLUE      0x001F
#define ST7735_WHITE     0xFFFF

// === SPI Send Function ===
void *memcpy(void *dest, const void *src, unsigned long n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void spi_send(uint8_t byte, uint8_t dc_flag) {
    SPI_DC = dc_flag;
    SPI_DATA = byte;
    SPI_CTRL = 1;
    while (SPI_BUSY);

}

// === Delay Loop (freestanding safe) ===
void st7735_delay_ms(uint32_t ms) {
    while (ms--) {
        for (volatile uint32_t i = 0; i < 100; i++) {
            __asm__ volatile("nop");
        }
    }
}

// === Display Write Helpers ===
void write_command(uint8_t cmd) {
    spi_send(cmd, 0);
}

void write_data(uint8_t data) {
    spi_send(data, 1);
}

void write_data_buffer(const uint8_t* data, uint16_t size) {
    for (uint16_t i = 0; i < size; i++) {
        write_data(data[i]);
    }
}

void set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    write_command(ST7735_CASET);
    write_data(0x00);
    write_data(x0);
    write_data(0x00);
    write_data(x1);

    write_command(ST7735_RASET);
    write_data(0x00);
    write_data(y0);
    write_data(0x00);
    write_data(y1);

    write_command(ST7735_RAMWR);
}
/**
void fill_screen(uint16_t color) {
    set_addr_window(0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    for (uint32_t i = 0; i < (ST7735_TFTWIDTH * ST7735_TFTHEIGHT); i++) {
		write_data(hi);
        write_data(lo);
    }
}
**/

void fill_screen(uint16_t color) {
    set_addr_window(0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);
    
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    static uint8_t buffer[256];  // 
    for (int i = 0; i < 256; i += 2) {
        buffer[i] = hi;
        buffer[i + 1] = lo;
    }

    for (uint32_t i = 0; i < (ST7735_TFTWIDTH * ST7735_TFTHEIGHT); i += 128) {
        write_data_buffer(buffer, 256);  
    }
}


void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ST7735_TFTWIDTH || y >= ST7735_TFTHEIGHT) return;
    set_addr_window(x, y, x, y);
    write_data(color >> 8);
    write_data(color & 0xFF);
}

// === ST7735 Initialization Sequence ===
void st7735_init(void) {
    write_command(ST7735_SWRESET);
    st7735_delay_ms(150);

    write_command(ST7735_SLPOUT);
    st7735_delay_ms(500);

    write_command(ST7735_FRMCTR1);
	write_data(0x00);
	write_data(0x06);
	write_data(0x03);


    write_command(ST7735_FRMCTR2);
	write_data(0x00);
	write_data(0x06);
	write_data(0x03);

    write_command(ST7735_FRMCTR3);
	write_data(0x00);
	write_data(0x06);
	write_data(0x03);	
	write_data(0x00);
	write_data(0x06);
	write_data(0x03);

    write_command(ST7735_INVCTR);
    write_data(0x07);

    write_command(ST7735_PWCTR1);
    write_data(0xA2);
    write_data(0x02);
    write_data(0x84);

    write_command(ST7735_PWCTR2);
    write_data(0xC5);

    write_command(ST7735_PWCTR3);
    write_data(0x0A);
    write_data(0x00);

    write_command(ST7735_PWCTR4);
    write_data(0x8A);
    write_data(0x2A);

    write_command(ST7735_PWCTR5);
    write_data(0x8A);
    write_data(0xEE);

    write_command(ST7735_VMCTR1);
    write_data(0x0E);

    write_command(ST7735_INVOFF);

    write_command(ST7735_MADCTL);
    write_data(0xC8);

    write_command(ST7735_COLMOD);
    write_data(0x05);

    write_command(ST7735_CASET);
    write_data(0x00);
    write_data(0x00);
    write_data(0x00);
    write_data(0x9F);

    write_command(ST7735_RASET);
    write_data(0x00);
    write_data(0x00);
    write_data(0x00);
    write_data(0x9F);

    uint8_t pos_gamma[16] = {
        0x02, 0x1C, 0x07, 0x12,
        0x37, 0x32, 0x29, 0x2D,
        0x29, 0x25, 0x2B, 0x39,
        0x00, 0x01, 0x03, 0x10
    };
    write_command(ST7735_GMCTRP1);
    for (int i = 0; i < 16; i++) {
        write_data(pos_gamma[i]);
    }

    uint8_t neg_gamma[16] = {
        0x03, 0x1D, 0x07, 0x06,
        0x2E, 0x2C, 0x29, 0x2D,
        0x2E, 0x2E, 0x37, 0x3F,
        0x00, 0x00, 0x02, 0x10
    };
    write_command(ST7735_GMCTRN1);
    for (int i = 0; i < 16; i++) {
        write_data(neg_gamma[i]);
    }

    write_command(ST7735_NORON);
    st7735_delay_ms(10);

    write_command(ST7735_DISPON);
    st7735_delay_ms(100);
}

// === Main Test ===
int main(void) {
    st7735_init();

    fill_screen(ST7735_RED);
    st7735_delay_ms(500);

    fill_screen(ST7735_GREEN);
    st7735_delay_ms(500);

    fill_screen(ST7735_BLUE);
    st7735_delay_ms(500);

    for (uint16_t y = 0; y < ST7735_TFTHEIGHT; y += 10) {
        for (uint16_t x = 0; x < ST7735_TFTWIDTH; x++) {
            draw_pixel(x, y, ST7735_WHITE);
        }
    }

    while (1);  // Idle loop

    return 0;
}
