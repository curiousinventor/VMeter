/*

    CLongMsg.cpp

    Implementation for the CLongMsg class.

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


#include "stdafx.h"
#include "LongMsg.h"
#include "MIDIOutDevice.h"


// Using declaration
using midi::CLongMsg;


//---------------------------------------------------------------------
// CLongMsg implementation
//---------------------------------------------------------------------


// Default constructor
CLongMsg::CLongMsg() :
m_Msg(0),
m_Length(0)
{}


// Constructor
CLongMsg::CLongMsg(const char *Msg, DWORD Length) :
m_Msg(0),
m_Length(0)
{
    SetMsg(Msg, Length);
}


// Constructor
CLongMsg::CLongMsg(const CLongMsg &Msg)
{
    m_Msg = 0;
    m_Length = 0;

    *this = Msg;
}


// Destructor
CLongMsg::~CLongMsg()
{
    // Release resources for this object if they exist
    if(m_Msg != 0)
    {
        delete [] m_Msg;
    }
}


// Assignment
CLongMsg &CLongMsg::operator = (const CLongMsg &Msg)
{
    // Test for self assignment
    if(this != &Msg)
    {
        SetMsg(Msg.m_Msg, Msg.m_Length);
    }

    return *this;
}


// Sends MIDI message
void CLongMsg::SendMsg(midi::CMIDIOutDevice &OutDevice)
{
    OutDevice.SendMsg(m_Msg, m_Length);
}


// Sets message
void CLongMsg::SetMsg(const char *Msg, DWORD Length)
{
    // Release old message if it exists
    if(m_Msg != 0)
    {
        delete [] m_Msg;
    }

    // 
    // Allocate and initialize new message
    //

    m_Msg = new char[Length];
    m_Length = Length;

    for(DWORD i = 0; i < m_Length; i++)
    {
        m_Msg[i] = Msg[i];
    }
}


// Subscripting
char &CLongMsg::operator [] (int i)
{
    // Bounds checking
    if(m_Length == 0 || i < 0 || i >= m_Length)
    {
        throw CLongMsgIndexOutOfBounds();
    }

    return m_Msg[i];
}