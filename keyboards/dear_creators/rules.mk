# MCU name
MCU = atmega32u4

# Bootloader selection
# BOOTLOADER = caterina       # Pro Micro
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes       # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = no            # Enable N-Key Rollover
BACKLIGHT_ENABLE = yes      # Enable keyboard backlight functionality
BACKLIGHT_DRIVER = pwm
RGBLIGHT_ENABLE = yes       # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output

VIA_ENABLE = yes

SPLIT_KEYBOARD = yes
ENCODER_ENABLE = yes
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
CUSTOM_MATRIX = yes
LTO_ENABLE = yes

SRC += matrix.c \
       ./util/encoder_action.c \
	   ./util/oled_helper.c
