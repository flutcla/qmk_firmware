/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

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

#include QMK_KEYBOARD_H

#include "quantum.h"

enum custom_keykodes {
  TB_SLOW = KEYBALL_SAFE_RANGE,
  MY_SCRL,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case TB_SLOW:  // 押しているときだけ感度を下げる
      if (record->event.pressed) {
        // キーが押されたときの動作
        keyball_set_cpi(1);  // CPIを200に設定
      } else {
        // キーが放されたときの動作
        keyball_set_cpi(3);  // CPIを400に設定
      }
      return false;
    case MY_SCRL:  // スクロール速度を最遅に固定
      keyball_set_scroll_div(7);
      keyball_set_scroll_mode(record->event.pressed);
      return false;
  }
  return true;
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_universal(
    KC_ESC   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_BSPC  ,
    KC_TAB   , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                        KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , KC_MINUS ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_QUOT  , _______  ,
    KC_LSFT  , KC_LGUI  , KC_LALT  , LCTL_T(KC_SPACE) , LT(4,KC_LANG2) ,                                     LT(2,KC_LANG1), LT(1, KC_ENTER), _______  , _______  , TO(4)
  ),

  [1] = LAYOUT_universal(
    KC_ESC   , KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                                         KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_BSPC  ,
    KC_TAB   , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                         KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , KC_F11   ,
    KC_LSFT  , KC_EQUAL , KC_PLUS  , KC_MINUS , KC_ASTR  , KC_SLASH ,                                         KC_BSLS  , KC_GRV   , _______  , KC_LBRC  , KC_RBRC  , KC_F12   ,
    KC_LSFT  , KC_LGUI  , KC_LALT  , LCTL_T(KC_SPACE)    , _______  ,                                         MO(3)  , _______  , _______       , _______  , _______
  ),

  [2] = LAYOUT_universal(
    _______  , KC_F1    , KC_F2    , KC_F3    , KC_F4    , _______  ,                                         _______  , _______  , KC_UP    , _______  , _______  , _______  ,
    _______  , KC_F5    , KC_F6    , KC_F7    , KC_F8    , _______  ,                                         _______  , KC_LEFT  , KC_DOWN  , KC_RIGHT  , _______  , _______  ,
    _______  , KC_F9    , KC_F10   , KC_F11   , KC_F12   , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    KC_LSFT  , KC_LGUI  , KC_LALT  , LCTL_T(KC_SPACE)    , _______  ,                              _______  , _______  , _______       , KBC_RST  , RESET
  ),

  [3] = LAYOUT_universal(
    RGB_TOG  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , KC_UP    , _______  , _______  , _______  ,
    RGB_MOD  , RGB_HUI  , RGB_SAI  , RGB_VAI  , _______  , SCRL_DVI ,                                        _______  , KC_LEFT  , KC_DOWN  , KC_RIGHT  , _______  , _______  ,
    RGB_RMOD , RGB_HUD  , RGB_SAD  , RGB_VAD  , _______  , SCRL_DVD ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
                  RESET    , KBC_RST  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , KBC_RST  , RESET
  ),

  [4] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , KC_BTN3  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , KC_BTN1  , MY_SCRL  , KC_BTN2  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , KC_BTN4  , TB_SLOW  , KC_BTN5  , _______  , _______  ,
               _______  , _______  , _______  ,        _______  , _______  ,                   _______  , _______  , _______       , _______  , TO(0)
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    // Auto enable scroll mode when the highest layer is 3
    keyball_set_scroll_mode(get_highest_layer(state) == 3);
    return state;
}

#ifdef OLED_ENABLE

#    include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
    keyball_oled_render_keyinfo();
    keyball_oled_render_ballinfo();
}
#endif
