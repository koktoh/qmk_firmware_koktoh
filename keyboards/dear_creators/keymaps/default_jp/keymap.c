// Copyright 2021 koktoh (@koktoh)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "dear_creators.h"
#include "keymap_japanese.h"

#ifdef OLED_ENABLE
#include "../../util/oled_helper.h"
#endif

#define CTL_WU LCTL(KC_WH_U)
#define CTL_WD LCTL(KC_WH_D)
#define SFT_WU LSFT(KC_WH_U)
#define SFT_WD LSFT(KC_WH_D)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [0] = LAYOUT(
        KC_VOLD, KC_VOLU, KC_WH_U, KC_WH_D,                                                              CTL_WU,  CTL_WD,  SFT_WD,  SFT_WU,
        JP_ZKHK, KC_ESC,  KC_TAB,  KC_MUTE, XXXXXXX,                                            XXXXXXX, XXXXXXX, JP_LBRC, JP_RBRC, KC_BSPC,
        JP_ZKHK, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    JP_MINS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JP_AT,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                             KC_H,    KC_J,    KC_K,    KC_L,    JP_SCLN, JP_COLN,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TO(1),          TO(2),   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                 KC_CAPS, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX, KC_SPC,         KC_ENT,  XXXXXXX, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT
    ),
    [1] = LAYOUT(
        _______, _______, _______, _______,                                                              _______, _______, _______, _______,
        _______, _______, _______, _______, XXXXXXX,                                            _______, _______, _______, _______, _______,
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                          _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, TO(2),          TO(0),   _______, _______, _______, _______, _______, _______,
                 _______, _______, _______, _______, _______, _______,        _______, _______, _______, _______, _______, _______
    ),
    [2] = LAYOUT(
        _______, _______, _______, _______,                                                              _______, _______, _______, _______,
        JP_ZKHK, KC_ESC,  KC_TAB,  KC_MUTE, XXXXXXX,                                            XXXXXXX, XXXXXXX, JP_LBRC, JP_RBRC, KC_BSPC,
        JP_ZKHK, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    JP_MINS,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JP_AT,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                             KC_H,    KC_J,    KC_K,    KC_L,    JP_SCLN, JP_COLN,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    TO(0),          TO(1),   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                 KC_CAPS, KC_LCTL, KC_LALT, KC_LGUI, XXXXXXX, KC_SPC,         KC_ENT,  XXXXXXX, KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT
    )
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

bool oled_task_user(void) {
    uint8_t current_layer = get_highest_layer(layer_state);

    render_logo();
    oled_write_P(PSTR("VITAL"), false);
    oled_write_P(PSTR("SIGNS"), false);

    oled_set_cursor(0, 7);

    oled_write_P(PSTR("Layer"), false);

    render_layer_state(current_layer, 0, OLED_FONT_HEIGHT * 8 + 4);

    return false;
}
#endif
