#include "stm32f4xx.h"
#include "iic.h"
//#include "delay.h"

/**************************************
//GPIO��ͳһ����gpio.c�г�ʼ������
void iic_init(void)
{
	//�˿�ʱ��ʹ��pb
	RCC->AHB1ENR |= 0x01<<1;
	//�˿�ģʽ����
	//SCL(PB8), SDA(PB9)----��ͨ���ܸ��տ�©���
	GPIOB->MODER &= ~(0X03<<16 | 0X03<<18);
	GPIOB->MODER |= 0X01<<16 | 0X01<<18;  //��ͨ�������
	GPIOB->OTYPER |= 0X01<<8 | 0X01<<9;  //��©���
	GPIOB->OSPEEDR &= ~(0X03<<16 | 0X03<<18);  
	GPIOB->OSPEEDR |= 0X01<<16 | 0X01<<18;   //����ٶ�25MHz
	GPIOB->PUPDR &= ~(0X02<<16 | 0X03<<18);  //����,��������
	
	IIC_SDA_OUT = 1;
	IIC_SCL = 1;
}
*******************************************************/

//��ʱ���600ns
static void iic_delay(void)
{
	u32 i=50;
	while(i--);
}



/* --------------------------------------------------------------------------------
/ ���º�������ֹͣ����������,�ڽ���ǰ����SCL��������, ��������Ϊ�˷���SDA��������ݱ��,
/ ���ҷ�ֹSCLΪ��ʱ,SDA�ϵĸ��ŵ�����ʼ������ֹͣ��������
/ ---------------------------------------------------------------------------------*/
//��ʼ����
void iic_start(void)
{
	IIC_SCL = 0;
	IIC_SDA_OUT = 1;
	IIC_SCL = 1;  //���ȷ��IIC����ǰ��״̬��Ӱ��
	iic_delay();//����24c02��tSU.STA, >0.6uS
	IIC_SDA_OUT = 0;
	iic_delay();//����24c02��tHD.STO, >0.6uS
	IIC_SCL = 0;		
}

//ֹͣ����
void iic_stop(void)
{
	IIC_SCL = 0;
	IIC_SDA_OUT = 0;
	IIC_SCL = 1;
	iic_delay();  //����24c02��tSU.STO, >100ns
	IIC_SDA_OUT = 1;
	iic_delay();  
	iic_delay(); //����24c02��tBUF, >1.2uS
}

//����һ���ֽ�����
void iic_write_byte(u8 _data)
{
	u8 i;
	for(i=0; i<8; i++)
	{
		IIC_SCL = 0;
		//����24c02��tHD.DAT=0S,���Բ���Ҫ��ʱ
		if(_data & 0x80)
		{
			IIC_SDA_OUT = 1;
		}
		else
		{
			IIC_SDA_OUT = 0;
		}
		//����24c02��tSU.DAT, >100nS. һ����λ����Ҳ������ʱ
		//����24c02��tLOW(SCL�͵�ƽ��Ҫ������ʱ��), >1.2uS
		iic_delay();
		iic_delay();
		_data = _data<<1;
		IIC_SCL = 1; //����֮��������Զ���������
		//����24c02��tHIGH(SCL�ߵ�ƽҪ������ʱ��), >0.6uS
		iic_delay();
		
	}
	IIC_SCL = 0;
}

//����һ���ֽ�����
u8 iic_read_byte(void)
{
	u8 i, rec_data;
	//�л�Ϊ��ģʽ
	IIC_SCL = 0;
	IIC_SDA_OUT = 1; //IO��Ϊ�ߺ�,��Ϊ�ǿ�©���,�൱�ڸ�����
	for(i=0; i<8; i++)
	{
		IIC_SCL = 0;
		iic_delay();iic_delay();//24c02��tLOW>1.2uS
		IIC_SCL = 1;
		rec_data = rec_data<<1;
		if(IIC_SDA_IN)
		{
			rec_data = rec_data | 0x01;
		}
		iic_delay(); //24c02��tHIGH > 0.6uS
	}
	IIC_SCL = 0;
	return rec_data;
	
}

//������Ӧ
//ack: ack=0(��Ӧ��)
void iic_put_ack(u8 ack)
{
	IIC_SCL = 0;
	if(ack)
	{
		IIC_SDA_OUT = 1;  //������Ӧ
	}
	else
	{
		IIC_SDA_OUT = 0;  //����Ӧ
	}
	iic_delay();iic_delay();//24c02��tLOW>1.2uS
	IIC_SCL = 1;
	iic_delay(); //24c02��tHIGH > 0.6uS
	IIC_SCL = 0;
}

//������Ӧ
u8 iic_get_ack(void)
{
	u8 ack = 0;
	//�л�����ģʽ
	IIC_SCL = 0;
	IIC_SDA_OUT = 1;
	iic_delay();iic_delay();//24c02��tLOW>1.2uS
	IIC_SCL = 1;
	if(IIC_SDA_IN) //û�ж�����Ӧ
	{
		ack = 1;
	}
	//else���������Ӧ��,ack����ΪĬ��ֵ0
	iic_delay(); //24c02��tHIGH > 0.6uS
	IIC_SCL = 0;
	return ack;
	
}
