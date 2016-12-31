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

#define TOUCH_ADJ_OK   1
#define TOUCH_ADJ_FAIL 0

//保存触摸屏的x,y坐标
typedef struct {
	u16 x;
	u16 y;
}_TOUCH_COOR_TYPEDEF;

//保存比例系数fx,fy和偏移量x_offset,y_offset
typedef struct
{
	float fx;
	s16 x_offset;
	float fy;
	s16 y_offset;
}_TOUCH_ADJ_TYPEDEF;


extern _TOUCH_ADJ_TYPEDEF touch_adj_parm;

void touch_init(void);
u16 get_touch_ad(u8 common);
void get_touch_ad_filled(_TOUCH_COOR_TYPEDEF *p);

u8 touch_adj(void);
void CNV_touch2lcd(_TOUCH_COOR_TYPEDEF *p);

void touch_scanf(_TOUCH_COOR_TYPEDEF *p,u8 mode);

#endif
