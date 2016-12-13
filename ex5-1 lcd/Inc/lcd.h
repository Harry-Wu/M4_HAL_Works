#ifndef _LCD_H_
#define _LCD_H_
#include "stm32f4xx.h"
//#include "sys.h"
#include "MyTypeDef.h"

//普通功能推挽输出,.C文件里会具体设置
#define LCD_CS PGout(12)  
#define LCD_RS PFout(12)  
#define LCD_WR PDout(5)  
#define LCD_RD PDout(4) 

#define LCD_BL PBout(15)  

#define LCD_D0 PDout(14)
#define LCD_D1 PDout(15)
#define LCD_D2 PDout(0)
#define LCD_D3 PDout(1)
#define LCD_D4 PEout(7)
#define LCD_D5 PEout(8)
#define LCD_D6 PEout(9)
#define LCD_D7 PEout(10)
#define LCD_D8 PEout(11)
#define LCD_D9 PEout(12)
#define LCD_D10 PEout(13)
#define LCD_D11 PEout(14)
#define LCD_D12 PEout(15)
#define LCD_D13 PDout(8)
#define LCD_D14 PDout(9)
#define LCD_D15 PDout(10)

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE		     0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

extern u16 BACK_COLOR;
extern u16 POINT_COLOR;

extern const u8 ASC_II[][16];
extern const u8 HZ[][32];

void lcd_init(void);
void lcd_clear(u16 color);
void LCD_DrawPoint(u16 x,u16 y);

void Draw_Circle(u16 x0,u16 y0,u16 r);
void Draw_SolidCircle(u16 x0,u16 y0,u16 r);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawSolidRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);


void show_ascii(u16 x, u16 y, s8 a, u8 mode);
void LCD_ShowString(u16 x, u16 y, u8 *p, u8 mode);
void LCD_Show_CH(u16 x, u16 y, u8 a, u8 mode);


#endif
