
// === Hardware Interface (custom memory-mapped) ===
#define LEDS        (*(volatile unsigned int*) 0x00010000)
#define SPI_DATA    (*(volatile unsigned int*) 0x00041000)
#define SPI_CTRL    (*(volatile unsigned int*) 0x00041004)
#define SPI_STATUS  (*(volatile unsigned int*) 0x00041008)
#define SPI_DC      (*(volatile unsigned int*) 0x0004100C)

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

#define ST7735_BLACK     0x0000
#define ST7735_BLUE      0xF100
#define ST7735_RED       0x008F
#define ST7735_GREEN     0x0E70
#define ST7735_CYAN      0xFF70
#define ST7735_MAGENTA   0xF18F
#define ST7735_YELLOW    0x0EFF
#define ST7735_WHITE     0xFFFF
/**
 * Standard 5x7 font for ASCII characters
 * Each character is 5 columns wide and stored column-wise 
 * Standard ASCII characters from 0x20 (space) to 0x7F (~)
 * Note its different form Adafruit library font, they have other specific characters
 */
static const unsigned char font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // Space - ASCII 32
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x08, 0x2A, 0x1C, 0x2A, 0x08, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x01, 0x01, // F
    0x3E, 0x41, 0x41, 0x49, 0x7A, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x04, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x07, 0x08, 0x70, 0x08, 0x07, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x7F, 0x41, 0x41, 0x00, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // backslash
    0x00, 0x41, 0x41, 0x7F, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x08, 0x14, 0x54, 0x54, 0x3C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x08, 0x04, 0x08, 0x10, 0x08, // ~
    0x00, 0x00, 0x00, 0x00, 0x00  // DEL
};


// Font metrics and properties
#define FONT_WIDTH 5       // Width of each character in pixels
#define FONT_HEIGHT 8      // Height of each character in pixels (includes padding)
#define FONT_FIRST_CHAR 32 // First character in the font array (Space)
#define FONT_LAST_CHAR 126 // Last character in the font array (~)

// === SPI Send Function ===
void *memcpy(void *dest, const void *src, unsigned long n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void spi_send(unsigned char byte, unsigned char dc_flag) {
    SPI_DC = dc_flag;
    SPI_DATA = byte;
    SPI_CTRL = 1;
    while (SPI_BUSY);

}

// === Delay Loop (freestanding safe) ===
void st7735_delay_ms(unsigned int ms) {
    while (ms--) {
        for (volatile unsigned int i = 0; i < 100; i++) {
            __asm__ volatile("nop");
        }
    }
}

// === Display Write Helpers ===
void write_command(unsigned char cmd) {
    spi_send(cmd, 0);
}

void write_data(unsigned char data) {
    spi_send(data, 1);
}

void write_data_buffer(const unsigned char* data, unsigned short size) {
    for (unsigned short i = 0; i < size; i++) {
        write_data(data[i]);
    }
}

void set_addr_window(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1) {
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
void fill_screen(unsigned short color) {
    set_addr_window(0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    for (unsigned int i = 0; i < (ST7735_TFTWIDTH * ST7735_TFTHEIGHT); i++) {
		write_data(hi);
        write_data(lo);
    }
}
**/

void fill_screen(unsigned short color) {
    set_addr_window(0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);
    
    unsigned char hi = color >> 8;
    unsigned char lo = color & 0xFF;

    static unsigned char buffer[256]; 
    for (int i = 0; i < 256; i += 2) {
        buffer[i] = hi;
        buffer[i + 1] = lo;
    }

    for (unsigned int i = 0; i < (ST7735_TFTWIDTH * ST7735_TFTHEIGHT); i += 128) {
        write_data_buffer(buffer, 256); 
    }
}


void draw_pixel(unsigned short x, unsigned short y, unsigned short color) {
    if (x >= ST7735_TFTWIDTH || y >= ST7735_TFTHEIGHT) return;
    set_addr_window(x, y, x, y);
    write_data(color >> 8);
    write_data(color & 0xFF);
}

//Draw a filled Rectangle

void st7735_fill_rect(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short color) {
    // Validate input parameters
    // Cancel if width or height is zero or negative, avoid wrong values
    if (w <= 0 || h <= 0) return;
    
    // Check if rectangle is completely off screen
    if (x >= ST7735_TFTWIDTH || y >= ST7735_TFTHEIGHT) return;
    
    // Clip rectangle to display boundaries
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if ((x + w) > ST7735_TFTWIDTH) { w = ST7735_TFTWIDTH - x; }
    if ((y + h) > ST7735_TFTHEIGHT) { h = ST7735_TFTHEIGHT - y; }
    
    // Set the address window for the rectangle
    // Has -1 because coordinates start from 0
    set_addr_window(x, y, x + w - 1, y + h - 1);
    
    // Calculate total number of pixels
    //unsigned int total_pixels = (unsigned int)w * (unsigned int)h;
    
    // Extract high and low bytes from the color
    unsigned char hi = color >> 8;
    unsigned char lo = color & 0xFF;
    
    // Fill the rectangle with the color
    for (unsigned char i = 0; i < y; i++) {
	    for (unsigned char j = 0; j < x; j++)
		    {
      			write_data(hi);  // High byte
       			write_data(lo);  // Low byte
		    }
    }
}

//Draw a single character
void st7735_draw_char(unsigned short x, unsigned short y, unsigned char c, 
                     unsigned short textColor, unsigned short bgColor, unsigned char size) {
    // Check if character is within supported range
    // If not inside ASCII 20-7F range, substitute with '?'
    if (c < FONT_FIRST_CHAR || c > FONT_LAST_CHAR) {
        c = '?'; // Substitute with question mark for unsupported characters
    }
    
    // Calculate offset in font array
    c -= FONT_FIRST_CHAR;
    
    // Ensure size is at least 1
    if (size < 1) size = 1;
    
    // Calculate character dimensions with scaling
    unsigned char char_width = FONT_WIDTH * size;
    unsigned char char_height = FONT_HEIGHT * size;
    
    // Check if character is completely off screen
    if ((x + char_width <= 0) || (y + char_height <= 0) || 
        (x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT)) {
        return;
    }
    
    // Process each column of the character
    for (unsigned char i = 0; i < FONT_WIDTH; i++) {
        unsigned char line = font[c * FONT_WIDTH + i];
        
        // Process each pixel in the column (row by row)
        for (unsigned char j = 0; j < FONT_HEIGHT; j++, line >>= 1) {
            // Check if pixel should be drawn (LSB first)
            if (line & 0x01) {
                if (size == 1) {
                    // Single pixel - direct draw
                    draw_pixel(x + i, y + j, textColor);
                } else {
                    // Scaled pixel - draw as rectangle
                    st7735_fill_rect(x + i * size, y + j * size, size, size, textColor);
                }
            } else if (bgColor != textColor) {
                // Draw background if colors are different
                if (size == 1) {
                    draw_pixel(x + i, y + j, bgColor);
                } else {
                    st7735_fill_rect(x + i * size, y + j * size, size, size, bgColor);
                }
            }
        }
    }
}

//Draw text string

void st7735_draw_text(unsigned short x, unsigned short y, const char *text,
                     unsigned short textColor, unsigned short bgColor, unsigned char size) {
    unsigned short cursor_x = x;
    unsigned short cursor_y = y;
    unsigned char char_width = FONT_WIDTH * size;
    unsigned char char_height = FONT_HEIGHT * size;
    unsigned char spacing = size; // Space between characters
    
    // Process each character in the string
    while (*text) {
        // Handle newline character
        if (*text == '\n') {
            cursor_x = x;
            cursor_y += char_height + spacing;
        }
        // Handle carriage return
        else if (*text == '\r') {
            cursor_x = x;
        }
        // Regular character
        else {
            // Auto wrap text if it would go off right edge
            if (cursor_x + char_width > ST7735_TFTWIDTH) {
                cursor_x = x;
                cursor_y += char_height + spacing;
            }
            
            // Stop if text would go off bottom edge
            if (cursor_y + char_height > ST7735_TFTHEIGHT) {
                break;
            }
            
            // Draw the character
            st7735_draw_char(cursor_x, cursor_y, *text, textColor, bgColor, size);
            
            // Move cursor to next character position
            cursor_x += char_width + spacing;
        }
        
        // Move to next character in string
        text++;
    }
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
    write_data(0x03);

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

    unsigned char pos_gamma[16] = {
        0x02, 0x1C, 0x07, 0x12,
        0x37, 0x32, 0x29, 0x2D,
        0x29, 0x25, 0x2B, 0x39,
        0x00, 0x01, 0x03, 0x10
    };
    write_command(ST7735_GMCTRP1);
    for (int i = 0; i < 16; i++) {
        write_data(pos_gamma[i]);
    }

    unsigned char neg_gamma[16] = {
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

    for (unsigned short y = 0; y < ST7735_TFTHEIGHT; y += 10) {
        for (unsigned short x = 0; x < ST7735_TFTWIDTH; x++) {
            draw_pixel(x, y, ST7735_WHITE);
        }
    }

	// Reminder: format of draw_text is:
        // st7735_draw_text(x, y, text, textColor, bgColor, size);
        // Display header text
        st7735_draw_text(5, 5, "ST7735 Text Test", ST7735_WHITE, ST7735_BLACK, 1);
    
        // Test different sizes
        st7735_draw_text(5, 20, "Size 1", ST7735_GREEN, ST7735_BLACK, 1);
        st7735_draw_text(5, 35, "Size 2", ST7735_GREEN, ST7735_BLACK, 2);
        st7735_draw_text(5, 55, "Size 3", ST7735_GREEN, ST7735_BLACK, 3);
        
        // Test different colors
        st7735_draw_text(5, 85, "RED", ST7735_RED, ST7735_BLACK, 1);
        st7735_draw_text(35, 85, "GREEN", ST7735_GREEN, ST7735_BLACK, 1);
        st7735_draw_text(85, 85, "BLUE", ST7735_BLUE, ST7735_BLACK, 1);
        
        // Test background colors
        st7735_draw_text(5, 100, "BG Test", ST7735_BLACK, ST7735_YELLOW, 1);
        
        // Test newlines
        st7735_draw_text(5, 115, "Line 1\nLine 2\nLine 3", ST7735_CYAN, ST7735_BLACK, 1);
        
        // Test wrapping (long text that will automatically wrap)
        st7735_draw_text(5, 145, "YOU SHOULD SEE THIS TEXT WRAP AROUND TO THE NEXT LINE", ST7735_MAGENTA, ST7735_BLACK, 1);
    while (1);  // Idle loop

    return 0;
}
