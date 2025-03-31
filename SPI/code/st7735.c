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
    st7735_write_data(0x7F);  // For 1.44" display, 0x9F for 1.8" display
    
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