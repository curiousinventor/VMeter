#ifndef MIDI_MSG_H
#define MIDI_MSG_H


/*

  MIDIMsg.h

  Interface for the CMIDIMsg class. This is the base class for all MIDI
  message classes.

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

*/


//---------------------------------------------------------------------
// Dependencies
//---------------------------------------------------------------------


#include "stdafx.h"    // For DWORD data type


//---------------------------------------------------------------------
// Forward declarations
//---------------------------------------------------------------------


namespace midi
{
    class CMIDIOutDevice;
}


namespace midi
{
    //------------------------------------------------------------------
    // CMIDIMsg class
    //
    // This class represents the base class for all MIDI messages.
    //------------------------------------------------------------------

    class CMIDIMsg
    {
    public:
        virtual ~CMIDIMsg() {}

        // Sends MIDI message
        virtual void SendMsg(midi::CMIDIOutDevice &OutDevice) = 0; 

        // Gets the MIDI message length
        virtual DWORD GetLength() const = 0;

        // Gets the MIDI message
        virtual const char *GetMsg() const = 0;

        // Get/Set time stamp
        DWORD GetTimeStamp() const { return m_TimeStamp; }
        void SetTimeStamp(DWORD TimeStamp) { m_TimeStamp = TimeStamp; }

    private:
        DWORD m_TimeStamp;
    };
}


#endif