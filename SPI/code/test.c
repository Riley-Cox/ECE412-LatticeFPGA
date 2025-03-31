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
