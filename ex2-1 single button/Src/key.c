#include "stm32f4xx.h"
#include "key.h"
#include "MyTypeDef.h"

void key_init(void)
{
	//�˿�ʱ��ʹ��
	RCC->AHB1ENR |= 0x01<<0;
	//�˿�����, PA0����Ϊ��ͨ����,��������
	GPIOA->MODER &=~(0x03<<0);//��ͨ����
	GPIOA->PUPDR &=~(0x03<<0);//��������
}

u8 key_scanf(void)
{
	//uint32_t i;
	u8 key = NO_KEY;
	if(KEY1)  //���ʶ��Ϊ�ߵ�ƽ
	{
		HAL_Delay(10);  //��ʱ����
		if(KEY1)  //�ٴ��ж��Ƿ�Ϊ�ߵ�ƽ
		{
			key = KEY1_OK;
			while (KEY1); //�жϰ����Ƿ��ͷ�
		}
	}
	return key;
}
