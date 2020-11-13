#include QMK_KEYBOARD_H

#include "jtu_custom_keycodes.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
		[0] = LAYOUT( /* Base */
			KC_ESC,  KC_1,    JU_2,            KC_3,    KC_4,    KC_5,    JU_6,    JU_7,    JU_8,    JU_9,    JU_0,    JU_MINS, JU_EQL,  KC_BSPC, KC_HOME,
		    KC_TAB,  KC_Q,    KC_W,            KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JU_LBRC, JU_RBRC, JU_BSLS, KC_PGUP,
			KC_LCTL, KC_A,    KC_S,            KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    JU_SCLN, JU_QUOT,          KC_ENT,  KC_PGDN,
		    KC_LSFT, KC_Z,    KC_X,            KC_C,    KC_V,    KC_B,    MO(1),   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,
		    JU_CAPS,          LALT_T(KC_HENK), KC_SPC,           KC_LGUI,          KC_SPC,           RALT_T(KC_MHEN),           KC_LEFT, KC_DOWN, KC_RIGHT),
		[1] = LAYOUT( /* FN */
			JU_GRV,  KC_F1,   KC_F2,           KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  RESET,   KC_HOME,
		    KC_TRNS, RGB_TOG, RGB_MOD,         RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, KC_TRNS, KC_PSCR, KC_SLCK, KC_PAUS, EEP_RST, KC_PGUP,
			KC_TRNS, RGB_SPI, RGB_SPD,         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_MUTE, KC_PGDN,
		    KC_LSFT, KC_TRNS, KC_TRNS,         KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_RSFT, KC_VOLU,
		    KC_TRNS,          KC_TRNS,         KC_TRNS,          KC_END,           KC_TRNS,          KC_TRNS,                   KC_MPRV, KC_VOLD, KC_MNXT),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	bool continue_process = process_record_user_jtu(keycode, record);
	if (continue_process == false) {
		return false;
	}

	return true;
}