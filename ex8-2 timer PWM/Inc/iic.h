#ifndef _IIC_H_
#define _IIC_H_
#include "stm32f4xx.h"
#include "MyTypeDef.h"

#define IIC_SCL PBout(8)
#define IIC_SDA_OUT PBout(9)
#define IIC_SDA_IN PBin(9)

void iic_init(void);
static void iic_delay(void);
void iic_start(void);
void iic_stop(void);
void iic_write_byte(u8 _data);
u8 iic_read_byte(void);
void iic_put_ack(u8 ack);
u8 iic_get_ack(void);	

#endif

