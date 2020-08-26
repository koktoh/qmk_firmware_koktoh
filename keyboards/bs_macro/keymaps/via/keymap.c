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

// Defines names for use in layer keycodes and the keymap
#define _BASE 0
#define _MEDIA 1
#define _FN 2
#define _CONFIG 3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base
        ,-------------------------------------------------------------.
        | Knob: Vol Down/Up |                    | Knob: Page Down/Up |
        | Click: Mute       | Change MEDIA layer | Click: Play/Pose   |
        |-------------------+--------------------+--------------------|
        | Mute              | Up                 | Play/Pose          |
        |-------------------+--------------------+--------------------|
        | Left              | Down               | Right              |
        `-------------------------------------------------------------'
    */
    [_BASE] = LAYOUT( \
        KC_MUTE, TO(_MEDIA), KC_MPLY, \
        KC_MUTE, KC_UP,      KC_MPLY, \
        KC_LEFT, KC_DOWN,    KC_RGHT \
    ),
    /* Media
        ,----------------------------------------------------------.
        | Knob: Vol Down/Up |                 | Knob: Page Down/Up |
        | Click: Mute       | Change FN layer | Click: Play/Pose   |
        |-------------------+-----------------+--------------------|
        | Mute              | Media Stop      | Play/Pose          |
        |-------------------+-----------------+--------------------|
        | Media Previous    | XXXXXXX         | Media Next         |
        `----------------------------------------------------------'
    */
    [_MEDIA] = LAYOUT( \
        _______, TO(_FN), _______, \
        _______, KC_STOP, _______, \
        KC_MPRV, XXXXXXX, KC_MNXT \
    ),
    /* Fn
        ,--------------------------------------------------------------.
        | Knob: Vol Down/Up |                     | Knob: Page Down/Up |
        | Click: Mute       | Change CONFIG layer | Click: Play/Pose   |
        |-------------------+---------------------+--------------------|
        | Insert            | Home                | Back Space         |
        |-------------------+---------------------+--------------------|
        | Print Screen      | End                 | Delete             |
        `--------------------------------------------------------------'
    */
    [_FN] = LAYOUT( \
        _______, TO(_CONFIG), _______, \
        KC_INS,  KC_HOME,     KC_BSPC, \
        KC_PSCR, KC_END,      KC_DEL \
    ),
    /* Config
        ,---------------------------------------------------------------------.
        | Knob: Vol Down/Up |                            | Knob: Page Down/Up |
        | Click: Mute       | Change BASE layer          | Click: Play/Pose   |
        |-------------------+----------------------------+--------------------|
        | Reset             | Backlight On/off           | RGB On/Off         |
        |-------------------+----------------------------+--------------------|
        | XXXXXXX           | Toggle backlight breathing | RGB Mode           |
        `---------------------------------------------------------------------'
    */
    [_CONFIG] = LAYOUT( \
        _______, TO(_BASE), _______, \
        RESET,   BL_TOGG,   RGB_TOG, \
        XXXXXXX, BL_BRTG,   RGB_MOD \
    )
};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLD);
        } else {
            tap_code(KC_VOLU);
        }
    }
    else if (index == 1) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
}

#ifdef OLED_DRIVER_ENABLE

void render_logo(void) {
    oled_set_cursor(0, 0);
    oled_write_P(bs_logo, false);
}

void render_layer(void) {
    oled_set_cursor(START_COL, START_ROW);

    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("BASE     "), false);
            break;
        case _MEDIA:
            oled_write_P(PSTR("MEDIA    "), false);
            break;
        case _FN:
            oled_write_P(PSTR("FN       "), false);
            break;
        case _CONFIG:
            oled_write_P(PSTR("CONFIG   "), false);
            break;
        default:
            oled_write_P(PSTR("Undefined"), false);
            break;
    }
}

void oled_task_user(void) {
    render_logo();
    render_layer();
}
#endif
