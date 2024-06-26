#include QMK_KEYBOARD_H
// TODO: change to one tap for Esc, hold for LCtl, double tap for Caps
#define MT_CLCP MT(MOD_LCTL, KC_CAPS)

// Define a type for as many tap dance states as you need
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
  MO_RGULT, // Our custom tap dance key; add any other tap dance keys to this enum
  MO_ECCP,  // Escape Ctrl CaPs (name needs work)
};

void led_update_ports(led_t led_state) {
  writePin(B1, !led_state.caps_lock);
}

/* bool led_update_user(led_t led_state) { */
/*   led_state.caps_lock ? (uint16_t)register_code(BL_ON) : (uint16_t)register_code(BL_OFF); */
/*   return true; */
/* } */

// Function associated with all tap dances
td_state_t cur_dance(qk_tap_dance_state_t *state);

// Functions associated with individual tap dances
void ql_finished(qk_tap_dance_state_t *state, void *user_data);
void ql_reset(qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_65_ansi_blocker_split_bs( /* Base */
  QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSLS, KC_GRV,  LT(2, KC_INS),
  KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC, KC_DEL,
  /*TD(MO_ECCP)*/MT_CLCP, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGUP,
  KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,          KC_UP,   KC_PGDN,
  KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                             KC_RCTL, TD(MO_RGULT), KC_LEFT, KC_DOWN, KC_RGHT
  ),
  [1] = LAYOUT_65_ansi_blocker_split_bs( /* FN */
  _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_F13,KC_F14,  _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,           KC_DEL,  KC_INS,
  KC_CAPS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,           KC_HOME,
  _______,  _______, _______, _______, _______, _______, KC_VOLD, KC_VOLU, KC_MUTE, _______, _______, _______,                   KC_MPLY, KC_END,
  _______, _______, _______,                  _______,                               _______,_______,           KC_MPRV, KC_MPLY, KC_MNXT
  ),
  [2] = LAYOUT_65_ansi_blocker_split_bs( /* FN */
  Q_RESET, BL_TOGG, BL_STEP,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_F13,  KC_F14, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, DM_REC1, DM_REC2, DM_RSTP, QK_BOOT,
  _______, _______, RGB_SAI, RGB_VAI, _______, _______, _______, _______, _______, _______, _______, _______,          _______,          DM_PLY1,
  _______, _______, RGB_SAD, RGB_VAD, _______, _______, _______, _______, _______, _______, _______, _______,                   _______, DM_PLY2,
  _______, _______, _______,                   _______,                            _______, _______,           _______, _______, _______
  ),
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
            layer_on(1);
            break;
        case TD_DOUBLE_HOLD:
            // Check to see if the layer is already set
            if (layer_state_is(1)) {
              // If already set, then switch it off
              layer_off(1);
            }
            // Send RGUI
            register_code(KC_RGUI);
            break;
        case TD_TRIPLE_HOLD:
            // Send RGUI and RALT
            register_code(KC_RGUI);
            register_code(KC_RALT);
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
        unregister_code(KC_RGUI);
        unregister_code(KC_RALT);
        break;
      default: layer_off(1);
    }
    
    ql_tap_state.state = TD_NONE;
}

void esc_ctrl_caps_finished(qk_tap_dance_state_t *state, void *user_data) {
  ql_tap_state.state = cur_dance(state);
  switch (ql_tap_state.state) {
    case TD_SINGLE_TAP:
      register_code(KC_ESC);
      break;
    case TD_SINGLE_HOLD:
      register_code(KC_LCTL);
      break;
    case TD_DOUBLE_TAP:
      if (layer_state_is(1)) {
        layer_off(1);
      }
      register_code(KC_CAPS);
      break;
    case TD_DOUBLE_HOLD:
        // Check to see if the layer is already set
        if (layer_state_is(1)) {
          // If already set, then switch it off
          layer_off(1);
        }
        // Send RGUI
        register_code(KC_LGUI);
        break;
    default: break;
    }
}

void esc_ctrl_caps_reset(qk_tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    switch(ql_tap_state.state) {
      case TD_SINGLE_TAP: unregister_code(KC_ESC); break;
      case TD_SINGLE_HOLD: unregister_code(KC_LCTL); break;
      case TD_DOUBLE_TAP: unregister_code(KC_CAPS); break;
      case TD_DOUBLE_HOLD: unregister_code(KC_LGUI); break;
      default: layer_off(1);
    }
    
    ql_tap_state.state = TD_NONE;
}

/* // Associate our tap dance key with its functionality */
qk_tap_dance_action_t tap_dance_actions[] = {
    [MO_RGULT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset),//, 150),
    /* [MO_ECCP] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_ctrl_caps_finished, esc_ctrl_caps_reset)//, 70) */
};
