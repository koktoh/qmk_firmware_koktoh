// Copyright 2021 koktoh (@koktoh)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"
#include "util/encoder_action.h"

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */

#define LAYOUT( \
    e00, e01, e20, e21,                               e30, e31, e40, e41, \
    l00, l01, l02, l03, l04,                     r01, r02, r03, r04, r05, \
    l10, l11, l12, l13, l14, l15,           r11, r12, r13, r14, r15, r16, \
    l20, l21, l22, l23, l24, l25,           r21, r22, r23, r24, r25, r26, \
    l30, l31, l32, l33, l34, l35,           r31, r32, r33, r34, r35, r36, \
    l40, l41, l42, l43, l44, l45, l46, r40, r41, r42, r43, r44, r45, r46, \
         l51, l52, l53, l54, l55, l56, r50, r51, r52, r53, r54, r55 \
) { \
    {   l04,   l03,   l02,   l01,   l00,   l10 }, \
    {   l15,   l14,   l13,   l12,   l11,   l20 }, \
    {   l25,   l24,   l23,   l22,   l21,   l30 }, \
    {   l35,   l34,   l33,   l32,   l31,   l40 }, \
    {   l46,   l45,   l44,   l43,   l42,   l41 }, \
    {   l56,   l55,   l54,   l53,   l52,   l51 }, \
    {   e00,   e01,   e20,   e21, KC_NO, KC_NO }, \
    {   r01,   r02,   r03,   r04,   r05,   r16 }, \
    {   r11,   r12,   r13,   r14,   r15,   r26 }, \
    {   r21,   r22,   r23,   r24,   r25,   r36 }, \
    {   r31,   r32,   r33,   r34,   r35,   r46 }, \
    {   r40,   r41,   r42,   r43,   r44,   r45 }, \
    {   r50,   r51,   r52,   r53,   r54,   r55 }, \
    {   e30,   e31,   e40,   e41, KC_NO, KC_NO }  \
}
