/*

Description:
This is the main code for VMeter (it contains int main(void)). 

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




#include "VMETER.h"
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


//#define TESTING
#define VER1 '1'
#define VER2 '.'
#define VER3 '3'
#define VER4 '1' // don't forget to change the TWO places in descriptors.c, also!

// ---------------- M A C R O S --------------------------- //

#define GET_SENSOR_STATE(SENSOR_NUMBER) qt_measure_data.qt_touch_status.sensor_states[0]
#define GET_ROTOR_SLIDER_POSITION(ROTOR_SLIDER_NUMBER) qt_measure_data.qt_touch_status.rotor_slider_values[ROTOR_SLIDER_NUMBER]
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
#define SENSITIVITY_CTRL_NUM 108
#define PITCH_WHEEL_RETURN_SPEED_CTRL_NUM 107



// individual LED control
#define LEDS_1_THRU_14_ID 0xAD // polyphonic aftertouch chan 14 (out of 1-16 midi channels)
#define LEDS_15_THRU_28_ID 0xAE // polyphonic aftertouch chan 15/16
#define LEDS_29_THRU_38_ID 0xAF // polyphonic aftertouch chan 16/16

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

#define SENSITIVITY_PARAM 100
#define PITCH_WHEEL_RETURN_SPEED_PARAM 100








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

/*
#define PRES_MIN_INPUT 50
#define PRES_MAX_INPUT 120
#define PRES_INPUT_RANGE 70 // max - min
#define PRES_MIN_OUTPUT 0
#define PRES_MAX_OUTPUT 127
#define PRES_OUTPUT_RANGE 127 // max - min.
#define PRES_MAX_CAP 127
*/

#define IGNORE 0
#define COLUMN 1
#define POS_ARRAY_SIZE 1

extern USB_Descriptor_String_t EEMEM ProductString;


// ---------------- P R O T O T Y P E S --------------------------- //

int calc_hys(int input, int prev_result, char *dir, char hys);
void measure_touch(void);
static void config_sensors(void);
void process_midi_in(MIDI_EventPacket_t *);
void prepare_midi_out(void);
void init_sequence(void);
void send_ctrl_msg(unsigned char ctrl_num, unsigned char value, unsigned char chan);
int scale_pres(long p);
int filter_pres(int p);
void Jump_To_Bootloader(void);
void store_to_eeprom(void);
void read_eeprom(void);
void reset_filter(void);
void send_out_settings(void); 

static void filter_data_mean_4( void );



// ---------------- G L O B A L S --------------------------- //
#ifdef _QMATRIX_

extern y_line_info_t y_line_info[NUM_Y_LINES];
extern x_line_info_t x_line_info[NUM_X_LINES];

/* Fill out the X-Line masks on the X- Port selected.
 * The order of X - Line numbering follows from the way the
 * X-Lines are filled as below.
 * Here, X0,X1,X2,X3,X4,X5,X6,X7 are on port-pin specified.
 * 1 - if to specify if X line is on PORT_X_1,pin on the same selected port.
 * 2 - if to specify if X line is on PORT_X_2,pin on the same selected port.
 * 3 - if to specify if X line is on PORT_X_3,pin on the same selected port.
 *
 * Note: 1. The Number entries should be based on NUM_X_LINES
 *          4 entries when NUM_X_LINES =4 and
 *          8 entries when NUM_X_LINES=8
*/
/*

*/
x_line_info_t x_line_info[NUM_X_LINES]= {
	FILL_OUT_X_LINE_INFO(1,4),
	FILL_OUT_X_LINE_INFO(1,5),
	FILL_OUT_X_LINE_INFO(1,6),
	FILL_OUT_X_LINE_INFO(1,7),
	FILL_OUT_X_LINE_INFO(2,4),
	FILL_OUT_X_LINE_INFO(2,5),
	FILL_OUT_X_LINE_INFO(2,6),
	FILL_OUT_X_LINE_INFO(2,7),
};
/* Fill out the Y-Line masks on the Y- Line port selected
 * The order of Y - Line numbering follows from the way the
 * Y-Lines are filled as below
 * Here, Y0,Y1,Y2,Y3 on 0,1,2,3
 * Note: 1. The Number entries should be based on NUM_X_LINES
 *          2 entries when NUM_Y_LINES=2
 *          4 entries when NUM_Y_LINES=4
 *          8 entries when NUM_Y_LINES=8
*/
/*
};*/
y_line_info_t y_line_info[NUM_Y_LINES]= {
	FILL_OUT_Y_LINE_INFO(2),
};
#endif/*_QMATRIX_*/


static uint16_t filter[QT_NUM_CHANNELS];
/* This configuration data structure parameters if needs to be changed will be
   changed in the qt_set_parameters function */
extern qt_touch_lib_config_data_t qt_config_data;
/* measurement data */
extern qt_touch_lib_measure_data_t qt_measure_data;
qt_touch_lib_measure_data_t *pqt_measure_data = &qt_measure_data;
/* Get sensor delta values */
extern int16_t qt_get_sensor_delta( uint8_t sensor);
//uint16_t qt_measurement_period_msec = 2u; //25u;
#define qt_measurement_period_msec 2u

//static volatile uint8_t time_to_measure_touch = 0u;


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

//	int light_pos;
} g;
//	int qtouch_pos = 0;

unsigned char last_weighted_sum;

#ifdef TESTING
	int raw[8];
	int refs[8];

#endif

unsigned char filter_mode = 0;

unsigned char cur_led_group_index;
unsigned char led_groups[5]; // the bits in this variable represent individual LEDs

/*
struct FILTER {
	int x0;
	int x1;
	int y1;
	int y2;
} f;
*/	


//	unsigned int stack_remaining;
	

//#ifdef TESTING
	unsigned char test_delay = 0;
//#endif	

	
	int pres_fil_out_stage1;
	int last_pres_fil_out_stage1;
	int pres_fil_out_stage2;


	
	
	
	static volatile uint16_t current_time_ms_touch = 0u;
	
	

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
	uint8_t  sensitivity; // 64 is 0, above or below gets added or subtracted from default touch detection in qt_enable_slider()
	uint8_t  pitch_wheel_return_speed;	
	
};
*/
// 64, 5

struct CONFIG cfg = {0,1,0,0,0, 0,0,0,0,0,0,0,0, 20,17,18,20,21,64,100,100,0,0,0,8, 64,6};

// this sets the default eeprom settings that are written to MIDI.eep
struct CONFIG EEMEM EE_config_struct = {0,1,0,0,0, 0,0,0,0,0,0,0,0, 20,17,18,20,21,64,100,100,0,0,0,8, 64,6};



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
	int ave_pres;
	volatile int special;
	int i,q;
	char temp_vel;
	int temp_int;
	static char temp_pitch;
	unsigned char pitch_wheel_returning_position = 128;
	start_boot_if_required();
	
	read_eeprom();
	SetupHardware();
		
	// check the first byte of the Product String to make sure the eeprom has valid descriptors.
	// If not, jump to boot loader.  This prevent potential bricking.
	if (eeprom_read_byte(&(ProductString.UnicodeString)) != 'V')
		start_boot();
	

	// ------------------------------------------------
	// MAIN APPLICATION LOOP //
	// ------------------------------------------------
	for (;;)
	{
		// Hardware reset to bootloader if new firmware bricks device.
		// check D7 and if held to ground reboot into bootloader 
		if (!(PIND & 0b10000000)) {
			start_boot();
		}


		// development timinig measurement
	/*	if (PORTD & (1<<PORTD5)) // toggle D5
			PORTD ^= (1<<PORTD5); 
		else 
			PORTD |= (1<<PORTD5); 
	*/
		//PORTD &= ~(1<<PD4);
		//PORTD ^= (1<<PORTD4);
		
		measure_touch();
		//PORTD &= ~(1<<PD4);
	
		//PORTD |= (1<<PD4);
		for (q=0;q<test_delay;q++) {
			_delay_ms(1);
		}


		// _________ LIGHTS __________//
		// touch overrides MIDI messages unless leds_ignore_touch==1
		// We want to set the LEDs if touch is active or it just went unactive. averaging can leave 
		// it not all the way to where the finger left
		if (g.state || g.on_off_state_changed) {
			if (!cfg.leds_ignore_touch) { // don't update lights if ignore mode is on.
				if (cfg.pitch_wheel_mode || cfg.cross_fade_mode) {
					// draw pitch wheel (h_light_pos)
					display_pitch_wheel(g.h_light_pos);
				}				
				else  {
					//set_column(last_weighted_sum);
					set_column(g.h_light_pos);
					//set_column(GET_ROTOR_SLIDER_POSITION(0));
				}
				// else cursor mode?
			}
		}

		// ____________ MIDI OUT ____________ //
#ifndef TESTING	
		if (g.on_off_state_changed) {
			g.on_off_state_changed = 0; // reset this		
			if (g.state == 0) { // we just lifted off, send one more 0 pressure
				if (cfg.pres_output_en) {
					send_ctrl_msg(cfg.pres_out_ctrl_num, 0,cfg.midi_chan);
					// also clear the pressure filter
					last_pres_fil_out_stage1 = 0;
					pres_fil_out_stage2 = 0;
				//	reset_filter();					
				}
				if (cfg.pitch_wheel_mode) { // initiate glide back to center				
					pitch_wheel_returning_position = g.touch_pos; //(g.touch_pos - 128) >> 5; // (over 32 cycles)		
 				}
					
				if (cfg.on_off_output_en)
					send_ctrl_msg(cfg.on_off_out_ctrl_num,0,cfg.midi_chan); // send an "OFF" message
					
				if (cfg.note_out_mode) 	{
					//temp_pitch = cfg.note_pitch_mode ? g.touch_pos>>1 : cfg.note_pitch;					
					temp_pitch = cfg.note_pitch_mode ? temp_pitch : cfg.note_pitch;
					temp_vel = cfg.note_vel_mode ? g.touch_pos>>1 : cfg.note_vel;		
					send_note_off(temp_pitch, temp_vel, cfg.midi_chan);
				}

				// put the pos out last msg here, too.
			}
			else { // touch active	
				pitch_wheel_returning_position = 128; // cancel glide back to center for pitch wheel.
				if (cfg.on_off_output_en)			
					send_ctrl_msg(cfg.on_off_out_ctrl_num,127,cfg.midi_chan);	
				if (cfg.note_out_mode) 	{
					temp_pitch = cfg.note_pitch_mode ? g.touch_pos>>1 : cfg.note_pitch;
					temp_vel = cfg.note_vel_mode ? g.touch_pos>>1 : cfg.note_vel;		
					send_note_on(temp_pitch, temp_vel, cfg.midi_chan);
				}
			}
		}
		
		if (pitch_wheel_returning_position != 128) { // pitch wheel gliding back to center position.
			if (pitch_wheel_returning_position > 128 + cfg.pitch_wheel_return_speed) {
				pitch_wheel_returning_position = pitch_wheel_returning_position - cfg.pitch_wheel_return_speed;
			}
			else if (pitch_wheel_returning_position < 128 - cfg.pitch_wheel_return_speed) {
				pitch_wheel_returning_position = pitch_wheel_returning_position + cfg.pitch_wheel_return_speed;
			}
			else {				
				pitch_wheel_returning_position = 128;
			}			
			send_pitch_wheel(scale_dead_zone(pitch_wheel_returning_position), cfg.midi_chan);
			display_pitch_wheel(pitch_wheel_returning_position);
		}
	

		if (cfg.pres_output_en && g.state) {
			//ave_pres = scale_pres(g.weight_sum);						
			ave_pres = filter_pres(g.delta_sum);
			send_ctrl_msg(cfg.pres_out_ctrl_num, (unsigned char)ave_pres,cfg.midi_chan);
			//send_ctrl_msg(cfg.pres_out_ctrl_num, (unsigned char)(g.weight_sum / 100),cfg.midi_chan);
	
		}


		//GET_ROTOR_SLIDER_POSITION(0) // this is the built-in QTouch library position finder, 
		// but we roll our own to adjust for our board and for a fast response.
	
		if (g.touch_pos_changed_since_last_output) {
			g.touch_pos_changed_since_last_output = 0;
			
			if (cfg.touch_pos_output_en)
				send_ctrl_msg(cfg.pos_out_ctrl_num, g.touch_pos>>1,cfg.midi_chan); // midi chan 1, touch_pos is 0-255, so / 2 to get 0-127
			if ((cfg.pitch_wheel_mode || cfg.cross_fade_mode) && g.state) {
				send_pitch_wheel(scale_dead_zone(g.touch_pos), cfg.midi_chan);
			}
		}
		
		//if (g.state) // if touching, send the filter mode
		//	send_ctrl_msg(cfg.on_off_out_ctrl_num,filter_mode,cfg.midi_chan);
		

#endif
#ifdef TESTING
		// special debugging output via the pressure 

		if (cfg.pres_output_en) {

	//	send_ctrl_msg(0,(unsigned char)filter_pres(scale_pres(g.weight_sum)),0);
/*
			send_ctrl_msg(0,(unsigned char)raw[0],0);
			send_ctrl_msg(1,(unsigned char)(raw[0]>>8),0);			
			send_ctrl_msg(2,(unsigned char)raw[1],0);		
			send_ctrl_msg(3,(unsigned char)(raw[1]>>8),0);			
			send_ctrl_msg(4,(unsigned char)raw[2],0);
			send_ctrl_msg(5,(unsigned char)(raw[2]>>8),0);
			send_ctrl_msg(6,(unsigned char)raw[3],0);
			send_ctrl_msg(7,(unsigned char)(raw[3]>>8),0); 
			send_ctrl_msg(8,(unsigned char)raw[4],0);
			send_ctrl_msg(9,(unsigned char)(raw[4]>>8),0);
			send_ctrl_msg(10,(unsigned char)raw[5],0);
			send_ctrl_msg(11,(unsigned char)(raw[5]>>8),0);
			send_ctrl_msg(12,(unsigned char)raw[6],0);
			send_ctrl_msg(13,(unsigned char)(raw[6]>>8),0); 
			send_ctrl_msg(14,(unsigned char)raw[7],0); 
			send_ctrl_msg(15,(unsigned char)(raw[7]>>8),0); 
*/
			send_ctrl_msg(0,(unsigned char)refs[0],0);
			send_ctrl_msg(1,(unsigned char)(refs[0]>>8),0);			
			send_ctrl_msg(2,(unsigned char)refs[1],0);		
			send_ctrl_msg(3,(unsigned char)(refs[1]>>8),0);			
			send_ctrl_msg(4,(unsigned char)refs[2],0);
			send_ctrl_msg(5,(unsigned char)(refs[2]>>8),0);
			send_ctrl_msg(6,(unsigned char)refs[3],0);
			send_ctrl_msg(7,(unsigned char)(refs[3]>>8),0); 
			send_ctrl_msg(8,(unsigned char)refs[4],0);
			send_ctrl_msg(9,(unsigned char)(refs[4]>>8),0);
			send_ctrl_msg(10,(unsigned char)refs[5],0);
			send_ctrl_msg(11,(unsigned char)(refs[5]>>8),0);
			send_ctrl_msg(12,(unsigned char)refs[6],0);
			send_ctrl_msg(13,(unsigned char)(refs[6]>>8),0); 
			send_ctrl_msg(14,(unsigned char)refs[7],0); 
			send_ctrl_msg(15,(unsigned char)(refs[7]>>8),0); 
	
			
		}
		if (cfg.on_off_output_en) {
			send_ctrl_msg(10,(unsigned char)refs[0],0);
		//	send_ctrl_msg(11,(unsigned char)refs[1],0);
		}
			
#endif
		// ____________ MIDI RECEIVE ______________ //
		MIDI_EventPacket_t ReceivedMIDIEvent;
		while (MIDI_Device_ReceiveEventPacket(&Keyboard_MIDI_Interface, &ReceivedMIDIEvent))
		{		
			process_midi_in(&ReceivedMIDIEvent);			
		}
		

		MIDI_Device_Flush(&Keyboard_MIDI_Interface);

		MIDI_Device_USBTask(&Keyboard_MIDI_Interface);
		USB_USBTask(); // let the USB library talk



	}
}


#define DEAD_ZONE_START 120
#define DEAD_ZONE_END 136
// this is used in cross fader mode so that there's a small dead-zone in the middle that sends out
// 128. The input is 0-255, the output is also 0-255
int scale_dead_zone(int in) {
	//in = 255;
	if (in >= DEAD_ZONE_START && in < DEAD_ZONE_END) {
		return 128;
	}
	else if (in < DEAD_ZONE_START) {
		return (in << 7) / DEAD_ZONE_START;
	}
	else if (in < 254) { // in >= DEAD_ZONE_END. not sure why this isn't 255...
		return ((in - DEAD_ZONE_END) << 7) / 119 + 128;
	}
	else
		return 255;
	
}





void store_to_eeprom() {
	char out, j;
	unsigned int delay;
	cli();
	eeprom_write_block(&cfg,&EE_config_struct, sizeof(struct CONFIG));
	eeprom_busy_wait();

	

	eeprom_write_byte(((uint8_t *)&(ProductString.UnicodeString)) + 24, cfg.midi_chan+65); // convert midi chan to ascii starting at 'A'
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






#define FILTER_SIZE 16
#define FILTER_SIZE_POW_2 4


//#define PRES_OFFSET 1500
#define PRES_OFFSET 65 // used to be 60

#define ave_size 32
int filter_pres(int p) {
	// two pole filter: y(n) = .25*x(n) + y(n-1) - .25*y(n-2). 
	int out;
	int delta;
	int stage_1_delta;

	// scale the input
	p -= PRES_OFFSET;
	p = p << 4;

	if (p<0)
		p = 0;	



	//p = p << 5; // multiply by 16 for numerical reasons, and an additional 2 for scaling.
	
	
	
	//y(n) = .0625x(n) + 1.5y(n-1) - .5625y(n-2)
	//out = p>>4 + (int)3*(last_pres_filter_1>>1) - (int)9*(last_pres_filter_2>>4);

/*	
	out = in>>4;
	out += 3*(last_pres_filter_1>>1) - 9*(last_pres_filter_2>>4);
	last_pres_filter_2 = last_pres_filter_1;
	last_pres_filter_1 = out;
	out = out >> 4;
*/

	//out = (p>>2) + last_pres_filter_1 - (last_pres_filter_2>>2);	
	// scale input by 16, then divide by 4, so only scale by 2
/*
	out = (p<<2) + last_pres_filter_1 - (last_pres_filter_2>>2);	
	last_pres_filter_2 = last_pres_filter_1;
	last_pres_filter_1 = out;
	out = out >> 4;
*/
	//return out>>4;

	// we're going to do a two stage filter here, too, where the 2nd stage is only engaged
	// if the first stage output is relatively steady.
	
	
	// first stage:	
	delta = p - last_pres_fil_out_stage1;	
	pres_fil_out_stage1 = (delta>>2) + last_pres_fil_out_stage1;
	stage_1_delta = pres_fil_out_stage1 - last_pres_fil_out_stage1;	
	last_pres_fil_out_stage1 = pres_fil_out_stage1;

#define SEC_PRES_FILTER_THRESH 32
#define SEC_PRES_FILTER_THRESH_NEG -32

	if (stage_1_delta < SEC_PRES_FILTER_THRESH && stage_1_delta > SEC_PRES_FILTER_THRESH_NEG) {
		// engage secondary filter since the output is holding relatively still
	//	filter_mode = 127;
		delta = pres_fil_out_stage1 - pres_fil_out_stage2;
		pres_fil_out_stage2 = (delta>>4) + pres_fil_out_stage2;
	}
	else { // hard set 2nd stage filter to first
	//	filter_mode = 0;
		pres_fil_out_stage2 = pres_fil_out_stage1;
	}

	out = pres_fil_out_stage2 >> 2;
	
	
	
	if (out > 127)
		return 127;
	else if (out < 0)
		return 0;
	else
		return out;


}
/*
#define PRES_MIN_INPUT 200
#define PRES_MAX_INPUT 2000
#define PRES_INPUT_RANGE 1800 // max - min
#define PRES_MIN_OUTPUT 0
#define PRES_MAX_OUTPUT 127
#define PRES_OUTPUT_RANGE 127 // max - min.
#define PRES_MAX_CAP 127
*/
int scale_pres(long p) {
	
	if (p > PRES_MAX_INPUT)
		p = PRES_MAX_INPUT;
	else if (p < PRES_MIN_INPUT)
		p = PRES_MIN_INPUT;

	p = ((p - PRES_MIN_INPUT) * PRES_OUTPUT_RANGE) / PRES_INPUT_RANGE; // + PRES_MIN_OUTPUT (which is 0)
	// >> 6 does a divide by 64? pretty close to the PRES_INPUT_RANGE
	//if (p < PRES_MIN_OUTPUT)
	//	p = PRES_MIN_OUTPUT;
	//else if (p > PRES_MAX_CAP)
	//	p = PRES_MAX_CAP;

	return (int)p;
}

void init_sequence() {
//	int delay;
	

}

void process_midi_in(MIDI_EventPacket_t *ReceivedMIDIEvent) {
	unsigned char d3_max_100;
	unsigned char midi_chan;
	unsigned char temp;
	unsigned char p;
	//static unsigned char sysex_counter = 0;

	if (ReceivedMIDIEvent->Data1 == 0b10110000) { // ctrl change on channel 1
		d3_max_100 = ReceivedMIDIEvent->Data3;
		if (d3_max_100 > 100)
			d3_max_100 = 100;
		switch (ReceivedMIDIEvent->Data2) { // switch on the ctrl number
		case MAIN_PARAM_CTRL_NUM: 	// we're getting a config param
			switch (ReceivedMIDIEvent->Data3) {
			case UPGRADE_FIRM_PARAM:
				start_boot();
				break;
			case UPSIDE_DOWN_OFF_PARAM:
				cfg.upside_down = 0;
				set_column(g.h_light_pos);
				break;
			case UPSIDE_DOWN_ON_PARAM:
				cfg.upside_down = 1;
				set_column(g.h_light_pos);
				break;
			case TOUCH_POS_OUT_ENABLE_PARAM:
				cfg.touch_pos_output_en = 1;
				break;
			case TOUCH_POS_OUT_DISABLE_PARAM:
				cfg.touch_pos_output_en = 0;
				break;
			case PRES_OUT_ENABLE_PARAM:
				cfg.pres_output_en = 1;
				break;
			case PRES_OUT_DISABLE_PARAM:
				cfg.pres_output_en = 0;
				break;
			case ON_OFF_OUT_ENABLE_PARAM:
				cfg.on_off_output_en = 1;
				break;
			case ON_OFF_OUT_DISABLE_PARAM:
				cfg.on_off_output_en = 0;
				break;
			case STORE_SETTINGS_PARAM:
				store_to_eeprom();
				break;
			case NOTE_OUT_ENABLE_PARAM:
				cfg.note_out_mode = 1;
				break;
			case NOTE_OUT_DISABLE_PARAM:
				cfg.note_out_mode = 0;
				break;
			case PITCH_WHEEL_ENABLE_PARAM:
				cfg.pitch_wheel_mode = 1;
				break;
			case PITCH_WHEEL_DISABLE_PARAM:
				cfg.pitch_wheel_mode = 0;
				break;
			case READ_SETTINGS_PARAM:
				send_out_settings();
				break;
			case RECALIBRATE_PARAM:
				qt_calibrate_sensing();
				break;
			case NOTEOUT_VEL_FROM_POS_PARAM:
				cfg.note_vel_mode = 1;
				break;
			case NOTEOUT_VEL_FROM_PRESET_PARAM:
				cfg.note_vel_mode = 0;
				break;
			case NOTEOUT_PITCH_FROM_POS_PARAM:
				cfg.note_pitch_mode = 1;
				break;		
			case NOTEOUT_PITCH_FROM_PRESET_PARAM:
				cfg.note_pitch_mode = 0;
				break;
			case LEDS_IGNORE_TOUCH_PARAM:
				cfg.leds_ignore_touch = 1;
				break;
			case LEDS_DONT_IGNORE_TOUCH_PARAM:
				cfg.leds_ignore_touch = 0;
				break;
			case CROSS_FADE_ENABLE_PARAM:
				cfg.cross_fade_mode = 1;
				break;
			case CROSS_FADE_DISABLE_PARAM:
				cfg.cross_fade_mode = 0;
				break;
			case GET_VERSION_PARAM:
				send_ctrl_msg(VERSION_OUTPUT_CTRL_NUM,VER1,0);
				send_ctrl_msg(VERSION_OUTPUT_CTRL_NUM,VER2,0);
				send_ctrl_msg(VERSION_OUTPUT_CTRL_NUM,VER3,0);
				send_ctrl_msg(VERSION_OUTPUT_CTRL_NUM,VER4,0);				
				break;
			case FILTER_ENABLE_PARAM:
				cfg.filter_en = 1;
				break;
			case FILTER_DISABLE_PARAM:
				cfg.filter_en = 0;
				break;
			
			} // end switch MAIN_PARAM_CTRL_NUM
			
			break; 
		case ECHO_CTRL_NUM:
			send_ctrl_msg(ECHO_CTRL_NUM,ReceivedMIDIEvent->Data3 ,0); 
			break;
		case CHANGE_MIDI_CHAN_CTRL_NUM:
			midi_chan = ReceivedMIDIEvent->Data3 - 1;
			if (midi_chan > 15)
				cfg.midi_chan = 15; // midi chan 1-16 --> 0-15. Have to remember to output midi_chan +1
			else
				cfg.midi_chan = midi_chan;
			break;
		case CHANGE_POS_OUT_CTRL_NUM:
			cfg.pos_out_ctrl_num = d3_max_100;
			break;
		case CHANGE_ON_OFF_OUT_CTRL_NUM:
			cfg.on_off_out_ctrl_num = d3_max_100;
			break;
		case CHANGE_PRES_OUT_CTRL_NUM:
			cfg.pres_out_ctrl_num = d3_max_100;
			break;
		case CHANGE_LIGHT_IN_CTRL_NUM:
			cfg.led_input_ctrl_num = d3_max_100;
			break;
		case CHANGE_BRIGHT_IN_CTRL_NUM:
			cfg.brightness_input_ctrl_num = d3_max_100;
			break;
		case CHANGE_NOTEOUT_NUMBER_CTRL_NUM:
//#ifdef TESTING
//			test_delay = ReceivedMIDIEvent->Data3;
//			if (test_delay < 0)
//				test_delay = 0;
//			else if (test_delay > 99)
//				test_delay = 99;
//else
			cfg.note_pitch = ReceivedMIDIEvent->Data3;
//#endif
			break;
		case CHANGE_NOTEOUT_VEL_CTRL_NUM:
			cfg.note_vel = ReceivedMIDIEvent->Data3;
			break;
		case SENSITIVITY_CTRL_NUM:
			cfg.sensitivity = ReceivedMIDIEvent->Data3;
			break;
		case PITCH_WHEEL_RETURN_SPEED_CTRL_NUM:
			cfg.pitch_wheel_return_speed = ReceivedMIDIEvent->Data3;
			break;
		} // end switch for ctrl number in channel 1
	

	}

	// Now process regular midi commands that aren't config changes
	
	midi_chan = ReceivedMIDIEvent->Data1;
	midi_chan &= 0x0F;

	// now we check for ctrl messages on our midi channel
	if (midi_chan == cfg.midi_chan) { // grab last 4bits of data1,
		temp = ReceivedMIDIEvent->Data1; 
		temp &= 0xF0; // get the type of MIDI message (controller, note on / off, pitch, etc)
		if (temp == 0b10110000) { // ctrl change
			if (ReceivedMIDIEvent->Data2 == cfg.led_input_ctrl_num) {
				set_column(ReceivedMIDIEvent->Data3 * 2);
			}
			else if (ReceivedMIDIEvent->Data2 == cfg.brightness_input_ctrl_num) {
				cfg.brightness_orig = ReceivedMIDIEvent->Data3;
				temp = cfg.brightness_orig + 1; // now it's 1 - 128
				temp = (temp >> 3); // now it's 0 - 16
				if (temp == 0) {
					cfg.brightness = 100;
				//	cur_led_group_index = 100; // this effectively turns off the LEDs.
				}
				else { // need to map 1-16 to 39 - 4.
					//if (cur_led_group_index == 100)
					//	cur_led_group_index = 0; // reset out of off mode
					if (temp > 8) // 9-16 --> 11 - 4
						cfg.brightness = 20 - temp;
					else if (temp > 2) { // skip by two.  3-8 --> 23 - 13
						cfg.brightness = 29 - (temp<<1);
					}
					else if (temp == 2)
						cfg.brightness = 29;
					else
						cfg.brightness = 39;

					
				}
				
				// I want to map 0 - 127 to 0 - 16
				//cfg.brightness = (temp >> 3) + 4; // divide by 8. 0 - 16
				//cfg.brightness = temp;
				//set_brightness();					
			} 			
		}
		else if (cfg.cross_fade_mode || cfg.pitch_wheel_mode) {
			if (temp == 0b11100000) { // pitch wheel change
				p = ((unsigned char)(ReceivedMIDIEvent->Data3)) << 1; // get the first 7 msbs.
				// since we're only displaying on about 6 bits, no need to get the last digit
				// out of Data2.
				//p = p | (((unsigned char)(ReceivedMIDIEvent->Data2)) >> 6); // get the last bit
			//	.Data2       = (val & 0b00000001) << 6, // contains the 7 least sig bits
			//	.Data3       = (val & 0b11111110) >> 1, // contains the 7 most sig bits	
			// pull out 8 bits from the 7 bits in data3 and data2.			
				display_pitch_wheel(p); // takes 0-255			
			}
		} 
		else if (temp == 0b10010000) { // note ON
			// Middle C is MIDI 60
			if (ReceivedMIDIEvent->Data3 == 0) { // another way to send note off
				set_column(0);
			}
			else
				set_column((unsigned char)(ReceivedMIDIEvent->Data2*2));
		}
		else if (temp == 0b10000000) { // note OFF
			// turn off all LEDs
			set_column(0);
		}
	}

	// process individual LED control commands	
	if (ReceivedMIDIEvent->Data1 == LEDS_1_THRU_14_ID) {
		led_groups[0] = ((ReceivedMIDIEvent->Data2) & 0b01111111) | ((ReceivedMIDIEvent->Data3 << 7) & 0b10000000);
		led_groups[1] = ((ReceivedMIDIEvent->Data3 >> 1) & 0b00111111) | (led_groups[1] & 0b11000000);
	}
	else if (ReceivedMIDIEvent->Data1 == LEDS_15_THRU_28_ID) {
		led_groups[1] = (led_groups[1] & 0b00111111) | ((ReceivedMIDIEvent->Data2 << 6) & 0b11000000);
		led_groups[2] = ((ReceivedMIDIEvent->Data2 >> 2) & 0b00011111) | ((ReceivedMIDIEvent->Data3 << 5) & 0b11100000);
		led_groups[3] = (led_groups[3] & 0b11110000) | ((ReceivedMIDIEvent->Data3 >> 3) & 0b00001111);
	}
	else if (ReceivedMIDIEvent->Data1 == LEDS_29_THRU_38_ID) {
		led_groups[3] = (led_groups[3] & 0b00001111) | ((ReceivedMIDIEvent->Data2 << 4) & 0b11110000);
		led_groups[4] = ((ReceivedMIDIEvent->Data2 >> 4) & 0b00000111) | ((ReceivedMIDIEvent->Data3 << 3) & 0b00111000);
	}
	
	/*
	switch (sysex_counter) {
		case 0:
			if (ReceivedMIDIEvent->Data1 == 0xF0) {
				if (ReceivedMIDIEvent->Data2 == 1) { // the LED control ID (in case we want to use sysex for something else in the future
					led_groups[0] = ReceivedMIDIEvent->Data3; // LEDs 0-6
					sysex_counter = 3; // if sysex_counter never makes it to 3, all bytes will be ignored.
				}
			}
			break;
		case 3:
			led_groups[0] = (ReceivedMIDIEvent->Data1 << 7) | (led_groups[0] & 0b01111111);
			led_groups[1] = ((ReceivedMIDIEvent->Data1 >> 1) & 0b00111111) | ((ReceivedMIDIEvent->Data2 << 6) & 0b11000000); // setting 0-5
			led_groups[2] = ((ReceivedMIDIEvent->Data2 >> 2) & 0b00011111) | ((ReceivedMIDIEvent->Data3 << 5) & 0b11100000);
			led_groups[3] = (ReceivedMIDIEvent->Data3 >> 3) & 0b00001111; 			
			sysex_counter = 6;
			break;
		case 6:
			led_groups[3] = (ReceivedMIDIEvent->Data1 << 4) | (led_groups[3] & 0b00001111);
			led_groups[4] = ((ReceivedMIDIEvent->Data1 >> 4) & 0b00000111) | ((ReceivedMIDIEvent->Data2 << 3) & 0b00111000);
			sysex_counter = 0;
			// data3 should be 0xF7, the end of the sysex packet.
			break;
	}
	*/
	


}

// total of 10 bytes in config
void send_out_settings() {
	unsigned char *ptr;
	unsigned char j;
	ptr = (unsigned char *)&cfg;
	send_ctrl_msg(MAIN_PARAM_CTRL_NUM,0x55,0);
	for (j=0;j<(sizeof(struct CONFIG));j++) {
		send_ctrl_msg(MAIN_PARAM_CTRL_NUM,(*(ptr++)) & 0b01111111,0);
	}
	

}

void send_ctrl_msg(unsigned char ctrl_num, unsigned char value, unsigned char chan) {
	// a controller message's first byte is 0b1011xxxx, where xxxx is the channel, 0-15
	MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t)
	{
		.CableNumber = 0,// We can have up to 16 virtual cables.
		.Command     = 0b00001011, //(0b10110000 >> 4), damn, no savings
		.Data1       = 0b10110000 | chan, // so it ignores Data1 and just uses CableNumber and Command?
		.Data2       = ctrl_num,
		.Data3       = value,
	};
	MIDI_Device_SendEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent);	
}

void send_note_on(unsigned char pitch, unsigned char vel, unsigned char chan) {
	// a controller message's first byte is 0b1011xxxx, where xxxx is the channel,0-15
	MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t)
	{
		.CableNumber = 0, 
		.Command     = (0b10010000 >> 4),
		.Data1       = 0b10010000 | chan, // so it ignores Data1 and just uses CableNumber and Command?
		.Data2       = pitch,
		.Data3       = vel,
	};
	MIDI_Device_SendEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent);	
}

void send_note_off(unsigned char pitch, unsigned char vel, unsigned char chan) {
	// a controller message's first byte is 0b1011xxxx, where xxxx is the channel,0-15
	MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t)
	{
		.CableNumber = 0,
		.Command     = (0b10000000 >> 4),
		.Data1       = 0b10000000 | chan, // so it ignores Data1 and just uses CableNumber and Command?
		.Data2       = pitch,
		.Data3       = vel,
	};
	MIDI_Device_SendEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent);	
}

// assuming val contains an 8bit value.
void send_pitch_wheel(unsigned char val, unsigned char chan) {
	// a controller message's first byte is 0b1011xxxx, where xxxx is the channel,0-15
	MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t)
	{
		.CableNumber = 0,
		.Command     = (0b11100000 >> 4),
		.Data1       = 0b11100000 | chan, // so it ignores Data1 and just uses CableNumber and Command?
		.Data2       = (val & 0b00000001) << 6, // contains the 7 least sig bits
		.Data3       = (val & 0b11111110) >> 1, // contains the 7 most sig bits
	};
	if (val == 255)
		MIDIEvent.Data2 = 0b01111111; // otherwise the max value we could send would be 255 << 6 =  16320
									  // when the real max value is 16383
	
	MIDI_Device_SendEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent);	
}








































// ----------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------//

//                           M E A S U R E    T O U C H 							//

// ----------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------//
// ----------------------------------------------------------------------------------//





int prim_iir_filter_out, last_prim_iir_filter_out, sec_iir_filter_out; 

 
void measure_touch() {
	int deltas[8];
	uint16_t status_flag = 0u;
	uint16_t burst_flag = 0u;
	static unsigned char last_touch_pos;
	char j;
	unsigned char max_delta;
	char num_to_ave;
	int ave_pos;
	int delta_pos;
	//int base_positions[8] = {-10, 36, 73, 110, 154, 190, 230, 262}; 
	int base_positions[8] = {-5, 34, 76, 112, 152, 186, 230, 262}; 
	//static unsigned char pos_array_index = 0;
	long weighted_sum;
	//int weight_sum;
	char max_delta_index;

	static char touch_dir = 1; // 1 is pos, -1 is neg
	static char light_dir = 1;
 
	//static int pos_array[POS_ARRAY_SIZE];
	int filter_diff;
	int temp_pos;
	
 
	//PORTD ^= (1<<PORTD4);
	current_time_ms_touch += 5; // approximation of loop time, used in
								// qt_measure_sensors() to reference sensor values over time.
		SPCR &= ~(1<<SPIE); // turn off SPI interrupt 		
		do {
			
			/*  one time measure touch sensors    */
			// Timer 0 is for the LEDs, we want to avoid the LED interrupt triggering in the middle
			// of a touch cycle because it will either cause massive noise in the touch or a flicker in 
			// the LEDs!
			// So, at the start of a loop, we syncronize with the LED interrupt to start the measurement.
			// I believe qt_measure_sensors() sets the Timer1 value allowing syncronization.
			// 
			// The LED interrupts fire every 380uS and take about 45uS to complete. Meanwhile, 
			// the QTouch library measures a different sensor about every 540uS or so, but the only
			// critical region is when it's measuring the discharge time of the Cs integration 
			// capacitor (not when it's pulsing or charging it up). I basically played with this 
			// delay on starting the touch measurement until I didn't see any noise. 
			// If the LED interrupt code changed, it would likely require another round of 
			// trial and error to avoid the interrupt happening during a measurement.

			//if (burst_flag)				
			while(TCNT0 != 15); // 47 also worked great
			//PORTD |= (1<<PD4); // debug signals
			//PORTD &= ~(1<<PD4);
						// note: do not set to the actual timer reset value stored in OCR0A, as it will
						// not interrupt on a direct setting.

			// We're not relying on QTouch library for any filtering, so the current_time_ms_touch 
			// doesn't matter. We just want the raw sensor values out.
			status_flag = qt_measure_sensors( current_time_ms_touch );
			

			// this is an indication from the QTouch library that more measurements are needed 
			// for recalibration, "compensate for drift, resolve detect integration", all of which
			// we're not utilizing. This was set when a finger was close but not touching and would 
			// force several measurement loops which could lead to a 20ms delay before MIDI is serviced.
			// No thanks! The VMeter use case is a bit different from a typical interface--your finger
			// is usually very close. Ignoring status_flag basically turns off the integration-based
			// touch detection. Haven't noticed many problems with false positive detections, though.

			
			// ignore burst_flag. Hence the while(0) below.
			//burst_flag = status_flag & QTLIB_BURST_AGAIN;        
			/*if (burst_flag) {
				send_ctrl_msg(9,(status_flag & QTLIB_BURST_AGAIN) >> 8,0); 
				send_ctrl_msg(10,(status_flag & QTLIB_RESOLVE_CAL) >> 8,0);
				send_ctrl_msg(11,(status_flag & QTLIB_RESOLVE_FILTERIN) >> 8,0); 
				send_ctrl_msg(12,(status_flag & QTLIB_RESOLVE_DI) >> 8,0);
				send_ctrl_msg(13,(status_flag & QTLIB_RESOLVE_POS_RECAL) >> 8,0);
			}*/
			/*Time critical host application code goes here*/
		
		//}while (  burst_flag) ;
		}while(0);		
		SPCR |= (1<<SPIE); // turn On SPI interrupt
		//PORTD |= (1<<PD4);
//	}
	_delay_ms(1);
	

#ifdef TESTING
	for (j=0;j<8;j++) {
		refs[j] = qt_measure_data.channel_references[j];
		raw[j] = qt_measure_data.channel_signals[j];
		
	}	
#endif

 	
  	deltas[7] = qt_measure_data.channel_references[7] - qt_measure_data.channel_signals[7];
	deltas[6] = qt_measure_data.channel_references[6] - qt_measure_data.channel_signals[6];
	deltas[5] = qt_measure_data.channel_references[5] - qt_measure_data.channel_signals[5];
	deltas[4] = qt_measure_data.channel_references[4] - qt_measure_data.channel_signals[4];
	deltas[3] = qt_measure_data.channel_references[3] - qt_measure_data.channel_signals[3];
	deltas[2] = qt_measure_data.channel_references[2] - qt_measure_data.channel_signals[2];
	deltas[1] = qt_measure_data.channel_references[1] - qt_measure_data.channel_signals[1];
	deltas[0] = qt_measure_data.channel_references[0] - qt_measure_data.channel_signals[0];


/*
	g.my_refs[0] = qt_measure_data.channel_signals[0];
	g.my_refs[1] = qt_measure_data.channel_signals[1];
	g.my_refs[2] = qt_measure_data.channel_signals[2];
	g.my_refs[3] = qt_measure_data.channel_signals[3];
	g.my_refs[4] = qt_measure_data.channel_signals[4];
	g.my_refs[5] = qt_measure_data.channel_signals[5];
	g.my_refs[6] = qt_measure_data.channel_signals[6];
	g.my_refs[7] = qt_measure_data.channel_signals[7];
*/		


	//qtouch_pos = GET_ROTOR_SLIDER_POSITION(0);	


			
	if (GET_SENSOR_STATE(0)) { // touch detected	
	
		
		max_delta = 0;
		for (j=0;j<8;j++) {
			if (deltas[j] > max_delta) {
				max_delta = deltas[j];
				max_delta_index = j;
			}
		}

		//weight_sum = 0;		
		weighted_sum = 0;			
		g.weight_sum = 0;
		//this_delta = deltas[max_delta_index];
		// now we know which key has the greatest delta
	
		// 0 out far away keys
		g.delta_sum =0;
		for (j=0;j<8;j++) {
			
			if (j - max_delta_index > 2 || max_delta_index -j > 2) {
				deltas[j] = 0;
			}
			g.delta_sum += deltas[j];
			deltas[j] = deltas[j] * deltas[j];
			g.weight_sum += deltas[j];

		}
		// square everything to apply a finger shaped profile, and reduce influence of weak signals
		//g.weight_sum = 0;
		//for (j=0;j<8;j++) {
			//g.weight_sum += deltas[j];
			
			 // and calc the weight_sum while we're here
		//}
		//g.weight_sum = weight_sum;
	
		// calculate weighted average.  For some reason, this breaks bad when it's in a for loop.
		weighted_sum += (long)deltas[0] * (long)base_positions[0];
		weighted_sum += (long)deltas[1] * (long)base_positions[1];
		weighted_sum += (long)deltas[2] * (long)base_positions[2];
		weighted_sum += (long)deltas[3] * (long)base_positions[3];
		weighted_sum += (long)deltas[4] * (long)base_positions[4];
		weighted_sum += (long)deltas[5] * (long)base_positions[5];
		weighted_sum += (long)deltas[6] * (long)base_positions[6];
		weighted_sum += (long)deltas[7] * (long)base_positions[7];	
		// do a weighted ave of these
		weighted_sum = weighted_sum / g.weight_sum;

// ud
		// unneeded:
	//	for (j=0;j<8;j++) {
	//		deltas[j] = deltas[j] >> 2; // divide by 4 so we can print them out in a byte.
	//	}

		// theoretically this shouldn't be needed either. well, some bases are outside actually...
		if (weighted_sum < 0)
			weighted_sum = 0;
		else if (weighted_sum > 255)
			weighted_sum = 255;


		last_weighted_sum = (int)weighted_sum;		

		if (cfg.upside_down == 1) {	
			last_weighted_sum = 255 - last_weighted_sum;
		}
		
		// we're going to multiply by 16 so we can effectively work with 16ths of integers,
		temp_pos = ((int)last_weighted_sum) << 4;  

		// Average the values using a single pole IIR filter, followed by another if we're not moving fast. 
		// The idea here is to provide a fast response when the finger is moving fast, but avoid any wiggling
		// when the finget is holding still. The format of the filter is special in that it has no overshoot.	
		
		// this makes sure it doesn't ramp to the first value, hard set filter initial condition to current pos.
		if (prim_iir_filter_out == -1000) { // flag to say we've been in non-touch 
			prim_iir_filter_out = temp_pos; 
			last_prim_iir_filter_out = temp_pos;
			sec_iir_filter_out = temp_pos;			 
		}												
		else {
			// single pole filter: y(n) = a*x(n) + (1-a)*y(n-1). small a means more filtering.
			filter_diff = temp_pos - prim_iir_filter_out;
			prim_iir_filter_out = prim_iir_filter_out + (filter_diff>>1); // add a portion of the most recent change
			// this is the light filter output, ie, stage 1.
			// we only filter more if the position is relatively close to the previous output			
			delta_pos = prim_iir_filter_out - last_prim_iir_filter_out;
			last_prim_iir_filter_out = prim_iir_filter_out;// store off previous value	
			
			// remember that we're dealing with a 0-256 scale times 16!!, 0 - 4096
#define SECOND_FILTER_RANGE 24
#define SECOND_FILTER_RANGE_NEG -24
			if (delta_pos < SECOND_FILTER_RANGE && delta_pos > SECOND_FILTER_RANGE_NEG) { // it's relatively close to the last position
				// apply extra filtering
				filter_diff = prim_iir_filter_out - sec_iir_filter_out;
				sec_iir_filter_out = (filter_diff>>3) + sec_iir_filter_out;
			//	filter_mode = 127; // use for debugging to output when secondary filter is active.
			//	sec_iir_filter_out =prim_iir_filter_out;

			}
			else { // preload the secondary filter and pass through the first filter's output
			//	filter_mode = 0;
				sec_iir_filter_out = prim_iir_filter_out;
			} 
			// two pole filter: y(n) = .25*x(n) + y(n-1) - .25*y(n-2). 
			// The cascade of two filters: y(n) = .5x(n) + .5y(n-1).  This filter has 0 overshoot (no ringing).
					
			//	iir_filtered_pos = (temp_pos>>2) + iir_filtered_pos_1 - (iir_filtered_pos_2>>2);	
			//	iir_filtered_pos_2 = iir_filtered_pos_1;
			//	iir_filtered_pos_1 = iir_filtered_pos;
			//	
		}
	

		ave_pos = sec_iir_filter_out >> 4; // scale back down again.
		
		

		// reduce jitter in lights with slight hysteresis
		g.h_light_pos = calc_hys(ave_pos, g.h_light_pos, &light_dir, 3); 

		// position hysteresis
		g.touch_pos = (unsigned char)calc_hys(ave_pos, g.touch_pos, &touch_dir, 1); // less hysteresis on touch
		
		if (last_touch_pos != g.touch_pos) {
			g.touch_pos_changed_since_last_output = 1; // this forces a position to be sent out midi			
		}	
		last_touch_pos = g.touch_pos;

		if (!g.was_just_in_touch) {
			g.touch_pos_changed_since_last_output = 1;
			g.on_off_state_changed = 1;
		}		
		g.was_just_in_touch = 1;
		
	} // end if sensor_touched
	else { 
		//state = 0;
		if (g.was_just_in_touch == 1) {
			// send last pos since the averaging may have prevented the lights from reaching the last touched pos.			
			//if (!leds_ignore_touch && !pitch_wheel_mode)
			//	pre_set_column((int)h_light_pos);
			g.h_light_pos = last_weighted_sum; // hard set to last value to catch fast swipes
			g.touch_pos	= g.h_light_pos;
			g.was_just_in_touch = 0;
			g.touch_pos_changed_since_last_output = 1; // this forces a position to be sent out midi
			g.on_off_state_changed = 1;			
		}
		else {
			// this is used as a "was just out of touch" flag. all the filter states get hard set to 
			// latest position.
			prim_iir_filter_out = -1000; 			
		}
		// We're still in NO_TOUCH_DETECTED zone..			
	}
	
	

	
	g.state = (GET_SENSOR_STATE(0));
	
}



/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{	
	//unsigned int startup_delay = 0;
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	unsigned char temp_brightness;
	clock_prescale_set(clock_div_1);

	//start_boot_if_required();

	// set D7 as input / should default to input!
	//DDRD &= ~( 1 << DDD7 );	
	PORTD |= 0b10000000; // enable internal pull up
		

//	DDRD = (1<<DDD5); // make D5 and output
//	PORTD |= 0b00010000;

	init_LEDs();	

	USB_Init();
	
	//while(startup_delay++ < 10000UL) {// wait a bit to prevent disorientating the touch sensors
	//	MIDI_Device_USBTask(&Keyboard_MIDI_Interface);
	//	USB_USBTask();
	//}

	

	
	//was 33u for detect thresh. higher is harder to trigger. Any higher causes touch to be lost
	// when sliding along edge of track.
	// was 34u for v0.24
	// 31 seemed to work ok for v2
	// was 35u for all version B until China shipment from Jack
#define DEFAULT_SENSITIVITY 35

	qt_enable_slider( CHANNEL_0, CHANNEL_7, NO_AKS_GROUP, DEFAULT_SENSITIVITY + (64 - cfg.sensitivity), HYST_6_25, RES_8_BIT, 0u  );
	// hysteresis was HYST_6_25 for most of the 

   	

	qt_init_sensing();

	qt_set_parameters();

	init_timer_isr();
	temp_brightness = cfg.brightness;
	cfg.brightness = 20;
	unsigned char j;
	
	for (j=0;j<255;j++) {
		set_column(j);	
		//MIDI_Device_USBTask(&Keyboard_MIDI_Interface);						
		USB_USBTask();
		_delay_ms(13);
	}	
	
	qt_calibrate_sensing();

	DDRD |= (1<<DDD4);
	PORTD |= (1<<PD4);

	if (!(PIND & 0b10000000)) {
			start_boot();
	}

	cfg.brightness = temp_brightness;

//	led_groups[0] = 0;
//	led_groups[1] = 0;
//	led_groups[2] = 0;
//	led_groups[3] = 0;
//	led_groups[4] = 0;
	led_groups[5] = 0;

	DDRB |= (1<<DDB3);
//	hardware_spi_init_as_slave();

//	while(1) {
//		USB_USBTask();
//	}
/*	
	led_groups[0] = 0b00000001;
	while(1) {
	//	PORTD |= (1<<PD4);
	//	PORTD &= ~(1<<PD4);
		
		for (j=0;j<255;j++) {
			//set_column(j);	
			//MIDI_Device_USBTask(&Keyboard_MIDI_Interface);						
			USB_USBTask();
			_delay_ms(5);
			
		}
		
		led_groups[0] = 0b00000001;
		//PORTD ^= (1<<PORTD4); // bitwise XOR toggle
	
		_delay_ms(100);
		
		
		//PORTD ^= (1<<PORTD4); // bitwise XOR toggle
		
		for (j=0;j<255;j++) {
			//set_column(255-j);	
			//MIDI_Device_USBTask(&Keyboard_MIDI_Interface);						
			USB_USBTask();
			_delay_ms(5);
			
		}
	led_groups[0] = 0b11111111;
		_delay_ms(100);

		if (!(PIND & 0b10000000)) {
			start_boot();
		}
	}
	*/
	
}



/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	//LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
	qt_calibrate_sensing();
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

int calc_hys(int input, int prev_result, char *dir, char hys) {
	int output = input; // default output equals input unless the direction changes by a small amount
	if (*dir == 1) {				
		if (input < prev_result && input >= (prev_result - hys)) { // changed dir, but not by enough
			output = prev_result;
		}		
	}
	else {			
		if (input > prev_result && input <= (prev_result + hys)) {
			output = prev_result;
		}	
	}		
	if (output > prev_result)
		*dir = 1;
	else if (output < prev_result)
		*dir = -1;
	// else don't change it.
	return output;
}









//
// __________________ TOUCH STUFF BELOW _________________ //
// 

// When a finger gets close, the reading goes down. If something gets close by, like a laptop
// or other VMeter, the reading will be artifically lower, so the reference value needs to go down.
// The negative drift rate, measured in 200ms increments, controls how fast this drops. You 
// don't want this too fast since you wouldn't want an approaching finger to lower the threshold.

// Positive drift rate is the opposite, and controls how fast the calibration ref goes back up.
// Say you had it near a laptop, and then moved it away, the reading would be higher than the
// ref, so you want the ref to quickly adjust back up.  This is the positive drift rate, which 
// is 5 by default (about 1s).


// Not sure when the recal_threshold triggers a recalibration. "If any key is found to have a 
// significant drop in signal delta, (on the negative side), it is deemed to be an error condition."
// --from the qtouch spec.

// I think this might be if the signal gets above the ref?
static void qt_set_parameters( void )
{
    /*  This will be modified by the user to different values   */
	qt_filter_callback = filter_data_mean_4; // 0;
    qt_config_data.qt_di              = 0; //DEF_QT_DI;
    qt_config_data.qt_neg_drift_rate  = 4; // about 1s //DEF_QT_NEG_DRIFT_RATE;
	// negative drift 
    qt_config_data.qt_pos_drift_rate  = 1; //DEF_QT_POS_DRIFT_RATE;
    qt_config_data.qt_max_on_duration = 0; // disable //DEF_QT_MAX_ON_DURATION;
    qt_config_data.qt_drift_hold_time = 15u; // time after touch detected that drifts are restricted from moving. //20u; //255u; //DEF_QT_DRIFT_HOLD_TIME; //default 20 = 4s, making this 255 didn't help issues with it becoming insensitive after sliding just off.
    qt_config_data.qt_recal_threshold = RECAL_100;   //DEF_QT_RECAL_THRESHOLD; (default is RECAL_50)
    qt_config_data.qt_pos_recal_delay = 3u; // DEF_QT_POS_RECAL_DELAY; (3u default). was 255

/*	qt_burst_lengths[0] = 32;
	qt_burst_lengths[1] = 32;
	qt_burst_lengths[2] = 32;
	qt_burst_lengths[3] = 32;
	qt_burst_lengths[4] = 32;
	qt_burst_lengths[5] = 32;
	qt_burst_lengths[6] = 32;
	qt_burst_lengths[7] = 32;
	*/

} // note: DON'T understand the recalibration settings (recal_threshold and pos_recal_delay).
  // If a key drops by RECAL_% of the threshold, it auto recalibrates after that condition
  // persists for qt_pos_recal_delay.


// this is a filter callback called from the touch library.
// It's not a mean anymore (previous version), but rather a 
// exponential moving average that requires less memory. 
// 8 values for 8 sensors rathaer than 8*4 values for a simple mean.
void filter_data_mean_4( void )
{
	unsigned char i;
	int delta;
	for( i = 0u; i < QT_NUM_CHANNELS; i++ ) // for all 8 sensors, average last 4 vals
	{
		// multiply by 16 so we can deal with partial integers (doesn't converge otherwise)
		delta = (qt_measure_data.channel_signals[i] << 4) - filter[i]; // previous filter value
		filter[i] = filter[i] + (delta>>2); // divide by 4
		qt_measure_data.channel_signals[i] = filter[i] >> 4;
		//qt_measure_data.channel_signals[i] = ( ( filter[i][0] +	filter[i][1] +filter[i][2] + filter[i][3]) >> 2); //+ filter[i][4] + filter[i][5] + filter[i][6] + filter[i][7]) / 8u );
	}

}


// timer1 is used for touch measurement, timer0 is used to control the LED brightness.
// These both control interrupts and may interfere with each other.
// Note: timer1 setting may get overwritten at runtime by QTouch library. 
// All 8 sensors get pulsed with 64 3.9uS period pulses which does not relate to the timer1 settings below.
static void init_timer_isr( void )
{
    /*  set timer compare value (how often timer ISR will fire) */
    // qt_measurement_period_msec = 2u.
    // TICKS_PER_MS = 1000
    OCR1A = ( TICKS_PER_MS * qt_measurement_period_msec); // 25 * 8 = 200.  new setting is 2000

    /*  enable timer ISR */
    TIMSK1 |= (1u << OCIE1A);

    /*  timer prescaler = system clock / 8  */
    TCCR1B |= (1u << CS11); // this means the timer will go off every 1MHz / 2000 = 500Hz

    /*  timer mode = CTC (count up to compare value, then reset)    */
    TCCR1B |= (1u << WGM12);

	
	// set up Timer0 (used to flash LEDs to control brightness)
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


// LED Interrupt
// This writes out the values stored in led_groups to the shift registers one group of 8 LEDs at a time.
// Since there are 5 groups of LEDs, this means the max duty cycle will be 20%. After flashing each 
// group of LEDs, a number of cycles are skipped with all LEDs off to dim the display. Up to about
// 35 dark cycles are used on the dimmest setting.

// WARNING: any changes to this code could result in interference with the touch measurement, which
// is also firing independently over about 4ms. The touch measrurement waits for TCNT0 to hit 15 before
// launching. See measure_touch() and adjust that delay point if changes are made here by experimenting
// and watching the sensor noise.
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

// spi byte received - initial prototyping of low-level SPI communication between VMeters.
ISR(SPI_STC_vect) {
	unsigned char r;
	set_column(SPDR);
	//r = SPDR;
}
