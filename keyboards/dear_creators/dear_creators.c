// Copyright 2021 koktoh (@koktoh)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "dear_creators.h"

void matrix_scan_kb(void) {
    encoder_action_unregister();
    matrix_scan_user();
}

bool encoder_update_kb(uint8_t index, bool clockwise) {
    encoder_action_register(index, clockwise);
    return true;
}
