/*
	Initial code to provide low level hardware communication between VMeters or VMeters
	and a host controller. The main obstacle is avoiding interference with the 
	LED display period (brightness) and touch measurement cycle.

------
	
Copyright (c) 2014 Curious Inventor, LLC
VMeter.net

MIT License:

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


*/


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
