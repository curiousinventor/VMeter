#ifndef SHORT_MSG_H
#define SHORT_MSG_H


/*

  ShortMsg.h

  CShortMsg class declaration. This class represents short MIDI messages.

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


#include "MIDIMsg.h"    // For CMIDIMsg base class
#include "StdAfx.h"


namespace midi
{
    // Constants
    const DWORD SHORT_MSG_LENGTH = 3;


    //-----------------------------------------------------------------
    // CShortMsg class
    //
    // This class represents short MIDI messages.
    //-----------------------------------------------------------------

    class CShortMsg : public CMIDIMsg
    {
    public:
        // Constructors
        explicit CShortMsg(DWORD TimeStamp = 0);
        CShortMsg(DWORD Msg, DWORD TimeStamp = 0);
        CShortMsg(unsigned char Status, unsigned char Data1,
            unsigned char Data2, DWORD TimeStamp);
        CShortMsg(unsigned char Command, unsigned char Channel,
            unsigned char Data1, unsigned char Data2, 
            DWORD TimeStamp);

        // Sends message
        void SendMsg(midi::CMIDIOutDevice &OutDevice);

        // Sends message without status byte
        void SendMsgNoStatus(midi::CMIDIOutDevice &OutDevice);

        //
        // Accessors
        // 

        DWORD GetLength() const
        { return midi::SHORT_MSG_LENGTH; }
        const char *GetMsg() const;
        unsigned char GetStatus() const;
        unsigned char GetChannel() const;
        unsigned char GetCommand() const;
        unsigned char GetData1() const;
        unsigned char GetData2() const;

        //
        // Mutators
        //

        void SetMsg(unsigned char Status, unsigned char Data1,
            unsigned char Data2);
        void SetMsg(unsigned char Command, unsigned char Channel,
            unsigned char Data1, unsigned char Data2);

        //
        // Class methods
        //


        // Packs short messages without status byte
        static DWORD PackShortMsg(unsigned char DataByte1,
                                  unsigned char DataByte2);


        // Packs short messages with status byte
        static DWORD PackShortMsg(unsigned char Status,
                                  unsigned char DataByte1,
                                  unsigned char DataByte2);

        // Packs short channel messages
        static DWORD PackShortMsg(unsigned char Command,
                                  unsigned char Channel,
                                  unsigned char DataByte1,
                                  unsigned char DataByte2);

        // Unpacks short messages
        static void UnpackShortMsg(DWORD Msg, unsigned char &Status,
                                   unsigned char &DataByte1,
                                   unsigned char &DataByte2);

        // Unpacks short channel messages
        static void UnpackShortMsg(DWORD Msg, unsigned char &Command,
                                   unsigned char &Channel,
                                   unsigned char &DataByte1,
                                   unsigned char &DataByte2);

    private:
        DWORD m_Msg;
        DWORD m_MsgNoStatus;
    };
}


#endif