/* ... (Copyright/Includes) ... */
#include QMK_KEYBOARD_H

// External variable from ploopyco.c
extern bool is_drag_scroll;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( MS_BTN4, MS_BTN5, DRAG_SCROLL, MS_BTN2, MS_BTN1, MS_BTN3 )
};

// Override the pointing device task to filter horizontal scrolling
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (is_drag_scroll) {
        // *** THE NEW FIX: Clear the raw horizontal movement (x) ***
        // This is the input that ploopyco.c uses to calculate mouse_report.h.
        // By zeroing it here, ploopyco.c will calculate horizontal scroll as zero.
        mouse_report.x = 0;

        // You can leave mouse_report.h = 0; for completeness, but clearing x is key now.
    }
    return mouse_report;
}
