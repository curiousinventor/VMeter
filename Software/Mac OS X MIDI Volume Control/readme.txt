This is a basic explanation of the VMeter computer-side code.  Please see vmeter.net/instructions for more informaiton, including the detailed protocol of the packets sent back and forth between the VMeter.

File content descriptions:

AppController.m - the meat of the program
hid.c - the lower level usb system stuff
MAAttachedWindow.m - handles the pop-up window and the systembar icon
MainMenu2.xib - the pop-up window interface, also contains the top menu lists
SoundC/SoundC.m - controls the system volume level, can detect / list all audio interfaces (not used). Could be used to control other USB audio device volumes.
Frameworks/VVMIDI & VVBasics - this library handles all the MIDI stuff


Basic overall description:

The program sends and receives packets from the VMeter about every 4ms. Those packets include touch information from the VMeter, and display and operating mode instructions from the computer.  Most of the action happens in these functions inside AppController.m:

- awakeFromNib: This is where everything starts. The pop window gets setup, the status-bar icon is setup, a timer starts that will periodically send and receive data with the VMeter, the drop-down lists for the pop-up window are defined, the system audio interfaces are detected. 

- init: this sets up the MIDI stuff, called sometime during startup.

- onTimer: called every 4ms (approximately), exchanges data with the VMeter.  Initiates MIDI messages depending on settings, and also controls lights and operating mode depending on settings.

-check_for_vmeter: called before reading or writing a USB packet, connects to a HID USB device via hidapi if not already connected

-receivedMIDI: route incoming MIDI messages to VMeter LEDs

-toggleAttachedWindowAtPoint: show / hide the control panel from the V in the statusbar







 








