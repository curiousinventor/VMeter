// support@vmeter.net

// Javascript MIDI processor
// Use to customize how the VMeter behaves

// MIDI from the VMeter comes in to 3 functions:
//
//   pitchbend
//   cc
//   note

// Each of those functions can be written to do a variety of remappings.
// Some examples are shown below, and include 
// -converting the pitchbend input into 2 cc messages going out.
// -convert the pitch bend to a single cc output message
// -use only the bottom 1/3rd of the VMeter touch surface for full scale
// -scale the note out from 0-127 to a restricted range. 

// most of the outputs have been disabled. Uncomment the lines that
// have "send_out..." in them.

// You can also write out to the VMeter, see send_out_cc_msg_to_vmeter()
// This could be used to control the LEDs differently


post("vmeter_midi.js reloaded");

outlets = 4;

setoutletassist(2,"VMeter Out");
setoutletassist(3,"External Out");

// -------------------------- //
// INPUT PROCESSING FUNCTIONS //
// -------------------------- //

// receive pitchbend message.
// convert to CC or rescale
function pitchbend() {
	// value = agruments[0] (0-16383)
	// midi_chan = arguments[1]
	
	// convert pitch bend info into two output cc's, one for
	// each half of the pitch bend. Recommend cross fader mode.
	var midi_chan = arguments[1];	
	var input = arguments[0];
	input = input / 64; // reduce input range down to 0-255.
	input = Math.floor( input ); // make sure not a float.
	//post("input: " + input);	// debugging output
	if (input < 128) {
		// we're on the lower half of the VMeter, 
		// want to have this converted to a CC output (to cc#30)
		// where the center of the VMeter is 0, and all 
		// the way at the bottom is 127.
		
		// 0-127
		input = 127 - input; // reverse input so center is 0.
		//send_out_cc_msg_to_external(input, 30, midi_chan);
	}
	else { 
		// want to make the top half of the VMeter output to 
		// cc#31
		input = input - 128; // convert from 128-255 down to 0 - 127
		//send_out_cc_msg_to_external(input, 31, midi_chan);
	}

	// other example routings:
	
	// copy input to external or back to VMeter:
	//send_out_pitchbend_to_external(arguments[0],arguments[1]);
	
	// convert pitchbend to cc message:
	//send_out_pitchbend_to_vmeter(arguments[0],arguments[1]);
	// divide input (0-16383) by 128 to get it down to 0-127 range.
	//send_out_cc_msg_to_vmeter(arguments[0] >> 7,20,1);
}


// receive control change message from VMeter
// rescale
// argument[0] is controller num
// argument[1] is value
// argument[2] is channel, comes in 1-16
function cc() {
	var value = arguments[1];
	var cc_num = arguments[0];
	var midi_chan = arguments[2];
	
	//post("input: " + value);	
	
	// scale input so that bottom 1/3 of VMeter controls full output.
	value = 3*value;
	if (value > 127) // cap max value.
		value = 127;
	

	//send_out_cc_msg_to_external(value, cc_num, midi_chan);
	
	// alternate output routings:
	
	// simply send back to VMeter: 
	// You could set VMeter LEDs to ignore touch, and reverse the LEDs for instance.
	//send_out_cc_msg_to_vmeter(arguments[1], arguments[0], arguments[2]);
}


// scale note range from 0-127 to [min_pitch, max_pitch]
function note(pitch, vel, channel) {
	var min_pitch = 60; // middle C
	var max_pitch = 79;
	var out_pitch;	
	
	out_pitch = Math.round((max_pitch - min_pitch) * (pitch / 127.0) + min_pitch);
	
	//post("pitch: " + pitch + ", out_pitch: " + out_pitch + ", vel: " + vel + "\n");
	//post("out: " + note_byte1 + ", " + out_pitch + ", " + vel + "\n");
	
	send_note_out(out_pitch,vel,channel); 
}





// -------------------------- //
// OUTPUT PROCESSING FUNCTIONS //
// -------------------------- //


// http://www.gweep.net/~prefect/eng/reference/protocol/midispec.html#Whl
// MIDI Protocol for pitch bend
// pitch bend is 14 << 4 & channel, low 7 bits, high 7 bits
// max value is 16383. (2^14 - 1)
function send_out_pitchbend_to_vmeter(value,channel) {
	var pitchbend_byte1;
	// 224 = 14 << 4
	pitchbend_byte1 = 224 | (channel - 1);	
	var low_6_bits, high_6_bits;
	low_6_bits = value & 127; // grab low 7 bits only.
	high_6_bits = (value >> 7) & 127;
	// send out to vmeter (outlet 2)
	outlet(2,pitchbend_byte1,low_6_bits,high_6_bits);
}

// 11 << 4 & channel, ctrl num, value
function send_out_cc_msg_to_vmeter(value,controller_num,channel) {
	// 176 = 11 << 4
	outlet(2,176 | channel-1, controller_num, value);
}

function send_out_cc_msg_to_external(value,controller_num,channel) {
	// 176 = 11 << 4
	outlet(3,176 | channel-1, controller_num, value);
}

function send_out_pitchbend_to_external(value,channel) {
	var pitchbend_byte1;
	// 224 = 14 << 4
	pitchbend_byte1 = 224 | (channel - 1);	
	var low_6_bits, high_6_bits;
	low_6_bits = value & 127; // grab low 7 bits only.
	high_6_bits = (value >> 7) & 127;
	// send out to vmeter (outlet 2)
	outlet(3,pitchbend_byte1,low_6_bits,high_6_bits);
}

function send_note_out(pitch,vel,channel) {
	var note_byte1 = 144 | (channel - 1); // note off is noteOn with vel = 0
	outlet(3,note_byte1,pitch,vel);
}

