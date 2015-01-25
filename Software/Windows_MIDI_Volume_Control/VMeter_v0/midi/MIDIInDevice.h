#ifndef MIDI_IN_DEVICE_H
#define MIDI_IN_DEVICE_H


/*******************************************************************************
 * MIDIInDevice.h - Interface for CMIDIInDevice and related classes.
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
    // CMIDIReceiver
    //
    // An abstract class representing objects that receive messages
    // from CMIDIInDevice objects. A CMIDIReceiver object registers
    // itself with a CMIDIInDevice object. It will then receive all
    // messages that are recorded.
    //----------------------------------------------------------------


    class CMIDIReceiver
    {
    public:
        virtual ~CMIDIReceiver() {}

        // Receives short messages
        virtual void ReceiveMsg(DWORD Msg, DWORD TimeStamp) = 0;

        // Receives long messages
        virtual void ReceiveMsg(LPSTR Msg, DWORD BytesRecorded,
                                DWORD TimeStamp) = 0;

        // Called when an invalid short message is received
        virtual void OnError(DWORD Msg, DWORD TimeStamp) = 0;

        // Called when an invalid long message is received
        virtual void OnError(LPSTR Msg, DWORD BytesRecorded,
                             DWORD TimeStamp) = 0;
    };


    //----------------------------------------------------------------
    // CMIDIInDevice exception classes
    //----------------------------------------------------------------

	/*
    // Encapsulates the midiInGetErrorText messages
    class CMIDIInException : public std::exception
    {
    public:
        CMIDIInException(MMRESULT ErrCode) throw()
        { ::midiInGetErrorText(ErrCode, m_ErrMsg, sizeof m_ErrMsg); }

        const wchar_t *what() const throw() { return m_ErrMsg; }

    private:
        wchar_t m_ErrMsg[128];
    };


    // Thrown when memory allocation fails within a CMIDIInDevice 
    // object
    class CMIDIInMemFailure : public std::bad_alloc
    {
    public:
        const wchar_t *what() const throw()
        { return L"Memory allocation within a CMIDIInDevice object failed."; }
    };


    // Thrown when a CMIDIInDevice is unable to create a signalling 
    // event
    class CMIDIInEventFailure : public std::exception
    {
    public:
        const wchar_t *what() const throw()
        { return L"Unable to create a signalling event for MIDIInDevice object."; }
    };


    // Thrown when a CMIDIInDevice is unable to create a worker thread
    class CMIDIInThreadFailure : public std::exception
    {
    public:
        const wchar_t *what() const throw()
        { return L"Unable to create worker thread for CMIDIInDevice object."; }
    };
	*/

    //----------------------------------------------------------------
    // CMIDIInDevice
    //
    // This class represents MIDI input devices. 
    //----------------------------------------------------------------


    class CMIDIInDevice
    {
    public:
        // For constructing a CMIDIInDevice object in an closed state
        CMIDIInDevice();

        // For constructing a CMIDIInDevice object in an closed state and
        // initializing the MIDI receiver
        CMIDIInDevice(CMIDIReceiver &Receiver);

        // For constructing a CMIDIInDevice object in an opened state
        CMIDIInDevice(UINT DeviceId, CMIDIReceiver &Receiver);

        // Destruction
        ~CMIDIInDevice();

        // Opens the MIDI input device
        void Open(UINT DeviceId);

        // Closes the MIDI input device
        void Close();

        // Adds a buffer to receive system exclusive messages
        void AddSysExBuffer(LPSTR Buffer, DWORD BufferLength);

        // Starts the recording process
        void StartRecording();

        // Stops the recording process
        void StopRecording();

        // Sets the current MIDI receiver. Returns the previous 
        // receiver.
        CMIDIReceiver *SetReceiver(CMIDIReceiver &Receiver);

        // Returns true if the device is open
        bool IsOpen() const;

        // Returns true if the device is recording
        bool IsRecording() const;

        // Gets Id for this device
        UINT GetDevID() const;

        // Gets the number of MIDI input devices on this system
        static UINT GetNumDevs() { return midiInGetNumDevs(); }

        // Gets the capabilities of a particular MIDI input device
        // The results are stored in the MIDIINCAPS parameter.
        static void GetDevCaps(UINT DeviceId, MIDIINCAPS &Caps);

    // Private methods
    private:
        // Copying and assignment not allowed
        CMIDIInDevice(const CMIDIInDevice &);
        CMIDIInDevice &operator = (const CMIDIInDevice &);

        // Creates an event for signalling the header thread
        bool CreateEvent();

        // Called by Windows when a MIDI input event occurs
        static void CALLBACK MidiInProc(HMIDIIN MidiIn, UINT Msg,
                                        DWORD_PTR Instance, DWORD_PTR Param1, 
                                        DWORD_PTR Param2);

        // Thread function for managing headers
        static DWORD WINAPI HeaderProc(LPVOID Parameter);

    // Private class declarations
    private:
        // Encapsulates the MIDIHDR structure for MIDI input
        class CMIDIInHeader
        {
        public:
            CMIDIInHeader(HMIDIIN DevHandle, LPSTR Buffer, 
                          DWORD BufferLength);
            ~CMIDIInHeader();

            // Add the buffer for receiving system exclusive messages
            void AddSysExBuffer();

        private:
            HMIDIIN m_DevHandle;
            MIDIHDR m_MIDIHdr;
        };


        // Thread safe queue for storing CMIDIInHeader objects
        class CHeaderQueue
        {
        public:
            CHeaderQueue();
            ~CHeaderQueue();

            void AddHeader(CMIDIInHeader *Header);
            void RemoveHeader();
            void RemoveAll();
            bool IsEmpty();

        private:
            std::queue<CMIDIInHeader *> m_HdrQueue;
            CRITICAL_SECTION m_CriticalSection;
        };

    // Private attributes and constants
    private:
        HMIDIIN        m_DevHandle;
        HANDLE         m_Event;
		CWinThread    *m_Thread;
        CMIDIReceiver *m_Receiver;
        CHeaderQueue   m_HdrQueue;
        enum State { CLOSED, OPENED, RECORDING } m_State;
    };
}


#endif
