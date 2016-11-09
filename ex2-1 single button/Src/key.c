#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"

void key_init(void)
{
	//端口时钟使能
	RCC->AHB1ENR |= 0x01<<0;
	//端口配置, PA0配置为普通功能,浮空输入
	GPIOA->MODER &=~(0x03<<0);//普通输入
	GPIOA->PUPDR &=~(0x03<<0);//无上下拉
}

u8 key_scanf(void)
{
	u8 key = NO_KEY;
	if(KEY1)  //如果识别为高电平
	{
		delay_ms(10);  //延时消抖
		if(KEY1)  //再次判断是否为高电平
		{
			key = KEY1_OK;
			while (KEY1); //判断按键是否释放
		}
	}
	return key;
}
