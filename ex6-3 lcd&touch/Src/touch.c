#include "stm32f4xx.h"
#include "touch.h"
#include "delay.h"

/****************************
//�˿���������GPIO�����ù���
void touch_init(void)
{
	//T_SCK(PB0)----��ͨ�����������  
	//T_PEN(PB1)----��ͨ����,��������   
	//T_CS(PC13)----��ͨ�����������
	//T_MISO(PB2)----��ͨ����,�������� 
	//T_MOSI(PF11)----��ͨ�����������
	//�˿�ʱ��ʹ��
	RCC->AHB1ENR |= 0X01<<1 | 0X01<<2| 0X01<<5;
	
	GPIOB->MODER &= ~(0X03<<0 | 0X03<<2 | 0X03<<4);
	GPIOB->MODER |= 0X01<<0;
	GPIOB->OTYPER &= ~(0X01<<0);
	GPIOB->OSPEEDR &= ~(0X03<<0);  //SCK 2M
	GPIOB->PUPDR  &=~(0X03<<0 | 0X03<<2 |0x03<<4);  //��������
	T_SCK = 0;
	
	GPIOC->MODER &= ~(0X03<<26);
	GPIOC->MODER |= 0X01<<26;
	GPIOC->OTYPER &= ~(0X01<<13);  //����
	GPIOC->OSPEEDR &= ~(0X03<<26); //CS 2M
	GPIOC->PUPDR  &=~(0X03<<26);  //��������
	T_CS = 1;
	
	GPIOF->MODER &= ~(0X03<<22);
	GPIOF->MODER |= 0X01<<22;
	GPIOF->OTYPER &= ~(0X01<<11);  //����
	GPIOF->OSPEEDR &= ~(0X03<<22);  //MOSI 2M
	GPIOF->PUPDR  &=~(0X03<<22);  //��������
	T_MOSI = 0;
	
}
*******************************/

//��ȡX/Y������������ת���Ľ��
//������commom=CMD_RDX ��ȡX����ת���Ľ��
//		commom=CMD_RDY ��ȡY����ת���Ľ��
//����ֵ��ADת�����
u16 get_touch_ad(u8 common)
{
	u8 i;
	u16 _data;
	T_MOSI = 0;
	T_SCK = 0;
	T_CS = 0;
	for(i=0; i<8; i++)  //����������
	{
		T_SCK = 0;
		if(common&0x80) T_MOSI = 1;  //��λ��ǰ
		else T_MOSI = 0;
		delay_us(1);
		T_SCK = 1; //���ߺ�,XPT2046��ʼ�ɼ�����
		delay_us(1);
		common = common<<1;
	}
	T_SCK = 0;
	
	delay_us(150);
	//�ȴ�ת������busy, ͨ������T_SCK����
	T_SCK = 1;
	delay_us(1);
	
	//��MISO��ȡ���ص�12λ����ֵ,����_data
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
	u16 buf[10],a;  //a������ʱ��Žϴ�ֵ���ܺ�
	
	//��x��ƽ��ֵ
	for(i=0; i<10; i++)
	{
		buf[i] = get_touch_ad(CMD_RDX);
	}
	//�ȴ�С��������, ��ȡ1~8��������ƽ��ֵ
	for(i=0; i<9; i++)  
		for(j=i+1; j<10; j++)  //forѭ��Ƕ��, �Ӳ��Ӵ����Ŷ�һ��
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
	
	//��y��ƽ��ֵ
	for(i=0; i<10; i++)
	{
		buf[i] = get_touch_ad(CMD_RDY);
	}
	//�ȴ�С��������, ��ȡ1~8��������ƽ��ֵ
	for(i=0; i<9; i++)  
		for(j=i+1; j<10; j++)  //forѭ��Ƕ��, �Ӳ��Ӵ����Ŷ�һ��
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
