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

enum layer_names {
    _BS,
    _QW,
    _FN
};

enum tap_dance_names {
    IME_CAPSLOCK,
    ALT_TABLE,
    COPY_PASTE_FNLOCK_SCREENSHOT,
    INSERT_SCREENSHOT_NUMPAD
};

/* Key Override */
const key_override_t arterisk_override = ko_make_basic(MOD_MASK_SHIFT, KC_ASTERISK, KC_CIRCUMFLEX); // *^
const key_override_t at_override =       ko_make_basic(MOD_MASK_SHIFT, KC_AT, KC_DOLLAR);           // @$
const key_override_t hash_override =     ko_make_basic(MOD_MASK_SHIFT, KC_HASH, KC_DOLLAR);         // #$
const key_override_t ampersand_override = ko_make_basic(MOD_MASK_SHIFT, KC_AMPERSAND, KC_DOLLAR);   // &$
const key_override_t slash_override =    ko_make_basic(MOD_MASK_SHIFT, KC_SLASH, KC_PERCENT);       // /%
const key_override_t left_paren_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_PAREN, KC_RIGHT_PAREN);
const key_override_t left_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LBRACKET, KC_RBRACKET);
const key_override_t left_curly_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_CURLY_BRACE, KC_RIGHT_CURLY_BRACE);
const key_override_t left_angle_bracket_override = ko_make_basic(MOD_MASK_SHIFT, KC_LEFT_ANGLE_BRACKET, KC_RIGHT_ANGLE_BRACKET);
const key_override_t question_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUESTION, KC_EXCLAIM);  // ?!
const key_override_t comma_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_SCOLON);         // ,;
const key_override_t dot_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLON);              // .:

const key_override_t **key_overrides = (const key_override_t *[]) {
    &arterisk_override,
    &at_override,
    &hash_override,
    &ampersand_override,
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

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    COLEMAK = SAFE_RANGE,   // colemak without home row modifier
    COLEMAKm,               // colemak with home row modifier
    DVORAK,
    QWERTY
};

#define TD_LANG TD(IME_CAPSLOCK)
#define CPY_PST TD(COPY_PASTE_FNLOCK_SCREENSHOT)
#define INS_SHT TD(INSERT_SCREENSHOT_NUMPAD)
#define GUI_ESC LGUI_T(KC_ESC)
#define OS_LSFT OSM(MOD_LSFT)
#define OS_RSFT OSM(MOD_RSFT)
#define FN_SPC  LT(_FN, KC_SPC)
#define FN_RCTL LM(_FN, MOD_RCTL)
#define FN_RALT LM(_FN, MOD_RALT)
// window management utility

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BS] = LAYOUT(
        KC_F6  , KC_SLSH, KC_B   , KC_G   , KC_V   , KC_E   ,     KC_SPC , KC_K   , KC_M   , KC_J   , KC_BSLS, KC_F7  ,
        KC_F5  , KC_AMPR, KC_P   , KC_T   , KC_D   , KC_LCTL,     MO(_FN), KC_H   , KC_N   , KC_L   , KC_LPRN, KC_F8  ,
        KC_F4  , KC_HASH, KC_F   , KC_S   , KC_C   , KC_LALT,     _______, KC_COMM, KC_I   , KC_U   , KC_LBRC, KC_F9  ,
        KC_F3  , KC_AT  , KC_W   , KC_R   , KC_X   , TG(_FN),     TG(_QW), KC_DOT , KC_O   , KC_Y   , KC_LCBR, KC_F10 ,
        KC_F2  , KC_ASTR, KC_Q   , KC_A   , KC_Z   , CPY_PST,     INS_SHT, KC_QUOT, KC_QUES, KC_MINS, KC_LABK, KC_F11 ,
        KC_F1  , KC_GRV , KC_TAB , GUI_ESC, OS_LSFT, KC_DEL ,     KC_BSPC, OS_RSFT, KC_ENT , KC_EQL , TD_LANG, KC_F12 
    ),
    [_QW] = LAYOUT(
        KC_F6  , KC_SLSH, KC_T   , KC_G   , KC_B   , KC_LSFT,     KC_SPC , KC_N   , KC_H   , KC_Y   , KC_BSLS, KC_F7  ,
        KC_F5  , KC_AMPR, KC_R   , KC_F   , KC_V   , KC_LCTL,     _______, KC_M   , KC_J   , KC_U   , KC_LPRN, KC_F8  ,
        KC_F4  , KC_HASH, KC_E   , KC_D   , KC_C   , KC_LALT,     _______, KC_COMM, KC_K   , KC_I   , KC_LBRC, KC_F9  ,
        KC_F3  , KC_AT  , KC_W   , KC_S   , KC_X   , CPY_PST,     _______, KC_DOT , KC_L   , KC_O   , KC_LCBR, KC_F10 ,
        KC_F2  , KC_ASTR, KC_Q   , KC_A   , KC_Z   , KC_DEL ,     INS_SHT, KC_QUOT, KC_P   , KC_MINS, KC_LABK, KC_F11 ,
        KC_F1  , KC_GRV , KC_TAB , GUI_ESC, OS_LSFT, KC_BSPC,     KC_ENT , KC_RSFT, KC_QUES, KC_EQL , TD_LANG, KC_F12 
    ),
    [_FN] = LAYOUT(
        _______, _______, KC_PAST, KC_PSLS, KC_PERC, _______,     _______, KC_MUTE, KC_BRID, KC_BRIU, _______, _______,
        _______, _______, KC_9   , KC_6   , KC_3   , _______,     _______, KC_VOLD, KC_LEFT, KC_HOME, _______, _______,
        _______, _______, KC_8   , KC_5   , KC_2   , _______,     _______, KC_VOLU, KC_DOWN, KC_UP  , _______, _______,
        _______, _______, KC_7   , KC_4   , KC_1   , _______,     _______, KC_MPRV, KC_RGHT, KC_END , _______, _______,
        _______, _______, KC_PPLS, KC_PMNS, KC_EQL , _______,     _______, KC_MPLY, KC_PGDN, KC_PGUP, _______, _______,
        _______, _______, KC_TAB , GUI_ESC, KC_LSFT, _______,     _______, KC_MNXT, KC_DEL , KC_BSPC, _______, _______
    )
};

/* Behavior */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case FN_SPC:
        return TAPPING_TERM - 25;
    case CPY_PST:
        return TAPPING_TERM - 75;
    case TD(ALT_TABLE):
        return TAPPING_TERM + 500;
    default:
        return TAPPING_TERM;
    }
}

bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case FN_SPC :
    case FN_RCTL:
    case FN_RALT:
        return true;
    default:
        return false;
    }
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case KC_ESC:
    case KC_GESC:
        clear_mods();
    case GUI_ESC:
    default:
        return;
    }
}

/* Tap Dance */
typedef enum {
    SINGLE_TAP, SINGLE_HOLD,
    DOUBLE_TAP, TAP_THEN_HOLD,
    OTHERWISE
} td_state_t;

td_state_t current_dance(qk_tap_dance_state_t *state) {
    switch (state->count) {
        case 1:  return (state->interrupted || !state->pressed) ? SINGLE_TAP : SINGLE_HOLD;
        case 2:  return (state->interrupted || !state->pressed) ? DOUBLE_TAP : TAP_THEN_HOLD;
        default: return OTHERWISE;
    }
}

void td_alt_tab_each_tap(qk_tap_dance_state_t *state, void *user_data);
void td_alt_tab_finished(qk_tap_dance_state_t *state, void *user_data);
void td_copy_paste_finished(qk_tap_dance_state_t *state, void *user_data);
void td_copy_paste_reset(qk_tap_dance_state_t *state, void *user_data);
void td_insert_screenshot_finished(qk_tap_dance_state_t *state, void *user_data);
void td_insert_screenshot_reset(qk_tap_dance_state_t *state, void *user_data);

qk_tap_dance_action_t tap_dance_actions[] = {
    [IME_CAPSLOCK] = ACTION_TAP_DANCE_DOUBLE(S(KC_LALT) , KC_CAPS),
    [ALT_TABLE] = ACTION_TAP_DANCE_FN_ADVANCED(td_alt_tab_each_tap, td_alt_tab_finished, NULL),
    [COPY_PASTE_FNLOCK_SCREENSHOT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_copy_paste_finished, td_copy_paste_reset),
    [INSERT_SCREENSHOT_NUMPAD] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_insert_screenshot_finished, td_insert_screenshot_reset),
};

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

void td_copy_paste_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = current_dance(state);
    switch (td_state) {
        case SINGLE_TAP:    register_code16(C(KC_V));   return;
        case SINGLE_HOLD:   tap_code16(C(KC_C));        return;
        case DOUBLE_TAP:    layer_invert(_FN);          return;
        case TAP_THEN_HOLD: tap_code16(C(KC_X));        return;
        default:            register_code16(LSG(KC_S)); return;
    }
}

void td_copy_paste_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:    unregister_code16(C(KC_V));   return;
        case SINGLE_HOLD:   tap_code16(C(KC_V));          return;
        case DOUBLE_TAP:    /* This line is necessary. */ return;
        case TAP_THEN_HOLD: /* This line is necessary. */ return;
        default:            unregister_code16(LSG(KC_S)); return;
    }
}

void td_insert_screenshot_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (get_mods() == 0) {
            td_state = current_dance(state);
            switch (td_state) {
            case SINGLE_TAP: register_code16(LSG(KC_S)); return;
            case DOUBLE_TAP: layer_invert(_QW);          return;
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