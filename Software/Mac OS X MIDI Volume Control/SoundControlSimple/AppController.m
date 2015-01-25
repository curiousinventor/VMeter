/*
 
VMeter Computer-side code. 7/16/2011
VMeter.net
 
Copyright (c) 2011 Curious Inventor, LLC

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#import "AppController.h"
#import "CustomView.h"
#import "MAAttachedWindow.h"
#import "VariableStore.h"
#import "SoundC.h"
#include "hidapi.h"
#include <IOKit/hid/IOHIDManager.h>
#include <IOKit/hid/IOHIDKeys.h>




#define WCHAR_ENCODING NSUTF32LittleEndianStringEncoding
#define VENDOR_ID 0x03eb
#define DEVICE_ID 0x1234
#define MIN_PRESSURE_INPUT 40
#define MAX_PRESSURE_INPUT 100
#define MIN_PRESSURE_OUTPUT 0
#define MAX_PRESSURE_OUTPUT 127
#define PRESSURE_FILTER_COEF .1
#define RAW_SENSOR_OFFSET 20

#define LED_COLUMN_HEIGHT_CTRL_NUM 20
#define BRIGHTNESS_CTRL_NUM 21
#define GEN_PARAM_CTRL_NUM 119
#define UPSIDE_DOWN_ON 125
#define UPSIDE_DOWN_OFF 126
#define UPGRADE_FIRMWARE 127
#define RECALIBRATE 112
#define STORE_SETTINGS 118

enum LED_CTL_SOURCE {
    TOUCH_ONLY = 0,
    MIDI,
    SYS_VOLUME
    };


@implementation AppController

@synthesize debug_text_field;
@synthesize debug_text_field2;
@synthesize touchCtrlNumPopList;
@synthesize ctrlNumStringArray;

@synthesize ledSourcePopList;
@synthesize ledCtrlNumPopList;
@synthesize pressureCtrlNumPopList;
@synthesize brightnessCtrlNumPopList;


@synthesize touchToCtrlCheckBox;
@synthesize touchToVolCheckBox;
@synthesize touchToNoteCheckBox;
@synthesize pressureToCtrlCheckBox;
@synthesize ignoreTouchCheckBox;
@synthesize ledBrightnessFromCtrlCheckBox;
@synthesize rawSensorsToCtrlsCheckBox;
@synthesize midiChannelPopList;
@synthesize touchOnOffToCtrlsCheckBox;
@synthesize touchOnOffCtrlNumPopList;


@synthesize ledSourceIndex;
@synthesize touchRouterCtrlNum;
@synthesize ledCtrlNum;
@synthesize pressureCtrlNum;
@synthesize brightnessCtrlNum;
@synthesize midiChannelNum;
@synthesize touchOnOffCtrlNum;



hid_device *handle;
int usb_out_counter;
unsigned long my_timer = 0;
float filtered_touch = 0.0;
char note_on = 0;
int last_note_on_pitch;
float ave_pressure = 0.0;
int raw_sensors[8];
int raw_sensors_ref[8];



// main init function, setup the pop-up window menus, the VMeter USB communication timer,
// control over system volume, 
- (void)awakeFromNib
{
    
    // Setup the V in the statusbar and the pop-up window
    float height = [[NSStatusBar systemStatusBar] thickness];
	float width = 30;
    NSRect viewFrame = NSMakeRect(0, 0, width, height);
    statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength:width] retain];
    [statusItem setToolTip:@"VMeter: click to configure"];
    [[SoundC sharedInstance] enumerateSoundDevices];
	//[self buildInputMenu];
	[self buildOutputMenu];  
    
    
    [slider setFloatValue:([[SoundC sharedInstance] getVolume])]; 
    
    // setup the timer that calls the function that periodically talks to the VMeter
    NSTimer * myTimer;
    myTimer = [NSTimer scheduledTimerWithTimeInterval:.1 target:self selector:@selector(onTimer:) userInfo:NULL repeats: YES];
    [[NSRunLoop currentRunLoop] addTimer:myTimer  forMode:NSEventTrackingRunLoopMode]; 
	
    // system sound stuff (for using the VMeter to control the system volume, an interface's volume, or another audio interface's volume

    
    [debug_text_field setStringValue:@""];
    GLOBALVAR->brightness = 16; // VMeter brightness goes from 0 to 16, 0 being off.
    
    // popup window setup
    [statusItem setView:[[[CustomView alloc] initWithFrame:viewFrame controller:self] autorelease]];      
    
    // fill all the drop-down lists in the pop-up window (things like the MIDI Channel, MIDI ctrl numbers, etc.)
    ctrlNumStringArray = [[NSMutableArray alloc] initWithCapacity:128];
//    int i;
//    NSString *temp;
//    for (i=0;i<128;i++) {
//        temp = [NSString stringWithFormat:@"%d",i];
//        [ctrlNumStringArray addObject:temp];
//        [temp release];        
//    }
//    NSArray *temp_array = [[NSArray alloc] initWithArray:ctrlNumStringArray];	
    //[touchCtrlNumPopList addItemsWithTitles:temp_array];
    //[ledCtrlNumPopList addItemsWithTitles:temp_array];
    //[pressureCtrlNumPopList addItemsWithTitles:temp_array];
    //[brightnessCtrlNumPopList addItemsWithTitles:temp_array];
    //[touchOnOffCtrlNumPopList addItemsWithTitles:temp_array];
//    [temp_array release];
    //[ctrlNumStringArray removeAllObjects];
//    for (i=0;i<16;i++) {
//        temp = [NSString stringWithFormat:@"%d",i];
//        [ctrlNumStringArray addObject:temp];
//        [temp release];        
//    }
//    NSArray *temp_array2 = [[NSArray alloc] initWithArray:ctrlNumStringArray];    
//    [midiChannelPopList addItemsWithTitles:temp_array2];
//    [temp_array2 release];    
//    [ctrlNumStringArray removeAllObjects];
//    [ctrlNumStringArray addObject:@"Touch Only"];
   // [ctrlNumStringArray addObject:@"System Volume"];
//    [ctrlNumStringArray addObject:@"MIDI Controller"];    
//    NSArray *temp_array3 = [[NSArray alloc] initWithArray:ctrlNumStringArray];
//    [ledSourcePopList addItemsWithTitles:temp_array3];
//    [ctrlNumStringArray release];
//    [temp_array3 release];
    
//	[NSApp setDelegate:self]; // standard app stuff?
}


// use this function to hide the popup window when another app is clicked on. Currently not used so the pop-up window stays
// on top of everything when you're using other programs. 
- (void)applicationDidResignActive:(NSNotification *)aNotification {
//	GLOBALVAR->clicked = NO;
//    [attachedWindow orderOut:self];
//	[[statusItem view] setNeedsDisplay:YES];
}


// volume slider moved, change lights on vmeter if volume control mode is enabled
- (IBAction) sliderMoved: (id) sender {
    float vol;
    vol = [slider floatValue]; // 0 to 100
	//[[SoundC sharedInstance] setVolume:vol];
    [self send_column_height_MIDI:vol*1.27];
}

// changes which side the light column is drawn from, top to bottom or bottom to top.
- (IBAction)upside_down_clicked:(id)sender {    
    
    if ([upside_down state] == NSOffState) {
        [self sendCtrlMsg:UPSIDE_DOWN_OFF ctrl:GEN_PARAM_CTRL_NUM];
    }
    else {
        [self sendCtrlMsg:UPSIDE_DOWN_ON ctrl:GEN_PARAM_CTRL_NUM];
    } 
    [self sendCtrlMsg:GLOBALVAR->last_system_volume*1.27 ctrl:LED_COLUMN_HEIGHT_CTRL_NUM];
 
       
}

- (IBAction)read_settings:(id)sender {
    NSLog(@"read settings");
    [self sendCtrlMsg:113 ctrl:GEN_PARAM_CTRL_NUM];
}

- (IBAction)load_defaults:(id)sender {
    NSLog(@"load defaults");
    // brightness controller num: 112, 21
    [self sendCtrlMsg:BRIGHTNESS_CTRL_NUM ctrl:112];
    
	// midi channel 117, 1
    [self sendCtrlMsg:1 ctrl:117];
    
	// upside down off: 126
    [self sendCtrlMsg:UPSIDE_DOWN_OFF ctrl:GEN_PARAM_CTRL_NUM];
    
	// LED column height controller num: 113, 20
    [self sendCtrlMsg:LED_COLUMN_HEIGHT_CTRL_NUM ctrl:113];
    
	// touch position controller num: 116, 20
    [self sendCtrlMsg:LED_COLUMN_HEIGHT_CTRL_NUM ctrl:116];
    
	// enable touch output, 119, 124
    [self sendCtrlMsg:124 ctrl:GEN_PARAM_CTRL_NUM];
    
	// disable pressure output 121
    [self sendCtrlMsg:121 ctrl:GEN_PARAM_CTRL_NUM];
    
	// disable on/off output 119
    [self sendCtrlMsg:119 ctrl:GEN_PARAM_CTRL_NUM];
    
    [self sendCtrlMsg:127 ctrl:BRIGHTNESS_CTRL_NUM];
    
	// disable pitch wheel
    [self sendCtrlMsg:114 ctrl:GEN_PARAM_CTRL_NUM];
    
	// disable cross fader mode
    [self sendCtrlMsg:104 ctrl:GEN_PARAM_CTRL_NUM];
    
	// lights ignore touch off
    [self sendCtrlMsg:106 ctrl:GEN_PARAM_CTRL_NUM];
    
	// disable note out
    [self sendCtrlMsg:116 ctrl:GEN_PARAM_CTRL_NUM];
    
	// store the settings
    [self sendCtrlMsg:STORE_SETTINGS ctrl:GEN_PARAM_CTRL_NUM];
}

// prep VMeter for firmware upgrade. Requires additional software and firmware. VMeter must be
// disconnected and reconnected to get out of this mode.
- (IBAction)upgrade_firmware:(id)sender {
    [self sendCtrlMsg:UPGRADE_FIRMWARE ctrl:GEN_PARAM_CTRL_NUM];
}

// forces recalibration--shouldn't normally be necessary as it's constantly adjusting to the surrounding environment.
- (IBAction)recalibrate:(id)sender {
    [self sendCtrlMsg:RECALIBRATE ctrl:GEN_PARAM_CTRL_NUM];
}

- (IBAction)save_settings_button: (id) sender {
    [self sendCtrlMsg:STORE_SETTINGS ctrl:GEN_PARAM_CTRL_NUM];
    
}

// this is the main communication with the VMeter.  This gets called every 4ms or so and reads a
// 16 byte packet from the VMeter.  The compelte protocol is detailed at the bottom of this file.
// Depending on what mode it's in, MIDI messages are triggered, the system volume is changed, etc.
-(void) onTimer: (NSTimer *)timer{
	int res, res2, i;
    int pressure_input;
    char buf[17];
    int val_to_send;
    unsigned char buf2[17];
    float temp;
    // if VMeter touched, set volume
    float v;
    int limited_pressure, scaled_pressure;    
    int dif[8];
    

    
//    [slider setFloatValue:temp*100.0];
//    [[SoundC sharedInstance] setVolume:temp*100.0];

    // v is 0 - 100.0
    
    // check to see if the slider has moved since the last cycle. This could happen due to 
    // manually moving the slider, or due to an incoming MIDI msg. 
    // When the slider is moved manually or via MIDI, only the slider itself is updated.
    // We only update the last_vol_slider_setpoint variable here when writing the 
    // the volume out to the system.
    
    // if the slider is in a new location, write that out to system volume.
    // else, check the system volume to see if that's changed since the last time we wrote to it,
    // if so, update VMeter and the slider.
    int slider_vol = [slider intValue];
    //NSLog(@"slider val: %d", slider_vol);
    if (slider_vol != GLOBALVAR->last_vol_slider_setpoint) {
        GLOBALVAR->last_vol_slider_setpoint = slider_vol;
        [[SoundC sharedInstance] setVolume:slider_vol];
        
    }
    else { // slider hasn't moved, read system volume and update VMeter and slider if changed.
        v = [[SoundC sharedInstance] getVolume];
        if (GLOBALVAR->last_system_volume != (int)v) {
            // update VMeter and slider
            GLOBALVAR->last_system_volume = (int)v;
            [self send_column_height_MIDI:v*1.27];
            [slider setFloatValue:(v)];             
        }
        
    }
    
//    v = [[SoundC sharedInstance] getVolume];
//    if (v != GLOBALVAR->last_led_setpoint) {
//        GLOBALVAR->last_led_setpoint = v;
//        //if ((v >= 0) && (v<=255)) {
//        [slider setFloatValue:(v)];  
//        val_to_send = v*1.27;
//        NSLog(@"sending MIDI column height: %d, orig: %f", val_to_send, v);
//        [self send_column_height_MIDI:val_to_send];
//        //[self send_column_to_USB:(v)];                    
//        //}	
//    }
    

}




// shows or hides the main settings windows when the V in the statusbar is clicked.
- (void)toggleAttachedWindowAtPoint:(NSPoint)pt
{
    // Attach/detach window.
    if (!attachedWindow) {
        attachedWindow = [[MAAttachedWindow alloc] initWithView:view 
                                                attachedToPoint:pt 
                                                       inWindow:nil 
                                                         onSide:MAPositionBottom 
                                                     atDistance:5.0];
		[attachedWindow setCollectionBehavior: NSWindowCollectionBehaviorCanJoinAllSpaces];
		[attachedWindow setLevel:NSFloatingWindowLevel];
		[attachedWindow setViewMargin:2];
		[attachedWindow setCornerRadius:8.0];
		[attachedWindow setBackgroundColor:[NSColor blackColor]];
		[attachedWindow setBackgroundColor:[NSColor colorWithCalibratedRed:0.0 green:0.0 blue:0.0 alpha:1.0]];
		[attachedWindow setBorderColor:[NSColor whiteColor]];
		//[self performSelector:@selector(onTimer:)];
			
		//[NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(onTimer:) userInfo:NULL repeats: YES];
	}	
    if ([attachedWindow isVisible] == NO) {
		//NSRect screenRect = [[NSScreen mainScreen] visibleFrame];
		//float w = MIN(screenRect.size.width - 20, 1024);
		//float h = MIN(screenRect.size.height - 40, 768);
		//[view setFrameSize:NSMakeSize(w, h)];
		[attachedWindow _redisplay];
        [attachedWindow makeKeyAndOrderFront:self];
		[NSApp activateIgnoringOtherApps:YES];
    } else {
        [attachedWindow orderOut:self];
    }    
}

- (void)drawStatusRect:(NSRect)rect High:(BOOL)hi {
	[statusItem drawStatusBarBackgroundInRect:rect withHighlight:hi];    
}
// detects all the audio output devices.
- (void) buildOutputMenu {
	[mOutputDevices removeAllItems];
	if ([SoundC sharedInstance]->lastDevOutput < 0) return;
	for (int i = 0; i <= [SoundC sharedInstance]->lastDevOutput; i++) {
		NSString *mName = [NSString stringWithCString:&[SoundC sharedInstance]->devOutput[i].mName[0] encoding:NSASCIIStringEncoding];
		[mOutputDevices addItemWithTitle:mName];
	}
}



- (IBAction)slider_brightness_moved:(id)sender {
    unsigned char b;
    b = (unsigned char)[slider_brightness intValue];   
    // b is 0 - 15?
    NSLog(@"brightness: %d", b);
    GLOBALVAR->brightness = b;    
    if (b > 0) {
        [self sendCtrlMsg:b*8-1 ctrl:BRIGHTNESS_CTRL_NUM];
    }
    else {
        [self sendCtrlMsg:0 ctrl:BRIGHTNESS_CTRL_NUM];
    }
   // [self send_brightness_to_USB: b]; 
    
}


- (IBAction)open_help:(id)sender {    
    NSWorkspace * ws = [NSWorkspace sharedWorkspace];
    NSURL *url = [NSURL URLWithString: @"http://vmeter.net/instructions"];
    [ws openURL:url];
}




// ------------------------------------------- //
// MIDI Stuff below:
- (id) init	{
	NSLog(@"AppController:init:");
	self = [super init];
	msgArray = [[NSMutableArray arrayWithCapacity:0] retain];
    midiManager = [[VVMIDIManager alloc] init];
    [midiManager setDelegate:self];
    [midiManager virtualDest].name = @"VMeter";
  //  NSLog(@"virtualDest: %@", [midiManager virtualDest].name);
    
    return self;
}
// when MIDI devices are added or taken away
- (void) setupChanged	{
//	NSLog(@"AppController:setupChanged:");
    int source_num = 0;
    int dest_num = 0;
    int i;
    id			midiDev;
    NSLog(@"setupChanged");
    
    source_num = [[midiManager sourceArray] count];
    dest_num = [[midiManager destArray] count];
    
    for (i=0;i<source_num;i++) {
        // if name includes VMeter
        
        midiDev = [[midiManager sourceArray] lockObjectAtIndex:i];       
        NSLog(@"checking %@", [midiDev name]);
        if (!([[midiDev name] rangeOfString:@"VMeter"].location == NSNotFound)) {
            [midiDev setEnabled:YES];
            NSLog(@"found VMeter in sourceArray");
        } 
        else {
            NSLog(@"did not find VMeter in sourceArray");
        }
    }
    for (i=0;i<dest_num;i++) {
        // if name includes VMeter
        midiDev = [[midiManager destArray] lockObjectAtIndex:i];     
        NSLog(@"checking %@", [midiDev name]);
        if (!([[midiDev name] rangeOfString:@"VMeter"].location == NSNotFound)) {
            [midiDev setEnabled:YES];
            NSLog(@"found VMeter in destArray");
            [self sendCtrlMsg:113 ctrl:GEN_PARAM_CTRL_NUM]; // read settings.
        } 
        else {
            NSLog(@"did not find VMeter in destArray");
        }
    }
    
    
    
    
	[sourcesTableView reloadData];
	[receiversTableView reloadData];
}


// this gets called when MIDI comes in. It generates a list of messages for the MIDI input display, and acts on the MIDI messages
// according to the settings to change the VMeter LEDs.
- (void) receivedMIDI:(NSArray *)a fromNode:(VVMIDINode *)n	{
//	NSLog(@"AppController:receivedMIDI:");
//	NSLog(@"virtualDest: %@", [midiManager virtualDest].name);
    NSEnumerator		*it = [a objectEnumerator];
	VVMIDIMessage		*msgPtr;
	NSMutableString		*mutString = [NSMutableString stringWithCapacity:0];
    int ctrl_num, value, channel;   
    static int received_midi_indicator_state = 0;
    static int settings_packet_index = 0;
    int vmeter_brightness = 0;
    
    //debug_text_field
    if (received_midi_indicator_state == 0) {
        received_midi_indicator_state++;
        [debug_text_field setStringValue:@"received midi -"];
    }
    else if (received_midi_indicator_state == 1) {
        received_midi_indicator_state++;
        [debug_text_field setStringValue:@"received midi /"];
    }
    else if (received_midi_indicator_state == 2) {
        received_midi_indicator_state = 0;
        [debug_text_field setStringValue:@"received midi \\"];
    }

    while ((msgPtr = [it nextObject])) {
        channel = [msgPtr channel];
        ctrl_num = [msgPtr data1];
        value = [msgPtr data2];
       
        if (channel == 0) { // ignore other channels
            if (ctrl_num == LED_COLUMN_HEIGHT_CTRL_NUM) {                    
                // set system volume and volume slider    
                NSLog(@"received touch volume: %d", value);
                [slider setFloatValue:((float)value)/1.27];
                //[[SoundC sharedInstance] setVolume:((float)value)/1.27];
            }
            else if (ctrl_num == 119) { // receiving settings packet
                if (value == 85) {
                    settings_packet_index = 0;
                }
                else {
                    GLOBALVAR->settings_packet[settings_packet_index++] = value;
                }
                if (settings_packet_index == 12) {
                    NSLog(@"received Settings");
                    settings_packet_index = 0;
                    // set brightness and upside-down mode in interface.
                    [upside_down setState:(GLOBALVAR->settings_packet[0] & 0x01)];
                    NSLog(@"upside down: %d", (GLOBALVAR->settings_packet[0] & 0x01));
                    vmeter_brightness = ((GLOBALVAR->settings_packet[9])+1) / 8;
                    NSLog(@"vmeter_brightness = %d, orig: %d", vmeter_brightness, GLOBALVAR->settings_packet[9]);
                    [slider_brightness setIntValue:vmeter_brightness];
                }
                
            }
        }                                  
    }
		
	
}

// takes value between 0-127
- (void) send_column_height_MIDI: (NSInteger)val {
    VVMIDIMessage		*msg = nil;    
    //+ (id) createFromVals:(Byte)t:(Byte)c:(Byte)d1:(Byte)d2;
    msg = [VVMIDIMessage createFromVals:
                 VVMIDIControlChangeVal:
                                    0:
             LED_COLUMN_HEIGHT_CTRL_NUM:
           val];
	if (msg != nil)
		[midiManager sendMsg:msg];      
}


- (void) sendCtrlMsg:(NSInteger)val ctrl:(NSInteger)cNum {
  //  NSLog(@"Ctrl: %d val: %d",cNum,val);
    VVMIDIMessage		*msg = nil;    
    msg = [VVMIDIMessage createFromVals:
                 VVMIDIControlChangeVal:
           midiChannelNum:
                     cNum:
           val];
	if (msg != nil)
		[midiManager sendMsg:msg];    
}



// MIDI device listings
- (NSInteger) numberOfRowsInTableView:(NSTableView *)tv	{
	if (tv == sourcesTableView)
		return [[midiManager sourceArray] count];
	else if (tv == receiversTableView)
		return [[midiManager destArray] count];
	
	return 0;
}
- (id) tableView:(NSTableView *)tv objectValueForTableColumn:(NSTableColumn *)tc row:(NSInteger)row	{
	if (tv == sourcesTableView)	{
		//NSLog(@"\t\tsources");
		id			midiSource = [[midiManager sourceArray] lockObjectAtIndex:row];
		
		if (midiSource == nil)
			return nil;
		
		if (tc == sourcesNameColumn)	{
			return [midiSource name];
		}
		else if (tc == sourcesEnableColumn)	{
			if ([midiSource enabled])
				return [NSNumber numberWithInt:NSOnState];
			else
				return [NSNumber numberWithInt:NSOffState];
		}
	}
	else if (tv == receiversTableView)	{
		//NSLog(@"\t\treceivers");
		id			midiSource = [[midiManager destArray] lockObjectAtIndex:row];
		
		if (midiSource == nil)
			return nil;
		
		if (tc == receiversNameColumn)	{
			return [midiSource name];
		}
		else if (tc == receiversEnableColumn)	{
			if ([midiSource enabled])
				return [NSNumber numberWithInt:NSOnState];
			else
				return [NSNumber numberWithInt:NSOffState];
		}
	}	
	return nil;
}

- (void) tableView:(NSTableView *)tv setObjectValue:(id)v forTableColumn:(NSTableColumn *)tc row:(NSInteger)row	{
	//NSLog(@"AppController:tableView:setObjectValue:forTableColun:row:");
	if (tv == sourcesTableView)	{
		//NSLog(@"\t\tsources");
		id			midiSource = [[midiManager sourceArray] lockObjectAtIndex:row];
		
		if (midiSource == nil)
			return;
		
		if (tc == sourcesEnableColumn)	{
			if ([v intValue] == NSOnState)
				[midiSource setEnabled:YES];
			else
				[midiSource setEnabled:NO];
		}
	}
	else if (tv == receiversTableView)	{
		//NSLog(@"\t\treceivers");
		id			midiSource = [[midiManager destArray] lockObjectAtIndex:row];
		
		if (midiSource == nil)
			return;
		
		if (tc == receiversEnableColumn)	{
			if ([v intValue] == NSOnState)
				[midiSource setEnabled:YES];
			else
				[midiSource setEnabled:NO];
		}
	}
}





- (void)dealloc
{
    [[NSStatusBar systemStatusBar] removeStatusItem:statusItem];
    [midiManager release];
    [touchCtrlNumPopList release];
    [ledSourcePopList release];
    [super dealloc];
}



@end





// additional code for detected and enumerating different audio devices.  For instance, you could change the volume of a 
// USB audio device.
/*
 //SELECTED DEVICES
 [[SoundC sharedInstance] getSelectedDevices];
 //SET INPUT MENU SELECTED ITEM
 NSString *deviceInputName = [NSString stringWithCString:[SoundC sharedInstance]->selectedInputDevice encoding:NSASCIIStringEncoding];
 for (int i = 0; i < [mInputDevices numberOfItems]; i++) {
 if ([[[mInputDevices itemAtIndex:i] title] isEqualToString:deviceInputName]) {
 [mInputDevices selectItemAtIndex:i];
 break;
 }
 }
 //SET OUTPUT MENU SELECTED ITEM
 NSString *deviceOutputName = [NSString stringWithCString:[SoundC sharedInstance]->selectedOutputDevice encoding:NSASCIIStringEncoding];
 for (int i = 0; i < [mOutputDevices numberOfItems]; i++) {
 if ([[[mOutputDevices itemAtIndex:i] title] isEqualToString:deviceOutputName]) {
 [mOutputDevices selectItemAtIndex:i];
 break;
 }
 }
 }
 - (IBAction)inputDeviceSelected:(id)sender {
 [[SoundC sharedInstance] setDeviceNamed:[mInputDevices titleOfSelectedItem] Type:kAudioTypeInput];
 }
 
 - (IBAction)outputDeviceSelected:(id)sender {
 [[SoundC sharedInstance] setDeviceNamed:[mOutputDevices titleOfSelectedItem] Type:kAudioTypeOutput];	
 }
 
 - (void) buildInputMenu {
 [mInputDevices removeAllItems];
 if ([SoundC sharedInstance]->lastDevInput < 0) return;
 for (int i = 0; i <= [SoundC sharedInstance]->lastDevInput; i++) {
 NSString *mName = [NSString stringWithCString:&[SoundC sharedInstance]->devInput[i].mName[0] encoding:NSASCIIStringEncoding];
 [mInputDevices addItemWithTitle:mName];
 }
 }
 
 - (void)applicationDidFinishLaunching:(NSNotification *)aNotification
 {
 // Insert code here to initialize your application
 [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(onTimer:) userInfo:NULL repeats: YES];
 [[SoundC sharedInstance] enumerateSoundDevices];
 //[self buildInputMenu];
 [self buildOutputMenu];
 }
 */

    /*       
--------------------------------------------------     
VMeter USB Protocol
--------------------------------------------------
     
                      
The VMeter continously sends packets of 16 bytes and receives packets of 8 bytes about every 4ms (subject to change).

Some of the bytes sent from the VMeter are diagnostic and may change.

8 BYTE PACKET SENT TO VMETER:
     
packet[0]: always required, not actually sent to VMeter but must be 0.     
packet[1]: command (see below)
packet[2]: command parameter
packet[3]: brightness: 0-16, 0 is off, 16 is max brightness
packet[4]: binary value of lights 0-7 (when packet[1] == 2)
packet[5]: binary value of lights 8-15 (when packet[1] == 2)
packet[6]: binrary value of lights 16-23 (when packet[1] == 2)
packet[7]: binrary value of lights 24-31 (when packet[1] == 2)
packet[8]: binrary value of lights 32-38 (38 lights in total) (when packet[1] == 2)
     
commands:
     1: When packet[1]==1, packet[2] contains the column height, 0 thru 38. If upside-down mode is on, it draws from the top
     2: When packet[1]==2, packet[4-8] contain the binary value for each group of 8 lights. For instance, sending a 6 in packet[4]
        would turn on the 2nd and 3rd lights since 6 == 0b00000110
     251: When packet[1]==251, packet[2] sets the ignoreTouch mode. If 1, touches don't change the lights. If 0, touches override
        whatever is currently controlling the lights (MIDI for instance).
     252: packet[2] turns on / off upside-down mode.
     253: recalibrate.  Sometimes this might be necessary if you move the VMeter.
     254: Prepare to update firmware. Puts the VMEter in a special state that will accept new firmware. Requires additional software
            and firmware
     
     
    
16 BYTE PACKET RECEIVED FROM VMETER:

packet[0]: Touch state, 0 for off, 1 for on
packet[1]: not used
packet[2]: current light position being displayed, 0 - 38
packet[3]: cap sensor 1 value, closest to VMeter label and USB port, can be negative
packet[4]: cap sensor 2 value, can be negative
packet[5]: cap sensor 3 value, can be negative
packet[6]: cap sensor 4 value, can be negative
packet[7]: cap sensor 5 value, can be negative
     
packet[8]: cap sensor 6 value, can be negative
packet[9]: cap sensor 7 value, can be negative
packet[10]: cap sensor 8 value, can be negative
packet[11]: diagnostic, not specified
packet[12]: touch_pos (0-255)
packet[13]: pressure, or a sum of all the cap sensor differentials (current value - ref value). Can be negative?
packet[14]: not used
packet[15]: not used
     

     
     */
