/* Copyright 2021 Joey Wu
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

enum layer_names {
    _BS,
    _FN
};

enum tap_dance_names {
    COPY_PASTE_FNLOCK_SCREENSHOT,
    INSERT_SCREENSHOT_NUMPAD
};
#define CPY_PST TD(COPY_PASTE_FNLOCK_SCREENSHOT)
#define INS_SHT TD(INSERT_SCREENSHOT_NUMPAD)

const key_override_t arterisk_override = ko_make_basic(MOD_MASK_SHIFT, KC_ASTERISK, KC_CIRCUMFLEX); // *^
const key_override_t slash_overrivve = ko_make_basic(MOD_MASK_SHIFT, KC_SLASH, KC_PERCENT);          // /%
const key_override_t left_paren_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_PAREN, KC_RIGHT_PAREN);
const key_override_t left_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LBRACKET, KC_RBRACKET);
const key_override_t left_curly_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_CURLY_BRACE, KC_RIGHT_CURLY_BRACE);
const key_override_t left_angle_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_ANGLE_BRACKET, KC_RIGHT_ANGLE_BRACKET);
const key_override_t question_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUESTION, KC_EXCLAIM);  // ?!
const key_override_t comma_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_SCOLON);         // ,;
const key_override_t dot_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLON);              // .:

const key_override_t **key_overrides = (const key_override_t *[]) {
    &arterisk_override,
    &slash_override,
    &left_paren_override,
    &left_bracket_override,
    &left_curly_bracket_override,
    &left_angle_bracket_override,
    &question_override,
    &comma_override,
    &dot_override,
    NULL // Null terminate the array of overrides!
};

#define GUI_ESC LGUI_T(KC_ESC)
#define GUI_ENT LGUI_T(KC_ENT)
#define OS_LSFT OSM(MOD_LSFT)
#define OS_RSFT OSM(MOD_RSFT)
#define FN_SPC  LT(_FN, KC_SPC)
#define FN_MINS LT(_BS, KC_MINS)
#define FN_EQL  LT(_BS, KC_EQL)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BS] = LAYOUT(
        KC_F6  , KC_QUES, CPY_PST, CPY_PST, CPY_PST, KC_BSPC,     GUI_ENT, INS_SHT, INS_SHT, INS_SHT, KC_GRV , KC_F7  ,
        KC_F5  , KC_AMPR, KC_B   , KC_G   , KC_V   , OS_LSFT,     FN_SPC , KC_K   , KC_M   , KC_J   , KC_LPRN, KC_F8  ,
        KC_F4  , KC_DLR , KC_P   , KC_T   , KC_D   , KC_LCTL,     FN_MINS, KC_H   , KC_N   , KC_L   , KC_LBRC, KC_F9  ,
        KC_F3  , KC_HASH, KC_F   , KC_S   , KC_C   , KC_LALT,     FN_EQL , KC_COMM, KC_E   , KC_U   , KC_LCBR, KC_F10 ,
        KC_F2  , KC_AT  , KC_W   , KC_R   , KC_X   , XXXXXXX,     XXXXXXX, KC_DOT , KC_I   , KC_Y   , KC_LABK, KC_F11 ,
        KC_F1  , KC_ASTR, KC_Q   , KC_A   , KC_Z   , XXXXXXX,     XXXXXXX, KC_QUOT, KC_O   , KC_SLSH, KC_BSLS, KC_F12
    ),
    [_FN] = LAYOUT(
        _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______,
        _______, _______, KC_PAST, KC_PSLS, KC_PERC, KC_PENT,     _______, _______, _______, _______, _______, _______,
        _______, _______, KC_9   , KC_6   , KC_3   , KC_0   ,     _______, _______, KC_LEFT, KC_HOME, _______, _______,
        _______, _______, KC_8   , KC_5   , KC_2   , KC_DOT ,     _______, _______, KC_DOWN, KC_UP  , _______, _______,
        _______, _______, KC_7   , KC_4   , KC_1   , XXXXXXX,     XXXXXXX, _______, KC_RGHT, KC_END , _______, _______,
        _______, _______, KC_TAB , GUI_ESC, KC_LSFT, XXXXXXX,     XXXXXXX, _______, KC_PGDN, KC_PGUP, _______, _______
    )
};

/* Behavior */
// LED configuration
void keyboard_pre_init_user(void) {
    setPinOutput(TXLED);
}

typedef enum {
    OFF, FLASH, ON
} led_status_t;

led_status_t  led_status;

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case _FN:
        led_status = ON;
        break;
    default:
        led_status = OFF;
        break;
    }
  return state;
}

bool led_pins_state = !LED_PIN_ON_STATE;

uint16_t _timer = 0;

void matrix_scan_user(void) {
    switch (led_status) {
    case ON:
        led_pins_state = LED_PIN_ON_STATE;
        break;
    case FLASH:
        if (timer_elapsed(_timer) > TAPPING_TERM) {
            led_pins_state = !led_pins_state;
            _timer = timer_read();
        }
        break;
    default:
        led_pins_state = !LED_PIN_ON_STATE;
        break;
    }
    writePin(TXLED, led_pins_state);
}

// Override configuration for specified key
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case GUI_ENT:
        return TAPPING_TERM + 50;
    case FN_SPC:
        return TAPPING_TERM - 50;
    case CPY_PST:
        return TAPPING_TERM - 75;
    default:
        return TAPPING_TERM;
    }
}

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case FN_SPC :
        return true;
    default:
        return false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_BS,KC_MINS):
            if (!record->tap.count && record->event.pressed) {
                register_mods(MOD_LCTL);
                layer_on(_FN);
                return false;
            } else {
                layer_off(_FN);
                unregister_mods(MOD_LCTL);
            }
            break;
        case LT(_BS,KC_EQL):
            if (!record->tap.count && record->event.pressed) {
                register_mods(MOD_LALT);
                layer_on(_FN);
                return false;
            } else {
                layer_off(_FN);
                unregister_mods(MOD_LALT);
            }
            break;
    }
    return true;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_ENT:
    case KC_ESC:
    case KC_GESC:
        clear_mods();
    case GUI_ENT:
    case GUI_ESC:
    default:
        return;
    }
}

/* Tap Dance */
void td_copy_paste_finished(qk_tap_dance_state_t *state, void *user_data);
void td_copy_paste_reset(qk_tap_dance_state_t *state, void *user_data);
void td_insert_screenshot_finished(qk_tap_dance_state_t *state, void *user_data);
void td_insert_screenshot_reset(qk_tap_dance_state_t *state, void *user_data);

qk_tap_dance_action_t tap_dance_actions[] = {
    [COPY_PASTE_FNLOCK_SCREENSHOT] =
            ACTION_TAP_DANCE_FN_ADVANCED(
                    NULL,
                    td_copy_paste_finished,
                    td_copy_paste_reset),
    [INSERT_SCREENSHOT_NUMPAD] =
            ACTION_TAP_DANCE_FN_ADVANCED(
                    NULL,
                    td_insert_screenshot_finished,
                    td_insert_screenshot_reset),
};

typedef enum {
     SINGLE_HOLD, SINGLE_TAP,
    TAP_AND_HOLD,   DUAL_TAP,
    TAP_TAP_HOLD, TRIPLE_TAP,
    TRI_TAP_HOLD,   QUAD_TAP,
     OTHERWISE
} td_state_t;

td_state_t current_dance(qk_tap_dance_state_t *state);

static td_state_t td_state;

void td_copy_paste_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = current_dance(state);
    switch (td_state) {
        case SINGLE_TAP:                   register_code16(C(KC_V)); return;
        case SINGLE_HOLD:                       tap_code16(C(KC_C)); return;
        case TAP_AND_HOLD:                      tap_code16(C(KC_X)); return;
        case TAP_TAP_HOLD: tap_code16(C(KC_A)); tap_code16(C(KC_C)); return;
        case TRI_TAP_HOLD: tap_code16(C(KC_A)); tap_code16(C(KC_X)); return;
        default:                                    layer_move(_BS); return;
    }
}

void td_copy_paste_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP: unregister_code16(C(KC_V)); return;
        case SINGLE_HOLD:
        case TAP_AND_HOLD:
        case TAP_TAP_HOLD:
        case TRI_TAP_HOLD:      tap_code16(C(KC_V)); return;
        default:       /* This line is necessary. */ return;
    }
}

static uint8_t mod_state;

void td_insert_screenshot_finished(qk_tap_dance_state_t *state, void *user_data) {
    mod_state = get_mods();
    if (mod_state == 0) {
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
    if (mod_state == 0) {
            switch (td_state) {
            case SINGLE_TAP: unregister_code16(LSG(KC_S)); return;
            default:         /* This line is necessary. */ return;
        }
    }
    unregister_code(KC_INS);
}

td_state_t current_dance(qk_tap_dance_state_t *state) {
    switch (state->count) {
        case 1:  return (state->interrupted || !state->pressed) ? SINGLE_TAP :  SINGLE_HOLD;
        case 2:  return (state->interrupted || !state->pressed) ?   DUAL_TAP : TAP_AND_HOLD;
        case 3:  return (state->interrupted || !state->pressed) ? TRIPLE_TAP : TAP_TAP_HOLD;
        case 4:  return (state->interrupted || !state->pressed) ?   QUAD_TAP : TRI_TAP_HOLD;
        default: return OTHERWISE;
    }
}
