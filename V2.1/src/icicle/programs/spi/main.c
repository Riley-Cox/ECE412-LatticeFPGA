#include <stdint.h>

#define LEDS		*((volatile uint32_t *) 0x00010000)
#define SPI_DATA 	*((volatile uint32_t *) 0x80000000)
#define SPI_CTRL	*((volatile uint32_t *) 0x80000004)
#define SPI_DC		*((volatile uint32_t *) 0x8000000C)


int main(void){
	LEDS = 0x55;
 	SPI_DC = 1;
	SPI_DATA = 6;
	SPI_CTRL = 1;

	return 0;
}

