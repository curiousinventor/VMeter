/*
 *
 * This program simply copies data into the eeprom, and then goes back into the bootloader.

	To upgrade a VMeter, first this code is loaded and run, afterwhich the VMeter resets back
	into bootloader. Then the main functional code is loaded.
 
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



------

VMeter is built on top of LUFA, a USB stack for Atmel devices made by Dean Camera. 

His copyright / warranty info follows:

------



             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/
 
/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/



#include "MIDI.h"
#include "SPI.h"
#include "LEDs.h"
#include "touch_api.h"

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include <avr/eeprom.h>

#include "start_boot.h"
#include "config.h"
#include "BitBangSPI_Master.h"
#include "hardware_spi.h"

#include "MIDI_eeprom.h"


//#define TESTING


#define VER1 '1'
#define VER2 '.'
#define VER3 '2'
#define VER4 '5'


// ---------------- M A C R O S --------------------------- //


#ifdef TICKS_PER_MS
#undef TICKS_PER_MS
#define TICKS_PER_MS  1000             // 8u
#endif


#define MAIN_PARAM_CTRL_NUM 119
#define ECHO_CTRL_NUM 118
#define CHANGE_MIDI_CHAN_CTRL_NUM 117
#define CHANGE_POS_OUT_CTRL_NUM 116
#define CHANGE_ON_OFF_OUT_CTRL_NUM 115
#define CHANGE_PRES_OUT_CTRL_NUM 114
#define CHANGE_LIGHT_IN_CTRL_NUM 113
#define CHANGE_BRIGHT_IN_CTRL_NUM 112
#define CHANGE_NOTEOUT_NUMBER_CTRL_NUM 111
#define CHANGE_NOTEOUT_VEL_CTRL_NUM 110

#define VERSION_OUTPUT_CTRL_NUM 109

// 119 ctrl params
#define UPGRADE_FIRM_PARAM 127
#define UPSIDE_DOWN_OFF_PARAM 126
#define UPSIDE_DOWN_ON_PARAM 125
#define TOUCH_POS_OUT_ENABLE_PARAM 124
#define TOUCH_POS_OUT_DISABLE_PARAM 123
#define PRES_OUT_ENABLE_PARAM 122
#define PRES_OUT_DISABLE_PARAM 121
#define ON_OFF_OUT_ENABLE_PARAM 120
#define ON_OFF_OUT_DISABLE_PARAM 119
#define STORE_SETTINGS_PARAM 118
#define NOTE_OUT_ENABLE_PARAM 117
#define NOTE_OUT_DISABLE_PARAM 116
#define PITCH_WHEEL_ENABLE_PARAM 115
#define PITCH_WHEEL_DISABLE_PARAM 114
#define READ_SETTINGS_PARAM 113
#define RECALIBRATE_PARAM 112
#define NOTEOUT_VEL_FROM_POS_PARAM 111
#define NOTEOUT_VEL_FROM_PRESET_PARAM 110
#define NOTEOUT_PITCH_FROM_POS_PARAM 109
#define NOTEOUT_PITCH_FROM_PRESET_PARAM 108
#define LEDS_IGNORE_TOUCH_PARAM 107
#define LEDS_DONT_IGNORE_TOUCH_PARAM 106

#define CROSS_FADE_ENABLE_PARAM 105
#define CROSS_FADE_DISABLE_PARAM 104
#define GET_VERSION_PARAM 103

#define FILTER_ENABLE_PARAM 102
#define FILTER_DISABLE_PARAM 101









#define MIDI_POS_OUT_CTRL_NUM 20
#define MIDI_TOUCH_ON_OFF_CTRL_NUM 17
#define MIDI_PRES_OUT_CTRL_NUM 18
#define MIDI_COL_HEIGHT_CTRL_NUM 20
#define MIDI_BRIGHTNESS_CTRL_NUM 21
#define UPSIDE_DOWN_MODE_CTRL_NUM 30
#define RECALIBRATE_TOUCH_SENSOR_CTRL_NUM 31
#define EN_TOUCH_POS_OUTPUT_CTRL_NUM 34
#define EN_ON_OFF_OUTPUT_CTRL_NUM 35
#define EN_PRES_OUTPUT_CTRL_NUM 36
#define UPGRADE_FIRMWARE_NOTE_NUM 60
#define STORE_SETTINGS_NOTE_NUM 67


#define PRES_MIN_INPUT 800
#define PRES_MAX_INPUT 2700
#define PRES_INPUT_RANGE 1900 // max - min
#define PRES_MIN_OUTPUT 0
#define PRES_MAX_OUTPUT 132 // some trickiness with the filter on this one... max is eventually 127.
#define PRES_OUTPUT_RANGE 132 // max - min.
#define PRES_MAX_CAP 132


#define IGNORE 0
#define COLUMN 1
#define POS_ARRAY_SIZE 1


// ---------------- P R O T O T Y P E S --------------------------- //

int calc_hys(int input, int prev_result, char *dir, char hys);
static void config_sensors(void);

void prepare_midi_out(void);
void init_sequence(void);
void Jump_To_Bootloader(void);
void store_to_eeprom(void);
void read_eeprom(void);



// GLOBALS //

static uint16_t filter[QT_NUM_CHANNELS];
/* This configuration data structure parameters if needs to be changed will be
   changed in the qt_set_parameters function */
extern qt_touch_lib_config_data_t qt_config_data;
/* measurement data */


/* Get sensor delta values */
extern int16_t qt_get_sensor_delta( uint8_t sensor);
//uint16_t qt_measurement_period_msec = 2u; //25u;
#define qt_measurement_period_msec 2u




struct GLOBALS {

//	int my_refs[8];
	char state; // 0x1 = touch detect | 0b01 = update_sensor
	char was_just_in_touch;
	char touch_pos_changed_since_last_output;
	char on_off_state_changed;
	int weight_sum;
	int delta_sum;
	unsigned char touch_pos;
	unsigned char h_light_pos;


} g;


unsigned char last_weighted_sum;

#ifdef TESTING
	int raw[8];
	int refs[8];

#endif

unsigned char filter_mode = 0;

unsigned char cur_led_group_index;
unsigned char led_groups[5]; // the bits in this variable represent individual LEDs


/*
struct CONFIG {
	unsigned upside_down:1;
	unsigned touch_pos_output_en:1;
	unsigned on_off_output_en:1;
	unsigned pres_output_en:1;
	unsigned spacer:4;
	unsigned pitch_wheel_mode:1;
	unsigned note_out_mode:1;
	unsigned leds_ignore_touch:1;
	unsigned note_vel_mode:1;
	unsigned note_pitch_mode:1;
	unsigned cross_fade_mode:1;
	unsigned filter_en:1;
	unsigned spacer3:1;
	uint8_t  pos_out_ctrl_num;
	uint8_t  on_off_out_ctrl_num;
	uint8_t  pres_out_ctrl_num;
	uint8_t  led_input_ctrl_num;
	uint8_t  brightness_input_ctrl_num;
	uint8_t  note_pitch;
	uint8_t  note_vel;
	uint8_t  brightness_orig;
	unsigned midi_chan:4;
	unsigned spacer2:4;
	uint8_t  name;
	uint8_t  brightness;
	
};
*/
struct CONFIG cfg = {0,1,0,0,0,0,0,0,0,0,0,0,20,17,18,20,21,64,100,16};

struct CONFIG EEMEM EE_config_struct = {0,1,0,0,0,0,0,0,0,0,0,0,20,17,18,20,21,64,100,16};

/*
// CONFIG VARIABLES:
uint8_t EEMEM EE_upside_down = 0; 
//uint8_t upside_down;
uint8_t EEMEM EE_touch_pos_output_en = 1;
//uint8_t touch_pos_output_en = 1;
uint8_t EEMEM EE_on_off_output_en = 0;
uint8_t on_off_output_en = 0;
uint8_t EEMEM EE_pres_output_en = 0;
uint8_t pres_output_en = 0;
uint8_t EEMEM EE_midi_chan = 16;
//uint8_t midi_chan = 0; // in ram, it's stored as 0-15.  the end user will send 1-16 via a midi ctrl command to set it, tho.
uint8_t EEMEM EE_pos_out_ctrl_num = 20;
uint8_t pos_out_ctrl_num = 20;
uint8_t EEMEM EE_on_off_out_ctrl_num = 17;
uint8_t on_off_out_ctrl_num = 17;
uint8_t EEMEM EE_pres_out_ctrl_num = 18;
uint8_t pres_out_ctrl_num = 18;
uint8_t EEMEM EE_pitch_wheel_mode = 0;
uint8_t pitch_wheel_mode = 0;
uint8_t EEMEM EE_note_out_mode = 0;
uint8_t note_out_mode = 0;
uint8_t EEMEM EE_led_input_ctrl_num = 20;
uint8_t led_input_ctrl_num = 20;
uint8_t EEMEM EE_brightness_input_ctrl_num = 21;
uint8_t brightness_input_ctrl_num = 21;
uint8_t EEMEM EE_note_pitch = 64;
uint8_t note_pitch = 64;
uint8_t EEMEM EE_note_pitch_mode = 0; // if 1, base pitch on current position
uint8_t note_pitch_mode = 0;
uint8_t EEMEM EE_note_vel = 100;
uint8_t note_vel = 100;
uint8_t EEMEM EE_note_vel_mode = 0; // if 1, base vel on current position
uint8_t note_vel_mode = 0;
uint8_t EEMEM EE_brightness = 16;
uint8_t brightness = 16;
uint8_t EEMEM EE_leds_ignore_touch = 0;
uint8_t leds_ignore_touch = 0;

*/

/** LUFA MIDI Class driver interface configuration and state information. This structure is
 *  passed to all MIDI Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_MIDI_Device_t Keyboard_MIDI_Interface =
	{
		.Config =
			{
				.StreamingInterfaceNumber = 1,

				.DataINEndpointNumber      = MIDI_STREAM_IN_EPNUM,
				.DataINEndpointSize        = MIDI_STREAM_EPSIZE,
				.DataINEndpointDoubleBank  = false,

				.DataOUTEndpointNumber     = MIDI_STREAM_OUT_EPNUM,
				.DataOUTEndpointSize       = MIDI_STREAM_EPSIZE,
				.DataOUTEndpointDoubleBank = false,
			},
	};


















// ---------------- M A I N --------------------------- //


int main(void)
{
	start_boot_if_required();

	SetupHardware();

	for (;;)
	{
		USB_USBTask();
	}
}






extern USB_Descriptor_String_t EEMEM ProductString;

void store_to_eeprom() {
	char out, j;
	unsigned int delay;
	cli();
	eeprom_write_block(&cfg,&EE_config_struct, sizeof(struct CONFIG));
	eeprom_busy_wait();

	eeprom_write_byte(((uint8_t *)&(ProductString.UnicodeString)) + 14, cfg.midi_chan+49); // convert midi chan to ascii
	eeprom_busy_wait();

	sei();

	for (j=0;j<10;j++) { // flash lights to show storage confirmation
		set_column(0);				
		for (delay=0;delay<10;delay++) {	
			out = 1;			
			_delay_ms(16);
		//	__asm__("nop");
		}
		set_column(255);				
		for (delay=0;delay<10;delay++) {				
			_delay_ms(16);
			out = 1;
		}
	}
	
}

void read_eeprom() {
	eeprom_read_block(&cfg, &EE_config_struct, sizeof(struct CONFIG));
}










/** Configures the board hardware  */
void SetupHardware(void)
{	
	int i;
	unsigned char * eeprom_address;

	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	clock_prescale_set(clock_div_1);


	PORTD |= 0b10000000; // enable internal pull up
		


	init_LEDs();	

	USB_Init();
	




	init_timer_isr();

	unsigned char j;
	
	for (j=255;j>0;j--) {
		set_column(j);							
		USB_USBTask();
		_delay_ms(13);
	}	
	

	DDRD |= (1<<DDD4);
	PORTD |= (1<<PD4);

	if (!(PIND & 0b10000000)) {
			start_boot();
	}
	DDRB |= (1<<DDB3);

	cli();// disable all interrupts
	
	
	eeprom_address = 0;
	for (i=0;i<MIDI_eeprom_size;i++) {
		eeprom_write_byte(eeprom_address++, MIDI_eeprom[i]);
		eeprom_busy_wait();
		USB_USBTask();
	} 
	


	sei(); // reenable interrupts

	led_groups[0] = 0b01010101;
	led_groups[1] = 0b01010101;
	led_groups[2] = 0b01010101;
	led_groups[3] = 0b01010101;
	led_groups[4] = 0b01010101;
	//led_groups[5] = 0b01010101;

	DDRB |= (1<<DDB3);
//	hardware_spi_init_as_slave();
	
	_delay_ms(13);
	for (j=255;j>0;j--) {
		USB_USBTask();
		_delay_ms(13);
	}	


	start_boot();

	while(1) {
		
		USB_USBTask();
	}

	
}



/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	//LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
	//	qt_calibrate_sensing();
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	//LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;
	ConfigSuccess &= MIDI_Device_ConfigureEndpoints(&Keyboard_MIDI_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	MIDI_Device_ProcessControlRequest(&Keyboard_MIDI_Interface);
}




// Timer1 likely not used for just writing to eeprom.
static void init_timer_isr( void )
{
    /*  set timer compare value (how often timer ISR will fire) */
    OCR1A = ( TICKS_PER_MS * qt_measurement_period_msec); // 25 * 8 = 200.  new setting is 2000

    /*  enable timer ISR */
    TIMSK1 |= (1u << OCIE1A);

    /*  timer prescaler = system clock / 8  */
    TCCR1B |= (1u << CS11); // this means the timer will go off every 1MHz / 200 = 5kHz

    /*  timer mode = CTC (count up to compare value, then reset)    */
    TCCR1B |= (1u << WGM12);

	
	// set up Timer0
	TCCR0B |= ((1 << CS01)  | (1 << CS00)); // 011 is /64, or 8e6/64 = 125kHz
				// first 3 bits of this register set clock source / prescalar. 101 = / 1024
				// just CS02 is clock / 256. Slowest speed at 8MHz is 122Hz.
	TCCR0A |= (1u << WGM01); // sets TOP (the point at which Timer0 resets) to OCR0A...
	OCR0A = 50; // this should be 8e6 / 256 / 12 = 2.604kHz. So my total update rate is 2.6kHz / 5 = 520Hz.
	TIMSK0 |= (1u << OCIE0A); // interrupt when timer0 matches OCR0A.

	sei(); // enable global interrupts
}

ISR(TIMER1_COMPA_vect) // note: the touch library appears to trigger this more often than just the timer1!
{
    /*  set flag: it's time to measure touch    */
 //   time_to_measure_touch = 1u;

    /*  update the current time */
 //   current_time_ms_touch += qt_measurement_period_msec;

/*	if (PORTD & (1<<PORTD5)) // toggle D5
		PORTD ^= (1<<PORTD5); 
	else 
		PORTD |= (1<<PORTD5); 
*/		

	//time++;
}

ISR(TIMER0_COMPA_vect) {

	//PORTD &= ~(1<<PD4);
	//PORTB &= ~(1<<PB3);
	
	PORTD &= ~( 1 << SS_BB );  // set latch low		
	if (cur_led_group_index < 5 && cfg.brightness != 100) {
		BitBangSPI_Send_Byte(~(1 << cur_led_group_index));	 // the first byte goes out to the group lines
										  // the group side of the LEDs is the cathode, or low side.
										  // setting a bit to 0 turns ON that group of LEDs.
		BitBangSPI_Send_Byte(led_groups[cur_led_group_index]); // the 2nd byte control the individual LEDs within a group of 8.
									  // These bits connect to anodes, so 1's turn them on, 0's off.
	}
	else {
		BitBangSPI_Send_Byte(0);
		BitBangSPI_Send_Byte(0);
	}
	
	PORTD |= (1 << SS_BB ); // switch latch high to write contents to output.
	
	//if (cur_led_group_index != 100)
	cur_led_group_index++;
	// if cur_led_group_index is set to be greater than cfg.brightness, the lights will just
	// be off.
	if (cur_led_group_index > cfg.brightness)//test_delay)
		cur_led_group_index = 0;
	
		
		
	//PORTD ^= (1<<PORTD4); // bitwise XOR toggle


	//PORTD |= (1<<PD4);
	//PORTB |= (1<<PB3);
}

// spi byte received
ISR(SPI_STC_vect) {
	unsigned char r;
	set_column(SPDR);
	//r = SPDR;
}
