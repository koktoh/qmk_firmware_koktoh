/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"
#include "config.h"
#include "split_common/split_util.h"
#include "split_common/transactions.h"

// #define ERROR_DISCONNECT_COUNT 5

#define ROWS_PER_HAND (MATRIX_ROWS / 2)

#ifndef MATRIX_IO_DELAY
#    define MATRIX_IO_DELAY 30
#endif

#define SPLIT_MUTABLE_ROW const
#define SPLIT_MUTABLE_COL const

static SPLIT_MUTABLE_ROW pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static SPLIT_MUTABLE_COL pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

/* matrix state(1:on, 0:off) */
static matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
static matrix_row_t matrix[MATRIX_ROWS];      // debounced values

// row offsets for each hand
uint8_t thisHand, thatHand;

// user-defined overridable functions
__attribute__((weak)) void matrix_init_pins(void);
__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row);
__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter);
__attribute__((weak)) void matrix_init_quantum(void) { matrix_init_kb(); }
__attribute__((weak)) void matrix_scan_quantum(void) { matrix_scan_kb(); }
__attribute__((weak)) void matrix_init_kb(void) { matrix_init_user(); }
__attribute__((weak)) void matrix_scan_kb(void) { matrix_scan_user(); }
__attribute__((weak)) void matrix_init_user(void) {}
__attribute__((weak)) void matrix_scan_user(void) {}
__attribute__((weak)) void matrix_slave_scan_kb(void) { matrix_slave_scan_user(); }
__attribute__((weak)) void matrix_slave_scan_user(void) {}

/*　`matrix_io_delay ()` exists for backwards compatibility. From now on, use matrix_output_unselect_delay().　*/
__attribute__((weak)) void matrix_io_delay(void) { wait_us(MATRIX_IO_DELAY); }
__attribute__((weak)) void matrix_output_select_delay(void) { waitInputPinDelay(); }
__attribute__((weak)) void matrix_output_unselect_delay(uint8_t line, bool key_pressed) { matrix_io_delay(); }

// helper functions

inline uint8_t matrix_rows(void) { return MATRIX_ROWS; }

inline uint8_t matrix_cols(void) { return MATRIX_COLS; }

static inline void setPinOutput_writeLow(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinLow(pin);
    }
}

static inline void setPinOutput_writeHigh(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinHigh(pin);
    }
}

static inline void setPinInputHigh_atomic(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinInputHigh(pin);
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return readPin(pin);
    } else {
        return 1;
    }
}

inline bool matrix_is_on(uint8_t row, uint8_t col) { return (matrix[row] & ((matrix_row_t)1 << col)); }

inline matrix_row_t matrix_get_row(uint8_t row) { return matrix[row]; }

#if (MATRIX_COLS <= 8)
#    define print_matrix_header() print("\nr/c 01234567\n")
#    define print_matrix_row(row) print_bin_reverse8(matrix_get_row(row))
#    define matrix_bitpop(row) bitpop(matrix_get_row(row))
#elif (MATRIX_COLS <= 16)
#    define print_matrix_header() print("\nr/c 0123456789ABCDEF\n")
#    define print_matrix_row(row) print_bin_reverse16(matrix_get_row(row))
#    define matrix_bitpop(row) bitpop16(matrix_get_row(row))
#elif (MATRIX_COLS <= 32)
#    define print_matrix_header() print("\nr/c 0123456789ABCDEF0123456789ABCDEF\n")
#    define print_matrix_row(row) print_bin_reverse32(matrix_get_row(row))
#    define matrix_bitpop(row) bitpop32(matrix_get_row(row))
#endif

void matrix_print(void) {
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        print_hex8(row);
        print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

uint8_t matrix_key_count(void) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += matrix_bitpop(i);
    }
    return count;
}

// matrix code

static bool select_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        setPinOutput_writeLow(pin);
        return true;
    }
    return false;
}

static void unselect_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
#            ifdef MATRIX_UNSELECT_DRIVE_HIGH
        setPinOutput_writeHigh(pin);
#            else
        setPinInputHigh_atomic(pin);
#            endif
    }
}

static void unselect_rows(void) {
    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        unselect_row(x);
    }
}

static bool select_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
        setPinOutput_writeLow(pin);
        return true;
    }
    return false;
}

static void unselect_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
#            ifdef MATRIX_UNSELECT_DRIVE_HIGH
        setPinOutput_writeHigh(pin);
#            else
        setPinInputHigh_atomic(pin);
#            endif
    }
}

static void unselect_cols(void) {
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        unselect_col(x);
    }
}

__attribute__((weak)) void matrix_init_pins(void) {
    unselect_rows();
    unselect_cols();

    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        if (col_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(col_pins[x]);
        }
    }

    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        if (row_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(row_pins[x]);
        }
    }
}

__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    if (!select_row(current_row)) { // Select row
        return;                     // skip NO_PIN row
    }
    matrix_output_select_delay();

    // For each col...
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
        uint8_t pin_state = readMatrixPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : row_shifter;
    }

    // Unselect row
    unselect_row(current_row);
    matrix_output_unselect_delay(current_row, current_row_value != 0); // wait for all Col signals to go HIGH

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {
    bool key_pressed = false;

    // Select col
    if (!select_col(current_col)) { // select col
        return;                     // skip NO_PIN col
    }
    matrix_output_select_delay();

    // For each row...
    for (uint8_t row_index = 0; row_index < ROWS_PER_HAND / 2; row_index++) {
        uint8_t virtual_row_index = row_index + ROWS_PER_HAND / 2;

        // Check row pin state
        if (readMatrixPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[virtual_row_index] |= row_shifter;
            key_pressed = true;
        } else {
            // Pin HI, clear col bit
            current_matrix[virtual_row_index] &= ~row_shifter;
        }
    }


    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed); // wait for all Row signals to go HIGH
}

void matrix_init(void) {
    split_pre_init();

    // Set pinout for right half if pinout for that half is defined
    // if (!isLeftHand) {
    //     const pin_t row_pins_right[MATRIX_ROWS] = MATRIX_ROW_PINS;
    //     for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    //         row_pins[i] = row_pins_right[i];
    //     }

    //     const pin_t col_pins_right[MATRIX_COLS] = MATRIX_COL_PINS;
    //     for (uint8_t i = 0; i < MATRIX_COLS; i++) {
    //         col_pins[i] = col_pins_right[i];
    //     }
    // }

    thisHand = isLeftHand ? 0 : (ROWS_PER_HAND);
    thatHand = ROWS_PER_HAND - thisHand;

    // initialize key pins
    matrix_init_pins();

    // initialize matrix state: all keys off
    memset(matrix, 0, sizeof(matrix));
    memset(raw_matrix, 0, sizeof(raw_matrix));
    // for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    //     raw_matrix[i] = 0;
    //     matrix[i]     = 0;
    // }

    debounce_init(ROWS_PER_HAND);

    matrix_init_quantum();

    split_post_init();
}

__attribute__((weak)) bool transport_master_if_connected(matrix_row_t master_matrix[], matrix_row_t slave_matrix[]) {
    transport_master(master_matrix, slave_matrix);
    return true;  // Treat the transport as always connected
}

bool matrix_post_scan(void) {
    bool changed = false;
    if (is_keyboard_master()) {
        static bool last_connected = false;
        matrix_row_t   slave_matrix[ROWS_PER_HAND] = {0};

        if (transport_master_if_connected(matrix + thatHand, slave_matrix)) {
            changed = memcmp(matrix + thatHand, slave_matrix, sizeof(slave_matrix)) != 0;

            last_connected = true;
        } else if (last_connected) {
            memset(slave_matrix, 0, sizeof(slave_matrix));
            changed = true;

            last_connected = false;
        }

        if (changed) memcpy(matrix + thatHand, slave_matrix, sizeof(slave_matrix));

        matrix_scan_quantum();
    } else {
        transport_slave(matrix + thatHand, matrix + thisHand);

        matrix_slave_scan_kb();
    }

    return changed;
}

uint8_t matrix_scan(void) {
    matrix_row_t curr_matrix[MATRIX_ROWS] = {0};

    // Set row, read cols
    for (uint8_t current_row = 0; current_row < ROWS_PER_HAND / 2; current_row++) {
        matrix_read_cols_on_row(curr_matrix, current_row);
    }

    // Set col, read rows
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        matrix_read_rows_on_col(curr_matrix, current_col, row_shifter);
    }

    bool changed = memcmp(raw_matrix, curr_matrix, sizeof(curr_matrix)) != 0;
    if (changed) memcpy(raw_matrix, curr_matrix, sizeof(curr_matrix));

    debounce(raw_matrix, matrix + thisHand, ROWS_PER_HAND, changed);
    changed = (changed || matrix_post_scan());

    return (uint8_t)changed;
}
