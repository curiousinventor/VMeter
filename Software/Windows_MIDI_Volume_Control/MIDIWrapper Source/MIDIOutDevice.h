#ifndef MIDI_OUT_DEVICE_H
#define MIDI_OUT_DEVICE_H


/*******************************************************************************
 * MIDIOutDevice.h - Interface for CMIDIOutDevice and related classes.
 *
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
 *
 * Note: You must link to the winmm.lib to use these classes.
 ******************************************************************************/


#pragma warning(disable:4786) // Disable annoying template warnings


//--------------------------------------------------------------------
// Dependencies
//--------------------------------------------------------------------


// Necessary for Windows data types
#include "stdafx.h"
#include <mmsystem.h>

// Necessary for exception classes derived from std::exception
#include <exception> 

// Necessary for header queue used by CHeaderQueue
#include <queue>


namespace midi
{
    //----------------------------------------------------------------
    // Class declarations
    //----------------------------------------------------------------


    //----------------------------------------------------------------
    // CMIDIOutDevice exception classes
    //----------------------------------------------------------------
//	WCHAR wstr[128];
//size_t convertedChars = sizeof(wstr)/sizeof(WCHAR);
//mbstowcs_s(&convertedChars, wstr, str.c_str(), _TRUNCATE);
/*
    // Encapsulates the midiOutGetErrorText messages
    class CMIDIOutException : public std::exception
    {
    public:
        CMIDIOutException(MMRESULT ErrCode) throw()
        { ::midiOutGetErrorText(ErrCode, m_ErrMsg, sizeof m_ErrMsg); }

        const wchar_t *what() const throw() { return m_ErrMsg; }

    private:
        wchar_t m_ErrMsg[128];
    };


    // Thrown when memory allocation fails within a CMIDIOutDevice 
    // object
    class CMIDIOutMemFailure : public std::bad_alloc
    {
    public:
        const wchar_t *what() const throw()
        { return L"Memory allocation within a CMIDIOutDevice object failed."; }
    };


    // Thrown when a CMIDIOutDevice is unable to create a signalling 
    // event
    class CMIDIOutEventFailure : public std::exception
    {
    public:
        const wchar_t *what() const throw()
        { return L"Unable to create a signalling event for CMIDIOutDevice object."; }
    };


    // Thrown when a CMIDIOutDevice is unable to create a worker 
    // thread
    class CMIDIOutThreadFailure : public std::exception
    {
    public:
        const wchar_t *what() const throw()
        { return L"Unable to create worker thread for CMIDIOutDevice object."; }
    };
	*/

    //----------------------------------------------------------------
    // CMIDIOutDevice
    //
    // This class represents MIDI output devices.
    //----------------------------------------------------------------


    class CMIDIOutDevice
    {
    public:
        // For constructing a CMIDIOutDevice in an closed state
        CMIDIOutDevice();

        // For constructing a CMIDIOutDevice in an opened state
        CMIDIOutDevice(UINT DeviceId);

        // Destruction
        ~CMIDIOutDevice();

        // Opens the MIDI output device
        void Open(UINT DeviceId);

        // Closes the MIDI output device
        void Close();

        // Sends short message
        void SendMsg(DWORD Msg);

        // Sends long message
        void SendMsg(LPSTR Msg, DWORD MsgLength);

        // Returns true if the device is open
        bool IsOpen() const;

        // Gets the Id for this device
        UINT GetDevID() const;

        // Gets the number of MIDI output devices on this system
        static UINT GetNumDevs() { return midiOutGetNumDevs(); }

        // Gets the capabilities of a particular MIDI output device
        // The results are stored in the MIDIOUTCAPS parameter.
        static void GetDevCaps(UINT DeviceId, MIDIOUTCAPS &Caps);


    // Private methods
    private:
        // Copying and assignment not allowed
        CMIDIOutDevice(const CMIDIOutDevice &);
        CMIDIOutDevice &operator = (const CMIDIOutDevice &);

        // Creates an event for signalling the header thread
        bool CreateEvent();

        // Called by Windows when a MIDI output event occurs
        static void CALLBACK MidiOutProc(HMIDIOUT MidiOut, UINT Msg,
                                         DWORD_PTR Instance, DWORD_PTR Param1, 
                                         DWORD_PTR Param2);

        // Thread function for managing headers
        static DWORD WINAPI HeaderProc(LPVOID Parameter);

    // Private class declarations
    private:
        // Encapsulates the MIDIHDR structure for MIDI output
        class CMIDIOutHeader
        {
        public:
            CMIDIOutHeader(HMIDIOUT DevHandle, LPSTR Msg, 
                           DWORD MsgLength);
            ~CMIDIOutHeader();

            void SendMsg();

        private:
            HMIDIOUT m_DevHandle;
            MIDIHDR  m_MIDIHdr;
        };


        // Thread safe queue for storing CMIDIOutHeader objects
        class CHeaderQueue
        {
        public:
            CHeaderQueue();
            ~CHeaderQueue();

            void AddHeader(CMIDIOutHeader *Header);
            void RemoveHeader();
            void RemoveAll();
            bool IsEmpty();

        private:
            std::queue<CMIDIOutHeader *> m_HdrQueue;
            CRITICAL_SECTION m_CriticalSection;
        };

    // Private attributes and constants
    private:
        HMIDIOUT       m_DevHandle;
        HANDLE         m_Event;
        CWinThread    *m_WorkerThread;
        CHeaderQueue   m_HdrQueue;
        enum State { CLOSED, OPENED } m_State;
    };
}


#endif
