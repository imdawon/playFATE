#ifndef CONTROLS_H
#define CONTROLS_H

#include <CoreGraphics/CoreGraphics.h>

void send_key_down(CGKeyCode key);
void send_key_up(CGKeyCode key);
void send_key_down_up(CGKeyCode key);
void send_key_with_command(CGKeyCode key);
void move_mouse(CGFloat x, CGFloat y);
void left_click_at(CGFloat x, CGFloat y);
void right_click_at(CGFloat x, CGFloat y);

#endif