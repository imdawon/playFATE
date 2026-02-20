#include "controls.h"
#include "window.h"
int main(void) {
    // your bot loop
    left_click_at(400, 300);
    send_key_down_up(0x00); // 'A'
    FateWindowInfo fate_window_info = get_fate_window_info();
    if (fate_window_info.id == kCGNullWindowID) {
        printf("FATE Window not found!\n");
        return -1;
    }
    printf("FATE Window ID: %d - FATE Window Bounds X,Y: %f,%f", fate_window_info.id, fate_window_info.bounds.origin.x, fate_window_info.bounds.origin.y);
    return 0;
}