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
