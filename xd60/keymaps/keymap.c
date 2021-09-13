#include QMK_KEYBOARD_H
#include "action_layer.h"

#define ______ KC_NO
enum {
  TD_FN_DEL,
  TD_RCTL_SPECIAL,
};

// Some custom tap dance definitions
// TODO: allow for single-tap space cadet rsft close-paren
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_FN_DEL] = ACTION_TAP_DANCE_LAYER_MOVE(KC_DEL,1),
  [TD_RCTL_SPECIAL] = ACTION_TAP_DANCE_DOUBLE(KC_RCTL,OSL(2)),
};

void dynmaic_macro_record_start_user(void) {
  dynamic_macro_led_blink();
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  // 0: Base Layer
  LAYOUT_all(
      KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS,  KC_EQL,  KC_BSLS,  KC_GRV,    \
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,    KC_P,    KC_LBRC,  KC_RBRC,           KC_BSPC,   \
      MT(MOD_LCTL, KC_CAPS), KC_A,    KC_S,    KC_D,    KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,    KC_SCLN, KC_QUOT,  ______,   KC_ENT,    \
      KC_LSPO, ______,  KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT, ______,MT(MOD_RSFT, KC_SLSH),KC_UP,LT(1, KC_DEL),     \
      KC_LCTL, KC_LGUI, KC_LALT,                          KC_SPC,                          MT(MOD_RCTL, OSL(2)), KC_RALT,  KC_LEFT, KC_DOWN,  KC_RIGHT),

  // 1: Function Layer
  LAYOUT_all(
      RESET,  KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,   KC_F10,  KC_F11,   KC_F12,  KC_F13,   KC_F14,    \
      ______, ______, KC_UP,    ______,  ______, ______, ______, ______, ______, ______,  ______,  ______,   ______,            KC_INS,    \
      ______,KC_LEFT, KC_DOWN, KC_RIGHT,______, ______, ______, ______, ______, ______,  ______,  ______,    ______,            KC_ENT,    \
      ______, ______, ______, ______, ______, ______, ______, KC_VOLD, KC_VOLU,  KC_MUTE, ______,  ______,   KC_RSPC,  KC_PGUP, ______,    \
      ______,  ______, ______,                          KC_MPLY,                          MO(2),  ______,   KC_HOME, KC_PGDOWN,KC_END),

  // 2: RGB Layer///
  LAYOUT_all(
      ______,  BL_TOGG, BL_STEP, ______,  ______, ______, ______, ______, ______, ______,  ______,  ______,   ______,  DM_PLY1,   DM_PLY2,   \
      ______,  RGB_TOG, RGB_MOD, ______,  ______, ______, ______, ______, ______, ______,  ______,  DM_REC1,  DM_REC2,            DM_RSTP,   \
      ______,  RGB_HUI, RGB_SAI, RGB_VAI, ______, ______, ______, ______, ______, ______,  ______,  ______,   ______,            ______,   \
      ______,  ______,  RGB_HUD, RGB_SAD, RGB_VAD,______, ______, ______, ______, ______,  ______,  ______,   MO(2),   KC_MPLY,  ______,   \
      ______,  ______,  ______,                           ______,                          ______,  ______,   KC_MPRV, KC_MPLY,  KC_MNXT),

};
