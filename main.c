#include "controls.h"
#include "window.h"
#include "screen.h"

int main(void) {
    FateWindowInfo fw = get_fate_window_info();
    if (fw.id == kCGNullWindowID) {
        printf("FATE window not found\n");
        return -1;
    }

    printf("Window ID: %u  bounds: x=%.1f y=%.1f w=%.1f h=%.1f\n",
           fw.id,
           fw.bounds.origin.x, fw.bounds.origin.y,
           fw.bounds.size.width, fw.bounds.size.height);

    float hp = get_health_percent(fw);
    printf("Health: %.1f%%\n", hp * 100.0f);

    return 0;
}