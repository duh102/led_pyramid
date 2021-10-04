#include <stdint.h>
#include <stdlib.h>
#include "../pyramid_structure.h"
#include "../hsv_rgb.h"
#include "fire.h"

uint16_t hsvState = 0;

void fire_loop() {
  uint8_t curState;
  // Loop over all the LEDs from top to bottom, reduce their intensity by one
  for(uint8_t panel = 0; panel < PANELS; panel++) {
    uint8_t panelAdd = (panel*LEDS_PER_PANEL);
    for( uint8_t led = 0; led < LEDS_PER_PANEL; led++ ) {
      uint8_t i = panelAdd+led;
      curState = get_state(i);
      if(curState > 0) {
        curState = (curState+1) % STATES;
        set_state(i, curState);
      }
    }
    // When we get to the very bottom, make randomly sized "spikes" of fire
    for(uint8_t led = 0; led < PYRAMID_WIDTH; led++) {
      uint8_t i = panelAdd+led+(LEDS_PER_PANEL-PYRAMID_WIDTH);
      //set_state(i, 1);
      if((rand() % 100) < 15) {
        uint8_t height = pyramid_height[led];
        uint8_t flame_height = rand()%3;
        for(uint8_t y = 0; y < PYRAMID_HEIGHT; y++) {
          uint8_t idx = get_idx(panel, led, y);
          if(idx == 255) {
            continue;
          }
          if(y < flame_height) {
            set_state(idx, 1);
          } else {
            uint8_t diff = y - flame_height;
            set_state(idx, 1+(2*diff));
          }
        }
      }
    }
  }
  
  // Then finally color them the color we've picked for this frame based on the fire intensity
  hsvState = (hsvState + 1) % MAX_HUE;
  uint8_t chosenColor[] = {0,0,0};
  for( uint32_t i = 0; i < MAX_LED; i++ ) {
      curState = get_state(i);
      getRGB(hsvState, fire_intensity[curState], chosenColor);
      set_color(i, chosenColor);
  }
}
