#define GLOBALVAR [VariableStore sharedInstance]

@interface VariableStore : NSObject
{
    // Place any "global" variables here
	@public
    BOOL clicked;
    int sys_vol;
    int brightness;
}

// message from which our instance is obtained
+ (VariableStore *)sharedInstance;
@end