#ifndef LONG_MSG_H
#define LONG_MSG_H


/*

  LongMsg.h

  CLongMsg class declaration. This class represents a system exclusive 
  MIDI message.

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
#include <exception>    // For std::exception


namespace midi
{
    //-----------------------------------------------------------------
    // CLongMsgIndexOutOfBounds class
    //
    // An exception class. Thrown when an index to a CLongMsg object is
    // out of bounds.
    //-----------------------------------------------------------------

    class CLongMsgIndexOutOfBounds : public std::exception
    {
    public:
        const char *what() const
        { return "Index to CLongMsg object is out of bounds."; }
    };


    //-----------------------------------------------------------------
    // CLongMsg class
    //
    // This class represents system exclusive messages.
    //-----------------------------------------------------------------

    class CLongMsg : public CMIDIMsg
    {
    public:
        // Constructors/Destructor
        CLongMsg();
        CLongMsg(const char *Msg, DWORD Length);
        CLongMsg(const CLongMsg &Msg);
        virtual ~CLongMsg();

        // Assignment
        CLongMsg &operator = (const CLongMsg &Msg);
        
        // Sends message
        void SendMsg(midi::CMIDIOutDevice &OutDevice);

        // Accessors/Mutators
        DWORD GetLength() const { return m_Length; }
        const char *GetMsg() const { return m_Msg;}
        void SetMsg(const char *Msg, DWORD Length);

    protected:
        // Subscript access. This is for derived classes to use in order
        // to access the individual bytes within a CLongMsg object.
        char &operator [] (int i);

    private:
        char *m_Msg;
        DWORD m_Length;        
    };
}


#endif