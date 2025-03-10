/**
 * ST7735 Display Driver Header
 * 
 * This header defines functions for initializing and drawing to
 * an ST7735 display.
 * 
 * Add more functions latter
 */

#ifndef ST7735_H
#define ST7735_H

#include "st7735_hw.h"

// Initialize the ST7735 display
void st7735_init(void);

// Set the address window for drawing
void st7735_set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

// Fill the entire screen with a single color
void st7735_fill_screen(uint16_t color);

// Draw a single pixel
void st7735_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

#endif // ST7735_H