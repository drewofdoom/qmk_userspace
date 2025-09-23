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

// Custom keycode for vertical-only scrolling
enum custom_keycodes {
    SCROLL_VERT = SAFE_RANGE,
};

// State variable to track vertical scroll mode
static bool vertical_scroll_mode = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Replace one of your buttons with SCROLL_VERT
    [0] = LAYOUT( KC_BTN5, SCROLL_VERT, DRAG_SCROLL, KC_BTN2, KC_BTN1, KC_BTN3 )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SCROLL_VERT:
            if (record->event.pressed) {
                // Toggle mode: press once to enable, press again to disable
                vertical_scroll_mode = !vertical_scroll_mode;
            }
            return false;
    }
    return true;
}

// Override the pointing device task to filter horizontal scrolling
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (vertical_scroll_mode && is_drag_scroll) {
        // When vertical scroll mode is active, zero out horizontal scroll
        mouse_report.h = 0;

        // Optionally, you can also disable regular mouse movement during vertical scroll
        // Uncomment the next two lines if you want cursor movement disabled during scroll mode
        // mouse_report.x = 0;
        // mouse_report.y = 0;
    }
    return mouse_report;
}
