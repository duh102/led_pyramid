#ifndef __PYRAMID_STRUCTURE_H__
#define __PYRAMID_STRUCTURE_H__

#include <stdint.h>
#include <stdbool.h>

#define PYRAMID_HEIGHT 6
#define PYRAMID_WIDTH 11
#define LEDS_PER_PANEL 36
#define PANELS 4
#define MAX_LED (PANELS * LEDS_PER_PANEL)

// Get the color of a pixel at index idx
uint8_t* get_color(uint8_t idx);
// Set the color of a pixel at index idx
void set_color(uint8_t idx, uint8_t* color);
// Set the color of a pixel at position x/y on panel
void set_color_pos(uint8_t panel, uint8_t x, uint8_t y, uint8_t* color);
// Get the state of a pixel at index idx
uint8_t get_state(uint8_t idx);
// Set the state of a pixel at index idx
void set_state(uint8_t idx, uint8_t state);
// Get the index of a pixel at position x/y, starting at the bottom left of the pyramid
uint8_t get_idx(uint8_t panel, uint8_t x, uint8_t y);
// Blank the screen
void blank_display();

// The indices (using top of the triangle as 0) of the tops of columns
#define HEADS_WIDTH 11
static const uint8_t start_leds[] = {
  25, 16, 9, 4, 1, 0, 3, 8, 15, 24, 35
};
/* Reference for the below maps; LEDs are laid out in a sequential pyramid,
   two more LEDs per row, for 6 rows, total of 36 LEDs
                0
             1  2  3
          4  5  6  7  8
       9 10 11 12 13 14 15
   16 17 18 19 20 21 22 23 24
25 26 27 28 29 30 31 32 33 34 35
*/

// Map from LED to the LED below it; if 255, means no LED below it
static const uint8_t below_leds[] = {
                          2,
                      5,  6,  7, 
                 10, 11, 12, 13, 14,
             17, 18, 19, 20, 21, 22, 23,
         26, 27, 28, 29, 30, 31, 32, 33, 34,
    255,255,255,255,255,255,255,255,255,255,255
};
// Map from LED to the LED above it; if 255, means no LED above it
static const uint8_t above_leds[] = {
                        255,
                    255,  0,255, 
                255,  1,  2,  3,255,
            255,  4,  5,  6,  7,  8,255,
        255,  9, 10, 11, 12, 13, 14, 15,255,
    255, 16, 17, 18, 19, 20, 21, 22, 23, 24,255
};
// Map from LED to the LED to the left of it; if 255, means no LED left of it
static const uint8_t left_leds[] = {
                        255,
                    255,  1,  2, 
                255,  4,  5,  6,  7,
            255,  9, 10, 11, 12, 13, 14,
        255, 16, 17, 18, 19, 20, 21, 22, 23,
    255, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34
};
// Map from LED to the LED to the right of it; if 255, means no LED right of it
static const uint8_t right_leds[] = {
                        255,
                      2,  3,255, 
                  5,  6,  7,  8,255,
             10, 11, 12, 13, 14, 15,255,
         17, 18, 19, 20, 21, 22, 23, 24,255,
     26, 27, 28, 29, 30, 31, 32, 33, 34, 35,255
};
// x/y map, indicating whether a given x/y position is an LED or not real.
// the max X is 10, max y is 5, so we define it as a series of 11 vertical slices
// LSB is the bottom of the pyramid
static const uint8_t pyramid_height[PYRAMID_WIDTH] = {
  1, // x = 0
  2,
  3,
  4,
  5,
  6, // x = 5
  5,
  4,
  3,
  2,
  1, // x = 10
};
static const uint8_t pyramid_xy_map[PYRAMID_HEIGHT][PYRAMID_WIDTH] = {
{255,255,255,255,255,  0,255,255,255,255,255},
{255,255,255,255,  1,  2,  3,255,255,255,255},
{255,255,255,  4,  5,  6,  7,  8,255,255,255},
{255,255,  9, 10, 11, 12, 13, 14, 15,255,255},
{255, 16, 17, 18, 19, 20, 21, 22, 23, 24,255},
{ 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
};
#endif //__PYRAMID_STRUCTURE_H__
