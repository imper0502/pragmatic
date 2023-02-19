/* Copyright 2021 James Sa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include "layer.h"

#include "key_override.h"

#include "tap_dance.h"

#ifdef COMBO_ENABLE
#    include "combo.h"
#endif

#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif

#include "keycode_user.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BS] = LAYOUT(
        CAPSWRD, KC_CIRC, KC_AT  , KC_HASH, KC_DLR , KC_AMPR,     KC_ASTR, KC_QUES, KC_EXLM, KC_GRV , KC_TILD, CAPSWRD,
        KC_DEL , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B   ,     KC_J   , KC_L   , KC_U   , KC_Y   , KC_SLSH, KC_BSLS,
        GUI_ESC, KC_A   , KC_R   , KC_S   , KC_T   , KC_G   ,     KC_M   , KC_N   , KC_E   , KC_I   , KC_O   , GUI_TAB,
        INS_PPS, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V   ,     KC_K   , KC_H   , KC_COMM, KC_DOT , KC_QUOT, KC_RSFT,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, ALT_LYS, KC_LCTL, OS_LSFT, KC_BSPC,     FN_ENT , FN_SPC , FN_MINS, FN_EQL , XXXXXXX, XXXXXXX 
    ),
    [_FN] = LAYOUT(
        KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  ,     KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,
        _______, KC_PPLS, KC_7   , KC_8   , KC_9   , KC_PAST,     PRV_WIN, KC_HOME, KC_UP  , KC_END , KC_PGUP, NXT_WIN,
        _______, KC_PMNS, KC_4   , KC_5   , KC_6   , KC_PSLS,A(S(KC_TAB)), KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, ALT_TAB,
        _______, KC_EQL , KC_1   , KC_2   , KC_3   , KC_PERC,     PRV_TAB, KC_LBRC, KC_LABK, KC_RABK, KC_RBRC, NXT_TAB,
        _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______,
        _______, _______, KC_DOT , KC_0   , KC_PENT, _______,     _______, TO(_BS), KC_RCTL, KC_RALT, _______, _______
    ),
    [CMD] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, CG_NORM, TG(_FN), KC_PSCR, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX,  RESET , KC_SLCK, CG_TOGG, XXXXXXX,     CG_SWAP, KC_NLCK, EEP_RST, EEP_RST, CG_TOGG, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
    )
};

/* LED Behavior */
void keyboard_pre_init_user(void) {
    setPinOutput(TXLED);
    setPinOutput(RXLED);
}

typedef enum {
    OFF, FLASH, ON
} led_status_t;

led_status_t  rx_led_status;
led_status_t  tx_led_status;
layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _FN:
        tx_led_status = ON;
        break;
    case CMD:
        tx_led_status = FLASH;
        rx_led_status = FLASH;
        break;
    default:
        tx_led_status = OFF;
        rx_led_status = OFF;
        break;
    }
  return state;
}

uint16_t _timer = 0;
bool next_led_pins_state = !LED_PIN_ON_STATE;
void matrix_scan_user(void) {
    switch (tx_led_status) {
    case ON:
        next_led_pins_state = LED_PIN_ON_STATE;
        break;
    case FLASH:
        if (timer_elapsed(_timer) > TAPPING_TERM) {
            next_led_pins_state = !next_led_pins_state;
            _timer = timer_read();
        }
        break;
    default:
        next_led_pins_state = !LED_PIN_ON_STATE;
        break;
    }
    switch (rx_led_status) {
    case FLASH:
        writePin(RXLED, !next_led_pins_state);
        break;
    default:
        writePin(RXLED, !host_keyboard_led_state().caps_lock);
        break;
    }
    writePin(TXLED, next_led_pins_state);
}

/* Key Behavior */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case FN_SPC:
        return TAPPING_TERM - 25;
    case INS_PPS:
        return TAPPING_TERM - 75;
    case ALT_TAB:
        return TAPPING_TERM + 500;
    default:
        return TAPPING_TERM;
    }
}

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case FN_SPC:
    case ALT_LYS:
        return true;
    default:
        return false;
    }
}

#ifdef CONSOLE_ENABLE
void keyboard_post_init_user(void) {
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
    dprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n",
            keycode,
            record->event.key.col,
            record->event.key.row,
            record->event.pressed,
            record->event.time,
            record->tap.interrupted,
            record->tap.count);
#endif
    switch (keycode) {
    case LT(0, KC_ENT):
        if (!record->tap.count) {
            if (record->event.pressed) {
                register_mods(MOD_LSFT);
                layer_on(_FN);
            }else {
                layer_off(_FN);
                unregister_mods(MOD_LSFT);
            }
            return false;
        }
        break;
    case LT(0, KC_MINS):
        if (!record->tap.count) {
            if (record->event.pressed) {
                register_mods(MOD_LCTL);
                layer_on(_FN);
            }else {
                layer_off(_FN);
                unregister_mods(MOD_LCTL);
            }
            return false;
        }
        break;
    case LT(0, KC_EQL):
        if (!record->tap.count) {
            if (record->event.pressed) {
                register_mods(MOD_LALT);
                layer_on(_FN);
            } else {
                layer_off(_FN);
                unregister_mods(MOD_LALT);
            }
            return false;
        }
        break;
    }
    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case LT(0, KC_ENT):
    case KC_LSFT:
    case KC_RSFT:
    case OS_LSFT:
    case OS_RSFT:
        get_mods() == MOD_MASK_SHIFT ? layer_on(CMD) : layer_off(CMD);
        break;
    case KC_ESC:
    case KC_GESC:
    case KC_CAPS:
        clear_mods();
    case GUI_ESC:
    default:
        return;
    }
}

/* Tap Dance */
void td_alt_tab_each_tap(qk_tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1: add_mods(MOD_BIT(KC_LALT));
        default:          tap_code(KC_TAB);
    }
}

void td_alt_tab_finished(qk_tap_dance_state_t *state, void *user_data) {
    del_mods(MOD_BIT(KC_LALT));
}

static td_state_t td_state;

void td_alt_layers_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = current_dance(state);
    switch (td_state) {
        case SINGLE_TAP:
        case SINGLE_HOLD: register_mods(MOD_BIT(KC_LALT)); return;
        case DUAL_TAP:             register_code(KC_CAPS); return;
        case TAP_THEN_HOLD:             layer_invert(_FN); return;
        default:             /* This line is necessary. */ return;
    }
}

void td_alt_layers_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:
        case SINGLE_HOLD: unregister_mods(MOD_BIT(KC_LALT)); return;
        case DUAL_TAP:             unregister_code(KC_CAPS); return;
        case TAP_THEN_HOLD:               layer_invert(_FN); return;
        default:               /* This line is necessary. */ return;
    }
}

uint8_t td_mod_state;
void td_copy_paste_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_mod_state = get_mods();
    if (td_mod_state == 0) {
        td_state = current_dance(state);
        switch (td_state) {
            case SINGLE_TAP: register_code16(C(KC_V)); return;
            case SINGLE_HOLD:     tap_code16(C(KC_C)); return;
            case TAP_THEN_HOLD:   tap_code16(C(KC_X)); return;
            case DUAL_TAP:          layer_invert(_FN); return;
            default:                  layer_move(_BS); return;
        }
    } else if (td_mod_state & MOD_MASK_GUI) {
        switch (td_state) {
            case SINGLE_TAP: register_code16(S(KC_S)); return;
            default:     /* This line is necessary. */ return;
        }
    }
    register_code(KC_INS);
}

void td_copy_paste_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (td_mod_state == 0) {
        switch (td_state) {
            case SINGLE_TAP: unregister_code16(C(KC_V)); return;
            case SINGLE_HOLD:
            case TAP_THEN_HOLD:     tap_code16(C(KC_V)); return;
            default:       /* This line is necessary. */ return;
        }
    } else if (td_mod_state & MOD_MASK_GUI)  {
        switch (td_state) {
            case SINGLE_TAP: unregister_code16(S(KC_S)); return;
            default:     /* This line is necessary. */   return;
        }
    }
    unregister_code(KC_INS);
}

void td_insert_screenshot_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (get_mods() == 0) {
            td_state = current_dance(state);
            switch (td_state) {
            case SINGLE_TAP: register_code16(LSG(KC_S)); return;
            case DUAL_TAP:            layer_invert(_FN); return;
            default:       /* This line is necessary. */ return;
        }
    }
    register_code(KC_INS);
}

void td_insert_screenshot_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (get_mods() == 0) {
            switch (td_state) {
            case SINGLE_TAP: unregister_code16(LSG(KC_S)); return;
            default:         /* This line is necessary. */ return;
        }
    }
    unregister_code(KC_INS);
}

/* Retry Encoders */
#ifdef ENCODER_ENABLE
uint8_t mod_state;
bool encoder_update_user(uint8_t index, bool clockwise) {
    mod_state = get_mods();
    switch (index) {
    case 0:
        if (IS_LAYER_ON(_FN)) {
            if (mod_state & MOD_MASK_CS) {
                clockwise ? tap_code(KC_BRIU) : tap_code(KC_BRID);
            } else {
                clockwise ? tap_code(KC_VOLU) : tap_code(KC_VOLD);
            }
        } else {
            if (mod_state == (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT)) ||
                mod_state == (MOD_BIT(KC_LCTL) | MOD_BIT(KC_RSFT)) ||
                mod_state == (MOD_BIT(KC_RCTL) | MOD_BIT(KC_LSFT)) ||
                mod_state == (MOD_BIT(KC_RCTL) | MOD_BIT(KC_RSFT))) {
                del_mods(MOD_MASK_CS);
                clockwise ? tap_code(KC_PGDN) : tap_code(KC_PGUP);
                set_mods(mod_state);
            } else if (mod_state & MOD_MASK_ALT) {
                del_mods(MOD_MASK_ALT);
                clockwise ? tap_code(KC_BTN5) : tap_code(KC_BTN4);
                set_mods(mod_state);
            } else if (mod_state & MOD_MASK_SG) {
                clockwise ? tap_code16(C(KC_RGHT)) : tap_code16(C(KC_LEFT));
            } else if (mod_state & MOD_MASK_CTRL) {
                clockwise ? tap_code(KC_Y) : tap_code(KC_Z);
            } else {
                clockwise ? tap_code(KC_WH_D) : tap_code(KC_WH_U);
            }
        }
        break;
    }
    return false;
}
#endif
