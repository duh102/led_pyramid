#include <stdlib.h>
#include <stdint.h>
#include "matrix.h"
#include "../pyramid_structure.h"


void matrix_loop() {
  // roll through all LED states, if any >0, copy current state to one below,
  //  decrement state
  // roll through heads, randomly choose to start at 1 (regardless of current state)
  // for all LED states > 0, copy in the colors from the state colors
  for( uint8_t panel = (PANELS-1); panel < PANELS; panel--) {
      // The amount to add to any panel-indexed values, indicates the 0th LED of that panel
      uint8_t panelAdd = (panel*LEDS_PER_PANEL);
      for( uint8_t led = (LEDS_PER_PANEL-1); led < LEDS_PER_PANEL; led-- ) {
          uint8_t i = panelAdd+led;
          uint8_t curState = get_state(i);
          if(curState > 0) {
              // propagate state to the LED below, if it exists
              if(below_leds[led] != 255) {
                  set_state(panelAdd+below_leds[led], curState);
              }
              curState = (curState+1) % STATES;
              set_state(i, curState);
          }
      }
      for( uint8_t head = 0; head < HEADS_WIDTH; head++) {
          if( (rand() % 100) < 5 ) {
              set_state(panelAdd+start_leds[head], 1);
          }
      }
  }
  for( uint32_t i = 0; i < MAX_LED; i++ ) {
      uint8_t curState = get_state(i);
      set_color(i, matrix_colors[curState]);
  }
}
