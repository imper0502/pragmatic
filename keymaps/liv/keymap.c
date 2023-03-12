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
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        INS_PLS, KC_CIRC, KC_AT  , KC_HASH, KC_DLR , KC_AMPR,     KC_ASTR, KC_QUES, KC_EXLM, KC_GRV , KC_TILD, KC_DEL ,
        KC_TAB , KC_Q   , KC_W   , KC_F   , KC_P   , KC_B   ,     KC_J   , KC_L   , KC_U   , KC_Y   , KC_MINS, KC_EQL ,
        GUI_ESC, KC_A   , KC_R   , KC_S   , KC_T   , KC_G   ,     KC_M   , KC_N   , KC_E   , KC_I   , KC_O   , KC_BSPC,
        KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_D   , KC_V   ,     KC_K   , KC_H   , KC_COMM, KC_DOT , KC_QUOT, KC_RSFT,
        XXXXXXX, XXXXXXX, XXXXXXX, FN_BSLS, FN_SLSH, FN_SPC ,     OS_RSFT, KC_LCTL, ALT_PLS, XXXXXXX, XXXXXXX, XXXXXXX 
    ),
    [_FN] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
      TD(TD_F1), KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  ,     KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,
        KC_PGUP, KC_HOME, KC_UP  , KC_END , KC_LBRC, KC_RBRC,     KC_PAST, KC_7   , KC_8   , KC_9   , KC_PPLS, KC_TAB ,
        KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_LABK, KC_RABK,     KC_PSLS, KC_4   , KC_5   , KC_6   , KC_PMNS, _______,
        _______, _______, _______, _______, _______, _______,     KC_PERC, KC_1   , KC_2   , KC_3   , KC_EQL , _______,
        XXXXXXX, XXXXXXX, XXXXXXX, KC_LALT, KC_LCTL, TO(_BS),     ST_ENT , KC_0   , KC_DOT , XXXXXXX, XXXXXXX, XXXXXXX
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
    case INS_PLS:
        return TAPPING_TERM - 75;
    default:
        return TAPPING_TERM;
    }
}

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case FN_SPC:
    case ALT_PLS:
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
    case FN_ENT:
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
    case FN_MINS:
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
    case FN_EQL:
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
    case FN_SLSH:
        if (!record->tap.count) {
            if (record->event.pressed) {
                register_mods(MOD_LCTL);
                layer_on(_FN);
            } else {
                layer_off(_FN);
                unregister_mods(MOD_LCTL);
            }
            return false;
        } else {
            if (get_mods() & MOD_MASK_SHIFT) {
                if (record->event.pressed) {
                    register_code16(KC_PERCENT);
                } else {
                    unregister_code16(KC_PERCENT);
                }
                return false;
            } else if (get_oneshot_mods() & MOD_MASK_SHIFT) {
                if (record->event.pressed) {
                    clear_oneshot_mods();
                    tap_code16(KC_PERCENT);
                }
                return false;
            }
        }
        break;
    case FN_BSLS:
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
    case KC_ESC:
    case KC_GESC:
    case KC_CAPS:
        clear_mods();
    default:
        return;
    }
}

/* Tap Dance */
static td_state_t td_state;

void td_alt_plus_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = current_dance(state);
    switch (td_state) {
        case SINGLE_TAP:
        case SINGLE_HOLD: register_mods(MOD_BIT(KC_LALT)); return;
        case DUAL_TAP:             register_code(KC_CAPS); return;
        case TAP_THEN_HOLD:             layer_invert(_FN); return;
        default:             /* This line is necessary. */ return;
    }
}

void td_alt_plus_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:
        case SINGLE_HOLD: unregister_mods(MOD_BIT(KC_LALT)); return;
        case DUAL_TAP:             unregister_code(KC_CAPS); return;
        case TAP_THEN_HOLD:               layer_invert(_FN); return;
        default:               /* This line is necessary. */ return;
    }
}

uint8_t mod_state;
void td_insert_plus_finished(qk_tap_dance_state_t *state, void *user_data) {
    mod_state = get_mods();
    td_state = current_dance(state);
    if (mod_state == 0) {
        switch (td_state) {
            case SINGLE_TAP: register_code16(C(KC_V)); return;
            case SINGLE_HOLD:     tap_code16(C(KC_C)); return;
            case TAP_THEN_HOLD:   tap_code16(C(KC_X)); return;
            case DUAL_TAP:          layer_invert(_FN); return;
            default:                  layer_move(_BS); return;
        }
    } else if (mod_state & MOD_MASK_GUI) {
        switch (td_state) {
            case SINGLE_TAP: register_code16(LSG(KC_S)); return;
            default:       /* This line is necessary. */ return;
        }
    }
    register_code(KC_INS);
}

void td_insert_plus_reset(qk_tap_dance_state_t *state, void *user_data) {
    if (mod_state == 0) {
        switch (td_state) {
            case SINGLE_TAP: unregister_code16(C(KC_V)); return;
            case SINGLE_HOLD:
            case TAP_THEN_HOLD:     tap_code16(C(KC_V)); return;
            default:       /* This line is necessary. */ return;
        }
    } else if (mod_state & MOD_MASK_GUI)  {
        switch (td_state) {
            case SINGLE_TAP: unregister_code16(LSG(KC_S)); return;
            default:       /* This line is necessary. */   return;
        }
    } else if (mod_state & MOD_MASK_SHIFT) {
        clear_mods();
        return; 
    }
    unregister_code(KC_INS);
}

void td_f1_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = current_dance(state);
    switch (td_state) {
        case SINGLE_TAP: register_code(KC_F1); return;
        case DUAL_TAP:      layer_invert(_FN); return;
        default:              layer_move(_BS); return;
    }
}

void td_f1_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP: unregister_code(KC_F1); return;
        default:   /* This line is necessary. */ return;
    }
}
