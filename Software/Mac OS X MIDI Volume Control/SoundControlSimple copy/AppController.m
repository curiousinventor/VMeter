#import "AppController.h"
#import "CustomView.h"
#import "MAAttachedWindow.h"
#import "VariableStore.h"
#import "SoundC.h"
#include "hidapi.h"




#define WCHAR_ENCODING NSUTF32LittleEndianStringEncoding
#define VENDOR_ID 0x03eb
#define DEVICE_ID 0x1234


@implementation AppController

@synthesize debug_text_field;
@synthesize debug_text_field2;

hid_device *handle;
int usb_out_counter;
unsigned long my_timer = 0;

- (IBAction) sliderMoved: (id) sender {
	[[SoundC sharedInstance] setVolume:[slider floatValue]];
    //[self send_vol_to_USB:((int)([slider floatValue]*2.55))]; 
}



-(void) check_for_vmeter {
    wchar_t mystring[256];
    float v;
    if (!handle) {
        handle = hid_open(VENDOR_ID, DEVICE_ID, NULL);
        if (handle) {
            hid_set_nonblocking(handle, 1);
            hid_get_product_string(handle, mystring, 255);
            //NSString *temp_string = [NSString 
           // NSString *temp_string = [NSString  stringWithFormat:@"Connected: Found %s", mystring];
         //   NSString *temp_string = [NSString stringWithCString:mystring encoding:NSUTF32LittleEndianStringEncoding];
          //  NSString * temp_string2 = [NSString  stringWithFormat:@"Connected: Found %@", temp_string];
           
             
            
            NSString * temp_string = [[NSString alloc] initWithBytes:mystring length:(80) encoding:WCHAR_ENCODING];
            NSString * temp_string2 = [NSString stringWithFormat:@"Connected: Found %@", temp_string];
            
            
            
             [debug_text_field setStringValue:temp_string2]; 
            
            // now set the lights to the current volume level
            v = [[SoundC sharedInstance] getVolume];           
            if ((v >= 0) && (v<=100)) {                 
                [self send_vol_to_USB:(v*2.55)];                
            }	
        }    
        else {  
            NSString *temp_string = [NSString stringWithFormat:@"Please connect VMeter"];
            [debug_text_field setStringValue:temp_string]; 
        }
    }
    else { // we have a handle, make sure it's still valid
        struct hid_device_info *hd = hid_enumerate(VENDOR_ID, DEVICE_ID);
        if (hd == NULL) {
            handle = NULL;
        }
    }
}

- (IBAction)upside_down_clicked:(id)sender {
    
    int return_val;
    unsigned char buf[17];
    unsigned char but_state;
    float v;
    
    
    if ([upside_down state] == NSOffState) {
        but_state = 0;
    }
    else {
        but_state = 1;
    }
  //  NSLog(@"Upside Down: %u", but_state);
   
 
    buf[0] = 0;
    buf[1] = 252; // this means buf[2] has the volume, 0-255
    buf[2] = but_state;
    buf[3] = GLOBALVAR->brightness;
    [self check_for_vmeter];
    if (handle) {
        return_val = hid_write(handle, buf, 9);
    }
    v = [[SoundC sharedInstance] getVolume];
   
    if ((v >= 0) && (v<=100)) {
      [slider setFloatValue:v];  
      [self send_vol_to_USB:(v*2.55)];      
    }	      
}
- (IBAction)upgrade_firmware:(id)sender {
    int return_val;
    unsigned char buf[17];    
    buf[0] = 0;
    buf[1] = 254; 
    buf[2] = 0;
    buf[3] = GLOBALVAR->brightness;
    [self check_for_vmeter];
    if (handle) {
        return_val = hid_write(handle, buf, 9);
    }
   
}

- (IBAction)recalibrate:(id)sender {
    int return_val;
    unsigned char buf[17];    
    buf[0] = 0;
    buf[1] = 253; 
    buf[2] = 0;
    buf[3] = GLOBALVAR->brightness;
    [self check_for_vmeter];
    if (handle) {
        return_val = hid_write(handle, buf, 9);
    }
}

-(void) onTimer: (NSTimer *)timer{
	int res, res2, i;
    unsigned char buf[17];
    unsigned char buf2[17];
    float temp, temp2;
    char ok_to_send_vol = 1;
    // if VMeter touched, set volume
    float v;
    
 //   NSLog(@"time");

    [self check_for_vmeter];
   if (handle) {       
       do { // since the mac could be buffering older messages, read them in until we get to 
           // the latest one, effectively throwing out the older ones.   
           [self check_for_vmeter];
           res2 = hid_read(handle, (unsigned char *)buf2, sizeof(buf2));
           if (res2==16) { // valid, copy to buf
               for (i=0;i<16;i++) {
                   buf[i] = buf2[i];                   
               }  
               res = res2;
           }           
       }
       while(res2 == 16);
       if (res2 == -1) {
			handle = NULL;
            return;
		}     
       if (res == 16) { // packet is valid
         //  NSString *temp_string = [NSString stringWithFormat:@"res: %02d, buf[0]: %02x, buf[12]: %03d", res,buf[0],buf[12]];
          // [debug_text_field setStringValue:temp_string];

           
			if (buf[0] == 1 || buf[0] == 3) { // touch is active
				temp = (float)((unsigned char)buf[12]) / 255.0 * 100.0;
				[slider setFloatValue:temp];
                GLOBALVAR->sys_vol = temp;
                temp2 = [slider floatValue];
                [[SoundC sharedInstance] setVolume:[slider floatValue]];
                ok_to_send_vol = 0;
               // NSLog(@"touch");
			}// end if touched
			else { // no touch deteced
                // else get the system volume
                v = [[SoundC sharedInstance] getVolume];
                if (v != GLOBALVAR->sys_vol) {
                    GLOBALVAR->sys_vol = v;
                    if ((v >= 0) && (v<=100)) {
                        [slider setFloatValue:v];  
                        [self send_vol_to_USB:(v*2.55)];
                    
                    }	
                }
			}		
		} // 16 sized packet (valid)	           
    }
    else { // no VMeter connected        
        float v = [[SoundC sharedInstance] getVolume];
        if ((v >= 0) && (v<=100)) {
            [slider setFloatValue:v];                              
        }	
       
    }
 
}



// expects 0 to 255                            
- (void)send_vol_to_USB:(int)v
{
    int return_val;
    unsigned char buf[17];
//    if (handle) {
//        do {
//            return_val = hid_read(handle, (unsigned char *)buf, sizeof(buf));
//        }
//        while(return_val != 0);
//    }
    
//    NSLog(@"sending data: %d, return val: %d", v, return_val);
    buf[0] = 0;
    buf[1] = 1; // this means buf[2] has the volume, 0-255
    buf[2] = (unsigned char)(v);
    buf[3] = GLOBALVAR->brightness;
//    usb_out_counter++;
//    if (usb_out_counter > 0) {
//        usb_out_counter = 0;
        if (handle) {
            return_val = hid_write(handle, buf, 9);
        }
//    }

    NSString *temp_string = [NSString stringWithFormat:@"ret: %02d", return_val];
   // [debug_text_field2 setStringValue:temp_string];
    
}

- (void)awakeFromNib
{

    // Create an NSStatusItem.
    float height = [[NSStatusBar systemStatusBar] thickness];
	float width = 30;
    NSRect viewFrame = NSMakeRect(0, 0, width, height);
    statusItem = [[[NSStatusBar systemStatusBar] statusItemWithLength:width] retain];
    [statusItem setToolTip:@"VMeter: click to configure"];
//    handle = hid_open(0x03eb, 0x1234, NULL);
//    if (handle) {
//        hid_set_nonblocking(handle, 1);
//    } 
    // myTimer = ...
    //[[NSRunLoop currentRunLoop] addTimer:myTimer  forMode:SEventTrackingRunLoopMod]; 
    // http://www.cocoabuilder.com/archive/cocoa/69075-nstimer-problem.html
    
    NSTimer * myTimer;
    myTimer = [NSTimer scheduledTimerWithTimeInterval:0.010 target:self selector:@selector(onTimer:) userInfo:NULL repeats: YES];
    [[NSRunLoop currentRunLoop] addTimer:myTimer  forMode:NSEventTrackingRunLoopMode]; 
	[[SoundC sharedInstance] enumerateSoundDevices];
	//[self buildInputMenu];
	[self buildOutputMenu];
  
    [debug_text_field setStringValue:@""];
    GLOBALVAR->brightness = 15;
    
    [statusItem setView:[[[CustomView alloc] initWithFrame:viewFrame controller:self] autorelease]];
   
	
	[NSApp setDelegate:self];
}

- (void)dealloc
{
    [[NSStatusBar systemStatusBar] removeStatusItem:statusItem];
    [midiManager release];
    [super dealloc];
}

- (void)applicationDidResignActive:(NSNotification *)aNotification {
	GLOBALVAR->clicked = NO;
    [attachedWindow orderOut:self];
	[[statusItem view] setNeedsDisplay:YES];
}


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

// ------------------------ original code from AppDelegate.m
/*
- (IBAction) sliderMoved: (id) sender {
	[[SoundC sharedInstance] setVolume:[slider floatValue]];
}
*/

/*
-(void) onTimer: (NSTimer *)timer{
	float v = [[SoundC sharedInstance] getVolume];
	//OUTPUT VOLUME
	if ((v >= 0) && (v<=100)) {
		[slider setFloatValue:v];
	}
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
*/
 
/*
- (IBAction)inputDeviceSelected:(id)sender {
	[[SoundC sharedInstance] setDeviceNamed:[mInputDevices titleOfSelectedItem] Type:kAudioTypeInput];
}

- (IBAction)outputDeviceSelected:(id)sender {
	[[SoundC sharedInstance] setDeviceNamed:[mOutputDevices titleOfSelectedItem] Type:kAudioTypeOutput];	
}
*/



/*
- (void) buildInputMenu {
	[mInputDevices removeAllItems];
	if ([SoundC sharedInstance]->lastDevInput < 0) return;
	for (int i = 0; i <= [SoundC sharedInstance]->lastDevInput; i++) {
		NSString *mName = [NSString stringWithCString:&[SoundC sharedInstance]->devInput[i].mName[0] encoding:NSASCIIStringEncoding];
		[mInputDevices addItemWithTitle:mName];
	}
}
*/
- (void) buildOutputMenu {
	[mOutputDevices removeAllItems];
	if ([SoundC sharedInstance]->lastDevOutput < 0) return;
	for (int i = 0; i <= [SoundC sharedInstance]->lastDevOutput; i++) {
		NSString *mName = [NSString stringWithCString:&[SoundC sharedInstance]->devOutput[i].mName[0] encoding:NSASCIIStringEncoding];
		[mOutputDevices addItemWithTitle:mName];
	}
}

- (IBAction)slider_brightness_moved:(id)sender {
    char buf[17];
    float v;
    unsigned char b;
    int return_val;
    v = [[SoundC sharedInstance] getVolume];
    

    b = (unsigned char)[slider_brightness floatValue];
   // NSLog(@"New Brightness: %u", b);
    NSString * temp = [NSString stringWithFormat:@"%f", [slider_brightness floatValue]];
   // [debug_text_field2 setStringValue:temp];   
     GLOBALVAR->brightness = b;
    
    buf[0] = 0;
    buf[1] = 1; // this means buf[2] has the volume, 0-255
    buf[2] = (unsigned char)(v*2.55);
    buf[3] = GLOBALVAR->brightness;
    
    [self check_for_vmeter];
    if (handle) {
        return_val = hid_write(handle, buf, 9);
    }
   
}

- (IBAction)open_help:(id)sender {
    
    NSWorkspace * ws = [NSWorkspace sharedWorkspace];
    NSURL *url = [NSURL URLWithString: @"http://vmeter.net/instructions"];
    [ws openURL:url];
}
/*
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// Insert code here to initialize your application
	[NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(onTimer:) userInfo:NULL repeats: YES];
	[[SoundC sharedInstance] enumerateSoundDevices];
	//[self buildInputMenu];
	[self buildOutputMenu];
}
*/


// MIDI Stuff below:


- (id) init	{
	NSLog(@"AppController:init:");
	self = [super init];
	msgArray = [[NSMutableArray arrayWithCapacity:0] retain];
    midiManager = [[VVMIDIManager alloc] init];
    [midiManager setDelegate:self];
	return self;
}

- (void) setupChanged	{
	NSLog(@"AppController:setupChanged:");
	[sourcesTableView reloadData];
	[receiversTableView reloadData];
}
- (void) receivedMIDI:(NSArray *)a fromNode:(VVMIDINode *)n	{
	NSLog(@"AppController:receivedMIDI:");
	NSEnumerator		*it = [a objectEnumerator];
	VVMIDIMessage		*msgPtr;
	NSMutableString		*mutString = [NSMutableString stringWithCapacity:0];
	
	//	technically, the @synchronized protocol isn't very fast- but hey, this is just a demo app...
	@synchronized(msgArray)	{
		//	add the received messages to the array
		while ((msgPtr = [it nextObject]))
			[msgArray addObject:[msgPtr lengthyDescription]];
		//	make sure the array only has 10 objects
		while ([msgArray count] > 10)
			[msgArray removeObjectAtIndex:0];
		//	assemble a string from the msg array
		it = [msgArray reverseObjectEnumerator];
		while ((msgPtr = [it nextObject]))
			[mutString appendFormat:@"%@\n",msgPtr];
		//	push the string to the view
		[receivedView performSelectorOnMainThread:@selector(setString:) withObject:[[mutString copy] autorelease] waitUntilDone:NO];
	}
}

- (IBAction) ctrlValSliderUsed:(id)sender	{
	NSLog(@"AppController:ctrlValSliderUsed:");
	VVMIDIMessage		*msg = nil;
	
	//	create a message
	msg = [VVMIDIMessage createFromVals:
                 VVMIDIControlChangeVal:
           [channelField intValue]:
           [ctrlField intValue]:
           floor(127*[sender floatValue])];
	
	/*
     NSArray		*tmpArray = [NSArray arrayWithObjects:[NSNumber numberWithUnsignedChar:0x7F],[NSNumber numberWithUnsignedChar:0x7E],nil];
     msg = [VVMIDIMessage createWithSysexArray:tmpArray];
     */
	//	tell the midi manager to send it
	if (msg != nil)
		[midiManager sendMsg:msg];
}

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



@end
