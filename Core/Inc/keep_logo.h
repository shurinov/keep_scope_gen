#ifndef __KEEP_LOGO_H__
#define __KEEP_LOGO_H__
#include "stm32f3xx_hal.h"

typedef struct {
  int16_t x;
  int16_t y;
}point_t;

typedef struct {
  int nodesNumb;
  int16_t *arrX;
  int16_t *arrY;
} brLine_t;

void drawKeep(void);
void drawPFK(void);
void drawTBTC(void);

#endif //__KEEP_LOGO_H__

