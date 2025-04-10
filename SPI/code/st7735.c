/**
 * ST7735 Display Driver Implementation
 * 
 * This file implements functions for initializing and drawing to
 * an ST7735 display. Based on the Adafruit ST7735 library.
 */

#include "st7735.h"

/**
 * Initialize the ST7735 display
 * This follows the initialization sequence from the Adafruit ST7735 library
 */
void st7735_init(void) {
    // Hardware reset the display
    st7735_hardware_reset();
    
    // Software reset
    st7735_write_command(ST7735_SWRESET);
    st7735_delay_ms(150);
    
    // Out of sleep mode
    st7735_write_command(ST7735_SLPOUT);
    st7735_delay_ms(500);
    
    // Frame rate control - normal mode
    st7735_write_command(ST7735_FRMCTR1);
    st7735_write_data(0x01);  // Rate = fosc/(1x2+40) * (LINE+2C+2D)
    st7735_write_data(0x2C);
    st7735_write_data(0x2D);
    
    // Frame rate control - idle mode
    st7735_write_command(ST7735_FRMCTR2);
    st7735_write_data(0x01);
    st7735_write_data(0x2C);
    st7735_write_data(0x2D);
    
    // Frame rate control - partial mode
    st7735_write_command(ST7735_FRMCTR3);
    st7735_write_data(0x01);
    st7735_write_data(0x2C);
    st7735_write_data(0x2D);
    st7735_write_data(0x01);
    st7735_write_data(0x2C);
    st7735_write_data(0x2D);
    
    // Display inversion control
    st7735_write_command(ST7735_INVCTR);
    st7735_write_data(0x07);  // No inversion
    
    // Power control 1
    st7735_write_command(ST7735_PWCTR1);
    st7735_write_data(0xA2);
    st7735_write_data(0x02);  // -4.6V
    st7735_write_data(0x84);  // AUTO mode
    
    // Power control 2
    st7735_write_command(ST7735_PWCTR2);
    st7735_write_data(0xC5);  // VGH25=2.4V, VGL=-8.0V
    
    // Power control 3 (normal mode)
    st7735_write_command(ST7735_PWCTR3);
    st7735_write_data(0x0A);  // Opamp current small
    st7735_write_data(0x00);  // Boost frequency
    
    // Power control 4 (idle mode)
    st7735_write_command(ST7735_PWCTR4);
    st7735_write_data(0x8A);  // Opamp current small
    st7735_write_data(0x2A);  // Boost frequency
    
    // Power control 5 (partial mode)
    st7735_write_command(ST7735_PWCTR5);
    st7735_write_data(0x8A);  // Opamp current small
    st7735_write_data(0xEE);  // Boost frequency
    
    // VCOM control 1
    st7735_write_command(ST7735_VMCTR1);
    st7735_write_data(0x0E);

    // Display Inversion Off
    st7735_write_command(ST7735_INVOFF); 

    // Memory access control (orientation)
    st7735_write_command(ST7735_MADCTL);
    st7735_write_data(0xC8);  // Row address/col address, bottom to top refresh
    
    // Color mode (16-bit color)
    st7735_write_command(ST7735_COLMOD);
    st7735_write_data(0x05);  // 16-bit (65K colors)

    // Next part is display tab variant, in this place 1.44" display green tab
    // Column address set
    st7735_write_command(ST7735_CASET);
    st7735_write_data(0x00);  // Start column = 0
    st7735_write_data(0x00);
    st7735_write_data(0x00);  // End column = 127
    st7735_write_data(0x7F);
    
    // Row address set
    st7735_write_command(ST7735_RASET);
    st7735_write_data(0x00);  // Start row = 0
    st7735_write_data(0x00);
    st7735_write_data(0x00);  // End row = 159/127 (depending on model)
    st7735_write_data(0x9F);  // For 1.44" display, 0x9F for 1.8" display
    
    // Gamma adjustments (positive polarity)
    st7735_write_command(ST7735_GMCTRP1);
    uint8_t pos_gamma[16] = {0x02, 0x1c, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2d,
                            0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10};
    st7735_write_data_buffer(pos_gamma, 16);
    
    // Gamma adjustments (negative polarity)
    st7735_write_command(ST7735_GMCTRN1);
    uint8_t neg_gamma[16] = {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
                            0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10};
    st7735_write_data_buffer(neg_gamma, 16);
    
    // Normal display on
    st7735_write_command(ST7735_NORON);
    st7735_delay_ms(10);
    
    // Display on
    st7735_write_command(ST7735_DISPON);
    st7735_delay_ms(100);
}

/**
 * Set the address window for drawing
 * 
 * @param x0 Start column
 * @param y0 Start row
 * @param x1 End column
 * @param y1 End row
 */
void st7735_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    // Column address set
    st7735_write_command(ST7735_CASET);
    st7735_write_data(0x00);
    st7735_write_data(x0);  // Start column
    st7735_write_data(0x00);
    st7735_write_data(x1);  // End column
    
    // Row address set
    st7735_write_command(ST7735_RASET);
    st7735_write_data(0x00);
    st7735_write_data(y0);  // Start row
    st7735_write_data(0x00);
    st7735_write_data(y1);  // End row
    
    // Write to RAM
    st7735_write_command(ST7735_RAMWR);
}

/**
 * Fill the entire screen with a single color
 * 
 * @param color 16-bit color value in 5-6-5 RGB format
 */
void st7735_fill_screen(uint16_t color) {
    // Set the address window to the entire display
    st7735_set_addr_window(0, 0, ST7735_TFTWIDTH - 1, ST7735_TFTHEIGHT - 1);
    
    // Calculate total number of pixels
    uint32_t total_pixels = (uint32_t)ST7735_TFTWIDTH * (uint32_t)ST7735_TFTHEIGHT;
    
    // Extract high and low bytes from the color
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    
    // Fill the screen with the color
    for (uint32_t i = 0; i < total_pixels; i++) {
        st7735_write_data(hi);  // High byte
        st7735_write_data(lo);  // Low byte
    }
}

/**
 * Draw a single pixel at specified coordinates
 * 
 * @param x X-coordinate
 * @param y Y-coordinate
 * @param color 16-bit color value in 5-6-5 RGB format
 */
void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
    // Check if coordinates are within display bounds
    if ((x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT)) {
        return;
    }
    
    // Set the address window to a single pixel
    st7735_set_addr_window(x, y, x, y);
    
    // Write the color data
    st7735_write_data(color >> 8);    // High byte
    st7735_write_data(color & 0xFF);  // Low byte
}

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

/**
 * Draw a filled rectangle
 * 
 * @param x X-coordinate of top-left corner
 * @param y Y-coordinate of top-left corner
 * @param w Width of rectangle in pixels
 * @param h Height of rectangle in pixels
 * @param color 16-bit color value in 5-6-5 RGB format
 */
void st7735_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
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
    st7735_set_addr_window(x, y, x + w - 1, y + h - 1);
    
    // Calculate total number of pixels
    uint32_t total_pixels = (uint32_t)w * (uint32_t)h;
    
    // Extract high and low bytes from the color
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    
    // Fill the rectangle with the color
    for (uint32_t i = 0; i < total_pixels; i++) {
        st7735_write_data(hi);  // High byte
        st7735_write_data(lo);  // Low byte
    }
}

/**
 * Draw a single character
 * 
 * @param x X-coordinate of top-left corner
 * @param y Y-coordinate of top-left corner
 * @param c Character to draw
 * @param textColor Color of the character
 * @param bgColor Background color
 * @param size Size multiplier (1 = normal, 2 = double size, etc.)
 */
void st7735_draw_char(int16_t x, int16_t y, unsigned char c, 
                     uint16_t textColor, uint16_t bgColor, uint8_t size) {
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
    uint8_t char_width = FONT_WIDTH * size;
    uint8_t char_height = FONT_HEIGHT * size;
    
    // Check if character is completely off screen
    if ((x + char_width <= 0) || (y + char_height <= 0) || 
        (x >= ST7735_TFTWIDTH) || (y >= ST7735_TFTHEIGHT)) {
        return;
    }
    
    // Process each column of the character
    for (uint8_t i = 0; i < FONT_WIDTH; i++) {
        uint8_t line = font[c * FONT_WIDTH + i];
        
        // Process each pixel in the column (row by row)
        for (uint8_t j = 0; j < FONT_HEIGHT; j++, line >>= 1) {
            // Check if pixel should be drawn (LSB first)
            if (line & 0x01) {
                if (size == 1) {
                    // Single pixel - direct draw
                    st7735_draw_pixel(x + i, y + j, textColor);
                } else {
                    // Scaled pixel - draw as rectangle
                    st7735_fill_rect(x + i * size, y + j * size, size, size, textColor);
                }
            } else if (bgColor != textColor) {
                // Draw background if colors are different
                if (size == 1) {
                    st7735_draw_pixel(x + i, y + j, bgColor);
                } else {
                    st7735_fill_rect(x + i * size, y + j * size, size, size, bgColor);
                }
            }
        }
    }
}

/**
 * Draw a text string
 * 
 * @param x X-coordinate of top-left corner
 * @param y Y-coordinate of top-left corner
 * @param text Text string to draw
 * @param textColor Color of the text
 * @param bgColor Background color
 * @param size Size multiplier (1 = normal, 2 = double size, etc.)
 */
void st7735_draw_text(int16_t x, int16_t y, const char *text,
                     uint16_t textColor, uint16_t bgColor, uint8_t size) {
    int16_t cursor_x = x;
    int16_t cursor_y = y;
    uint8_t char_width = FONT_WIDTH * size;
    uint8_t char_height = FONT_HEIGHT * size;
    uint8_t spacing = size; // Space between characters
    
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

