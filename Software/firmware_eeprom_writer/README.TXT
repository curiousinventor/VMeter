this program copies data into the eeprom since there's no way to use the hid_bootloader to copy data into the eeprom.

http://www.deadnode.org/sw/bin2h/
Using that program to convert the binary eeprom file into a c header file.

MUST do make clean before compiling original and the eeprom writer.
the hex2bin and other utilities are in /VMeter/software/utilities/bin (which is in the path)

Usage: starting with the .eep file, convert it to a binary file with 
hex2bin -e eepbin MIDI.eep 
This creates MIDI.eepbin

Then create a .h file representing the binary file
bin2h MIDI.eepbin MIDI_eeprom.h

Copy MIDI_eeprom.h into the eeprom writer directory

“make clean all” the vm_eeprom project

Load the eeprom
hid_bootloader_cli.exe -mmcu=at90usb162 -v vm_eeprom.hex

wait 10s

unplug and replug

Finally, load the flash

hid_bootloader_cli.exe -mmcu=at90usb162 -v MIDI.hex

On OS X, open Audio MIDI Setup,
In Window menu at top, select “Show MIDI Window”

while device is NOT connected, select any VMeter devices and remove them. This will force the name to reload in other applications. The name will change from VMeter x.xx 0 to VMeter 1.26 X, where X is A-P depending on the MIDI channel.

whoops, this is a change just so i can commit another push to git with 1.28 instead of 1.26.