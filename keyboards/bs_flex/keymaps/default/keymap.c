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
#include "quantum/rgblight_list.h"
#include "jtu_custom_keycodes.h"
#include "keymap_jp.h"

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _QWERTY,
    _RAISE,
    _LOWER,
};

#ifdef RGBLIGHT_LAYERS

#define LEFT_OUTER 0
#define LEFT_INNER 1
#define RIGHT_INNER 2
#define RIGHT_OUTER 3

const rgblight_segment_t PROGMEM capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {LEFT_INNER, 1, HSV_ORANGE},
    {RIGHT_INNER, 1, HSV_ORANGE}
);

const rgblight_segment_t PROGMEM numlock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {LEFT_INNER, 1, HSV_AZURE},
    {RIGHT_INNER, 1, HSV_AZURE}
);

const rgblight_segment_t PROGMEM cnlock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {LEFT_INNER, 1, HSV_GREEN},
    {RIGHT_INNER, 1, HSV_GREEN}
);

const rgblight_segment_t PROGMEM qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {LEFT_OUTER, 1, HSV_CYAN},
    {LEFT_INNER, 1, HSV_BLACK},
    {RIGHT_INNER, 1, HSV_BLACK},
    {RIGHT_OUTER, 1, HSV_CYAN}
);

const rgblight_segment_t PROGMEM raise_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {LEFT_OUTER, 1, HSV_MAGENTA},
    {LEFT_INNER, 1, HSV_BLACK},
    {RIGHT_INNER, 1, HSV_BLACK},
    {RIGHT_OUTER, 1, HSV_MAGENTA}
);

const rgblight_segment_t PROGMEM lower_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {LEFT_OUTER, 1, HSV_YELLOW},
    {LEFT_INNER, 1, HSV_BLACK},
    {RIGHT_INNER, 1, HSV_BLACK},
    {RIGHT_OUTER, 1, HSV_YELLOW}
);

const rgblight_segment_t* const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    qwerty_layer,
    raise_layer,
    lower_layer,
    capslock_layer,
    numlock_layer,
    cnlock_layer
);

void keyboard_post_init_user(void) {
    rgblight_layers = rgb_layers;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(3, led_state.caps_lock && !led_state.num_lock);
    rgblight_set_layer_state(4, !led_state.caps_lock && led_state.num_lock);
    rgblight_set_layer_state(5, led_state.caps_lock && led_state.num_lock);

    return true;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(0, layer_state_cmp(state, _QWERTY));
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, _RAISE));
    rgblight_set_layer_state(2, layer_state_cmp(state, _LOWER));
    return state;
}

#endif

#define TO_QWRT TO(_QWERTY)
#define TO_LOWR TO(_LOWER)
#define TO_RAIS TO(_RAISE)
#define MO_LOWR MO(_LOWER)
#define MO_RAIS MO(_RAISE)
#define LT_LWMH LT(_LOWER, KC_MHEN)
#define LT_RSHN LT(_RAISE, KC_HENK)

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    KC_P00 = JTU_SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* QWERTY
        ,----------------------------------------. ,--------------------------------------------------------.                       ,---------------------------------------------------------. ,-----------------------------------------.
        | Num Lock | XXXXXXX | XXXXXXX |  Back   | |     F1    |  F2   |  F3   |   F4    |  F5   |    F6    |                       |   F7    |    F8     |  F9   |  F10  |  F11 |     F12    | | XXXXXXX | Print   | Scroll  |   Pause   |
        |          |         |         |  Space  | `--------------------------------------------------------'                       `---------------------------------------------------------' |         | Screen  | Lock    |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        | XXXXXXX  |    /    |    *    |    -    | |    ESC    |   1   |   2   |    3    |   4   |     5    |                       |    6    |     7     |   8   |   9   |   0  | Back Space | | XXXXXXX | Insert  |  Home   |  Page Up  |
        |          |         |         |         | `--------------------------------------------------------'                       `---------------------------------------------------------' |         |         |         |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        |     7    |    8    |    9    |    +    | |    Tab    |   Q   |   W   |    E    |   R   |     T    |                       |    Y    |     U     |   I   |   O   |   P  |      \     | | XXXXXXX | Delete  |  End    | Page Down |
        |----------+---------+---------+---------| |-----------+-------+-------+---------+-------+----------|                       |---------+-----------+-------+-------+------+------------| |---------+---------+---------+-----------|
        |     4    |    5    |    6    |    +    | |   L Ctrl  |   A   |   S   |    D    |   F   |     G    |                       |    H    |     J     |   K   |   L   |   ;  |      '     | | XXXXXXX | XXXXXXX | XXXXXXX |  XXXXXXX  |
        |----------+---------+---------+---------| |-----------+-------+-------+---------+-------+----------|                       |---------+-----------+-------+-------+------+------------| |---------+---------+---------+-----------|
        |     1    |    2    |    3    |  Enter  | |  L Shift  |   Z   |   X   |    C    |   V   |     B    |                       |    N    |     M     |   ,   |   .   |   /  |   R Shift  | | XXXXXXX | XXXXXXX |   Up    |  XXXXXX   |
        |          |         |         |         | `--------------------------------------------------------'                       `---------------------------------------------------------' |         |         |         |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        |     0    |   00    |    .    |  Enter  | | Caps Lock | L Gui | L Alt | XXXXXXX |  Home |    End   |                       | Page Up | Page Down | Left  |  Up   | Down |    Right   | | XXXXXXX |  Left   |  Down   |   Right   |
        `----------------------------------------' `--------------------------------------------------------'                       `---------------------------------------------------------' `-----------------------------------------'
                                                   ,----------------------------------------------------------------.       ,-----------------------------------------------------------------.
                                                   |   LOWER   |                         | LOWER | Muhenkan | Space |       | Enter |  Henkan |   RAISE   |                      |    RAISE   |
                                                   `----------------------------------------------------------------'       `-----------------------------------------------------------------'
    */
    [_QWERTY] = LAYOUT(
        KC_NLCK, XXXXXXX, XXXXXXX, KC_BSPC, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,  XXXXXXX, KC_PSCR, KC_SLCK, KC_PAUS,
        XXXXXXX, KC_PSLS, KC_PAST, KC_PMNS, KC_ESC,  KC_1,    JU_2,    KC_3,    KC_4,    KC_5,                             JU_6,    JU_7,    JU_8,    JU_9,    JU_0,     KC_BSPC, XXXXXXX, KC_INS,  KC_HOME, KC_PGUP,
        KC_P7,   KC_P8,   KC_P9,   KC_PPLS, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,     JU_BSLS, XXXXXXX, KC_DELT, KC_END,  KC_PGDN,
        KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                             KC_H,    KC_J,    KC_K,    KC_L,    JU_SCLN,  JU_QUOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        KC_P1,   KC_P2,   KC_P3,   KC_ENT,  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT, XXXXXXX, XXXXXXX, KC_UP,   XXXXXXX,
        KC_P0,   KC_P00,  KC_PDOT, KC_ENT,  JU_CAPS, KC_LGUI, KC_LALT, XXXXXXX, KC_HOME, KC_END,                           KC_PGUP, KC_PGDN, KC_LEFT, KC_UP,   KC_DOWN,  KC_RGHT, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT,
                                            MO_LOWR,                            TO_LOWR, LT_LWMH, KC_SPC,         KC_ENT,  LT_RSHN, TO_RAIS,                             MO_RAIS
    ),

    /* QWERTY
        ,----------------------------------------. ,--------------------------------------------------------.                       ,---------------------------------------------------------. ,-----------------------------------------.
        | Num Lock | XXXXXXX | XXXXXXX |  Back   | |     F1    |  F2   |  F3   |   F4    |  F5   |    F6    |                       |   F7    |    F8     |  F9   |  F10  |  F11 |     F12    | | XXXXXXX | Print   | Scroll  |   Pause   |
        |          |         |         |  Space  | `--------------------------------------------------------'                       `---------------------------------------------------------' |         | Screen  | Lock    |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        | XXXXXXX  |    /    |    *    |    -    | |    ESC    |   1   |   2   |    3    |   4   |     5    |                       |    6    |     7     |   8   |   9   |   0  | Back Space | | XXXXXXX | Insert  |  Home   |  Page Up  |
        |          |         |         |         | `--------------------------------------------------------'                       `---------------------------------------------------------' |         |         |         |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        |     7    |    8    |    9    |    +    | |     F1    |  F2   |  F3   |   F4    |  F5   |    F6    |                       |   F7    |    F8     |  F9   |  F10  |  F11 |     F12    | | XXXXXXX | Delete  |  End    | Page Down |
        |----------+---------+---------+---------| |-----------+-------+-------+---------+-------+----------|                       |---------+-----------+-------+-------+------+------------| |---------+---------+---------+-----------|
        |     4    |    5    |    6    |    +    | |   L Ctrl  |   !   |   @   |    #    |   $   |     %    |                       |    ^    |     &     |   *   |   (   |   )  |      ~     | | XXXXXXX | XXXXXXX | XXXXXXX |  XXXXXXX  |
        |----------+---------+---------+---------| |-----------+-------+-------+---------+-------+----------|                       |---------+-----------+-------+-------+------+------------| |---------+---------+---------+-----------|
        |     1    |    2    |    3    |  Enter  | |  L Shift  |   {   |   }   |    [    |   ]   |     |    |                       |    -    |     =     |   ,   |   .   |   /  |   R Shift  | | XXXXXXX | XXXXXXX |   Up    |  XXXXXX   |
        |          |         |         |         | `--------------------------------------------------------'                       `---------------------------------------------------------' |         |         |         |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        |     0    |   00    |    .    |  Enter  | | Caps Lock | L Gui | L Alt | XXXXXXX |  Home |    End   |                       | Page Up | Page Down | Left  |  Up   | Down |    Right   | | XXXXXXX |  Left   |  Down   |   Right   |
        `----------------------------------------' `--------------------------------------------------------'                       `---------------------------------------------------------' `-----------------------------------------'
                                                   ,----------------------------------------------------------------.       ,-----------------------------------------------------------------.
                                                   |   LOWER   |                         | QWERT | Muhenkan | Space |       | Enter |  Henkan |   RAISE   |                      |    RAISE   |
                                                   `----------------------------------------------------------------'       `-----------------------------------------------------------------'
    */
    [_RAISE] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                             KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, _______, _______,
        _______, _______, _______, _______, _______, JP_EXLM, JP_AT,   JP_HASH, JP_DLR,  JP_PERC,                           JP_CIRC, JP_AMPR, JP_ASTR, JP_LPRN, JP_RPRN, JU_GRV,  _______, _______, _______, _______,
        _______, _______, _______, _______, _______, JP_LCBR, JP_RCBR, JP_LBRC, JP_RBRC, JP_PIPE,                           JU_MINS, JU_EQL,  _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                           _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                            _______,                            TO_QWRT, _______, _______,         _______, _______, TO_RAIS,                            _______
    ),

    /* QWERTY
        ,----------------------------------------. ,--------------------------------------------------------.                       ,---------------------------------------------------------. ,-----------------------------------------.
        | Num Lock | XXXXXXX | XXXXXXX |  Back   | |     F1    |  F2   |  F3   |   F4    |  F5   |    F6    |                       |   F7    |    F8     |  F9   |  F10  |  F11 |     F12    | | XXXXXXX | Print   | Scroll  |   Pause   |
        |          |         |         |  Space  | `--------------------------------------------------------'                       `---------------------------------------------------------' |         | Screen  | Lock    |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        | XXXXXXX  |    /    |    *    |    -    | |    ESC    |   1   |   2   |    3    |   4   |     5    |                       |    6    |     7     |   8   |   9   |   0  | Back Space | | XXXXXXX | Insert  |  Home   |  Page Up  |
        |          |         |         |         | `--------------------------------------------------------'                       `---------------------------------------------------------' |         |         |         |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        |     7    |    8    |    9    |    +    | |    ESC    |   1   |   2   |    3    |   4   |     5    |                       |    6    |     7     |   8   |   9   |   0  | Back Space | | XXXXXXX | Delete  |  End    | Page Down |
        |----------+---------+---------+---------| |-----------+-------+-------+---------+-------+----------|                       |---------+-----------+-------+-------+------+------------| |---------+---------+---------+-----------|
        |     4    |    5    |    6    |    +    | |   L Ctrl  | Caps  | Left  |   UP    | Right | XXXXXXX  |                       | XXXXXXX |  Insert   | Home  | PG UP | BSPC |   Enter    | | XXXXXXX | XXXXXXX | XXXXXXX |  XXXXXXX  |
        |----------+---------+---------+---------| |-----------+-------+-------+---------+-------+----------|                       |---------+-----------+-------+-------+------+------------| |---------+---------+---------+-----------|
        |     1    |    2    |    3    |  Enter  | |  L Shift  | L Gui | L Alt |  Down   |XXXXXXX| XXXXXXX  |                       | XXXXXXX |  Delete   |  End  | PG DN |   /  |   R Shift  | | XXXXXXX | XXXXXXX |   Up    |  XXXXXX   |
        |          |         |         |         | `--------------------------------------------------------'                       `---------------------------------------------------------' |         |         |         |           |
        |----------+---------+---------+---------| ,--------------------------------------------------------.                       ,---------------------------------------------------------. |---------+---------+---------+-----------|
        |     0    |   00    |    .    |  Enter  | | Caps Lock | L Gui | L Alt | XXXXXXX |  Home |    End   |                       | Page Up | Page Down | Left  |  Up   | Down |    Right   | | XXXXXXX |  Left   |  Down   |   Right   |
        `----------------------------------------' `--------------------------------------------------------'                       `---------------------------------------------------------' `-----------------------------------------'
                                                   ,----------------------------------------------------------------.       ,-----------------------------------------------------------------.
                                                   |   LOWER   |                         | LOWER | Muhenkan | Space |       | Enter |  Henkan |   QWERTY  |                      |    RAISE   |
                                                   `----------------------------------------------------------------'       `-----------------------------------------------------------------'
    */
    [_LOWER] = LAYOUT(
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                           _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                           _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, KC_ESC,  KC_1,    JU_2,    KC_3,    KC_4,    KC_5,                              JU_6,    JU_7,     JU_8,    JU_9,    JU_0,    KC_BSPC, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, JU_CAPS, KC_LEFT, KC_UP,   KC_RGHT, XXXXXXX,                           XXXXXXX, KC_INS,   KC_HOME, KC_PGUP, KC_BSPC, KC_ENT,  _______, _______, _______, _______,
        _______, _______, _______, _______, _______, KC_LGUI, KC_LALT, KC_DOWN, XXXXXXX, XXXXXXX,                           XXXXXXX, KC_DEL,   KC_END,  KC_PGDN, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                            _______,                            TO_LOWR, _______, _______,         _______, _______, TO_QWRT,                            _______
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // for jtu_custom_keycodes
    bool continue_process = process_record_user_jtu(keycode, record);
    if (continue_process == false) {
        return false;
    }

    switch (keycode) {
        case KC_P00:
            if (record->event.pressed) {
                SEND_STRING("00");
            }
            break;
    }
    return true;
}


/*
void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

bool led_update_user(led_t led_state) {
    return true;
}
*/
