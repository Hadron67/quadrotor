#ifndef DELAY_H
#define DELAY_H
#define CRISTAL_FREQUENCY 72
#include "stm32f10x.h"
void delay_us(u32 nus);
void delay_ms(u32 nms);
#endif
