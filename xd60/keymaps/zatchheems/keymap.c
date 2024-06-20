#include QMK_KEYBOARD_H
#include "action_layer.h"

#define ______ KC_NO

typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_SINGLE_HOLD,
  TD_DOUBLE_TAP,
  TD_DOUBLE_HOLD,
  TD_TRIPLE_TAP,
  TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
  bool is_press_action;
  td_state_t state;
} td_tap_t;

enum {
  TD_FN_DEL,
  TD_RCTL,
};

void dynmaic_macro_record_start_user(void) {
  dynamic_macro_led_blink();
}

// Function associated with all tap dances
td_state_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // 0: Base Layer
  [0] = LAYOUT_60_ansi_arrow_split_bs_7u_spc(
      QK_GESC, KC_1,         KC_2,    KC_3,    KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,        KC_0,        KC_MINS, KC_EQL,  KC_BSLS,  KC_GRV,    \
      KC_TAB,  KC_Q,         KC_W,    KC_E,    KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,        KC_P,        KC_LBRC, KC_RBRC, KC_BSPC,   \
      MT(MOD_LCTL, KC_CAPS), KC_A,    KC_S,    KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,        KC_L,        KC_SCLN, KC_QUOT, KC_ENT,    \
      KC_LSPO, KC_Z,         KC_X,    KC_C,    KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM,KC_DOT,      MT(MOD_RSFT, KC_SLSH),KC_UP,   LT(1, KC_DEL),     \
      KC_LCTL, KC_LGUI, KC_LALT,                          KC_SPC,                      TD(TD_RCTL), KC_RALT,     KC_LEFT, KC_DOWN, KC_RIGHT),

  // 1: Function Layer
  [1] = LAYOUT_60_ansi_arrow_split_bs_7u_spc(
      QK_BOOT,  KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,   KC_F10,  KC_F11,   KC_F12,  KC_F13,   KC_F14,    \
      ______, ______, KC_UP,    ______,  ______, ______, ______, ______, ______, ______,  ______,  ______,   ______,            KC_INS,    \
      ______,KC_LEFT, KC_DOWN, KC_RIGHT,______, ______, ______, ______, ______, ______,  ______,  ______,    ______,            KC_ENT,    \
      ______, ______, ______, ______, ______, ______, KC_VOLD, KC_VOLU,  KC_MUTE, ______,     KC_RSPC,  KC_PGUP, ______,    \
      ______,  ______, ______,                          KC_MPLY,                          MO(2),  ______,   KC_HOME, KC_PGDN,KC_END),

  // 2: RGB Layer///
  [2] = LAYOUT_60_ansi_arrow_split_bs_7u_spc(
      ______,  BL_TOGG, BL_STEP, ______,  ______, ______, ______, ______, ______, ______,  ______,  ______,   ______,  DM_PLY1,   DM_PLY2,   \
      ______,  RGB_TOG, RGB_MOD, ______,  ______, ______, ______, ______, ______, ______,  ______,  DM_REC1,  DM_REC2,            DM_RSTP,   \
      ______,  RGB_HUI, RGB_SAI, RGB_VAI, ______, ______, ______, ______, ______, ______,  ______,  ______,   ______,            ______,   \
      ______,  RGB_HUD, RGB_SAD, RGB_VAD,______, ______, ______, ______, ______,  ______,    MO(2),   KC_MPLY,  ______,   \
      ______,  ______,  ______,                           ______,                          ______,  ______,   KC_MPRV, KC_MPLY,  KC_MNXT),

};


// From: https://docs.qmk.fm/using-qmk/software-features/feature_tap_dance#example-6-using-tap-dance-for-momentary-layer-switch-and-layer-toggle-keys-id-example-6
// Determine the current tap dance state
td_state_t cur_dance(qk_tap_dance_state_t *state) {
  if (state->count == 1) {
    if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
    else return TD_SINGLE_HOLD;
  } else if (state->count == 2) {
    if (state->interrupted) return TD_DOUBLE_TAP;
    else if (state->pressed) return TD_DOUBLE_HOLD;
  } else if (state->count == 3) {
    if (state->interrupted) return TD_TRIPLE_TAP;
    else if (state->pressed) return TD_TRIPLE_HOLD;
  }
  return TD_UNKNOWN;
}

// Initialize tap structure associated with fn-rgui tap dance key
static td_tap_t ql_tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// Functions that control what our tap dance key does
void ql_finished(qk_tap_dance_state_t *state, void *user_data) {
    ql_tap_state.state = cur_dance(state);
    switch (ql_tap_state.state) {
        case TD_SINGLE_TAP: /* Placeholder. YAGNI most likely*/ break;
        case TD_SINGLE_HOLD:
            register_code(KC_RCTL);
            break;
        case TD_DOUBLE_HOLD:
            /* // Check to see if the layer is already set */
            /* if (layer_state_is(2)) { */
            /*   // If already set, then switch it off */
            /*   layer_off(2); */
            /* } */
            // Send RGUI
            register_code(KC_RGUI);
            break;
        case TD_TRIPLE_HOLD:
            layer_on(2);
            break;
        default:
            break;
    }
}

void ql_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch(ql_tap_state.state) {
      case TD_SINGLE_HOLD:
        layer_off(1);
        break;
      case TD_DOUBLE_HOLD:
        unregister_code(KC_RGUI);
        break;
      case TD_TRIPLE_HOLD:
        layer_off(2);
      default: layer_off(1);
    }
    
    ql_tap_state.state = TD_NONE;
}

/* // Associate our tap dance key with its functionality */
// TODO: allow for single-tap space cadet rsft close-paren
qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_RCTL] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset),//, 150),
    /* [MO_ECCP] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_ctrl_caps_finished, esc_ctrl_caps_reset)//, 70) */
};
