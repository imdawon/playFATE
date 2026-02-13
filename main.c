#include "controls.h"

int main(void) {
    // your bot loop
    left_click_at(400, 300);
    send_key_down_up(0x00); // 'A'
    return 0;
}