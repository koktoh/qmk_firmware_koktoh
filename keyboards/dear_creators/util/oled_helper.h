#include "quantum.h"
#include <oled_driver.h>

#define LAYER_FONT_WIDTH 16
#define LAYER_FONT_HEIGHT 21

void render_logo(void);
void render_layer_state(uint8_t layer_state, uint8_t x, uint8_t y);

static const char PROGMEM vs_logo[] = {
    0x80, 0x81, 0x82, 0x83, 0x84,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0x00
};

static const uint16_t PROGMEM layer_fonts[][LAYER_FONT_HEIGHT] =
{
    { 0x0FF0, 0x1FF8, 0x381C, 0x700E, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x6006, 0x700E, 0x381C, 0x1FF8, 0x0FF0 },
    { 0x0180, 0x0180, 0x01F0, 0x01F0, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x3FFC, 0x3FFC },
    { 0x0FF0, 0x1FF8, 0x381C, 0x300C, 0x700E, 0x6006, 0x6006, 0x7006, 0x3000, 0x3800, 0x1C00, 0x0E00, 0x0700, 0x0380, 0x01C0, 0x00E0, 0x0070, 0x0038, 0x001C, 0x7FFE, 0x7FFE },
    { 0x0FF0, 0x1FF8, 0x381C, 0x300C, 0x700E, 0x6006, 0x6006, 0x6006, 0x7000, 0x3FC0, 0x1FC0, 0x3000, 0x7000, 0x6006, 0x6006, 0x6006, 0x700E, 0x300C, 0x381C, 0x1FF8, 0x0FF0 },
    { 0x0700, 0x0780, 0x0780, 0x07C0, 0x06C0, 0x06E0, 0x0660, 0x0670, 0x0630, 0x0638, 0x0618, 0x061C, 0x060C, 0x7FFE, 0x7FFE, 0x0600, 0x0600, 0x0600, 0x0600, 0x0600, 0x0600 },
    { 0x3FFE, 0x3FFE, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0006, 0x0FF6, 0x1FFE, 0x381E, 0x300E, 0x7006, 0x6000, 0x6000, 0x6006, 0x700E, 0x300C, 0x381C, 0x1FF8, 0x0FF0 },
    { 0x0FF0, 0x1FF8, 0x381C, 0x700C, 0x600E, 0x6006, 0x0006, 0x0006, 0x0FF6, 0x1FFE, 0x381E, 0x300E, 0x700E, 0x6006, 0x6006, 0x6006, 0x700E, 0x300C, 0x381C, 0x1FF8, 0x0FF0 },
    { 0x7FFE, 0x7FFE, 0x6006, 0x6006, 0x7000, 0x3800, 0x1C00, 0x0C00, 0x0E00, 0x0600, 0x0600, 0x0700, 0x0300, 0x0300, 0x0300, 0x0380, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180 },
    { 0x0FF0, 0x1FF8, 0x381C, 0x300C, 0x300C, 0x300C, 0x381C, 0x1C38, 0x0FF0, 0x1FF8, 0x381C, 0x300C, 0x700E, 0x6006, 0x6006, 0x6006, 0x700E, 0x300C, 0x381C, 0x1FF8, 0x0FF0 },
    { 0x0FF0, 0x1FF8, 0x381C, 0x300C, 0x700E, 0x6006, 0x6006, 0x6006, 0x700E, 0x700C, 0x781C, 0x7FF8, 0x6FF0, 0x6000, 0x6000, 0x6006, 0x7006, 0x300E, 0x381C, 0x1FF8, 0x0FF0 }
};
