/* Copyright 2023 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// External variable from ploopyco.c
extern bool is_drag_scroll;

// Custom keycode for our modified drag scroll
enum custom_keycodes {
    DRAG_SCROLL_CUSTOM = SAFE_RANGE,
};

// State variables
static bool vertical_scroll_mode = false;
static uint16_t drag_scroll_timer = 0;
static bool mode_changed_on_hold = false;

// Threshold in milliseconds to distinguish tap from hold
#define DRAG_SCROLL_TAP_TERM 200

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN4, KC_BTN5, DRAG_SCROLL_CUSTOM, KC_BTN2, KC_BTN1, KC_BTN3 )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DRAG_SCROLL_CUSTOM:
            if (record->event.pressed) {
                // Start timer on press
                drag_scroll_timer = timer_read();
                mode_changed_on_hold = false;
            } else {
                // On release, check if it was a tap or hold
                if (timer_elapsed(drag_scroll_timer) < DRAG_SCROLL_TAP_TERM && !mode_changed_on_hold) {
                    // It was a tap - toggle scroll mode on/off
                    is_drag_scroll = !is_drag_scroll;
                }
            }
            return false;
    }
    return true;
}

// Check for long press in matrix scan
void matrix_scan_user(void) {
    if (drag_scroll_timer > 0 &&
        timer_elapsed(drag_scroll_timer) >= DRAG_SCROLL_TAP_TERM &&
        !mode_changed_on_hold) {
        // Long press detected - toggle vertical/omni mode
        vertical_scroll_mode = !vertical_scroll_mode;
        mode_changed_on_hold = true;
    }
}

// Override the pointing device task to filter horizontal scrolling
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (vertical_scroll_mode && is_drag_scroll) {
        // When vertical scroll mode is active, zero out horizontal scroll
        mouse_report.h = 0;
    }
    return mouse_report;
}
