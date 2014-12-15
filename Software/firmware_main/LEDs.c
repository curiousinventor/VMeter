/*

Displays a column (LEDs filled from bottom or top to height) or pitch wheel
(LEDs filled from center out). Writes out values to led_groups which is then
sent to the shift registers on the main interrupt.

led_groups



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



#include "LEDs.h"
#include <avr/io.h>
//#include "SPI.h"
#include "BitBangSPI_Master.h"

#include "config.h"

extern struct CONFIG cfg;
extern unsigned char led_groups[5];

// sets up SPI and initializes the led driver chip.

void init_LEDs() {
	//// set PB0(/SS), PB2(MOSI), PB1(SCK) as output 
    
	//SPI_Init(SPI_SPEED_FCPU_DIV_4 | SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_FALLING | SPI_SAMPLE_LEADING | SPI_MODE_MASTER);
	BitBangSPI_Master_Init();
	//DDRB    = (1<<PB0)|(1<<PB2)|(1<<PB1);
	//PORTB |= (1<<PB0);
//	set_property_LED(DISPLAY_TEST,0);
//	set_property_LED(SCAN_LIMIT,7);  
//	set_property_LED(INTENSITY,15);  
//	set_property_LED(DECODE_MODE,0);  
//	set_property_LED(DISPLAY_TEST,0); 
//	set_property_LED(SHUT_DOWN_MODE, 1); // exit shutdown mode
		
}


void set_property_LED(unsigned char prop, unsigned char val) {	
/*	PORTB &= ~(1<<PB0);
	//SPI_SendByte(prop);
	BitBangSPI_Send_Byte(prop);
	//SPI_SendByte(val);
	BitBangSPI_Send_Byte(val);
	PORTB |= (1<<PB0);
	*/
}




/*
void set_all_lights(int *digits) { // digits[] contains 5 bytes
	set_property_LED(1,digits[0]);
	set_property_LED(2,digits[1]);
	set_property_LED(3,digits[2]);
	set_property_LED(4,digits[3]);
	set_property_LED(5,digits[4]);
}
*/

// turn on one light in the middle, and draw from there.
// choosing light # 19 and 20 to always be on.  That should be the center.

void display_pitch_wheel(unsigned char t) {
	unsigned char r;
	if (cfg.upside_down) {
		t = 255 - t;
	}
	t = pre_set_column(t); // convert from 0-255 to 0 - 38.
	r = t % 8;
	/*if (t >= 20) {// draw top half up to touch
			set_property_LED(1,0x00);
	}
	else { // draw bottom half up to midway, 
		set_property_LED(4,0);
		set_property_LED(5,0); // top two bars are off
		if (t > 8) {
			
			set_property_LED(1,0);
		}
		if (t > 16) {
			set_property_LED(2,0);
			set_property_LED(3,);
		}
	}
	*/
	// just go block by block
	// bottom block (d1):
	if (t > 8) 
	//	set_property_LED(1,0);
		led_groups[0] = 0;
	else { // 0 to all 8 lights will be on.
		//set_property_LED(1,0xFF<<t);
		led_groups[0] = 0xFF<<t;
	}

	// block d2
	if (t > 15)
		//set_property_LED(2,0);
		led_groups[1] = 0;
	else if (t < 9) 
		//set_property_LED(2,0xFF);
		led_groups[1] = 0xFF;
	else {
		//set_property_LED(2,0xFF<<r);
		led_groups[1] =	0xFF<<r;
	}

	// block d4
	if (t < 25)
	//	set_property_LED(4,0);
		led_groups[3] = 0;
	else if(t > 31) 
//		set_property_LED(4,0xFF);
		led_groups[3] = 0xFF;
	else {
	//	set_property_LED(4,~(0xFF<<r));
		led_groups[3] = ~(0xFF<<r);
	}

	// block d5
	if (t < 33)
	//	set_property_LED(5,0);
		led_groups[4] = 0;
	else {
	//	set_property_LED(5,~(0xFF<<r));
		led_groups[4] = ~(0xFF<<r);
	}	

	// middle block
	if (t >23) {
	//	set_property_LED(3,0b11111100);
		led_groups[2] = 0b11111100;
	}
	else if (t<17)
	//	set_property_LED(3,0b00001111);
		led_groups[2] = 0b00001111;
	else if (t> 20) // it's 21-23
	//	set_property_LED(3,~(0xFF<<r) & 0b11111100);
		led_groups[2] = ~(0xFF<<r) & 0b11111100;
	else if (t<19)
	//	set_property_LED(3,(0xFF<<r) & 0b00001111);
		led_groups[2] = (0xFF<<r) & 0b00001111;
	else
	//	set_property_LED(3,0b00001100);
		led_groups[2] = 0b00001100;
	 
		



/*
	switch(t) {
	case 24:
		set_property_LED(3,0b11111100);
		break;
	case 23:
		set_property_LED(3,0b01111100);
		break;
	case 22:
		set_property_LED(3,0b00111100);
		break;
	case 21:
		set_property_LED(3,0b00011100);
		break;
	case 18:
		set_property_LED(3,0b00001110);
		break;
	case 17:
		set_property_LED(3,0b00001111);
		break;
	default:
		set_property_LED(3,0b00001100);
	}
	*/

}

int pre_set_column(unsigned char h) { // takes 0 - 255 input
	unsigned char light_pos;
	light_pos = (unsigned char)((((int)h+4) * 38) >> 8); // offset slightly, scale from 255 to 38.
	return light_pos;
//	set_column(light_pos); 
}

// 0 is all off.
// lights are arranged in groups of 8 by segment, starting with A(0-7), B(8-15), C(16-23), D(24-31), E(32-39)
// Each group of 8 is represented by Digits 0-7.

// with the shift registers, lights are arranged in groups as before, using the variable led_groups
void set_column(unsigned char height) {
	unsigned char d,q,r;
	volatile int special2;
//	int i;
//	int delay = 5000;
//	int delay_off = 0;

/*
		// this turns on the first 8.
		BitBangSPI_Send_Byte(0b11111110);
		BitBangSPI_Send_Byte(0b11111111);

		// 2nd group of 8...
		BitBangSPI_Send_Byte(0b11111101);
		BitBangSPI_Send_Byte(0b11111111);

		// the first LED of the group is off.
		BitBangSPI_Send_Byte(0b11111101);
		BitBangSPI_Send_Byte(0b11111110);

		// first byte controls which digit (group) is on, 0 through 4.
		0bxxx43210 (digits according to circuit board). a 0 turns that group ON.
		// 2nd byte controls which LEDs within a digit are on, or the SEGs.
		0b(DP)(A)(B)(C)(D)(E)(F)(G). Seg G is closest to the logo.


	while(1) {

		PORTD &= ~( 1 << SS_BB );  // set latch low		
		BitBangSPI_Send_Byte(0b11111110);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); // switch latch high to write contents to output.
		for (i=0;i<delay;i++) {
			special2 = i;
		}
		PORTD &= ~( 1 << SS_BB );  // set latch low		
		BitBangSPI_Send_Byte(0b11111111);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); // switch latch high to write contents to output.
		for (i=0;i<delay_off;i++) {
			special2 = i;
		}


		PORTD &= ~( 1 << SS_BB );  
		BitBangSPI_Send_Byte(0b11111101);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); 
		for (i=0;i<delay;i++) {
			special2 = i;
		}
		PORTD &= ~( 1 << SS_BB );  // set latch low		
		BitBangSPI_Send_Byte(0b11111111);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); // switch latch high to write contents to output.
		for (i=0;i<delay_off;i++) {
			special2 = i;
		}

		PORTD &= ~( 1 << SS_BB );  
		BitBangSPI_Send_Byte(0b11111011);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); 
		for (i=0;i<delay;i++) {
			special2 = i;
		}
		PORTD &= ~( 1 << SS_BB );  // set latch low		
		BitBangSPI_Send_Byte(0b11111111);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); // switch latch high to write contents to output.
		for (i=0;i<delay_off;i++) {
			special2 = i;
		}

		PORTD &= ~( 1 << SS_BB );  
		BitBangSPI_Send_Byte(0b11110111);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); 
		for (i=0;i<delay;i++) {
			special2 = i;
		}
		PORTD &= ~( 1 << SS_BB );  // set latch low		
		BitBangSPI_Send_Byte(0b11111111);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); // switch latch high to write contents to output.
		for (i=0;i<delay_off;i++) {
			special2 = i;
		}

		PORTD &= ~( 1 << SS_BB );  
		BitBangSPI_Send_Byte(0b11101111);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); 
		for (i=0;i<delay;i++) {
			special2 = i;
		}
		PORTD &= ~( 1 << SS_BB );  // set latch low		
		BitBangSPI_Send_Byte(0b11111111);
		BitBangSPI_Send_Byte(0b11111111);
		PORTD |= (1 << SS_BB ); // switch latch high to write contents to output.
		for (i=0;i<delay_off;i++) {
			special2 = i;
		}	
	}
*/

	height = pre_set_column(height);
	// note: no bounds checking is done.
	if (height == 0) {
		for (d=0;d<5;d++) {
			led_groups[d] = 0;
		}
	}
	else {				
		if (cfg.upside_down) {
			height = 38 - height; // guaranteed to be > 0.  Now height represents the height of an OFF column
		}
		q = (height) / 8; // number of full blocks
		r = (height) % 8;
		if (cfg.upside_down) {
			for (d=0;d<q;d++) {
				led_groups[d] =0x00;
			}			
			led_groups[d] = (0xFF<<r);				
			d++;
			for (d;d<5;d++) {
				led_groups[d] = 0xFF;
			}				
		}
		else {
			for (d=0;d<q;d++) {
				led_groups[d] = 0xFF;
			}
			led_groups[d] = ~(0xFF<<r);
			d++;

			for (d;d<5;d++) {
				led_groups[d] = 0;
			}
		}	
					
	}
	

}
