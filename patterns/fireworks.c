#include <stdint.h>
#include <stdlib.h>
#include "fireworks.h"
#include "../pyramid_structure.h"

#define NUM_FIREWORKS 20
#define FW_PAGE 0
#define FW_X 1
#define FW_Y 2
#define FW_STATE 3
#define FW_COLOR 4
#define FW_NUM_COLORS 6
#define FW_COLOR_RED 0
#define FW_COLOR_BLUE 1
#define FW_COLOR_GREEN 2
#define FW_COLOR_YELLOW 3
#define FW_COLOR_TEAL 4
#define FW_COLOR_PURPLE 5
// Map of up to 20 fireworks
// Individual firework is [face, x, y, state, color]
// If state is 0, firework is available to be refreshed
uint8_t firework_position_states[NUM_FIREWORKS][5] = {0};

uint8_t draw_circle_points(uint8_t panel, uint8_t cenX, uint8_t cenY, uint8_t x, uint8_t y, uint8_t* color) {
  set_color_pos(panel, cenX+x, cenY+y, color);
  set_color_pos(panel, cenX-x, cenY+y, color);
  set_color_pos(panel, cenX+x, cenY-y, color);
  set_color_pos(panel, cenX-x, cenY-y, color);
  set_color_pos(panel, cenX+y, cenY+x, color);
  set_color_pos(panel, cenX-y, cenY+x, color);
  set_color_pos(panel, cenX+y, cenY-x, color);
  set_color_pos(panel, cenX-y, cenY-x, color);
}
void draw_circle(uint8_t panel, uint8_t cenX, uint8_t cenY, uint8_t r, uint8_t* color) {
    if(r == 0) {
      set_color_pos(panel, cenX, cenY, color);
      return;
    }
    uint8_t x = 0, y = r;
    uint8_t d = 3 - 2 * r;
    draw_circle_points(panel, cenX, cenY, x, y, color);
    while (y >= x) {
        // for each pixel we will
        // draw all eight pixels
         
        x++;
 
        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        draw_circle_points(panel, cenX, cenY, x, y, color);
    }
}


void firework_loop() {
  // Because the fireworks may leave behind some junk, blank out the entire thing first
  blank_display();
  // Loop through all the fireworks and update their explosions
  for(uint8_t fw = 0; fw < NUM_FIREWORKS; fw++) {
    uint8_t state = firework_position_states[fw][FW_STATE];
    if( state == 0 && (rand() % 100) < 5) {
      state = 1;
      firework_position_states[fw][FW_COLOR] = rand() % FW_NUM_COLORS;
      firework_position_states[fw][FW_PAGE] = rand() % PANELS;
      uint8_t x = rand()%PYRAMID_WIDTH;
      firework_position_states[fw][FW_X] = x;
      firework_position_states[fw][FW_Y] = rand()%pyramid_height[x];
    } else if (state != 0) {
      state = (state+1)%STATES;
      uint8_t cenY = firework_position_states[fw][FW_Y];
      if(state < 3) {
        firework_position_states[fw][FW_Y]++;
      } else if(state > 6 && cenY > 0) {
        firework_position_states[fw][FW_Y]--;
      }
    }
    firework_position_states[fw][FW_STATE] = state;

    if(state != 0) {
      uint8_t colorVal = firework_position_states[fw][FW_COLOR]+1;
      uint8_t* color = (uint8_t[]){firework_colors[state][colorVal&1?1:0], firework_colors[state][colorVal&1<<1?1:0], firework_colors[state][colorVal&1<<2?1:0]};
      uint8_t panel = firework_position_states[fw][FW_PAGE];
      uint8_t cenX = firework_position_states[fw][FW_X];
      uint8_t cenY = firework_position_states[fw][FW_Y];
      draw_circle(panel, cenX, cenY, firework_radius[state], color);
    }
  }
}
