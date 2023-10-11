#include QMK_KEYBOARD_H
enum keycodes {
    KC_CYCLE_LAYERS = QK_USER,
};

/// 1st layer on the cycle
#define LAYER_CYCLE_START 0
// Last layer on the cycle
#define LAYER_CYCLE_END   3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     [0]=LAYOUT(
            KC_CYCLE_LAYERS, KC_2, KC_3, KC_4,
            KC_5, KC_6, KC_7, KC_8,
            KC_9, KC_A, KC_B, KC_C,
            KC_D, KC_E, KC_F, KC_G,
            KC_H, KC_I, KC_J, KC_K
            ),
             [1]=LAYOUT(
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
            ),
        [2]=LAYOUT(
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
            ),
        [3]= LAYOUT(
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
        ),
};
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
      [0] =  { ENCODER_CCW_CW(KC_VOLU, KC_VOLD)  },
        [1]=  { ENCODER_CCW_CW(KC_VOLU, KC_VOLD)  },
    [2] =  { ENCODER_CCW_CW(KC_VOLU, KC_VOLD)  },
    [3]=  { ENCODER_CCW_CW(KC_VOLU, KC_VOLD)  },

};
#endif
#ifdef RGBLIGHT_ENABLE
void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom(); // Enables RGB, without saving settings
  rgblight_sethsv_noeeprom(HSV_ORANGE);
  rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
}
#endif
#ifdef OLED_ENABLE
static void render_logo(void) {
      #undef OLED_FONT_WIDTH
    #define OLED_FONT_WIDTH 12
    #undef OLED_FONT_HEIGHT
     #define OLED_FONT_HEIGHT 14
    oled_write_ln(PSTR("    CoolPad    "), true);

    //oled_write_P(qmk_logo, false);
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_CYCLE_LAYERS:
      // Our logic will happen on presses, nothing is done on releases
      if (!record->event.pressed) {
        // We've already handled the keycode (doing nothing), let QMK know so no further code is run unnecessarily
        return false;
      }

      uint8_t current_layer = get_highest_layer(layer_state);

      // Check if we are within the range, if not quit
      if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
        return false;
      }

      uint8_t next_layer = current_layer + 1;
      if (next_layer > LAYER_CYCLE_END) {
          next_layer = LAYER_CYCLE_START;
      }
      layer_move(next_layer);

      return false;

    // Process other keycodes normally
    default:
      return true;
  }
}


#ifdef OLED_ENABLE
bool oled_task_user(void) {

    render_logo();
    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR(" Default \n"), true);
             rgblight_sethsv_noeeprom(HSV_ORANGE);
            break;
        case 1:
            oled_write_P(PSTR(" FN \n"), true);
             rgblight_sethsv_noeeprom(HSV_BLUE);
             // rgblight_mode(RGBLIGHT_EFFECT_BREATHE_MAX);
            break;
        case 2:
            oled_write_P(PSTR(" System \n"), true);
             rgblight_sethsv_noeeprom(HSV_WHITE);
            break;
        case 3:
            oled_write_P(PSTR(" Custom \n"), true);
             rgblight_sethsv_noeeprom(HSV_RED);
            break;

        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR(" Undefined "), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);

    return false;
}
#endif

