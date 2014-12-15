/*

This file tells the computer what kind of USB device it is (MIDI, HID USB), as 
well as the device name, manufacturer, and VID/PIDs.

Note in this case the above values are stored in EEPROM. The device name is overwritten
when the MIDI device channel number changes to assist in differentiating multiple
devices in Windows.

----
	
Copyright (c) 2014 Curious Inventor, LLC
VMeter.net

MIT License:

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.



------

VMeter is built on top of LUFA, a USB stack for Atmel devices made by Dean Camera. 

His copyright / warranty info follows:

------


             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org

  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  USB Device Descriptors, for library use when in USB device mode. Descriptors are special
 *  computer-readable structures which the host requests upon device enumeration, to determine
 *  the device's capabilities and functions.
 */

#include "Descriptors.h"

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
const USB_Descriptor_Device_t EEMEM DeviceDescriptor =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(01.10),
	.Class                  = USB_CSCP_NoDeviceClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = 0x16D0,
	.ProductID              = 0x0668,
	.ReleaseNumber          = VERSION_BCD(01.32), // this seems to subtract 1 from what I set... 1.29 comes out as 1.28?

	.ManufacturerStrIndex   = 0x01,
	.ProductStrIndex        = 0x02,
	.SerialNumStrIndex      = USE_INTERNAL_SERIAL,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
const USB_Descriptor_Configuration_t EEMEM ConfigurationDescriptor =
{
	.Config =
		{
			.Header                   = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize   = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces          = 2,

			.ConfigurationNumber      = 1,
			.ConfigurationStrIndex    = NO_DESCRIPTOR,

			.ConfigAttributes         = (USB_CONFIG_ATTR_BUSPOWERED | USB_CONFIG_ATTR_SELFPOWERED),

			.MaxPowerConsumption      = USB_CONFIG_POWER_MA(100)
		},

	.Audio_ControlInterface =
		{
			.Header                   = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber          = 0,
			.AlternateSetting         = 0,

			.TotalEndpoints           = 0,

			.Class                    = AUDIO_CSCP_AudioClass,
			.SubClass                 = AUDIO_CSCP_ControlSubclass,
			.Protocol                 = AUDIO_CSCP_ControlProtocol,

			.InterfaceStrIndex        = NO_DESCRIPTOR
		},

	.Audio_ControlInterface_SPC =
		{
			.Header                   = {.Size = sizeof(USB_Audio_Descriptor_Interface_AC_t), .Type = DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_Header,

			.ACSpecification          = VERSION_BCD(01.00),
			.TotalLength              = sizeof(USB_Audio_Descriptor_Interface_AC_t),

			.InCollection             = 1,
			.InterfaceNumber          = 1,
		},

	.Audio_StreamInterface =
		{
			.Header                   = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber          = 1,
			.AlternateSetting         = 0,

			.TotalEndpoints           = 2,

			.Class                    = AUDIO_CSCP_AudioClass,
			.SubClass                 = AUDIO_CSCP_MIDIStreamingSubclass,
			.Protocol                 = AUDIO_CSCP_StreamingProtocol,

			.InterfaceStrIndex        = NO_DESCRIPTOR
		},

	.Audio_StreamInterface_SPC =
		{
			.Header                   = {.Size = sizeof(USB_MIDI_Descriptor_AudioInterface_AS_t), .Type = DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_General,

			.AudioSpecification       = VERSION_BCD(01.00),

			.TotalLength              = (sizeof(USB_Descriptor_Configuration_t) -
			                             offsetof(USB_Descriptor_Configuration_t, Audio_StreamInterface_SPC))
		},

	.MIDI_In_Jack_Emb =
		{
			.Header                   = {.Size = sizeof(USB_MIDI_Descriptor_InputJack_t), .Type = DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_InputTerminal,

			.JackType                 = MIDI_JACKTYPE_Embedded,
			.JackID                   = 0x01,

			.JackStrIndex             = NO_DESCRIPTOR
		},

	.MIDI_In_Jack_Ext =
		{
			.Header                   = {.Size = sizeof(USB_MIDI_Descriptor_InputJack_t), .Type = DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_InputTerminal,

			.JackType                 = MIDI_JACKTYPE_External,
			.JackID                   = 0x02,

			.JackStrIndex             = NO_DESCRIPTOR
		},

	.MIDI_Out_Jack_Emb =
		{
			.Header                   = {.Size = sizeof(USB_MIDI_Descriptor_OutputJack_t), .Type = DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_OutputTerminal,

			.JackType                 = MIDI_JACKTYPE_Embedded,
			.JackID                   = 0x03,

			.NumberOfPins             = 1,
			.SourceJackID             = {0x02},
			.SourcePinID              = {0x01},

			.JackStrIndex             = NO_DESCRIPTOR
		},

	.MIDI_Out_Jack_Ext =
		{
			.Header                   = {.Size = sizeof(USB_MIDI_Descriptor_OutputJack_t), .Type = DTYPE_CSInterface},
			.Subtype                  = AUDIO_DSUBTYPE_CSInterface_OutputTerminal,

			.JackType                 = MIDI_JACKTYPE_External,
			.JackID                   = 0x04,

			.NumberOfPins             = 1,
			.SourceJackID             = {0x01},
			.SourcePinID              = {0x01},

			.JackStrIndex             = NO_DESCRIPTOR
		},

	.MIDI_In_Jack_Endpoint =
		{
			.Endpoint =
				{
					.Header              = {.Size = sizeof(USB_Audio_Descriptor_StreamEndpoint_Std_t), .Type = DTYPE_Endpoint},

					.EndpointAddress     = (ENDPOINT_DESCRIPTOR_DIR_OUT | MIDI_STREAM_OUT_EPNUM),
					.Attributes          = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
					.EndpointSize        = MIDI_STREAM_EPSIZE,
					.PollingIntervalMS   = 0x01
				},

			.Refresh                  = 0,
			.SyncEndpointNumber       = 0
		},

	.MIDI_In_Jack_Endpoint_SPC =
		{
			.Header                   = {.Size = sizeof(USB_MIDI_Descriptor_Jack_Endpoint_t), .Type = DTYPE_CSEndpoint},
			.Subtype                  = AUDIO_DSUBTYPE_CSEndpoint_General,

			.TotalEmbeddedJacks       = 0x01,
			.AssociatedJackID         = {0x01}
		},

	.MIDI_Out_Jack_Endpoint =
		{
			.Endpoint =
				{
					.Header              = {.Size = sizeof(USB_Audio_Descriptor_StreamEndpoint_Std_t), .Type = DTYPE_Endpoint},

					.EndpointAddress     = (ENDPOINT_DESCRIPTOR_DIR_IN | MIDI_STREAM_IN_EPNUM),
					.Attributes          = (EP_TYPE_BULK | ENDPOINT_ATTR_NO_SYNC | ENDPOINT_USAGE_DATA),
					.EndpointSize        = MIDI_STREAM_EPSIZE,
					.PollingIntervalMS   = 0x01
				},

			.Refresh                  = 0,
			.SyncEndpointNumber       = 0
		},

	.MIDI_Out_Jack_Endpoint_SPC =
		{
			.Header                   = {.Size = sizeof(USB_MIDI_Descriptor_Jack_Endpoint_t), .Type = DTYPE_CSEndpoint},
			.Subtype                  = AUDIO_DSUBTYPE_CSEndpoint_General,

			.TotalEmbeddedJacks       = 0x01,
			.AssociatedJackID         = {0x03}
		}
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
const USB_Descriptor_String_t EEMEM LanguageString =
{
	.Header                 = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},

	.UnicodeString          = {LANGUAGE_ID_ENG}
};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t EEMEM ManufacturerString =
{
	.Header                 = {.Size = USB_STRING_LEN(21), .Type = DTYPE_String},

	.UnicodeString          = L"Curious Inventor, LLC"
};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t EEMEM ProductString =
{
	.Header                 = {.Size = USB_STRING_LEN(13), .Type = DTYPE_String}, // may need to hard set the size below
																				 // see DTYPE_String and case 0x02

	// don't forget to change size, too!
	.UnicodeString          = L"VMeter 1.31 0" // the first letter of this MUST be 'V'. The main program checks this 
												// to determine if the eeprom has valid descriptors.
};		// ***** don't forget to change the TWO places in descriptors.c, and the VER4 define in MIDI.c!



/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void* Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	switch (DescriptorType)
	{
		case DTYPE_Device:
			Address = &DeviceDescriptor;
			Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration:
			Address = &ConfigurationDescriptor;
			Size    = sizeof(USB_Descriptor_Configuration_t);
			break;
		case DTYPE_String:
			switch (DescriptorNumber)
			{
				case 0x00:
					Address = &LanguageString;
					Size    = eeprom_read_byte(&LanguageString.Header.Size);
					//pgm_read_byte(&LanguageString.Header.Size);
					break;
				case 0x01:
					Address = &ManufacturerString;
					Size    = eeprom_read_byte(&ManufacturerString.Header.Size);
					//pgm_read_byte(&ManufacturerString.Header.Size);
					break;
				case 0x02:
					Address = &ProductString;
					Size    = eeprom_read_byte(&ProductString.Header.Size);
					//pgm_read_byte(&ProductString.Header.Size);
					break;
			}

			break;
	}

	*DescriptorAddress = Address;
	return Size;
}

