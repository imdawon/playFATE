#include "controls.h"
#include <unistd.h>

#define KEY_DELAY_US   10000   // 10ms between down/up
#define CLICK_DELAY_US 20000   // 20ms between down/up

void send_key_down(CGKeyCode key) {
    CGEventRef e = CGEventCreateKeyboardEvent(NULL, key, true);
    if (e) {
        CGEventPost(kCGHIDEventTap, e);
        CFRelease(e);
    }
}

void send_key_up(CGKeyCode key) {
    CGEventRef e = CGEventCreateKeyboardEvent(NULL, key, false);
    if (e) {
        CGEventPost(kCGHIDEventTap, e);
        CFRelease(e);
    }
}

void send_key_down_up(CGKeyCode key) {
    send_key_down(key);
    usleep(KEY_DELAY_US);
    send_key_up(key);
}

void send_key_with_command(CGKeyCode key) {
    CGEventRef down = CGEventCreateKeyboardEvent(NULL, key, true);
    if (!down) return;
    CGEventSetFlags(down, kCGEventFlagMaskCommand);
    CGEventPost(kCGHIDEventTap, down);
    CFRelease(down);

    usleep(KEY_DELAY_US);

    CGEventRef up = CGEventCreateKeyboardEvent(NULL, key, false);
    if (up) {
        CGEventPost(kCGHIDEventTap, up);
        CFRelease(up);
    }
}

void move_mouse(CGFloat x, CGFloat y) {
    CGEventRef e = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved,
    CGPointMake(x, y), kCGMouseButtonLeft);
    if (e) {
        CGEventPost(kCGHIDEventTap, e);
        CFRelease(e);
    }
}

void left_click_at(CGFloat x, CGFloat y) {
    CGPoint pos = CGPointMake(x, y);
    CGEventRef down = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown,
    pos, kCGMouseButtonLeft);
    if (!down) return;
    CGEventPost(kCGHIDEventTap, down);
    CFRelease(down);

    usleep(CLICK_DELAY_US);

    CGEventRef up = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp,
    pos, kCGMouseButtonLeft);
    if (up) {
        CGEventPost(kCGHIDEventTap, up);
        CFRelease(up);
    }
}

void right_click_at(CGFloat x, CGFloat y) {
    CGPoint pos = CGPointMake(x, y);
    CGEventRef down = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown,
    pos, kCGMouseButtonRight);
    if (!down) return;
    CGEventPost(kCGHIDEventTap, down);
    CFRelease(down);

    usleep(CLICK_DELAY_US);

    CGEventRef up = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp,
    pos, kCGMouseButtonRight);
    if (up) {
        CGEventPost(kCGHIDEventTap, up);
        CFRelease(up);
    }
}