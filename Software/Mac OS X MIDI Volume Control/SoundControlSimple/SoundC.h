#include <CoreServices/CoreServices.h>
#include <CoreAudio/CoreAudio.h>

typedef enum { 
  	kAudioTypeUnknown = 0, 
    kAudioTypeInput   = 1, 
    kAudioTypeOutput  = 2,
    kAudioTypeSystemOutput = 3
} ASDeviceType;

struct Device {
	char			mName[200];
};

@interface SoundC : NSObject
{
//global variables if we need any
@public
	struct Device	devInput[20];
	struct Device	devOutput[20];
	char			selectedInputDevice[200];
	char			selectedOutputDevice[200];
  //  NSAppleScript *script;
	int lastDevInput;
	int lastDevOutput;
}

//@property (retain) NSAppleScript *script;
// message from which our instance is obtained
+ (SoundC *)sharedInstance;

// INITIALIZATION:
// In order to use SoundC object please include SoundC.h in any module where you want to use it

// setVolume function 
// - does not return any value
// - sets volume of the current autio output
// - call from any module as [[SoundC sharedInstance] setVolume:<float value>];
// - <float value> can range from 0.0 to 100.0
- (void) setVolume:(float)vol;

// getVolume function
// - returns current volume values as float
// - no parameters
// - call from any module as float currentVolume = [[SoundC sharedInstance] getVolume];
// - currentVolume can have value between 0.0 and 100.0
- (float) getVolume;

// enumerateSoundDevices function
// - fills both devInput and devOutput structures
// - lastDevInput and lastDevOutput can be used in for loops like for(i=0;i<=[SoundC sharedInstance]->lastDevInput;i++)
- (void)enumerateSoundDevices;

// getSelectedDevices function
// fills selectedInputDevice and selectedOutputDevice strings
- (void) getSelectedDevices;

// setDeviceNamed: Type: function
// set input: [[SoundC sharedInstance] setDeviceNamed:<NSString deviceName> Type:kAudioTypeInput];
// set output: [[SoundC sharedInstance] setDeviceNamed:<NSString deviceName> Type:kAudioTypeOutput];
- (void) setDeviceNamed:(NSString *)deviceName Type:(ASDeviceType)type;

@end