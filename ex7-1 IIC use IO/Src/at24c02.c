#include "stm32f4xx.h"
#include "iic.h"
#include "at24c02.h"
#include "delay.h"

//写数据_data到at24c02内部addr地址
//返回值:错误类型
u8 at24c02_write_byte(u16 addr, u8 _data)
{
	iic_start();  //发送起始条件
	iic_write_byte(AT24C02_ADDR & 0xFE);  //发送器件地址,写模式
	if(iic_get_ack())  //如果没有读到响应, 没有从设备响应
	{
		iic_stop();
		return ACK_NO_SLAVE;
	}
	iic_write_byte(addr);  //发送器件内部地址
	if(iic_get_ack())  //如果没有读到响应, 内部地址无效
	{
		iic_stop();
		return ACK_ADDR_ERROR;
	}
	iic_write_byte(_data);  //写数据到器件
	if(iic_get_ack())  //如果没有读到响应, 写数据错误
	{
		iic_stop();
		return ACK_WRITE_ERROR;
	}
	iic_stop();
	delay_ms(5);
	return ACK_OK;
}

//从AT24C02内部地址addr读取一个字节数据
//存储到指针变量p所指向的地址空间
//返回值:错误类型
u8 at24c02_read_byte(u16 addr, u8 *p)
{
	iic_start();  //发送起始条件
	iic_write_byte(AT24C02_ADDR & 0xFE);  //发送器件地址,写模式
	if(iic_get_ack())  //如果没有读到响应, 没有从设备响应
	{
		iic_stop();
		return ACK_NO_SLAVE;
	}
	iic_write_byte(addr);  //发送器件内部地址
	if(iic_get_ack())  //如果没有读到响应, 内部地址无效
	{
		iic_stop();
		return ACK_ADDR_ERROR;
	}
	
	iic_start();  //发送重复起始条件
	iic_write_byte(AT24C02_ADDR | 0x01);  //发送器件地址,读模式
	if(iic_get_ack())  //如果没有读到响应, 没有从设备响应
	{
		iic_stop();
		return ACK_NO_SLAVE2;
	}
	*p = iic_read_byte();  //读数据
	iic_put_ack(1);  //不给响应
	iic_stop();
	return ACK_OK;
}

//从AT24C02内部地址addr连续读取num个字节数据
//存储到指针变量p所指向的地址空间
//返回值:错误类型
u8 at24c02_read_bytes(u16 addr, u8 num, u8 *p)
{
	iic_start();  //发送起始条件
	iic_write_byte(AT24C02_ADDR & 0xFE);  //发送器件地址,写模式
	if(iic_get_ack())  //如果没有读到响应, 没有从设备响应
	{
		iic_stop();
		return ACK_NO_SLAVE;
	}
	iic_write_byte(addr);  //发送器件内部地址
	if(iic_get_ack())  //如果没有读到响应, 内部地址无效
	{
		iic_stop();
		return ACK_ADDR_ERROR;
	}
	
	iic_start();  //发送重复起始条件
	iic_write_byte(AT24C02_ADDR | 0x01);  //发送器件地址,读模式
	if(iic_get_ack())  //如果没有读到响应, 没有从设备响应
	{
		iic_stop();
		return ACK_NO_SLAVE2;
	}
	for(; num>0; num--)
	{
		*p++ = iic_read_byte();  //读数据
		if(num==1)
		{
			iic_put_ack(1);
			break;
		}
		iic_put_ack(0);  //给响应
	}
	iic_put_ack(1);  //不给响应
	iic_stop();
	return ACK_OK;
}

//写数据_data到at24c02内部addr地址
//往AT24C02内部地址innter连续写num个字节数
//存储到指针变量p所指向的地址空间
//返回值:错误类型
u8 at24c02_write_bytes(u16 addr, u8 num, u8 *p)
{
	u8 i;
	u16 less_addr;
	while(1)
		{
		less_addr = 8-addr%8;  //起始地址所在的页还剩多少地址可写
		
		if(less_addr>=num)  //如果还能写的地址大于想写的地址数,就把num赋给剩余能写的地址
		{
			less_addr = num;
		}
		
		iic_start();  //发送起始条件
		iic_write_byte(AT24C02_ADDR & 0xFE);  //发送器件地址,写模式
		if(iic_get_ack())  //如果没有读到响应, 没有从设备响应
		{
			iic_stop();
			return ACK_NO_SLAVE;
		}
		iic_write_byte(addr);  //发送器件内部地址
		if(iic_get_ack())  //如果没有读到响应, 内部地址无效
		{
			iic_stop();
			return ACK_ADDR_ERROR;
		}
		iic_write_byte(*p++);  //写数据到器件
		if(iic_get_ack())  //如果没有读到响应, 写数据错误
		{
			iic_stop();
			return ACK_WRITE_ERROR;
		}
		iic_stop();
		delay_ms(5);
		if(less_addr==num)
		{
			break;
		}
		//
		addr += less_addr;
		num = num-less_addr; 
		return ACK_OK;
	}
}
