#ifndef __MATRIX_H__
#define __MATRIX_H__
#include "../dim_curve.h"

#define STATES 10
static const uint8_t matrix_colors[STATES][3] = {
    {0, 0, 0},
    {dim_curve[100], dim_curve[100], dim_curve[100]}, // white
    {dim_curve[50], dim_curve[100], dim_curve[50]}, // which fades to green
    {dim_curve[20], dim_curve[90], dim_curve[20]},
    {dim_curve[10], dim_curve[60], dim_curve[10]},
    {dim_curve[5], dim_curve[30], dim_curve[5]},
    {0, dim_curve[20], 0},
    {0, dim_curve[10], 0},
    {0, dim_curve[7], 0},
    {0, dim_curve[5], 0}
};

void matrix_loop();

#endif //__MATRIX_H__
