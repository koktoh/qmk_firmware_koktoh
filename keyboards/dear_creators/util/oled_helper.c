#include "oled_helper.h"

void render_logo(void)
{
    oled_write_P(vs_logo, false);
}

void oled_write_layer(uint8_t num, uint8_t x, uint8_t y) {
    const uint16_t* const character = layer_fonts[num];

    for (int offset_y = 0; offset_y < LAYER_FONT_HEIGHT; offset_y++) {
        uint16_t data = character[offset_y];

        for (int offset_x = 0; offset_x < LAYER_FONT_WIDTH; offset_x++) {
            bool pixel = data >> offset_x & 1;

            oled_write_pixel(x + offset_x, y + offset_y, pixel);
        }
    }
}

void render_layer_state(uint8_t layer_state, uint8_t x, uint8_t y) {
    uint8_t first_digit = layer_state % 10;
    uint8_t second_digit = layer_state / 10;

    oled_write_layer(second_digit, x, y);
    oled_write_layer(first_digit, x + LAYER_FONT_WIDTH, y);
}
