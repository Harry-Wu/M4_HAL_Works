#include "stm32f4xx.h"
#include "iic.h"
//#include "delay.h"

/**************************************
//GPIO口统一都在gpio.c中初始化过了
void iic_init(void)
{
	//端口时钟使能pb
	RCC->AHB1ENR |= 0x01<<1;
	//端口模式设置
	//SCL(PB8), SDA(PB9)----普通功能浮空开漏输出
	GPIOB->MODER &= ~(0X03<<16 | 0X03<<18);
	GPIOB->MODER |= 0X01<<16 | 0X01<<18;  //普通功能输出
	GPIOB->OTYPER |= 0X01<<8 | 0X01<<9;  //开漏输出
	GPIOB->OSPEEDR &= ~(0X03<<16 | 0X03<<18);  
	GPIOB->OSPEEDR |= 0X01<<16 | 0X01<<18;   //最大速度25MHz
	GPIOB->PUPDR &= ~(0X02<<16 | 0X03<<18);  //浮空,无上下拉
	
	IIC_SDA_OUT = 1;
	IIC_SCL = 1;
}
*******************************************************/

//延时大概600ns
static void iic_delay(void)
{
	u32 i=50;
	while(i--);
}



/* --------------------------------------------------------------------------------
/ 以下函数除了停止条件函数外,在结束前都把SCL线拉低了, 这样做是为了方便SDA上面的数据变更,
/ 并且防止SCL为高时,SDA上的干扰导致起始条件或停止条件满足
/ ---------------------------------------------------------------------------------*/
//起始条件
void iic_start(void)
{
	IIC_SCL = 0;
	IIC_SDA_OUT = 1;
	IIC_SCL = 1;  //这句确保IIC不受前面状态的影响
	iic_delay();//根据24c02的tSU.STA, >0.6uS
	IIC_SDA_OUT = 0;
	iic_delay();//根据24c02的tHD.STO, >0.6uS
	IIC_SCL = 0;		
}

//停止条件
void iic_stop(void)
{
	IIC_SCL = 0;
	IIC_SDA_OUT = 0;
	IIC_SCL = 1;
	iic_delay();  //根据24c02的tSU.STO, >100ns
	IIC_SDA_OUT = 1;
	iic_delay();  
	iic_delay(); //根据24c02的tBUF, >1.2uS
}

//发送一个字节数据
void iic_write_byte(u8 _data)
{
	u8 i;
	for(i=0; i<8; i++)
	{
		IIC_SCL = 0;
		//根据24c02的tHD.DAT=0S,所以不需要延时
		if(_data & 0x80)
		{
			IIC_SDA_OUT = 1;
		}
		else
		{
			IIC_SDA_OUT = 0;
		}
		//根据24c02的tSU.DAT, >100nS. 一条移位命令也会有延时
		//根据24c02的tLOW(SCL低电平需要持续的时间), >1.2uS
		iic_delay();
		iic_delay();
		_data = _data<<1;
		IIC_SCL = 1; //拉高之后接收器自动接收数据
		//根据24c02的tHIGH(SCL高电平要持续的时间), >0.6uS
		iic_delay();
		
	}
	IIC_SCL = 0;
}

//接收一个字节数据
u8 iic_read_byte(void)
{
	u8 i, rec_data;
	//切换为读模式
	IIC_SCL = 0;
	IIC_SDA_OUT = 1; //IO口为高后,因为是开漏输出,相当于浮空了
	for(i=0; i<8; i++)
	{
		IIC_SCL = 0;
		iic_delay();iic_delay();//24c02的tLOW>1.2uS
		IIC_SCL = 1;
		rec_data = rec_data<<1;
		if(IIC_SDA_IN)
		{
			rec_data = rec_data | 0x01;
		}
		iic_delay(); //24c02的tHIGH > 0.6uS
	}
	IIC_SCL = 0;
	return rec_data;
	
}

//发送响应
//ack: ack=0(给应答)
void iic_put_ack(u8 ack)
{
	IIC_SCL = 0;
	if(ack)
	{
		IIC_SDA_OUT = 1;  //不给响应
	}
	else
	{
		IIC_SDA_OUT = 0;  //给响应
	}
	iic_delay();iic_delay();//24c02的tLOW>1.2uS
	IIC_SCL = 1;
	iic_delay(); //24c02的tHIGH > 0.6uS
	IIC_SCL = 0;
}

//接收响应
u8 iic_get_ack(void)
{
	u8 ack = 0;
	//切换到读模式
	IIC_SCL = 0;
	IIC_SDA_OUT = 1;
	iic_delay();iic_delay();//24c02的tLOW>1.2uS
	IIC_SCL = 1;
	if(IIC_SDA_IN) //没有读到响应
	{
		ack = 1;
	}
	//else如果读到响应了,ack保持为默认值0
	iic_delay(); //24c02的tHIGH > 0.6uS
	IIC_SCL = 0;
	return ack;
	
}
