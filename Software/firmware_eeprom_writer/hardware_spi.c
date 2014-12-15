#include "hardware_spi.h"
#include <avr/io.h>


void hardware_spi_init_as_master() {
	DDRB  |=  ((1 << 1) | (1 << 2) | (1 << 0)); // set MOSI and SCK to output,
												// MISO is set to input by SPI system
												// also set SS (B0) as output
	//DDRB  &= ~((1 << 0) | (1 << 3)); // set 0 and 3 as inputs

	PORTB |=  ((1 << 0) | (1 << 3)); // set the SS pin high

	SPCR   = (1<<SPE)|(1<<MSTR);
	// the default frequency is fosc / 4, or 2MHz
}

void hardware_spi_init_as_slave() {
	DDRB |= (1<<3); // set MISO as output, others are set to input by SPI system
	SPCR = ((1<<SPE) | (1<<SPIE)); // enable SPI and SPI Interrupt


}


void hardware_spi_send_byte(unsigned char b) {
	PORTB &= ~(1<<0); // set SS low
	SPDR = b;
	// Wait for transmission complete 
	while(!(SPSR & (1<<SPIF)));
	PORTB |= (1<<0); // set SS high
	// out = SPDR; // get response byte?

}
