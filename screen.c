// 640 x 508 is the default
// 800 x 628 is also supported
// 1024 x 768 is also supported
// any other aspect ratio will not work!
#include "screen.h"
#include <stdio.h>
#include <ImageIO/ImageIO.h>

#define HEALTH_X   139
#define HEALTH_Y   -8
#define HEALTH_W   80
#define HEALTH_H   16

#define RED_R_MIN  150
#define RED_G_MAX  80
#define RED_B_MAX  80

static void save_debug_image(CGImageRef image, const char *path) {
    CFStringRef cf_path = CFStringCreateWithCString(NULL, path, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, cf_path, kCFURLPOSIXPathStyle, false);
    CGImageDestinationRef dest = CGImageDestinationCreateWithURL(url, CFSTR("public.png"), 1, NULL);
    if (dest) {
        CGImageDestinationAddImage(dest, image, NULL);
        CGImageDestinationFinalize(dest);
        CFRelease(dest);
    }
    CFRelease(url);
    CFRelease(cf_path);
}


static float analyze_health_image(CGImageRef image) {
    if (!image) return -1.0f;

    CFDataRef data = CGDataProviderCopyData(CGImageGetDataProvider(image));
    if (!data) return -1.0f;

    const uint8_t *pixels = CFDataGetBytePtr(data);
    size_t width          = CGImageGetWidth(image);
    size_t height         = CGImageGetHeight(image);
    size_t bytes_per_row  = CGImageGetBytesPerRow(image);
    size_t bytes_per_px   = CGImageGetBitsPerPixel(image) / 8;

    // scan the middle row only — avoids border pixels top/bottom
    size_t mid_y = height / 2;
    int last_red_x = 0;
    int first_red_x = -1;

    for (size_t x = 0; x < width; x++) {
        const uint8_t *px = pixels + mid_y * bytes_per_row + x * bytes_per_px;
        uint8_t b = px[0], g = px[1], r = px[2];
        if (r >= RED_R_MIN && g <= RED_G_MAX && b <= RED_B_MAX) {
            if (first_red_x == -1) first_red_x = (int)x;
            last_red_x = (int)x;
        }
    }

    CFRelease(data);

    if (first_red_x == -1) {
        printf("screen: no red pixels found in middle row\n");
        return 0.0f;
    }

    // health = how far the red bar extends across the inner bar width
    int bar_start = first_red_x;
    int bar_end   = (int)width - first_red_x; // mirror — right border same width as left
    int red_width = last_red_x - first_red_x;
    int full_width = bar_end - bar_start;

    float percent = (float)red_width / (float)full_width;
    printf("screen: health = %.1f%% (red x: %d-%d of %d-%d)\n",
           percent * 100.f, first_red_x, last_red_x, bar_start, bar_end);

    return percent;
}

float get_health_percent(FateWindowInfo window) {
    float scale = window.bounds.size.width / 640.0f;
    // 480 - HEALTH_Y = 488 (the Y offset at 640x480 base resolution)
    // 28px macOS title bar is fixed, only the game portion (460px) scales
    float base_game_y = (480 - HEALTH_Y) - 28;  // 460
    CGRect health_rect = CGRectMake(
        window.bounds.origin.x + (int)(HEALTH_X * scale),
        window.bounds.origin.y + 28 + (int)(base_game_y * scale),
        (int)(HEALTH_W * scale),
        (int)(HEALTH_H * scale)
    );

    CGImageRef image = CGWindowListCreateImage(
        health_rect,
        kCGWindowListOptionAll,
        kCGNullWindowID,
        kCGWindowImageDefault
    );

    if (!image) {
        printf("screen: failed to capture health bar region\n");
        return -1.0f;
    }

    printf("screen: capturing rect x=%.0f y=%.0f w=%.0f h=%.0f (scale=%.2f)\n",
           health_rect.origin.x, health_rect.origin.y,
           health_rect.size.width, health_rect.size.height, scale);

    save_debug_image(image, "/tmp/health_debug.png");

    float result = analyze_health_image(image);
    CGImageRelease(image);
    return result;
}