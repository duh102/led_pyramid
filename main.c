#include <samd21e15l.h>
#include <stdint.h>
#include <stdlib.h>
#include "pyramid_structure.h"
#include "dim_curve.h"
#include "patterns/matrix.h"
#include "patterns/fireworks.h"

// reserving 3*(leds) bytes for keeping the data easily accessible
uint8_t colors[MAX_LED][3];
uint8_t state_indices[MAX_LED];

#define DISPLAYS 3
uint8_t state = 0;
uint8_t display = 0;

uint8_t* get_color(uint8_t idx) {
  return colors[idx];
}
void set_color(uint8_t idx, uint8_t* color) {
  colors[idx][0] = color[0];
  colors[idx][1] = color[1];
  colors[idx][2] = color[2];
}
void set_color_pos(uint8_t panel, uint8_t x, uint8_t y, uint8_t* color) {
  uint8_t idx = get_idx(panel,x,y);
  if(idx == 255) {
    return;
  }
  set_color(idx, color);
}
uint8_t get_state(uint8_t idx) {
  return state_indices[idx];
}
void set_state(uint8_t idx, uint8_t state) {
  state_indices[idx] = state;
}
void blank_display() {
  for(uint8_t px = 0; px < PANELS*LEDS_PER_PANEL; px++) {
    set_color(px, (uint8_t[]){0,0,0});
  }
}

uint8_t get_idx(uint8_t panel, uint8_t x, uint8_t y) {
  if( x >= PYRAMID_WIDTH || y >= PYRAMID_HEIGHT || panel >= PANELS) {
    return 255;
  }
  uint8_t idx = pyramid_xy_map[(PYRAMID_HEIGHT-1)-y][x];
  if(idx == 255) {
    return idx;
  }
  return idx + (panel*LEDS_PER_PANEL);
}

uint8_t pattern = 0;

void delay(int n)
{
    int i;

    for (;n >0; n--)
    {
        for (i=0;i<100;i++)
            __asm("nop");
    }
}

void spiInit(void)
{
    // See section 27.7 for register details!
    PM->APBCMASK.bit.SERCOM1_ = 1;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_ID_SERCOM1_CORE;
    while(GCLK->STATUS.bit.SYNCBUSY);
    const SERCOM_SPI_CTRLA_Type ctrla = {
      .bit.DORD = 0, // MSB first
      .bit.CPHA = 0, // Mode 0
      .bit.CPOL = 0,
      .bit.FORM = 0, // SPI frame
      .bit.DIPO = 3, // MISO on PAD[3]
      .bit.DOPO = 0, // MOSI on PAD[0], SCK on PAD[1], SS_ on PAD[2]
      .bit.MODE = 3  // Master
    };
    SERCOM1->SPI.CTRLA.reg = ctrla.reg;
    const SERCOM_SPI_CTRLB_Type ctrlb = {
      .bit.RXEN = 0,   // RX enabled
      .bit.MSSEN = 0,  // HW SS
      .bit.CHSIZE = 0 // 8-bit
    };
    SERCOM1->SPI.CTRLB.reg = ctrlb.reg;	

    SERCOM1->SPI.BAUD.reg = 0; // Rate is clock / 2 

    // Mux for SERCOM1 PA16,PA17,PA18,PA19
    const PORT_WRCONFIG_Type wrconfig = {
      .bit.WRPINCFG = 1,
      .bit.WRPMUX = 1,
      .bit.PMUX = MUX_PA16C_SERCOM1_PAD0,
      .bit.PMUXEN = 1,
      .bit.HWSEL = 1,
      .bit.PINMASK = (uint16_t)((PORT_PA16 | PORT_PA17 | PORT_PA18 | PORT_PA19) >> 16)
    };
    PORT->Group[0].WRCONFIG.reg = wrconfig.reg;

    SERCOM1->SPI.CTRLA.bit.ENABLE = 1;
    while(SERCOM1->SPI.SYNCBUSY.bit.ENABLE);
}

uint8_t spiSendWs2812(uint8_t r, uint8_t g, uint8_t b)
{
    while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
    SERCOM1->SPI.DATA.reg = g;
    while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
    SERCOM1->SPI.DATA.reg = r;
    while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
    SERCOM1->SPI.DATA.reg = b;
    return 0;
}

int main(void)
{
    //SystemInit();
    srand(100);
    for( uint32_t i = 0; i < MAX_LED; i++ ) {
        if(rand()%100 < 5) {
            state_indices[i]  = rand()%10;
        }
    }
    spiInit();

    while(1) {

        state++;
        if(state == 255) {
          display = (display+1) % DISPLAYS;
          blank_display();
        }
        switch(display) {
          case 0:
            firework_loop();
            delay(150);
            break;
          case DISPLAYS-1:
          default:
            matrix_loop();
            delay(100);
        }

        for( uint32_t i = 0; i < MAX_LED; i++) {
          spiSendWs2812(colors[i][0], colors[i][1], colors[i][2]);
        }
    }
}
