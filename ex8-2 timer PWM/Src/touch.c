#include "stm32f4xx.h"
#include "touch.h"
#include "delay.h"
#include "lcd.h"
#include "at24c02.h"


//�˿���������GPIO�����ù���, ����ֻ���д���У׼
void touch_init(void)
{
	u8 touch_adj_or_not;  //�������Ƿ�У׼��ʶ��
	
	//T_SCK(PB0)----��ͨ�����������  
	//T_PEN(PB1)----��ͨ����,��������   
	//T_CS(PC13)----��ͨ�����������
	//T_MISO(PB2)----��ͨ����,�������� 
	//T_MOSI(PF11)----��ͨ�����������
	//�˿�ʱ��ʹ��
//	RCC->AHB1ENR |= 0X01<<1 | 0X01<<2| 0X01<<5;
//	
//	GPIOB->MODER &= ~(0X03<<0 | 0X03<<2 | 0X03<<4);
//	GPIOB->MODER |= 0X01<<0;
//	GPIOB->OTYPER &= ~(0X01<<0);
//	GPIOB->OSPEEDR &= ~(0X03<<0);  //SCK 2M
//	GPIOB->PUPDR  &=~(0X03<<0 | 0X03<<2 |0x03<<4);  //��������
//	T_SCK = 0;
//	
//	GPIOC->MODER &= ~(0X03<<26);
//	GPIOC->MODER |= 0X01<<26;
//	GPIOC->OTYPER &= ~(0X01<<13);  //����
//	GPIOC->OSPEEDR &= ~(0X03<<26); //CS 2M
//	GPIOC->PUPDR  &=~(0X03<<26);  //��������
//	T_CS = 1;
//	
//	GPIOF->MODER &= ~(0X03<<22);
//	GPIOF->MODER |= 0X01<<22;
//	GPIOF->OTYPER &= ~(0X01<<11);  //����
//	GPIOF->OSPEEDR &= ~(0X03<<22);  //MOSI 2M
//	GPIOF->PUPDR  &=~(0X03<<22);  //��������
//	T_MOSI = 0;
	
	//��ȡ�������Ƿ��Ѿ�У׼������
	at24c02_read_byte(TOUCH_ADJ_OR, &touch_adj_or_not);
	
	if(touch_adj_or_not!=0x51)//���û��У׼��,����У׼����
	{
		//touch_adj();
		if(touch_adj()) LCD_ShowString(20,160,"TOUCH ADJUST PASS!",0);
		delay_ms(500);
		delay_ms(500);
		touch_adj_or_not=0x51;  //��ʶ����Ϊ��У׼
		at24c02_write_byte(TOUCH_ADJ_OR, touch_adj_or_not);
		at24c02_write_bytes(TOUCH_ADJ_ADDR, sizeof(_TOUCH_ADJ_TYPEDEF), (u8 *)&touch_adj_parm);
	}
	at24c02_read_bytes(TOUCH_ADJ_ADDR, sizeof(_TOUCH_ADJ_TYPEDEF), (u8 *)&touch_adj_parm);
}


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

void get_touch_ad_filled(_TOUCH_COOR_TYPEDEF *p)
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

//��ʮ��
void LCD_DrawCross(u16 x, u16 y, u16 len)
{
	LCD_DrawLine(x-len/2,y,x+len/2,y);
	LCD_DrawLine(x,y-len/2,x,y+len/2);
}

_TOUCH_ADJ_TYPEDEF touch_adj_parm;

u8 touch_adj(void)
{
	_TOUCH_COOR_TYPEDEF touch[4];
	float f1, f2;
	READJ:
	LCD_DrawCross(20, 20, 10);  //��LCD�ϻ���һ����
	while(T_PEN);     					 //�ȴ�������������  
	get_touch_ad_filled(&touch[0]);	//��ȡ��������
	while(T_PEN==0);			//�ȴ����������ͷ�
	
	POINT_COLOR=BACK_COLOR;
	LCD_DrawCross(20,20, 10);  //���LCD�ϵ�һ����
	delay_ms(100);
	
	POINT_COLOR=BLACK;
	LCD_DrawCross(239-20,20, 10);   					 //��LCD�ϻ���2����
	while(T_PEN);     					 //�ȴ�������������  
	get_touch_ad_filled(&touch[1]);	//��ȡ��������
	while(T_PEN==0);			//�ȴ����������ͷ�	
	
	POINT_COLOR=BACK_COLOR;
	LCD_DrawCross(239-20,20, 10);   //���LCD�ϵ�2����
	delay_ms(100);
	
	POINT_COLOR=BLACK;
	LCD_DrawCross(20,319-20, 10);   					 //��LCD�ϻ���3����
	while(T_PEN);     					 //�ȴ�������������  
	get_touch_ad_filled(&touch[2]);	//��ȡ��������
	while(T_PEN==0);			//�ȴ����������ͷ�
	
	POINT_COLOR=BACK_COLOR;
	LCD_DrawCross(20,319-20, 10);    //���LCD�ϵ�3����
	delay_ms(100);
		
	POINT_COLOR=BLACK;
	LCD_DrawCross(239-20,319-20, 10);   					 //��LCD�ϻ���4����
	while(T_PEN);     					 //�ȴ�������������  
	get_touch_ad_filled(&touch[3]);	//��ȡ��������
	while(T_PEN==0);			//�ȴ����������ͷ�
	
	POINT_COLOR=BACK_COLOR;
	LCD_DrawCross(239-20,319-20, 10);    //���LCD�ϵ�4����
	POINT_COLOR=BLACK;
	
	//��֤�����Ƿ�׼ȷ
	//��1 2����䴥���������ƽ��
	f1=(touch[1].x - touch[0].x)*(touch[1].x - touch[0].x)
		+(touch[1].y - touch[0].y)*(touch[1].y - touch[0].y);
	
	//��3 4����䴥���������ƽ��
	f2=(touch[3].x - touch[2].x)*(touch[3].x - touch[2].x)
		+(touch[3].y - touch[2].y)*(touch[3].y - touch[2].y);
		
	if(f1/f2>1.1f  || f1/f2<0.9f)
	{
		goto READJ;
	}
		
	//��1 3����䴥���������ƽ��
	f1=(touch[2].x - touch[0].x)*(touch[2].x - touch[0].x)
		+(touch[2].y - touch[0].y)*(touch[2].y - touch[0].y);
	
	//��2 4����䴥���������ƽ��
	f2=(touch[3].x - touch[1].x)*(touch[3].x - touch[1].x)
		+(touch[3].y - touch[1].y)*(touch[3].y - touch[1].y);
		
	if(f1/f2>1.1f  || f1/f2<0.9f)  //�����������10%, ������ƫ��,�򷵻�����У׼
	{
		goto READJ;
	}
			
	//��1 4����䴥���������ƽ��
	f1=(touch[3].x - touch[0].x)*(touch[3].x - touch[0].x)
		+(touch[3].y - touch[0].y)*(touch[3].y - touch[0].y);
	
	//��2 2����䴥���������ƽ��
	f2=(touch[2].x - touch[1].x)*(touch[2].x - touch[1].x)
		+(touch[2].y - touch[1].y)*(touch[2].y - touch[1].y);
		
	if(f1/f2>1.1f  || f1/f2<0.9f)  //�����������10%
	{
		goto READJ;
	}
			
	/********************************************************
	//��LCD����������Ӧ��ϵ
	//�����������LCD�����ϵ��

	//��ʽ��lcd_x=fx*touch_x+xoffset;
	//lcd_x��		ĳ���Ӧlcd X����
	//touch_x��	ĳ���Ӧ������ X����
	//fx��			����ϵ��
	//xoffset��		ƫ����

	//�з��������ϵ����ƫ����
	//20 =fx*touchxy[0].x+xoffset
	//220=fx*touchxy[1].x+xoffset

	//fx=((240-20) -20)/(touchxy[1].x-touchxy[0].x)
	//xoffset=20-fx*touchxy[0].x
	************************************************************/
	//x�������ϵ��
	touch_adj_parm.fx = (float)(239-20 - 20)/(touch[1].x - touch[0].x);
	//x����ƫ����
	touch_adj_parm.x_offset = 20 - touch_adj_parm.fx * touch[0].x;
	
	//y�������ϵ��
	touch_adj_parm.fy=(float)(319-20 - 20)/(touch[2].y - touch[0].y);
	//y����ƫ����
	touch_adj_parm.y_offset=20 - touch_adj_parm.fy * touch[0].y;
	
	return TOUCH_ADJ_OK;
}

void CNV_touch2lcd(_TOUCH_COOR_TYPEDEF *p)
{
	if(T_PEN==0)   //�������б�����
	{
		get_touch_ad_filled(p);
		p->x=touch_adj_parm.fx * p->x +touch_adj_parm.x_offset;
		p->y=touch_adj_parm.fy * p->y +touch_adj_parm.y_offset;
	}
	else
	{
		p->x=0xffff;
		p->y=0xffff;
	}
}

//��������ʶ��
//mode=0, ������ʶ��, mode=1, ����ʶ��

void touch_scanf(_TOUCH_COOR_TYPEDEF *p, u8 mode)
{
	static u8 touch_sta=1;  //touch_sta=1����ʶ��
	_TOUCH_COOR_TYPEDEF touch;
	touch.x = 0xffff;
	touch.y = 0xffff;
	
	if(mode)  //����ʶ��
	{
		touch_sta = 1;  //����û���ͷ�ʱ, �ظ�����touch_scanf����, �����ٴ�ʶ��
	}
	if((touch_sta==1)&&(T_PEN==0))  //������ʶ��, ���Ҵ�����������
	{
		delay_ms(5);  //��ʱ����
		if(T_PEN==0)
		{
			get_touch_ad_filled(p);  //�õ���������x,yֵ
			touch.x=touch_adj_parm.fx * p->x +touch_adj_parm.x_offset;
			touch.y=touch_adj_parm.fy * p->y +touch_adj_parm.y_offset;
			touch_sta=0;    //����
		}
	}
	else if(T_PEN == 1)
	{
		touch_sta = 1;
	}
	*p = touch;
}

