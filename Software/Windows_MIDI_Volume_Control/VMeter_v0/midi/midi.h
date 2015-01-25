#ifndef MIDI_H
#define MIDI_H


/*******************************************************************************
    midi.h - Common MIDI declarations.
 
    Copyright (c) 2008 Leslie Sanford

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

    Contact: Leslie Sanford (jabberdabber@hotmail.com)

    Last modified: 01/23/2008
 ******************************************************************************/


namespace midi
{
    //----------------------------------------------------------------
    // Constants
    //----------------------------------------------------------------


    // Status byte for Active Sensing message
    const unsigned char ACTIVE_SENSING = 0xFE;

    // Command value for Channel Pressure (Aftertouch)
    const unsigned char CHANNEL_PRESSURE = 0xD0;

    // Status byte for Continue message
    const unsigned char CONTINUE = 0xFB;

    // Command value for Control Change message
    const unsigned char CONTROL_CHANGE = 0xB0;

    // Status byte for System Exclusive message
    const unsigned char SYSTEM_EXCLUSIVE = 0xF0;

    // Status byte for End of System Exclusive message
    const unsigned char END_OF_EXCLUSIVE = 0xF7;

    // Status byte for MIDI Time Code Quarter Fram message
    const unsigned char MIDI_TIME_CODE = 0xF1;

    // Command value for Note Off message
    const unsigned char NOTE_OFF = 0x80;

    // Command value for Note On message
    const unsigned char NOTE_ON = 0x90;

    // Command value for Pitch Bend message
    const unsigned char PITCH_BEND = 0xE0;

    // Command value for Polyphonic Key Pressure (Aftertouch)
    const unsigned char POLY_PRESSURE = 0xA0;

    // Command value for Program Change message
    const unsigned char PROGRAM_CHANGE = 0xC0;
    
    // Status byte for Song Position Pointer message
    const unsigned char SONG_POSITION_POINTER = 0xF2;

    // Status byte for MIDI Song Select message
    const unsigned char SONG_SELECT = 0xF3;

    // Status byte for Start message
    const unsigned char START = 0xFA;

    // Status byte for Stop message
    const unsigned char STOP = 0xFC;

    // Status byte for System Reset message
    const unsigned char SYSTEM_RESET = 0xFF;

    // Status byte for Timing Clock message
    const unsigned char TIMING_CLOCK = 0xF8;

    // Status byte for Tune Request message
    const unsigned char TUNE_REQUEST = 0xF6;

    //
    // For unpacking and packing short messages
    //
    const unsigned char SHORT_MSG_MASK = 15;
    const unsigned char SHORT_MSG_SHIFT = 8;
}


#endif

