/* Copyright 2020 Neil Brian Ramirez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 2022 modified by koktoh
 */

#include "encoder_action.h"

#ifdef ENCODERS
static uint8_t encoder_state[ENCODERS] = {0};
static keypos_t encoder_cw[ENCODERS] = ENCODERS_CW_KEY;
static keypos_t encoder_ccw[ENCODERS] = ENCODERS_CCW_KEY;
#endif

void encoder_action_unregister(void) {
#ifdef ENCODERS
    for (uint8_t index = 0; index < ENCODERS; index++) {
        uint8_t this_hand_index = index;

#   if defined(ENCODERS_LEFT) && defined(ENCODERS_RIGHT)
        if (is_keyboard_left() && !is_keyboard_master()) {
            this_hand_index = ENCODERS - ENCODERS_RIGHT - index;
        }
#   endif

        if (encoder_state[this_hand_index]) {
            keyevent_t encoder_event = (keyevent_t) {
                .key = encoder_state[this_hand_index] >> 1 ? encoder_cw[this_hand_index] : encoder_ccw[this_hand_index],
                .pressed = false,
                .time = (timer_read() | 1)
            };
            encoder_state[this_hand_index] = 0;
            action_exec(encoder_event);
        }
    }
#endif
}

void encoder_action_register(uint8_t index, bool clockwise) {
#ifdef ENCODERS
    uint8_t this_hand_index = index;

#   if defined(ENCODERS_LEFT) && defined(ENCODERS_RIGHT)
        if (is_keyboard_left() && !is_keyboard_master()) {
            this_hand_index = ENCODERS - ENCODERS_RIGHT - index;
        }
#   endif

    keyevent_t encoder_event = (keyevent_t) {
        .key = clockwise ? encoder_cw[this_hand_index] : encoder_ccw[this_hand_index],
        .pressed = true,
        .time = (timer_read() | 1)
    };
    encoder_state[this_hand_index] = (clockwise ^ 1) | (clockwise << 1);
    action_exec(encoder_event);
#endif
}
