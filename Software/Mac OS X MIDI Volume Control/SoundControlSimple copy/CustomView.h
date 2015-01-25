#import <Cocoa/Cocoa.h>

@class AppController;
@interface CustomView : NSView {
    __weak AppController *controller;
}

- (id)initWithFrame:(NSRect)frame controller:(AppController *)ctrlr;

@end
