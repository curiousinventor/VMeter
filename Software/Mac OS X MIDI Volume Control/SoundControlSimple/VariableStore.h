/*
 
 VMeter Computer-side code. 7/16/2011
 VMeter.net
 
 Copyright (c) 2011 Curious Inventor, LLC
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#define GLOBALVAR [VariableStore sharedInstance]

@interface VariableStore : NSObject
{
    // Place any "global" variables here
	@public
    BOOL clicked;
    int last_led_setpoint; // 0 to 255
    int last_vol_slider_setpoint;
    int last_system_volume;
    int brightness; // 0 to 16
    unsigned char settings_packet[30];
    BOOL touch_is_active;
    BOOL ignore_touch;
}

// message from which our instance is obtained
+ (VariableStore *)sharedInstance;
@end