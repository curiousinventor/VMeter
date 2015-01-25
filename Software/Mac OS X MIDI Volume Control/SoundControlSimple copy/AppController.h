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
}

- (IBAction) sliderMoved: (id) sender;
- (IBAction)upgrade_firmware:(id)sender;
- (IBAction)recalibrate:(id)sender;
- (void)toggleAttachedWindowAtPoint:(NSPoint)pt;
- (void)drawStatusRect:(NSRect)rect High:(BOOL)hi;
- (void) buildOutputMenu;
- (IBAction)slider_brightness_moved:(id)sender;
- (IBAction)open_help:(id)sender;
- (void)send_vol_to_USB:(int)v;
- (void) check_for_vmeter;
- (IBAction)upside_down_clicked:(id)sender;

- (void) setupChanged;
- (void) receivedMIDI:(NSArray *)a fromNode:(VVMIDINode *)n;

- (IBAction) ctrlValSliderUsed:(id)sender;

@property (nonatomic, retain) IBOutlet NSTextField *debug_text_field;
@property (nonatomic, retain) IBOutlet NSTextField *debug_text_field2;
//- (IBAction)inputDeviceSelected:(id)sender;
//- (IBAction)outputDeviceSelected:(id)sender;



@end
