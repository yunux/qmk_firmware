/* Copyright 2020 tominabox1
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

// Layers
enum layers {
  _QWERTY,
  _OPTIM,
  _LOWER,
  _RAISE,
  _SPCFN,
  _ADJUST
};

// Custom Keycodes
enum custom_keycodes {
    OCQUOTE,
    OCPAREN,
	Y_LHYPER,    // my hyper keycode
	Y_RHYPER,    // my hyper keycode
	Y_ALT       // my alt modifier
};

#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

// Window hopping
// the code is in "encoders" section
#define ALT_TAB_TIMEOUT 1000
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

void matrix_scan_user(void) {
  // release alt if tab not pressed within 1000 ms
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

// Tapping term per key
// use longer tapping term for tap dance
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
	return 300;
  case LT(_SPCFN, KC_SPC):
  case LT(_RAISE, KC_SCLN):
	return 250;
  default:
	return TAPPING_TERM;
  }
}

// Encoders
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 4) { /* upper-right encoder */
        if (clockwise) {
 		  register_code(KC_LALT);
		  tap_code(KC_N);
		  unregister_code(KC_LALT);
        } else {
		  register_code(KC_LALT);
		  tap_code(KC_P);
		  unregister_code(KC_LALT);
        }
    } else if (index == 0) { /* upper-left encoder */
       if (clockwise) {
		 if (!is_alt_tab_active) {
			is_alt_tab_active = true;
			register_code(KC_LALT);
		  }
		  alt_tab_timer = timer_read();
		  tap_code16(KC_TAB);
		} else {
		  if (!is_alt_tab_active) {
			is_alt_tab_active = true;
			register_code(KC_LALT);
		  }
		  alt_tab_timer = timer_read();
		  tap_code16(S(KC_TAB));
		}
    }
    return false;
}


// Combos
const uint16_t PROGMEM combo1[] = {KC_LPRN, KC_RPRN, COMBO_END};
const uint16_t PROGMEM combo2[] = {KC_LBRC, KC_RBRC, COMBO_END};
const uint16_t PROGMEM combo3[] = {KC_LCBR, KC_RCBR, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo1, KC_NO),
    COMBO(combo2, KC_NO),
	COMBO(combo3, KC_NO)
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
  case 0:
	  if (pressed) {
        SEND_STRING("()");
		tap_code16(KC_LEFT);
      }
      break;
  case 1:
      if (pressed) {
        SEND_STRING("[]");
        tap_code16(KC_LEFT);     // go left
      }
      break;
  case 2:
	if (pressed) {
        SEND_STRING("{}");
        tap_code16(KC_LEFT);     // go left
      }
      break;
  }
}
// Combos end

// Tap Dance for open-close quotes
enum {
  DQUOTE,
  SQUOTE,
  BACKTICK,
  DOLLAR
};

// Double quote
void dance_doublequote(tap_dance_state_t *state, void *user_data) {
    switch(state->count) {
	case 1:
	  tap_code16(KC_AT);
	  reset_tap_dance(state);
	  break;
	case 2:
	  tap_code16(KC_AT);
	  tap_code16(KC_AT);
	  tap_code16(KC_LEFT);
	  reset_tap_dance(state);
	  break;
	case 3:
	  for (int i = 0; i < 3; i++) {
		tap_code16(KC_AT);
		tap_code16(KC_AT);
		tap_code16(KC_LEFT);
	  }
	  reset_tap_dance(state);
	  break;
	}
}

// Single quotes
void dance_singlequote(tap_dance_state_t *state, void *user_data) {
    switch(state->count) {
	case 1:
	  tap_code(KC_QUOT);
	  reset_tap_dance(state);
	  break;
	case 2:
	  tap_code(KC_QUOT);
	  tap_code(KC_QUOT);
	  tap_code16(KC_LEFT);
	  reset_tap_dance(state);
	  break;
	case 3:
	  for (int i = 0; i < 3; i++) {
		tap_code(KC_QUOT);
		tap_code(KC_QUOT);
		tap_code16(KC_LEFT);
	  }
	  reset_tap_dance(state);
	  break;
	}
}

// backtick
void dance_backtick(tap_dance_state_t *state, void *user_data) {
    switch(state->count) {
	case 1:
	  tap_code(KC_GRV);
	  reset_tap_dance(state);
	  break;
	case 2:
	  tap_code(KC_GRV);
	  tap_code(KC_GRV);
	  tap_code16(KC_LEFT);
	  reset_tap_dance(state);
	  break;
	case 3:
	  for (int i = 0; i < 3; i++) {
		tap_code(KC_GRV);
		tap_code(KC_GRV);
		tap_code16(KC_LEFT);
	  }
	  reset_tap_dance(state);
	  break;
	}
}

// dollar sign
void dance_dollar(tap_dance_state_t *state, void *user_data) {
    switch(state->count) {
	case 1:
	  register_code(KC_RSFT);
	  tap_code(KC_4);
	  unregister_code(KC_RSFT);
	  reset_tap_dance(state);
	  break;
	case 2:
	  register_code(KC_RSFT);
	  tap_code(KC_4);
	  tap_code(KC_4);
	  unregister_code(KC_RSFT);
	  tap_code16(KC_LEFT);
	  reset_tap_dance(state);
	  break;
	case 3:
	  for (int i = 0; i < 3; i++) {
		register_code(KC_RSFT);
		tap_code(KC_4);
		tap_code(KC_4);
		unregister_code(KC_RSFT);
		tap_code16(KC_LEFT);
	  }
	  reset_tap_dance(state);
	  break;
	}
}

tap_dance_action_t tap_dance_actions[] = {
  [DQUOTE] = ACTION_TAP_DANCE_FN(dance_doublequote),
  [SQUOTE] = ACTION_TAP_DANCE_FN(dance_singlequote),
  [BACKTICK] = ACTION_TAP_DANCE_FN(dance_backtick),
  [DOLLAR] = ACTION_TAP_DANCE_FN(dance_dollar)
};
// Tap Dance end


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t my_hash_timer;
  
    switch (keycode) {
        // Intercept Capsword on leftshift tap
	    case LCTL_T(CW_TOGG):
            if (record->tap.count && record->event.pressed) {
                /* tap_code16(CW_TOGG); // Send CW_TOGG on tap */
			  caps_word_toggle();
			  return false;        // Return false to ignore further processing of key
            }
			break;
			/*case Y_ALT:
		  if(record->event.pressed) {
			tap_code(KC_SCROLL_LOCK); // register scroll lock
		  } else {
			tap_code(KC_SCROLL_LOCK); // unregister scroll lock
			}*/

	    case Y_RHYPER:
		  if(record->event.pressed) {
			my_hash_timer = timer_read();
			tap_code16(KC_SCROLL_LOCK); // Change the key to be held here
		  } else {
			tap_code16(KC_SCROLL_LOCK); // Change the key that was held here, too!
			if (timer_elapsed(my_hash_timer) < TAPPING_TERM) {
			  //SEND_STRING("/"); // Change the character(s) to be sent on tap here
			  tap_code16(KC_SLASH);
			}
		  }
		  break;

	case Y_LHYPER:
		  if(record->event.pressed) {
			my_hash_timer = timer_read();
			tap_code16(KC_SCROLL_LOCK); // Change the key to be held here
		  } else {
			tap_code16(KC_SCROLL_LOCK); // Change the key that was held here, too!
			if (timer_elapsed(my_hash_timer) < TAPPING_TERM) { /////////
			  //SEND_STRING("\\"); // Change the character(s) to be sent on tap here
			  tap_code(KC_QUOTE);
			}
		  }

		  
		  
    }
    return true;
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | BSPC |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | TAB  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  | ENTER|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | SHIFT|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | SHIFT|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | CTRL | ALT  | SUPER| META | LOWER|    SPACE    |RAISE | META | SUPER| ALT  | CTRL |
 * `-----------------------------------------------------------------------------------'
 */
/* NEW Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | BSPC |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | TAB  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   .  | ENTER|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | SHIFT|   \  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   Z  |   /  | SHIFT|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | CTRL | ALT  | SUPER| META | LOWER|    SPACE    |RAISE | META | SUPER| ALT  | CTRL |
 * `-----------------------------------------------------------------------------------'
 */
[_QWERTY] = LAYOUT_planck_mit(
    KC_ESC,  KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,      KC_U,    KC_I,    KC_O,    KC_P,            KC_BSPC,
    KC_TAB,  KC_A,     KC_S,    KC_D,    KC_F,    KC_G,    KC_H,      KC_J,    KC_K,    KC_L,    KC_DOT,         KC_ENT,
    KC_LSFT,  Y_LHYPER, KC_X,    KC_C,    KC_V,    KC_B,    KC_N,   KC_M,    KC_COMM, KC_Z,  Y_RHYPER, RSFT_T(KC_MINS),
    KC_LCTL, KC_APP,   KC_LGUI, KC_LALT, MO(_LOWER),   LT(_SPCFN, KC_SPC), LT(_RAISE, KC_SCLN),   KC_LALT, KC_RGUI, KC_APP,  KC_LCTL
),
/* VYWB
 * ,-----------------------------------------------------------------------------------.
 * | ESC  |   V  |   Y  |   W  |   B  |   /  |   ;  |   R  |   C  |   F  |   K  | BSPC |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | TAB  |   H  |   N  |   O  |   T  |   U  |   M  |   E  |   A  |   I  |   S  | ENTER|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | SHIFT|   P  |   X  |   Z  |   L  |   ,  |   .  |   D  |   Q  |   J  |   G  | SHIFT|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | CTRL | ALT  | SUPER| META | LOWER|    SPACE    |RAISE | META | SUPER| ALT  | CTRL |
 * `-----------------------------------------------------------------------------------'
 */
[_OPTIM] = LAYOUT_planck_mit(
    KC_ESC , KC_V,         KC_C,    KC_D,    KC_U,    KC_SLSH, KC_SCLN,   KC_L,    KC_R,    KC_F,    KC_K,         _______,
    _______, KC_S,         KC_N,    KC_A,    KC_T,    KC_W,    KC_M,      KC_E,    KC_O,    KC_I,    KC_H,         _______,
    _______, RCTL_T(KC_Y), KC_X,    KC_Z,    KC_P,    KC_COMM, KC_DOT,    KC_B,    KC_Q,    KC_J,    RCTL_T(KC_G), _______,
	_______, _______,      _______, _______, _______,      _______,       _______, _______, _______, _______,      _______
),

/* RAISE LAYER: SYMBOLS
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   "  |   &  |   *  |   ^  |   {  |   }  |  F7  |  F8  |  F9  |  F10 | BSPC |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   `  |   @  |   $  |   %  |   #  |   (  |   )  |  F4  |  F5  |  F6  |  F11 |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   ~  |   '  |   !  |   @  |      |   [  |   ]  |  F1  |  F2  |  F3  |  F12 |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      | Next | Vol- | Vol+ |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = LAYOUT_planck_mit(
    _______,      KC_PIPE,    KC_AMPR,    KC_NUHS,    KC_CIRC,    KC_LCBR, KC_RCBR, KC_F7, KC_F8, KC_F9, KC_F10, KC_BSPC,
    _______,      S(KC_QUOT), TD(DOLLAR), KC_PERC,    TD(DQUOTE), KC_LPRN, KC_RPRN, KC_F4, KC_F5, KC_F6, KC_F11,  _______,
    TD(BACKTICK), KC_NUBS,    KC_EXLM,    S(KC_NUBS), _______,    KC_LBRC, KC_RBRC, KC_F1, KC_F2, KC_F3, KC_F12, _______,
    _______, _______, _______, _______,    _______, _______, _______, _______,    KC_MNXT, KC_VOLD, KC_VOLU
),

//[_RAISE] = LAYOUT_planck_mit(
//    _______,      TD(DQUOTE), KC_AMPR, KC_ASTR, KC_CIRC,  KC_LCBR, KC_RCBR, KC_F7, KC_F8, KC_F9, KC_F10, KC_BSPC,
//    TD(BACKTICK), S(KC_QUOT), TD(DOLLAR),  KC_PERC,   KC_NUHS, KC_LPRN, KC_RPRN, KC_F4, KC_F5, KC_F6, KC_F11,  _______,
//    KC_PIPE, TD(SQUOTE), KC_EXLM, S(KC_QUOT),    _______, KC_LBRC, KC_RBRC, KC_F1, KC_F2, KC_F3, KC_F12, _______,
//    _______, _______, _______, _______,    _______, _______, _______, _______,    KC_MNXT, KC_VOLD, KC_VOLU
//),

/* LOWER LAYER: NUMBERS
 * ,-----------------------------------------------------------------------------------.
 * |   ~  |      |      |      |      |      |      |   7  |   8  |   9  |   0  |  ^   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |   4  |   5  |   6  |      |  =   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |   1  |   2  |   3  |   *  |  +   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_LOWER] = LAYOUT_planck_mit(
    S(KC_NUHS), _______, _______, _______,    _______,    _______, _______, KC_7,    KC_8,    KC_9,    KC_0,    KC_CIRC,
    _______,    _______, _______, _______,    _______,    _______, _______, KC_4,    KC_5,    KC_6,    _______, KC_EQUAL,
    _______,    _______, _______, _______,    _______,    _______, _______, KC_1,    KC_2,    KC_3,    KC_ASTR, KC_PLUS,
    _______,    _______, _______, _______,    _______,    CW_TOGG, _______, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY
),

/* SPACE FUNCTION LAYER: NAVIGATION ETC.
 * ,-----------------------------------------------------------------------------------.
 * |      | ESC  | INS  | END  | BSPC |C-BSPC|      |      |  UP  | A-TAB| C-UP | DEL  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | HOME | PSCR | DEL  |      | C-DEL| C-LFT| LFT  | DOWN | RIGHT|C-RGHT|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      | APP  |      |      |      | C-DWN|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_SPCFN] = LAYOUT_planck_mit(
	_______, KC_ESC , KC_INS,  KC_END,  KC_BSPC, C(KC_BSPC), _______,    CW_TOGG,  KC_UP,    A(KC_TAB),  C(KC_UP), KC_DEL ,
    _______, KC_HOME, KC_PSCR, KC_DEL , _______, C(KC_DEL),  C(KC_LEFT), KC_LEFT,  KC_DOWN,  KC_RIGHT,   C(KC_RIGHT), _______,  
    _______, _______, KC_APP , KC_CAPS, _______, _______,    C(KC_DOWN), _______,  _______,  _______,    _______,  _______,
    _______, _______, _______, _______, _______, _______,          _______,        _______,  _______,    _______,  _______
),

/* Adjust (Lower + Raise)
 *                      v------------------------RGB CONTROL--------------------v
 * ,-----------------------------------------------------------------------------------.
 * |      | RESET| DEBUG| RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-| DEL  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */
[_ADJUST] = LAYOUT_planck_mit(
    KC_PWR,  QK_BOOT, DB_TOGG, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD,  RGB_VAI, RGB_VAD, TG(_OPTIM),
    _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,  _______,  _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, _______
)

};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}



