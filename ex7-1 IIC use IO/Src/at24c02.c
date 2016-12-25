#include "stm32f4xx.h"
#include "iic.h"
#include "at24c02.h"
#include "delay.h"

//д����_data��at24c02�ڲ�addr��ַ
//����ֵ:��������
u8 at24c02_write_byte(u16 addr, u8 _data)
{
	iic_start();  //������ʼ����
	iic_write_byte(AT24C02_ADDR & 0xFE);  //����������ַ,дģʽ
	if(iic_get_ack())  //���û�ж�����Ӧ, û�д��豸��Ӧ
	{
		iic_stop();
		return ACK_NO_SLAVE;
	}
	iic_write_byte(addr);  //���������ڲ���ַ
	if(iic_get_ack())  //���û�ж�����Ӧ, �ڲ���ַ��Ч
	{
		iic_stop();
		return ACK_ADDR_ERROR;
	}
	iic_write_byte(_data);  //д���ݵ�����
	if(iic_get_ack())  //���û�ж�����Ӧ, д���ݴ���
	{
		iic_stop();
		return ACK_WRITE_ERROR;
	}
	iic_stop();
	delay_ms(5);
	return ACK_OK;
}

//��AT24C02�ڲ���ַaddr��ȡһ���ֽ�����
//�洢��ָ�����p��ָ��ĵ�ַ�ռ�
//����ֵ:��������
u8 at24c02_read_byte(u16 addr, u8 *p)
{
	iic_start();  //������ʼ����
	iic_write_byte(AT24C02_ADDR & 0xFE);  //����������ַ,дģʽ
	if(iic_get_ack())  //���û�ж�����Ӧ, û�д��豸��Ӧ
	{
		iic_stop();
		return ACK_NO_SLAVE;
	}
	iic_write_byte(addr);  //���������ڲ���ַ
	if(iic_get_ack())  //���û�ж�����Ӧ, �ڲ���ַ��Ч
	{
		iic_stop();
		return ACK_ADDR_ERROR;
	}
	
	iic_start();  //�����ظ���ʼ����
	iic_write_byte(AT24C02_ADDR | 0x01);  //����������ַ,��ģʽ
	if(iic_get_ack())  //���û�ж�����Ӧ, û�д��豸��Ӧ
	{
		iic_stop();
		return ACK_NO_SLAVE2;
	}
	*p = iic_read_byte();  //������
	iic_put_ack(1);  //������Ӧ
	iic_stop();
	return ACK_OK;
}

//��AT24C02�ڲ���ַaddr������ȡnum���ֽ�����
//�洢��ָ�����p��ָ��ĵ�ַ�ռ�
//����ֵ:��������
u8 at24c02_read_bytes(u16 addr, u8 num, u8 *p)
{
	iic_start();  //������ʼ����
	iic_write_byte(AT24C02_ADDR & 0xFE);  //����������ַ,дģʽ
	if(iic_get_ack())  //���û�ж�����Ӧ, û�д��豸��Ӧ
	{
		iic_stop();
		return ACK_NO_SLAVE;
	}
	iic_write_byte(addr);  //���������ڲ���ַ
	if(iic_get_ack())  //���û�ж�����Ӧ, �ڲ���ַ��Ч
	{
		iic_stop();
		return ACK_ADDR_ERROR;
	}
	
	iic_start();  //�����ظ���ʼ����
	iic_write_byte(AT24C02_ADDR | 0x01);  //����������ַ,��ģʽ
	if(iic_get_ack())  //���û�ж�����Ӧ, û�д��豸��Ӧ
	{
		iic_stop();
		return ACK_NO_SLAVE2;
	}
	for(; num>0; num--)
	{
		*p++ = iic_read_byte();  //������
		if(num==1)
		{
			iic_put_ack(1);
			break;
		}
		iic_put_ack(0);  //����Ӧ
	}
	iic_put_ack(1);  //������Ӧ
	iic_stop();
	return ACK_OK;
}

//д����_data��at24c02�ڲ�addr��ַ
//��AT24C02�ڲ���ַinnter����дnum���ֽ���
//�洢��ָ�����p��ָ��ĵ�ַ�ռ�
//����ֵ:��������
u8 at24c02_write_bytes(u16 addr, u8 num, u8 *p)
{
	u8 i;
	u16 less_addr;
	while(1)
		{
		less_addr = 8-addr%8;  //��ʼ��ַ���ڵ�ҳ��ʣ���ٵ�ַ��д
		
		if(less_addr>=num)  //�������д�ĵ�ַ������д�ĵ�ַ��,�Ͱ�num����ʣ����д�ĵ�ַ
		{
			less_addr = num;
		}
		
		iic_start();  //������ʼ����
		iic_write_byte(AT24C02_ADDR & 0xFE);  //����������ַ,дģʽ
		if(iic_get_ack())  //���û�ж�����Ӧ, û�д��豸��Ӧ
		{
			iic_stop();
			return ACK_NO_SLAVE;
		}
		iic_write_byte(addr);  //���������ڲ���ַ
		if(iic_get_ack())  //���û�ж�����Ӧ, �ڲ���ַ��Ч
		{
			iic_stop();
			return ACK_ADDR_ERROR;
		}
		iic_write_byte(*p++);  //д���ݵ�����
		if(iic_get_ack())  //���û�ж�����Ӧ, д���ݴ���
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
