// config for VMeter


// since this is sent as control messages, can't use the MSbit in each byte.
// This is fine for most params, as they only store numbers from 0-127
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
