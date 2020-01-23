#include QMK_KEYBOARD_H
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "keymap_jp.h"

#include "jtu_custom_keycodes.h"

#define BASE 0 // default layer
#define ACTN 1 // actions

enum custom_keycodes {
  PLACEHOLDER = JTU_SAFE_RANGE // can always be here
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Keymap 0: Basic layer
 *
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * | ESC    |   1  |   2  |   3  |   4  |   5  | BSPC |           | PrtSc|   6  |   7  |   8  |   9  |   0  |   -    |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * | TAB    |   Q  |   W  |   E  |   R  |   T  | ENT  |           |   {  |   Y  |   U  |   I  |   O  |   P  |   }    |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * | LCtrl  |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |   '    |
 * |--------+------+------+------+------+------| BSLS |           |   =  |------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  | RShift |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |LSftCaps| LGui |Kana| Alt  |   `  |                                       | Down |  Up  | Right| Left | RGui |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,---------------.
 *                                        | VolUp|VolDwn|       | Mute |        |
 *                                 ,------|------|------|       |------+--------+------.
 *                                 |      |      |      |       |      |        |      |
 *                                 | MO(1)| Space|------|       |------| Enter  | MO(1)|
 *                                 |      |      |Toggle|       |Toggle|        |      |
 *                                 `--------------------'       `----------------------'
 */
// If it accepts an argument (i.e, is a function), it doesn't need KC_.
// Otherwise, it needs KC_*
[BASE] = LAYOUT_ergodox(  // layer 0 : default
        // left hand
        KC_ESC,           KC_1,    JU_2,    KC_3,   KC_4,   KC_5,   KC_BSPC,
        KC_TAB,           KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,   KC_ENT,
        KC_LCTL,          KC_A,    KC_S,    KC_D,   KC_F,   KC_G,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   JU_BSLS,
        LSFT(KC_CAPS), KC_LGUI, JP_ZHTG, KC_LALT, JU_GRV,
                                                    KC_VOLU, KC_VOLD,
                                                               KC_NO,
                                            MO(ACTN), KC_SPC, TO(ACTN),
        // right hand
             KC_PSCR,    JU_6,     JU_7,      JU_8,      JU_9,      JU_0,   JU_MINS,
             JU_LBRC,    KC_Y,     KC_U,      KC_I,      KC_O,      KC_P,   JU_RBRC,
                         KC_H,     KC_J,      KC_K,      KC_L,   JU_SCLN,   JU_QUOT,
             JU_EQL,     KC_N,     KC_M,   KC_COMM,    KC_DOT,   KC_SLSH,   KC_RSFT,
                                KC_DOWN,     KC_UP,   KC_LEFT,   KC_RGHT,   KC_RGUI,
             KC_MUTE,   KC_NO,
             KC_NO,
             TO(ACTN), KC_ENT, MO(ACTN)
    ),
/* Keymap 1: Symbol Layer
 *
 * ,---------------------------------------------------.           ,--------------------------------------------------.
 * |_________|  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |   F11  |
 * |---------+------+------+------+------+------+------|           |------+------+------+------+------+------+--------|
 * |_________|      |      |  Up  |      |      |      |           |      |      |      |      |      |      |   F12  |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |_________|      | Left | Down | Right|      |------|           |------|      | Ins  | Home | PgUp | BSPS |        |
 * |---------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |_________|      |      |      |      |      |      |           |      |      | Del  |  End |PgDown|      |________|
 * `---------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   |_______|______|______|______|      |                                       |      |      |      |      |      |
 *   `-----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        |      |      |       |      |      |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      |      |       |      |      |      |
 *                                 |______|______|------|       |------|______|______|
 *                                 |      |      |Toggle|       |Toggle|      |      |
 *                                 `--------------------'       `--------------------'
 */
// ACTNOLS
[ACTN] = LAYOUT_ergodox(
       // left hand
       KC_TRNS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,  KC_TRNS,
       KC_TRNS,   KC_NO,   KC_NO,   KC_UP,   KC_NO,   KC_NO,  KC_TRNS,
       KC_TRNS,   KC_NO, KC_LEFT, KC_DOWN, KC_RGHT,   KC_NO,
       KC_TRNS,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_TRNS,
       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
                                                      KC_NO,    KC_NO,
                                                              KC_TRNS,
                                           KC_TRNS, KC_TRNS, TO(BASE),
       // right hand
       KC_TRNS,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
       KC_TRNS,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_F12,
                  KC_NO,  KC_INS, KC_HOME, KC_PGUP, KC_BSPC, KC_TRNS,
       KC_TRNS,   KC_NO,  KC_DEL,  KC_END, KC_PGDN,   KC_NO, KC_TRNS,
                         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
         KC_NO,   KC_NO,
         KC_NO,
      TO(BASE), KC_TRNS, KC_TRNS
),
};

const uint16_t PROGMEM fn_actions[] = {
    [1] = ACTION_LAYER_TAP_TOGGLE(ACTN)                // FN1 - Momentary Layer 1 (Symbols)
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool continue_process = process_record_user_jtu(keycode, record);
  if (continue_process == false) {
    return false;
  }
  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

};


// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      // TODO: Make this relevant to the ErgoDox EZ.
        case 1:
            ergodox_right_led_1_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            break;
        default:
            // none
            break;
    }

};
