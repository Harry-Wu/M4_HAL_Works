#include "stm32f4xx.h"
#include "lcd.h"
#include "delay.h"
#include "MyTypeDef.h"
#include "stdlib.h"

u16 BACK_COLOR=0XFFFF;  //����ɫ����������,��ʱû�õ�
u16 POINT_COLOR=0X0000;  //��ͼ��ɫ

//void lcd_port_init(void)
//{
//	//�˿�ʱ��ʹ��PB,PD,PE,PF,PG
//	RCC->AHB1ENR |= 0x01<<1 | 0x01<<3 | 0x01<<4 | 0x01<<5 | 0x01<<6;
//	//�˿�ģʽ����--��ͨ�����������
//	//PB15
//	GPIOB->MODER &= ~(0x03U<<30);
//	GPIOB->MODER |= 0X01U<<30;
//	//������Ĭ��ֵ,�Ժ�Ĭ��ֵ��������
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
	//��������
	LCD_RS = 0;
	LCD_RD = 1;
	LCD_WR = 1;
	LCD_CS = 0;
	LCD_WR = 0;  //��ʼ׼������
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

//�����������
void LCD_ILI9341_Parameter(u8 common)
{
	LCD_RS = 1;
	LCD_RD = 1;
	LCD_WR = 1;
	LCD_CS = 0;
	LCD_WR = 0;  //��ʼ׼������
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

//��������
void lcd_send_show_data(u16 common)
{	
	LCD_RS = 1;
	LCD_RD = 1;
	LCD_WR = 1;
	LCD_CS = 0;
	LCD_WR = 0;  //��ʼ׼������
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
	//�˿ڳ�ʼ��
	//lcd_port_init( );  //GPIO���Ѿ���ʼ������
	LCD_CS=1;
	
	
	//ili9341��ʼ��
		//************* Reset LCD Driver ****************//
	//��λ�źŽӵ����ǵ�Ƭ���ĸ�λ����
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
	
    /* ��������� */
	LCD_ILI9341_CMD(0X3A); //�趨16BPP
	LCD_ILI9341_Parameter(0X55);
	
	//LCD_ExitSleep_ILI9341();//�˳�˯��״̬
	LCD_ILI9341_CMD(0x11); // Sleep out
	HAL_Delay(120);
	LCD_ILI9341_CMD(0x29); // Display on
	
    lcd_clear(0xffff);//��LCD��ɰ���
    
	//GPIOB->ODR |=  (0X1<<15);//��������
	LCD_BL = 1;  //��������
	
}

//����,дָ������ɫ,�൱�����ñ���ɫ
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

//ָ��λ�û�һ����
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
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u16 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
				
	}
}

//////////////////////////////////////////////////////////////////////	 
//��ָ��λ�û�һ��ָ����С��ʵ��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_SolidCircle(u16 x0,u16 y0,u16 r)
{
	u16 i;
	for(i=r; i>1; i--)
	{
		Draw_Circle(x0, y0, i);
	}
}

//////////////////////////////////////////////////////////////////////	 
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}

//////////////////////////////////////////////////////////////////////	 
//��ʵ�ľ���	  
//(x1,y1),(x2,y2):���εĶԽ�����
//color: ʵ�ĵ���ɫ
void LCD_DrawSolidRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u32 i;
	LCD_ILI9341_CMD(0X2A);  //����x����
	LCD_ILI9341_Parameter(x1>>8);
	LCD_ILI9341_Parameter(x1&0xff);
	LCD_ILI9341_Parameter(x2>>8);
	LCD_ILI9341_Parameter(x2&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //����y����
	LCD_ILI9341_Parameter(y1>>8);
	LCD_ILI9341_Parameter(y1&0xff);
	LCD_ILI9341_Parameter(y2>>8);
	LCD_ILI9341_Parameter(y2&0xff);
	
	LCD_ILI9341_CMD(0X2C);  //дGRAMָ�֧������д
	for(i=(abs(x2-x1)+1)*(abs(y2-y1)+1); i>0; i--)
	{
		lcd_send_show_data(color);
	}
	//д��֮���x,y�޶�������Ļس�ȫ��Ļ
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

//mode: 0:����ģʽ, 1:����ģʽ
void show_ascii(u16 x, u16 y, s8 a, u8 mode)
{
	u16 i,j,_data;
	u16 color = POINT_COLOR;
	u16 x0 = x;
	a = a - ' '; //�õ�����ʾ�ַ�����ģ�ڿ����к�
	for(i=0; i<16; i++)
	{
		_data = ASC_II[a][i];
		for(j=0; j<8; j++)
		{
			if(_data&0x80)  //�ж����λ��û�����ص�
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
			//һ���㴦�������еĶ���
			_data = _data<<1;
			x++;
			if(x-x0==8)  //�ж�һ���Ƿ�����������Ļ���x��������һ�У�y������һ��
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


//mode: 0:����ģʽ, 1:����ģʽ
//Ŀǰֻ��ָ������������������a
void LCD_Show_CH(u16 x, u16 y, u8 a, u8 mode)
{
	u16 i,j,_data;
	u16 color = POINT_COLOR;
	u16 x0 = x, y0=y;
	
	//�ȴ�����벿�ֺ���, 16��
	for(i=0; i<16; i++)
	{
		_data = HZ[a][i];
		for(j=0; j<8; j++)
		{
			if(_data&0x80)  //�ж����λ��û�����ص�
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
			//һ���㴦�������еĶ���
			_data = _data<<1;
			x++;
			if(x-x0==8)  //�ж�һ���Ƿ�����������Ļ���x��������һ�У�y������һ��
			{
				x = x0;
				y++;
			}
		}
	}
	x0 = x0 + 8;
	x = x0;
	y=y0;  //�ص�������һ��

	//�ٴ����ְ벿�ֺ���, 16��	
	for(i=16; i<32; i++)  //����ռ16��, ����һ��һ�д����ұ�һ��
	{
		_data = HZ[a][i];
		for(j=0; j<8; j++)
		{
			if(_data&0x80)  //�ж����λ��û�����ص�
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
			//һ���㴦�������еĶ���
			_data = _data<<1;
			x++;
			if(x-x0==8)  //�ж�һ���Ƿ�����������Ļ���x��������һ�У�y������һ��
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
	pic_type = (HEADCOLOR *)p;  //ǿת��HEADCOLOR���ͽṹ���������Ȼ�ᱨ��
	
	LCD_ILI9341_CMD(0X2A);  //����x���귶Χ
	LCD_ILI9341_Parameter(x>>8);
	LCD_ILI9341_Parameter(x&0xff);
	LCD_ILI9341_Parameter((x+pic_type->w-1)>>8);
	LCD_ILI9341_Parameter((x+pic_type->w-1)&0xff);
	
	LCD_ILI9341_CMD(0X2B);  //����y���귶Χ
	LCD_ILI9341_Parameter(y>>8);
	LCD_ILI9341_Parameter(y&0xff);
	LCD_ILI9341_Parameter((y+pic_type->h-1)>>8);
	LCD_ILI9341_Parameter((y+pic_type->h-1)&0xff);
	
	LCD_ILI9341_CMD(0X2C);  //дGRAMָ�֧������д
	for(i=0; i<pic_type->w*pic_type->h; i++)
	{
		lcd_send_show_data(*(p+8+2*i) | (*(p+8+2*i+1)<<8));  //ͼƬ�����������������16λ��RGB����ֵ�����Һ�һ���Ǹ�λ����������8λƴ����
		//HAL_Delay(1);
		delay_us(10);
	}
	
	//д��֮���x,y�޶�������Ļس�ȫ��Ļ
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
