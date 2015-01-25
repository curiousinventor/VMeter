/*
 
 VMeter Computer-side code. 7/16/2011
 VMeter.net
 
 Copyright (c) 2011 Curious Inventor, LLC
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */


#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import <WebKit/WebPolicyDelegate.h>
#import <VVMIDI/VVMIDI.h>

@class MAAttachedWindow;
@interface AppController : NSObject <VVMIDIDelegateProtocol> {
    NSStatusItem *statusItem;
    MAAttachedWindow *attachedWindow;
    
    IBOutlet NSView *view;
	IBOutlet NSSlider *slider;
    
    IBOutlet NSTextField *debug_text_field;
   
    IBOutlet NSButton *upside_down;
    IBOutlet NSSlider *slider_brightness;
    IBOutlet NSTextField *debug_text_field2;
	IBOutlet NSPopUpButton *mInputDevices;
	IBOutlet NSPopUpButton *mOutputDevices;
    
    IBOutlet NSButton *touchToCtrlCheckBox;
    IBOutlet NSButton *touchToVolCheckBox;
    IBOutlet NSButton *touchToNoteCheckBox;
    IBOutlet NSButton *pressureToCtrlCheckBox;
    IBOutlet NSButton *ignoreTouchCheckBox;
    IBOutlet NSButton *ledBrightnessFromCtrlCheckBox;
    IBOutlet NSButton *rawSensorsToCtrlsCheckBox;
    IBOutlet NSButton *touchOnOffToCtrlsCheckBox;
   
    
    IBOutlet VVMIDIManager		*midiManager;
	IBOutlet NSTableView		*sourcesTableView;
	IBOutlet NSTableColumn		*sourcesNameColumn;
	IBOutlet NSTableColumn		*sourcesEnableColumn;
	
	IBOutlet NSTableView		*receiversTableView;
	IBOutlet NSTableColumn		*receiversNameColumn;
	IBOutlet NSTableColumn		*receiversEnableColumn;
	
	NSMutableArray				*msgArray;
	IBOutlet NSTextView			*receivedView;
	
	IBOutlet NSTextField		*channelField;
	IBOutlet NSTextField		*ctrlField;
    
    IBOutlet NSPopUpButton      *touchCtrlNumPopList;
    IBOutlet NSPopUpButton      *ledSourcePopList;
    IBOutlet NSPopUpButton      *ledCtrlNumPopList;
    IBOutlet NSPopUpButton      *pressureCtrlNumPopList;
    IBOutlet NSPopUpButton      *brightnessCtrlNumPopList;
    IBOutlet NSPopUpButton      *midiChannelPopList;
    IBOutlet NSPopUpButton      *touchOnOffCtrlNumPopList;
    
    NSMutableArray              *ctrlNumStringArray;
    NSInteger                   touchRouterCtrlNum;
    NSInteger                   ledSourceIndex;
    NSInteger                   ledCtrlNum;
    NSInteger                   pressureCtrlNum;
    NSInteger                   brightnessCtrlNum;
    NSInteger                   midiChannelNum;
    NSInteger                   touchOnOffCtrlNum;
    
}

- (IBAction) sliderMoved: (id) sender;
- (IBAction)upgrade_firmware:(id)sender;
- (IBAction)recalibrate:(id)sender;
- (IBAction)touchRouteCtrlNumChange: (id) sender;
- (IBAction)ledSourceChange: (id) sender;
- (IBAction)ledCtrlNumChange: (id) sender;
- (IBAction)presCtrlNumChange: (id) sender;
- (IBAction)brightnessCtrlNumChange: (id) sender;
- (IBAction)midiChanNumChange: (id) sender;
- (IBAction)touchOnOffCtrlNumChange: (id) sender;
- (IBAction)ignoreTouchChange: (id) sender;
- (IBAction)save_settings_button: (id) sender;

- (void)send_individual_light_control;

- (void)toggleAttachedWindowAtPoint:(NSPoint)pt;
- (void)drawStatusRect:(NSRect)rect High:(BOOL)hi;
- (void) buildOutputMenu;
- (IBAction)slider_brightness_moved:(id)sender;
- (IBAction)open_help:(id)sender;
- (void)send_column_to_USB:(int)v;
- (void) check_for_vmeter;
- (IBAction)upside_down_clicked:(id)sender;
- (IBAction)read_settings:(id)sender;

- (IBAction)load_defaults:(id)sender;
- (void) setupChanged;
- (void) receivedMIDI:(NSArray *)a fromNode:(VVMIDINode *)n;

- (IBAction) ctrlValSliderUsed:(id)sender;
- (void) sendCtrlMsg: (NSInteger)val;
- (void) sendNoteOnMsg: (NSInteger)pitch velocity:(NSInteger)vel;
- (void) sendNoteOffMsg: (NSInteger)pitch;
- (void) send_pressure_via_MIDI: (NSInteger)pres;
- (void) send_touch_on_off_via_MIDI: (NSInteger)val;
- (void) sendCtrlMsg:(NSInteger)val ctrl:(NSInteger)cNum;

- (void) send_column_height_MIDI: (NSInteger)val;

// use nonatomic for non-threaded environment, like this one?
// only need retain for objects.  NSInteger is just a typedef'd int.  assign is default?
@property (retain) IBOutlet NSTextField *debug_text_field;
@property (retain) IBOutlet NSTextField *debug_text_field2;
@property (retain) IBOutlet NSPopUpButton      *touchCtrlNumPopList;
@property (retain) IBOutlet NSPopUpButton      *ledSourcePopList;
@property (retain) IBOutlet NSPopUpButton      *ledCtrlNumPopList;
@property (retain) IBOutlet NSPopUpButton      *pressureCtrlNumPopList;
@property (retain) IBOutlet NSPopUpButton      *brightnessCtrlNumPopList;
@property (retain) IBOutlet NSPopUpButton      *touchOnOffCtrlNumPopList;
@property IBOutlet NSButton *touchOnOffToCtrlsCheckBox;


@property (retain) NSMutableArray *ctrlNumStringArray;
@property NSInteger touchRouterCtrlNum;
@property NSInteger ledSourceIndex;


@property NSInteger ledCtrlNum;
@property NSInteger pressureCtrlNum;
@property NSInteger brightnessCtrlNum;
@property NSInteger midiChannelNum;
@property NSInteger touchOnOffCtrlNum;

@property (retain) IBOutlet NSButton *touchToCtrlCheckBox;
@property IBOutlet NSButton *touchToVolCheckBox;
@property IBOutlet NSButton *touchToNoteCheckBox;
@property IBOutlet NSButton *pressureToCtrlCheckBox;
@property IBOutlet NSButton *ignoreTouchCheckBox;
@property IBOutlet NSButton *ledBrightnessFromCtrlCheckBox;
@property IBOutlet NSButton *rawSensorsToCtrlsCheckBox;

@property IBOutlet NSPopUpButton *midiChannelPopList;

//- (IBAction)inputDeviceSelected:(id)sender;
//- (IBAction)outputDeviceSelected:(id)sender;



@end
