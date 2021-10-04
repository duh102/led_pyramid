#ifndef __FIRE_H__
#define __FIRE_H__
#include "../dim_curve.h"

#define STATES 16
static const uint8_t fire_intensity[STATES] = {
   0,100,80,60,
  40, 30,20,10,
   5,  4, 3, 2,
   1,  1, 1, 1,
};

void fire_loop();

#endif //__FIRE_H__
