/* Copyright 2020 koktoh
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


// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    HNYR = SAFE_RANGE,
    HNKBD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [0] = LAYOUT( \
        HNYR, HNKBD, \
        KC_VOLD, KC_VOLU \
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case HNYR:
            if (record->event.pressed) {
                SEND_STRING("HAPPY NEW YEAR!!\n");
            }
            break;
        case HNKBD:
            if (record->event.pressed) {
                SEND_STRING("HAPPY NEW KEYBOARD!!\n");
            }
            break;
    }
    return true;
}

#ifdef OLED_DRIVER_ENABLE

void render_akeome(void) {
    oled_write_P(akeome, false);
}

void oled_task_user(void) {
    render_akeome();
}

#endif
