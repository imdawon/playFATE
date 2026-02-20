#ifndef WINDOW_H
#define WINDOW_H

#include <CoreGraphics/CoreGraphics.h>

typedef struct {
    CGWindowID id;
    CGRect bounds;
} FateWindowInfo;

FateWindowInfo get_fate_window_info();

#endif