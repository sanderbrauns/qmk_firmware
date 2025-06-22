// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "uart.h"
#include "common/smart_ble.h"
#include "al68.h"

void keyboard_pre_init_kb(void) {
    AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_SWJ_CFG_Msk);
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_DISABLE; // diable JTAG (GD32 different to STM32)
    gpio_set_pin_output(A8);
    gpio_write_pin_high(A8); // ENABLE USB
    uart_init(460800);
    wait_ms(400);
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    if (record->event.pressed) {
        if ((keycode >= KC_BLE1) && (keycode <= KC_24G)) {
            wireless_start(keycode - KC_USB);
            return false;
        } else if (keycode == KC_USB) {
            wireless_stop();
            return false;
        }
    }
    return process_record_user(keycode, record);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  //  uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
  return true;
}


bool rgb_matrix_indicators_user(void) {



    if (host_keyboard_led_state().caps_lock) {
        // Caps Lock is ON, turn LED 10 red
        // Make sure you're using the correct LED index for your AL68's Caps Lock.
        rgb_matrix_set_color(3, 255, 0, 0); // Example LED index, replace with actual
        rgb_matrix_set_color(4, 0, 0, 255); // Example LED index, replace with actual


      } else {
        // Caps Lock is OFF, turn LED 10 off
        rgb_matrix_set_color(3, 0, 0, 0);
        rgb_matrix_set_color(4, 0, 0, 0);
    }
    // Return true to indicate that this function handled the indicator display.
    // Returning true generally means "this function handled the indicator part,
    // and the RGB Matrix core shouldn't try to apply its default indicator behavior."
    return true;
}
