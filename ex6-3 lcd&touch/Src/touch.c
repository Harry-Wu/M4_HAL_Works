#include "stm32f4xx.h"
#include "touch.h"
#include "delay.h"

/****************************
//端口设置已在GPIO中设置过了
void touch_init(void)
{
	//T_SCK(PB0)----普通功能推挽输出  
	//T_PEN(PB1)----普通输入,无上下拉   
	//T_CS(PC13)----普通功能推挽输出
	//T_MISO(PB2)----普通输入,无上下拉 
	//T_MOSI(PF11)----普通功能推挽输出
	//端口时钟使能
	RCC->AHB1ENR |= 0X01<<1 | 0X01<<2| 0X01<<5;
	
	GPIOB->MODER &= ~(0X03<<0 | 0X03<<2 | 0X03<<4);
	GPIOB->MODER |= 0X01<<0;
	GPIOB->OTYPER &= ~(0X01<<0);
	GPIOB->OSPEEDR &= ~(0X03<<0);  //SCK 2M
	GPIOB->PUPDR  &=~(0X03<<0 | 0X03<<2 |0x03<<4);  //无上下拉
	T_SCK = 0;
	
	GPIOC->MODER &= ~(0X03<<26);
	GPIOC->MODER |= 0X01<<26;
	GPIOC->OTYPER &= ~(0X01<<13);  //推挽
	GPIOC->OSPEEDR &= ~(0X03<<26); //CS 2M
	GPIOC->PUPDR  &=~(0X03<<26);  //无上下拉
	T_CS = 1;
	
	GPIOF->MODER &= ~(0X03<<22);
	GPIOF->MODER |= 0X01<<22;
	GPIOF->OTYPER &= ~(0X01<<11);  //推挽
	GPIOF->OSPEEDR &= ~(0X03<<22);  //MOSI 2M
	GPIOF->PUPDR  &=~(0X03<<22);  //无上下拉
	T_MOSI = 0;
	
}
*******************************/

//获取X/Y方向触摸屏坐标转换的结果
//参数：commom=CMD_RDX 获取X方向转换的结果
//		commom=CMD_RDY 获取Y方向转换的结果
//返回值：AD转换结果
u16 get_touch_ad(u8 common)
{
	u8 i;
	u16 _data;
	T_MOSI = 0;
	T_SCK = 0;
	T_CS = 0;
	for(i=0; i<8; i++)  //发送命令字
	{
		T_SCK = 0;
		if(common&0x80) T_MOSI = 1;  //高位在前
		else T_MOSI = 0;
		delay_us(1);
		T_SCK = 1; //拉高后,XPT2046开始采集数据
		delay_us(1);
		common = common<<1;
	}
	T_SCK = 0;
	
	delay_us(150);
	//等待转换完清busy, 通过拉高T_SCK来清
	T_SCK = 1;
	delay_us(1);
	
	//从MISO获取返回的12位坐标值,赋给_data
	for(i=0; i<12; i++)
	{
		T_SCK = 0;
		delay_us(1);
		T_SCK = 1;
		_data = _data<<1;
		if(T_MISO)
		{
			_data |= 0x01;
		}
		delay_us(1);		
	}
	T_SCK = 0;
	T_CS = 1;
	T_MOSI = 0;
	return _data;
}

void get_touch_ad_filled(_TOUCH_TYPEDEF *p)
{
	u8 i,j;
	u16 buf[10],a;  //a用来临时存放较大值和总和
	
	//求x的平均值
	for(i=0; i<10; i++)
	{
		buf[i] = get_touch_ad(CMD_RDX);
	}
	//先从小到大排序, 再取1~8的数组求平均值
	for(i=0; i<9; i++)  
		for(j=i+1; j<10; j++)  //for循环嵌套, 加不加大括号都一样
		{
			if(buf[i]>buf[j])
			{
				a = buf[i];
				buf[i] = buf[j];
				buf[j] = a;
			}
		}
		a = 0;
	for(i=1; i<9; i++)
	{
		a+= buf[i];
	}
	p->x = a/8;
	
	//求y的平均值
	for(i=0; i<10; i++)
	{
		buf[i] = get_touch_ad(CMD_RDY);
	}
	//先从小到大排序, 再取1~8的数组求平均值
	for(i=0; i<9; i++)  
		for(j=i+1; j<10; j++)  //for循环嵌套, 加不加大括号都一样
		{
			if(buf[i]>buf[j])
			{
				a = buf[i];
				buf[i] = buf[j];
				buf[j] = a;
			}
		}
		a = 0;
	for(i=1; i<9; i++)
	{
		a+= buf[i];
	}
	p->y = a/8;
}
