#include "window.h"

FateWindowInfo get_fate_window_info() {
    FateWindowInfo result = { kCGNullWindowID, CGRectZero };
    CFArrayRef active_windows = CGWindowListCopyWindowInfo(kCGWindowListOptionAll, kCGNullWindowID);   
    if (!active_windows) {
        return result;
    }

    CFIndex count = CFArrayGetCount(active_windows);

    for (CFIndex i = 0; i < count; i++) {
        // Each element in the array is a CFDicationaryRef
        CFDictionaryRef window = (CFDictionaryRef)CFArrayGetValueAtIndex(active_windows, i);

        CFStringRef owner = (CFStringRef)CFDictionaryGetValue(window, kCGWindowOwnerName);
        CFStringRef name = (CFStringRef)CFDictionaryGetValue(window, kCGWindowName);

        if (owner && CFStringCompare(owner, CFSTR("fate.exe"), 0) == kCFCompareEqualTo) {
            CFDictionaryRef bounds_dict = (CFDictionaryRef)CFDictionaryGetValue(window, kCGWindowBounds);
            if (bounds_dict) {
                CGRect bounds;
                CGRectMakeWithDictionaryRepresentation(bounds_dict, &bounds);
                // keep the largest window
                if (bounds.size.width * bounds.size.height > result.bounds.size.width * result.bounds.size.height) {
                    CFNumberRef id_ref = (CFNumberRef)CFDictionaryGetValue(window, kCGWindowNumber);
                    if (id_ref) CFNumberGetValue(id_ref, kCFNumberIntType, &result.id);
                    result.bounds = bounds;
                }
            }

            CFNumberRef window_id_ref = (CFNumberRef)CFDictionaryGetValue(window, kCGWindowNumber);
            if (window_id_ref) {
                CFNumberGetValue(window_id_ref, kCFNumberIntType, &result.id);
            }
        }
    }

    CFRelease(active_windows);
    return result;
}