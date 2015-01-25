/*
 
 VMeter Computer-side code. 7/16/2011
 VMeter.net
 
 Copyright (c) 2011 Curious Inventor, LLC
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
 */

#import "CustomView.h"
#import "AppController.h"
#import "VariableStore.h"


@implementation CustomView


- (id)initWithFrame:(NSRect)frame controller:(AppController *)ctrlr
{
    if ((self = [super initWithFrame:frame])) {
        controller = ctrlr; // deliberately weak reference.
    }
    
    return self;
}


- (void)dealloc
{
    controller = nil;
    [super dealloc];
}


- (void)drawRect:(NSRect)rect {
    // Draw background if appropriate.
	[controller drawStatusRect:rect High:GLOBALVAR->clicked];
    
    // Draw some text, just to show how it's done.
	NSPoint where;
	where.x = ((rect.size.width / 2) - 8);
	where.y = ((rect.size.height / 2) - 7);
    if (GLOBALVAR->clicked) {
		[[NSImage imageNamed:@"toolbar-icon-2.png"] drawAtPoint:where fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:1];
	} else {
		[[NSImage imageNamed:@"toolbar-icon.png"] drawAtPoint:where fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:1];
	}

}


- (void)mouseDown:(NSEvent *)event
{
    NSRect frame = [[self window] frame];
    NSPoint pt = NSMakePoint(NSMidX(frame), NSMinY(frame));
    [controller toggleAttachedWindowAtPoint:pt];
    GLOBALVAR->clicked = !GLOBALVAR->clicked;
    [self setNeedsDisplay:YES];
}


@end
