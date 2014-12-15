#ifndef __LEDs_H__
#define __LEDs_H__

	// defines:
		// LED spi chip:
		#define INTENSITY 0x0A
		#define DECODE_MODE 0x09
		#define SCAN_LIMIT 0x0B
		#define SHUT_DOWN_MODE 0x0C
		#define DISPLAY_TEST 0x0F
		#define NOOP 0x00


	void init_LEDs(void);
	void set_property_LED(unsigned char prop, unsigned char val);
	void set_column(unsigned char height);
	int pre_set_column(unsigned char h);
	void display_pitch_wheel(unsigned char);
//	void set_all_lights(int *);

#endif
