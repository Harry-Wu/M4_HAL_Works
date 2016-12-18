#include "stm32f4xx.h"
#include "lcd.h"
#include "delay.h"
#include "MyTypeDef.h"
#include "stdlib.h"

u16 BACK_COLOR=0XFFFF;  //背景色在这里设置,暂时没用到
u16 POINT_COLOR=0X0000;  //绘图颜色

//void lcd_port_init(void)
//{
//	//端口时钟使能PB,PD,PE,PF,PG
//	RCC->AHB1ENR |= 0x01<<1 | 0x01<<3 | 0x01<<4 | 0x01<<5 | 0x01<<6;
//	//端口模式配置--普通功能推挽输出
//	//PB15
//	GPIOB->MODER &= ~(0x03U<<30);
//	GPIOB->MODER |= 0X01U<<30;
//	//推挽是默认值,以后默认值不再设置
//	GPIOB->OSPEEDR |= 0x03U<<30;
//	
//	//PD0,1,4,5,8,9,10,14,15
//	GPIOD->MODER &=~(0X03<<0 | 0X03<<2 | 0X03<<8 | 0X03<<10 | 0X03<<16 | 0X03<<18 | 0X03<<20 | 0X03<<28 | 0X03<<30  );
//	GPIOD->MODER |=0X01<<0 | 0X01<<2 | 0X01<<8 | 0X01<<10 | 0X01<<16 | 0X01<<18 | 0X01<<20 | 0X01<<28 | 0X01<<30;
//	GPIOD->OSPEEDR |=0X03<<0 | 0X03<<2 | 0X03<<8 | 0X03<<10 | 0X03<<16 | 0X03<<18 | 0X03<<20 | 0X03<<28 | 0X03<<30  ;
//	//PE 7~15
//	GPIOE->MODER &=~(0X03<<14 |0X03<<16  |0X03<<18  |0X03<<20  |0X03<<22 |0X03<<24 |0X03<<26 |0X03<<28 |0X03<<30);
//	GPIOE->MODER |=(0X01<<14 |0X01<<16  |0X01<<18  |0X01<<20  |0X01<<22 |0X01<<24 |0X01<<26 |0X01<<28 |0X01<<30);
//	GPIOE->OSPEEDR |=(0X03<<14 |0X03<<16  |0X03<<18  |0X03<<20  |0X03<<22 |0X03<<24 |0X03<<26 |0X03<<28 |0X03<<30);
//	//PF 12
//	GPIOF->MODER &=~(0X03<<24);
//	GPIOF->MODER |=0X01<<24;
//	GPIOF->OSPEEDR |=(0X03<<24);
//	// PG 12
//	GPIOG->MODER &=~(0X03<<24);
//	GPIOG->MODER |=0X01<<24;
//	GPIOG->OSPEEDR |=(0X03<<24);
//}

void LCD_ILI9341_CMD(u8 common)
{
	//发送命令
	LCD_RS = 0;
	LCD_RD = 1;
	LCD_WR = 1;
	LCD_CS = 0;
	LCD_WR = 0;  //开始准备数据
	if(common & 0x01) LCD_D0 = 1; else LCD_D0 = 0;
	if(common & 0x02) LCD_D1 = 1; else LCD_D1 = 0;
	if(common & 0x04) LCD_D2 = 1; else LCD_D2 = 0;
	if(common & 0x08) LCD_D3 = 1; else LCD_D3 = 0;
	if(common & 0x10) LCD_D4 = 1; else LCD_D4 = 0;
	if(common & 0x20) LCD_D5 = 1; else LCD_D5 = 0;
	if(common & 0x40) LCD_D6 = 1; else LCD_D6 = 0;
	if(common & 0x80) LCD_D7 = 1; else LCD_D7 = 0;
	LCD_WR = 1;
	LCD_CS =1;
	
}

//发送命令参数
void LCD_ILI9341_Parameter(u8 common)
{
	LCD_RS = 1;
	LCD_RD = 1;
	LCD_WR = 1;
	LCD_CS = 0;
	LCD_WR = 0;  //开始准备数据
	if(common & 0x01) LCD_D0 = 1; else LCD_D0 = 0;
	if(common & 0x02) LCD_D1 = 1; else LCD_D1 = 0;
	if(common & 0x04) LCD_D2 = 1; else LCD_D2 = 0;
	if(common & 0x08) LCD_D3 = 1; else LCD_D3 = 0;
	if(common & 0x10) LCD_D4 = 1; else LCD_D4 = 0;
	if(common & 0x20) LCD_D5 = 1; else LCD_D5 = 0;
	if(common & 0x40) LCD_D6 = 1; else LCD_D6 = 0;
	if(common & 0x80) LCD_D7 = 1; else LCD_D7 = 0;
	LCD_WR = 1;
	LCD_CS =1;
	
}

//发送数据
void lcd_send_show_data(u16 common)
{	
	LCD_RS = 1;
	LCD_RD = 1;
	LCD_WR = 1;
	LCD_CS = 0;
	LCD_WR = 0;  //开始准备数据
	if(common & 0x01) LCD_D0 = 1; else LCD_D0 = 0;
	if(common & 0x02) LCD_D1 = 1; else LCD_D1 = 0;
	if(common & 0x04) LCD_D2 = 1; else LCD_D2 = 0;
	if(common & 0x08) LCD_D3 = 1; else LCD_D3 = 0;
	if(common & 0x10) LCD_D4 = 1; else LCD_D4 = 0;
	if(common & 0x20) LCD_D5 = 1; else LCD_D5 = 0;
	if(common & 0x40) LCD_D6 = 1; else LCD_D6 = 0;
	if(common & 0x80) LCD_D7 = 1; else LCD_D7 = 0;
	
	if(common & 0x0100) LCD_D8 = 1; else LCD_D8 = 0;
	if(common & 0x0200) LCD_D9 = 1; else LCD_D9 = 0;
	if(common & 0x0400) LCD_D10 = 1; else LCD_D10 = 0;
	if(common & 0x0800) LCD_D11 = 1; else LCD_D11 = 0;
	if(common & 0x1000) LCD_D12 = 1; else LCD_D12 = 0;
	if(common & 0x2000) LCD_D13 = 1; else LCD_D13 = 0;
	if(common & 0x4000) LCD_D14 = 1; else LCD_D14 = 0;
	if(common & 0x8000) LCD_D15 = 1; else LCD_D15 = 0;
	LCD_WR = 1;
	LCD_CS =1;
	
}

void lcd_init(void)
{
	//端口初始化
	//lcd_port_init( );  //GPIO中已经初始化过了
	LCD_CS=1;
	
	
	//ili9341初始化
		//************* Reset LCD Driver ****************//
	//复位信号接到我们单片机的复位引脚
	HAL_Delay(120); // Delay 120 ms
	//************* Start Initial Sequence **********//
	LCD_ILI9341_CMD(0xCF);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x83);
	LCD_ILI9341_Parameter (0X30);
	LCD_ILI9341_CMD(0xED);
	LCD_ILI9341_Parameter (0x64);
	LCD_ILI9341_Parameter (0x03);
	LCD_ILI9341_Parameter (0X12);
	LCD_ILI9341_Parameter (0X81);
	LCD_ILI9341_CMD(0xE8);
	LCD_ILI9341_Parameter (0x85);
	LCD_ILI9341_Parameter (0x01);
	LCD_ILI9341_Parameter (0x79);
	LCD_ILI9341_CMD(0xCB);
	LCD_ILI9341_Parameter (0x39);
	LCD_ILI9341_Parameter (0x2C);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x34);
	LCD_ILI9341_Parameter (0x02);
	LCD_ILI9341_CMD(0xF7);
	LCD_ILI9341_Parameter (0x20);

	LCD_ILI9341_CMD(0xEA);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_CMD(0xC0); //Power control
	LCD_ILI9341_Parameter (0x1D); //VRH[5:0]
	LCD_ILI9341_CMD(0xC1); //Power control
	LCD_ILI9341_Parameter (0x11); //SAP[2:0];BT[3:0]
	LCD_ILI9341_CMD(0xC5); //VCM control
	LCD_ILI9341_Parameter (0x33);
	LCD_ILI9341_Parameter (0x34);
	LCD_ILI9341_CMD(0xC7); //VCM control2
	LCD_ILI9341_Parameter (0Xbe);
	LCD_ILI9341_CMD(0x36); // Memory Access Control
	LCD_ILI9341_Parameter (0x08);
	LCD_ILI9341_CMD(0xB1);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x1B);
	LCD_ILI9341_CMD(0xB6); // Display Function Control
	LCD_ILI9341_Parameter (0x0A);
	LCD_ILI9341_Parameter (0xA2);
	LCD_ILI9341_CMD(0xF2); // 3Gamma Function Disable
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_CMD(0x26); //Gamma curve selected
	LCD_ILI9341_Parameter (0x01);
	LCD_ILI9341_CMD(0xE0); //Set Gamma
	LCD_ILI9341_Parameter (0x0F);
	LCD_ILI9341_Parameter (0x23);
	LCD_ILI9341_Parameter (0x1F);

	LCD_ILI9341_Parameter (0x09);
	LCD_ILI9341_Parameter (0x0f);
	LCD_ILI9341_Parameter (0x08);
	LCD_ILI9341_Parameter (0x4B);
	LCD_ILI9341_Parameter (0Xf2);
	LCD_ILI9341_Parameter (0x38);
	LCD_ILI9341_Parameter (0x09);
	LCD_ILI9341_Parameter (0x13);
	LCD_ILI9341_Parameter (0x03);
	LCD_ILI9341_Parameter (0x12);
	LCD_ILI9341_Parameter (0x07);
	LCD_ILI9341_Parameter (0x04);
	LCD_ILI9341_CMD(0XE1); //Set Gamma
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x1d);
	LCD_ILI9341_Parameter (0x20);
	LCD_ILI9341_Parameter (0x02);
	LCD_ILI9341_Parameter (0x11);
	LCD_ILI9341_Parameter (0x07);
	LCD_ILI9341_Parameter (0x34);
	LCD_ILI9341_Parameter (0x81);
	LCD_ILI9341_Parameter (0x46);
	LCD_ILI9341_Parameter (0x06);
	LCD_ILI9341_Parameter (0x0e);
	LCD_ILI9341_Parameter (0x0c);
	LCD_ILI9341_Parameter (0x32);
	LCD_ILI9341_Parameter (0x38);
	LCD_ILI9341_Parameter (0x0F);
	
    /* 由我们添加 */
	LCD_ILI9341_CMD(0X3A); //设定16BPP
	LCD_ILI9341_Parameter(0X55);
	
	//LCD_ExitSleep_ILI9341();//退出睡眠状态
	LCD_ILI9341_CMD(0x11); // Sleep out
	HAL_Delay(120);
	LCD_ILI9341_CMD(0x29); // Display on
	
    lcd_clear(0xffff);//把LCD清成白屏
    
	//GPIOB->ODR |=  (0X1<<15);//开启背光
	LCD_BL = 1;  //开启背光
	
}

//清屏,写指定的颜色,相当于设置背景色
void lcd_clear(u16 color)
{
	u32 i;
	LCD_ILI9341_CMD(0X2A);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (239>>8);
	LCD_ILI9341_Parameter (239&0xff);
	
	LCD_ILI9341_CMD(0X2B);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (319>>8);
	LCD_ILI9341_Parameter (319&0xff);
	
	LCD_ILI9341_CMD(0X2C);
	for(i=0;i<240*320;i++)
	{
		lcd_send_show_data(color);
	}
}

//指定位置画一个点
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_ILI9341_CMD(0X2A);
	LCD_ILI9341_Parameter (x>>8);
	LCD_ILI9341_Parameter (x&0xff);

	
	LCD_ILI9341_CMD(0X2B);
	LCD_ILI9341_Parameter(y>>8);
	LCD_ILI9341_Parameter(y&0xff);

	LCD_ILI9341_CMD(0X2C);
	lcd_send_show_data(POINT_COLOR);

}


//////////////////////////////////////////////////////////////////////	 
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0,u16 y0,u16 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
				
	}
}

//////////////////////////////////////////////////////////////////////	 
//在指定位置画一个指定大小的实心圆
//(x,y):中心点
//r    :半径
void Draw_SolidCircle(u16 x0,u16 y0,u16 r)
{
	u16 i;
	for(i=r; i>1; i--)
	{
		Draw_Circle(x0, y0, i);
	}
}

//////////////////////////////////////////////////////////////////////	 
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		}
		
	}  
}   


//////////////////////////////////////////////////////////////////////	 
//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

//////////////////////////////////////////////////////////////////////	 
//画实心矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
//color: 实心的颜色
void LCD_DrawSolidRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u32 i;
	LCD_ILI9341_CMD(0X2A);  //发送x坐标
	LCD_ILI9341_Parameter(x1>>8);
	LCD_ILI9341_Parameter(x1&0xff);
	LCD_ILI9341_Parameter(x2>>8);
	LCD_ILI9341_Parameter(x2&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //发送y坐标
	LCD_ILI9341_Parameter(y1>>8);
	LCD_ILI9341_Parameter(y1&0xff);
	LCD_ILI9341_Parameter(y2>>8);
	LCD_ILI9341_Parameter(y2&0xff);
	
	LCD_ILI9341_CMD(0X2C);  //写GRAM指令，支持连续写
	for(i=(abs(x2-x1)+1)*(abs(y2-y1)+1); i>0; i--)
	{
		lcd_send_show_data(color);
	}
	//写完之后把x,y限定的区域改回成全屏幕
	LCD_ILI9341_CMD(0X2A);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (239>>8); 
	LCD_ILI9341_Parameter (239&0xff);
	LCD_ILI9341_CMD(0X2B);
	LCD_ILI9341_Parameter (0x00); 
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (319>>8); 
	LCD_ILI9341_Parameter (319&0xff);
}

//mode: 0:覆盖模式, 1:叠加模式
void show_ascii(u16 x, u16 y, s8 a, u8 mode)
{
	u16 i,j,_data;
	u16 color = POINT_COLOR;
	u16 x0 = x;
	a = a - ' '; //得到待显示字符的字模在库中行号
	for(i=0; i<16; i++)
	{
		_data = ASC_II[a][i];
		for(j=0; j<8; j++)
		{
			if(_data&0x80)  //判断最高位有没有像素点
			{
				LCD_DrawPoint(x,y);
			}
			else
			{
				if(mode==0)
				{
					POINT_COLOR=BACK_COLOR;
					LCD_DrawPoint(x,y);
					POINT_COLOR=color;
				}
			}
			//一个点处理完后进行的动作
			_data = _data<<1;
			x++;
			if(x-x0==8)  //判断一行是否结束，结束的话，x跳到初第一列，y跳到下一行
			{
				x = x0;
				y++;
			}
		}	
	}
}

void LCD_ShowString(u16 x, u16 y, u8 *p, u8 mode)
{
	while(*p != '\0')
	{
		show_ascii(x, y, *p, mode);
		p++;
		x = x+8;
	}
}


//mode: 0:覆盖模式, 1:叠加模式
//目前只能指定汉字所在数组的序号a
void LCD_Show_CH(u16 x, u16 y, u8 a, u8 mode)
{
	u16 i,j,_data;
	u16 color = POINT_COLOR;
	u16 x0 = x, y0=y;
	
	//先处理左半部分汉字, 16行
	for(i=0; i<16; i++)
	{
		_data = HZ[a][i];
		for(j=0; j<8; j++)
		{
			if(_data&0x80)  //判断最高位有没有像素点
			{
				LCD_DrawPoint(x,y);
			}
			else
			{
				if(mode==0)
				{
					POINT_COLOR=BACK_COLOR;
					LCD_DrawPoint(x,y);
					POINT_COLOR=color;
				}
			}
			//一个点处理完后进行的动作
			_data = _data<<1;
			x++;
			if(x-x0==8)  //判断一行是否结束，结束的话，x跳到初第一列，y跳到下一行
			{
				x = x0;
				y++;
			}
		}
	}
	x0 = x0 + 8;
	x = x0;
	y=y0;  //回到最上面一行

	//再处理又半部分汉字, 16行	
	for(i=16; i<32; i++)  //汉字占16列, 现在一行一行处理右边一半
	{
		_data = HZ[a][i];
		for(j=0; j<8; j++)
		{
			if(_data&0x80)  //判断最高位有没有像素点
			{
				LCD_DrawPoint(x,y);
			}
			else
			{
				if(mode==0)
				{
					POINT_COLOR=BACK_COLOR;
					LCD_DrawPoint(x,y);
					POINT_COLOR=color;
				}
			}
			//一个点处理完后进行的动作
			_data = _data<<1;
			x++;
			if(x-x0==8)  //判断一行是否结束，结束的话，x跳到初第一列，y跳到下一行
			{
				x = x0;
				y++;
			}
		}
	}
}


void LCD_ShowPic(u16 x, u16 y, u8 *p)
{
	u32 i;
	HEADCOLOR *pic_type;
	pic_type = (HEADCOLOR *)p;  //强转成HEADCOLOR类型结构体变量，不然会报错
	
	LCD_ILI9341_CMD(0X2A);  //发送x坐标范围
	LCD_ILI9341_Parameter(x>>8);
	LCD_ILI9341_Parameter(x&0xff);
	LCD_ILI9341_Parameter((x+pic_type->w-1)>>8);
	LCD_ILI9341_Parameter((x+pic_type->w-1)&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //发送y坐标范围
	LCD_ILI9341_Parameter(y>>8);
	LCD_ILI9341_Parameter(y&0xff);
	LCD_ILI9341_Parameter((y+pic_type->h-1)>>8);
	LCD_ILI9341_Parameter((y+pic_type->h-1)&0xff);
	
	LCD_ILI9341_CMD(0X2C);  //写GRAM指令，支持连续写
	for(i=0; i<pic_type->w*pic_type->h; i++)
	{
		lcd_send_show_data(*(p+8+2*i) | (*(p+8+2*i+1)<<8));  //图片数组中两个数据组成16位的RGB像素值，而且后一个是高位，所以左移8位拼起来
		//HAL_Delay(1);
		delay_us(10);
	}
	
	//写完之后把x,y限定的区域改回成全屏幕
	LCD_ILI9341_CMD(0X2A);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (239>>8); 
	LCD_ILI9341_Parameter (239&0xff);
	LCD_ILI9341_CMD(0X2B);
	LCD_ILI9341_Parameter (0x00); 
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (319>>8); 
	LCD_ILI9341_Parameter (319&0xff);
}
