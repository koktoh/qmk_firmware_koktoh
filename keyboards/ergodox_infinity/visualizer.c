/*
Copyright 2016 Fred Sundvik <fsundvik@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Currently we are assuming that both the backlight and LCD are enabled
// But it's entirely possible to write a custom visualizer that use only
// one of them
#ifndef LCD_BACKLIGHT_ENABLE
#error This visualizer needs that LCD backlight is enabled
#endif

#ifndef LCD_ENABLE
#error This visualizer needs that LCD is enabled
#endif

#include "visualizer.h"
#include "visualizer_keyframes.h"
#include "lcd_keyframes.h"
#include "lcd_backlight_keyframes.h"
#include "system/serial_link.h"
#include "default_animations.h"
#include "keymaps/koktoh/hibiki_visualizer/hibiki_lcd_keyframes.h"

static const uint32_t logo_background_color = LCD_COLOR(0x00, 0x00, 0xFF);
static const uint32_t initial_color = LCD_COLOR(0, 0, 0xFF);

static const uint32_t led_emulation_colors[4] = {
    LCD_COLOR(130, 255, 87),
    LCD_COLOR(255, 255, 255),
    LCD_COLOR(84, 255, 255),
    LCD_COLOR(168, 255, 255),
};

static uint32_t next_led_target_color = 0;

typedef enum {
    LCD_STATE_INITIAL,
    LCD_STATE_LAYER_BITMAP,
    LCD_STATE_BITMAP_AND_LEDS,
} lcd_state_t;

static lcd_state_t lcd_state = LCD_STATE_INITIAL;

typedef struct {
    uint8_t led_on;
    uint8_t led1;
    uint8_t led2;
    uint8_t led3;
} visualizer_user_data_t;

// Don't access from visualization function, use the visualizer state instead
static visualizer_user_data_t user_data_keyboard = {
    .led_on = 0,
    .led1 = LED_BRIGHTNESS_HI,
    .led2 = LED_BRIGHTNESS_HI,
    .led3 = LED_BRIGHTNESS_HI,
};

_Static_assert(sizeof(visualizer_user_data_t) <= VISUALIZER_USER_DATA_SIZE,
    "Please increase the VISUALIZER_USER_DATA_SIZE");

// Feel free to modify the animations below, or even add new ones if needed

static bool is_anime_started = false;

bool swap_led_target_color(keyframe_animation_t* animation, visualizer_state_t* state) {
    uint32_t temp = next_led_target_color;
    next_led_target_color = state->target_lcd_color;
    state->target_lcd_color = temp;
    return false;
}

// The color animation animates the LCD color when you change layers
static keyframe_animation_t two_led_colors = {
    .num_frames = 2,
    .loop = false,
    .frame_lengths = {gfxMillisecondsToTicks(1000), gfxMillisecondsToTicks(0)},
    .frame_functions = {lcd_backlight_keyframe_set_color, swap_led_target_color},
};

// The LCD animation alternates between the layer name display and a
// bitmap that displays all active layers
static keyframe_animation_t lcd_bitmap_animation = {
    .num_frames = 20,
    .loop = true,
    .frame_lengths = {gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200), gfxMillisecondsToTicks(200)},
    .frame_functions = {lcd_keyframe_draw_hibiki_0, lcd_keyframe_draw_hibiki_1, lcd_keyframe_draw_hibiki_2, lcd_keyframe_draw_hibiki_3, lcd_keyframe_draw_hibiki_4, lcd_keyframe_draw_hibiki_5, lcd_keyframe_draw_hibiki_6, lcd_keyframe_draw_hibiki_7, lcd_keyframe_draw_hibiki_8, lcd_keyframe_draw_hibiki_9, lcd_keyframe_draw_hibiki_10, lcd_keyframe_draw_hibiki_11, lcd_keyframe_draw_hibiki_12, lcd_keyframe_draw_hibiki_13, lcd_keyframe_draw_hibiki_14, lcd_keyframe_draw_hibiki_15, lcd_keyframe_draw_hibiki_16, lcd_keyframe_draw_hibiki_17, lcd_keyframe_draw_hibiki_18, lcd_keyframe_draw_hibiki_19},
};

void initialize_user_visualizer(visualizer_state_t* state) {
    // The brightness will be dynamically adjustable in the future
    // But for now, change it here.
    lcd_backlight_brightness(LED_BRIGHTNESS_HI);
    state->current_lcd_color = initial_color;
    state->target_lcd_color = logo_background_color;
    lcd_state = LCD_STATE_INITIAL;
    is_anime_started = false;
    start_keyframe_animation(&default_startup_animation);
}

static inline bool is_led_on(visualizer_user_data_t* user_data, uint8_t num) {
    return user_data->led_on & (1u << num);
}

static uint8_t get_led_index_master(visualizer_user_data_t* user_data) {
    for (int i=0; i < 3; i++) {
        if (is_led_on(user_data, i)) {
            return i + 1;
        }
    }
    return 0;
}

static uint8_t get_led_index_slave(visualizer_user_data_t* user_data) {
    uint8_t master_index = get_led_index_master(user_data);
    if (master_index!=0) {
        for (int i=master_index; i < 3; i++) {
            if (is_led_on(user_data, i)) {
                return i + 1;
            }
        }
    }

    return 0;
}

static uint8_t get_secondary_led_index(visualizer_user_data_t* user_data) {
    if (is_led_on(user_data, 0) &&
            is_led_on(user_data, 1) &&
            is_led_on(user_data, 2)) {
        return 3;
    }
    return 0;
}

static void update_emulated_leds(visualizer_state_t* state, visualizer_keyboard_status_t* prev_status) {
    visualizer_user_data_t* user_data_new = (visualizer_user_data_t*)state->status.user_data;
    visualizer_user_data_t* user_data_old = (visualizer_user_data_t*)prev_status->user_data;

    uint8_t new_index;
    uint8_t old_index;

    if (is_serial_link_master()) {
        new_index = get_led_index_master(user_data_new);
        old_index = get_led_index_master(user_data_old);
    }
    else {
        new_index = get_led_index_slave(user_data_new);
        old_index = get_led_index_slave(user_data_old);
    }
    uint8_t new_secondary_index = get_secondary_led_index(user_data_new);
    uint8_t old_secondary_index = get_secondary_led_index(user_data_old);

    if (lcd_state == LCD_STATE_INITIAL ||
            new_index != old_index ||
            new_secondary_index != old_secondary_index) {

        state->target_lcd_color = change_lcd_color_intensity(
            led_emulation_colors[new_index], LED_BRIGHTNESS_HI);
        next_led_target_color = change_lcd_color_intensity(
            led_emulation_colors[new_secondary_index], LED_BRIGHTNESS_HI);

        lcd_state = LCD_STATE_BITMAP_AND_LEDS;

        stop_keyframe_animation(&two_led_colors);
        start_keyframe_animation(&two_led_colors);
    }
}

void update_user_visualizer_state(visualizer_state_t* state, visualizer_keyboard_status_t* prev_status) {
    // Check the status here to start and stop animations
    // You might have to save some state, like the current animation here so that you can start the right
    // This function is called every time the status changes

    // NOTE that this is called from the visualizer thread, so don't access anything else outside the status
    // This is also important because the slave won't have access to the active layer for example outside the
    // status.

    if(is_anime_started == false) {
        start_keyframe_animation(&lcd_bitmap_animation);
        is_anime_started = true;
    }
    update_emulated_leds(state, prev_status);
}

void user_visualizer_suspend(visualizer_state_t* state) {
    state->layer_text = "Suspending...";
    uint8_t hue = LCD_HUE(state->current_lcd_color);
    uint8_t sat = LCD_SAT(state->current_lcd_color);
    state->target_lcd_color = LCD_COLOR(hue, sat, 0);
    start_keyframe_animation(&default_suspend_animation);
}

void user_visualizer_resume(visualizer_state_t* state) {
    state->current_lcd_color = initial_color;
    state->target_lcd_color = logo_background_color;
    lcd_state = LCD_STATE_INITIAL;
    is_anime_started = false;
    start_keyframe_animation(&default_startup_animation);
}

void ergodox_board_led_on(void){
    // No board led support
}

void ergodox_right_led_1_on(void){
    user_data_keyboard.led_on |= (1u << 0);
    visualizer_set_user_data(&user_data_keyboard);
}

void ergodox_right_led_2_on(void){
    user_data_keyboard.led_on |= (1u << 1);
    visualizer_set_user_data(&user_data_keyboard);
}

void ergodox_right_led_3_on(void){
    user_data_keyboard.led_on |= (1u << 2);
    visualizer_set_user_data(&user_data_keyboard);
}

void ergodox_board_led_off(void){
    // No board led support
}

void ergodox_right_led_1_off(void){
    user_data_keyboard.led_on &= ~(1u << 0);
    visualizer_set_user_data(&user_data_keyboard);
}

void ergodox_right_led_2_off(void){
    user_data_keyboard.led_on &= ~(1u << 1);
    visualizer_set_user_data(&user_data_keyboard);
}

void ergodox_right_led_3_off(void){
    user_data_keyboard.led_on &= ~(1u << 2);
    visualizer_set_user_data(&user_data_keyboard);
}

void ergodox_right_led_1_set(uint8_t n) {
    user_data_keyboard.led1 = n;
    visualizer_set_user_data(&user_data_keyboard);
}

void ergodox_right_led_2_set(uint8_t n) {
    user_data_keyboard.led2 = n;
    visualizer_set_user_data(&user_data_keyboard);
}

void ergodox_right_led_3_set(uint8_t n) {
    user_data_keyboard.led3 = n;
    visualizer_set_user_data(&user_data_keyboard);
}
