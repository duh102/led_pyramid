#ifndef __FIREWORKS_H__
#define __FIREWORKS_H__
#include "../dim_curve.h"

#define STATES 16
static const uint8_t firework_colors[STATES][2] = {
    {0,0},
    {dim_curve[100], dim_curve[100]}, // white
    {dim_curve[100], dim_curve[50]}, // which fades to the main color
    {dim_curve[90], dim_curve[20]},
    {dim_curve[60], dim_curve[10]},
    {dim_curve[30], dim_curve[5]},
    {dim_curve[20], 0},
    {dim_curve[10], 0},
    {dim_curve[7], 0},
    {dim_curve[5], 0},
    {dim_curve[5], 0},
    {dim_curve[5], 0},
    {dim_curve[5], 0},
    {dim_curve[5], 0},
    {dim_curve[5], 0},
    {dim_curve[5], 0}
};

static const uint8_t firework_radius[STATES] = {
 0,0,1,2,2,2,2,3,3,3,2,2,1,1,0,0
};

void firework_loop();

#endif //__FIREWORKS_H__
