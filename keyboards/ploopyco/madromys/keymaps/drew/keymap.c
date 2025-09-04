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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN4, KC_BTN5, DRAG_SCROLL, KC_BTN2, KC_BTN1, KC_BTN3 )
};

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static uint16_t scroll_timer = 0;
    static int8_t last_dominant = 0; // 1=v, 2=h

    int8_t h_abs = abs(mouse_report.h);
    int8_t v_abs = abs(mouse_report.v);

    // Apply deadzone first
    if (h_abs <= H_SCROLL_DEADZONE) mouse_report.h = 0;

    // Update values after deadzone
    h_abs = abs(mouse_report.h);
    v_abs = abs(mouse_report.v);

    // Determine and lock to dominant axis
    if (h_abs > 0 || v_abs > 0) {
        scroll_timer = timer_read();

        if (v_abs > h_abs * SCROLL_SNAP_RATIO) {
            last_dominant = 1;
        } else if (h_abs > v_abs * SCROLL_SNAP_RATIO) {
            last_dominant = 2;
        }
    }

    // Apply axis locking based on recent dominant direction
    if (timer_elapsed(scroll_timer) < SCROLL_TIMEOUT) {
        if (last_dominant == 1) {
            mouse_report.h = 0;
        } else if (last_dominant == 2) {
            mouse_report.v = 0;
        }
    } else {
        last_dominant = 0;  // Reset after timeout
    }

    return mouse_report;
}
