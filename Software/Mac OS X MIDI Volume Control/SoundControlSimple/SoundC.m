#include "SoundC.h"

//SINGLETON INITIALIZATION
@implementation SoundC
+ (SoundC *)sharedInstance
{
    // the instance of this class is stored here
    static SoundC *myInstance = nil;
	
    // check to see if an instance already exists
    if (nil == myInstance) {
        myInstance  = [[[self class] alloc] init];
        // initialize variables here
		myInstance->lastDevInput = -1;
		myInstance->lastDevOutput = -1;
    }
    // return the instance of this class
    return myInstance;
}

// returns kAudioTypeInput or kAudioTypeOutput
ASDeviceType getDeviceType(AudioDeviceID deviceID) {
	UInt32 propertySize = 256;
	
	// if there are any output streams, then it is an output
	AudioDeviceGetPropertyInfo(deviceID, 0, false, kAudioDevicePropertyStreams, &propertySize, NULL);
	if (propertySize > 0) return kAudioTypeOutput;
	
	// if there are any input streams, then it is an input
	AudioDeviceGetPropertyInfo(deviceID, 0, true, kAudioDevicePropertyStreams, &propertySize, NULL);
	if (propertySize > 0) return kAudioTypeInput;
	
	return kAudioTypeUnknown;
}

// fills deviceName
void getDeviceName(AudioDeviceID deviceID, char * deviceName) {
	UInt32 propertySize = 256;
	AudioDeviceGetProperty(deviceID, 0, false, kAudioDevicePropertyDeviceName, &propertySize, deviceName);  
}

//FILLS STRUCTURES, AS DEFINED IN .h
- (void)enumerateSoundDevices {
	self->lastDevInput = -1;
	self->lastDevOutput = -1;
	
	UInt32 propertySize;
	AudioDeviceID dev_array[64];
	int numberOfDevices = 0;
	ASDeviceType device_type;
	char deviceName[200];
	
	AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices, &propertySize, NULL);
	
	AudioHardwareGetProperty(kAudioHardwarePropertyDevices, &propertySize, dev_array);
	numberOfDevices = (propertySize / sizeof(AudioDeviceID));
	
	for(int i = 0; i < numberOfDevices; ++i) {
		device_type = getDeviceType(dev_array[i]);
		getDeviceName(dev_array[i], deviceName);
		switch(device_type) {
			case kAudioTypeInput:
				strcpy(devInput[++lastDevInput].mName,deviceName);
				NSLog(@"INPUT %s %s",deviceName, devInput[lastDevInput].mName);
				break;
			case kAudioTypeOutput:
				strcpy(devOutput[++lastDevOutput].mName,deviceName);
				NSLog(@"OUTPUT %s %s",deviceName, devOutput[lastDevOutput].mName);
				break;
			case kAudioTypeSystemOutput:
				if (device_type != kAudioTypeOutput) continue;
				break;
			default:
				break;
		}
		//NSLog(@"%s",deviceName);
	}
}

AudioDeviceID getCurrentlySelectedDeviceID(ASDeviceType typeRequested) {
	UInt32 propertySize;
	AudioDeviceID deviceID = kAudioDeviceUnknown;
	
	// get the default output device
	propertySize = sizeof(deviceID);
	switch(typeRequested) {
		case kAudioTypeInput: 
			verify_noerr(AudioHardwareGetProperty(kAudioHardwarePropertyDefaultInputDevice, &propertySize, &deviceID));
			break;
		case kAudioTypeOutput:
			AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice, &propertySize, &deviceID);
			break;
		case kAudioTypeSystemOutput:
			AudioHardwareGetProperty(kAudioHardwarePropertyDefaultSystemOutputDevice, &propertySize, &deviceID);
			break;
		default:
			break;			
	}	
	return deviceID;
}

- (void) getSelectedDevices {
	//input device
	AudioDeviceID idInput = getCurrentlySelectedDeviceID(kAudioTypeInput);
	getDeviceName(idInput, selectedInputDevice);
	//output device
	AudioDeviceID idOutput = getCurrentlySelectedDeviceID(kAudioTypeOutput);
	getDeviceName(idOutput, selectedOutputDevice);
}

AudioDeviceID getRequestedDeviceID(const char * requestedDeviceName, ASDeviceType typeRequested) {
	UInt32 propertySize;
	AudioDeviceID dev_array[64];
	int numberOfDevices = 0;
	char deviceName[256];
	
	AudioHardwareGetPropertyInfo(kAudioHardwarePropertyDevices, &propertySize, NULL);
	// printf("propertySize=%d\n",propertySize);
	
	AudioHardwareGetProperty(kAudioHardwarePropertyDevices, &propertySize, dev_array);
	numberOfDevices = (propertySize / sizeof(AudioDeviceID));
	// printf("numberOfDevices=%d\n",numberOfDevices);
	
	for(int i = 0; i < numberOfDevices; ++i) {
		switch(typeRequested) {
			case kAudioTypeInput:
			case kAudioTypeOutput:
				if (getDeviceType(dev_array[i]) != typeRequested) continue;
				break;
			case kAudioTypeSystemOutput:
				if (getDeviceType(dev_array[i]) != kAudioTypeOutput) continue;
				break;
			default:
				break;
		}
		
		getDeviceName(dev_array[i], deviceName);
		// printf("For device %d, id = %d and name is %s\n",i,dev_array[i],deviceName);
		if (strcmp(requestedDeviceName, deviceName) == 0) {
			return dev_array[i];
		}
	}
	
	return kAudioDeviceUnknown;
}

void setDevice(AudioDeviceID newDeviceID, ASDeviceType typeRequested) {
	UInt32 propertySize = sizeof(UInt32);
	
	switch(typeRequested) {
		case kAudioTypeInput: 
			AudioHardwareSetProperty(kAudioHardwarePropertyDefaultInputDevice, propertySize, &newDeviceID);
			break;
		case kAudioTypeOutput:
			AudioHardwareSetProperty(kAudioHardwarePropertyDefaultOutputDevice, propertySize, &newDeviceID);
			break;
		case kAudioTypeSystemOutput:
			AudioHardwareSetProperty(kAudioHardwarePropertyDefaultSystemOutputDevice, propertySize, &newDeviceID);
			break;
		default:
			break;
	}
	
}

- (void) setDeviceNamed:(NSString *)deviceName Type:(ASDeviceType)type {
	AudioDeviceID deviceID = getRequestedDeviceID([deviceName cStringUsingEncoding:NSASCIIStringEncoding], type);
	setDevice(deviceID, type);
}

//FUNCTION SECURELY CALLS APPLE SCRIPT
- (NSString *)executeAppleScript:(NSString *)cmd {
	NSString *resultStr = @"";
	NSAppleScript *script = [[NSAppleScript allocWithZone:[self zone]] initWithSource:cmd];
	if (script) {
		NSDictionary *errorInfo;
		NSAppleEventDescriptor *result = [script executeAndReturnError:&errorInfo];
		if (result) {
			resultStr = [result stringValue];
        }
		[script release];
	}
    return resultStr;
}

//SETS VOLUME, AS DEFINED IN .h
- (void) setVolume:(float)vol {
	[self executeAppleScript:[NSString stringWithFormat:@"set volume output volume %f",vol]];
}

//GETS VOLUME, AS DEFINED IN .h
- (float) getVolume {
	return [[self executeAppleScript:[NSString stringWithFormat:@"output volume of (get volume settings)"]] floatValue];
}

@end