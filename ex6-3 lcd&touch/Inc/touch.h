#ifndef _TOUCH_H_
#define _TOUCH_H_
#include "stm32f4xx.h"
#include "MyTypeDef.h"

#define T_CS 	PCout(13)
#define T_SCK	PBout(0)
#define T_MOSI 	PFout(11)
#define T_MISO	PBin(2)
#define T_PEN	PBin(1)

#define CMD_RDX 0XD0
#define CMD_RDY 0X90

//记录x,y轴数值结构体
typedef struct {
	u16 x;
	u16 y;
}_TOUCH_TYPEDEF;



//void touch_init(void);
u16 get_touch_ad(u8 common);
void get_touch_ad_filled(_TOUCH_TYPEDEF *p);

#endif
