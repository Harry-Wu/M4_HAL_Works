#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f4xx.h"

#define KEY1 (GPIOA->IDR & (0x01<<0))
#define NO_KEY 0xFF
#define KEY1_OK 1

void key_init(void);
u8 key_scanf(void);

#endif
