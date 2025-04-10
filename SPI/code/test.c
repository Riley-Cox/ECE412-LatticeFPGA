/**
 * ST7735 Display Test Program
 * Easiest test
 * Add more after
 * 
 */

#include "st7735.h"

int main(void) {
    // Initialize the ST7735 display
    st7735_init();
    
    // Fill the screen with red
    st7735_fill_screen(ST7735_RED);
    st7735_delay_ms(1000);  // Wait 1 second
    
    // Fill the screen with green
    st7735_fill_screen(ST7735_GREEN);
    st7735_delay_ms(1000);  // Wait 1 second
    
    // Fill the screen with blue
    st7735_fill_screen(ST7735_BLUE);
    st7735_delay_ms(1000);  // Wait 1 second
    
    // Create a simple pattern
    for (int i = 0; i < ST7735_TFTHEIGHT; i += 10) {
        // Draw horizontal lines
        for (int j = 0; j < ST7735_TFTWIDTH; j++) {
            st7735_draw_pixel(j, i, ST7735_WHITE);
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
                        ST7735_MAGENTA, ST7735_BLACK, 1);
    // Main loop
    while (1) {
        // Could add more functionality here
    }
    
    return 0;  // Should never reach this point
}

//Assuming a RISC-V GCC toolchain is installed, use commands like
/*
riscv32-gcc -c st7735_hw.c -o st7735_hw.o
riscv32-gcc -c st7735.c -o st7735.o
riscv32-gcc -c test.c -o test.o
riscv32--gcc st7735_hw.o st7735.o test.o -o display_test
*/
