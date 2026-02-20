#ifndef SCREEN_H
#define SCREEN_H

#include <CoreGraphics/CoreGraphics.h>
#include "window.h"

float get_health_percent(FateWindowInfo window);
static float analyze_health_image(CGImageRef image);

#endif