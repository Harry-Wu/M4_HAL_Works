#ifndef _AT24C02_H_
#define _AT24C02_H_
#include "stm32f4xx.h"
#include "MyTypeDef.h"

#define AT24C02_ADDR 0xA0
#define ACK_OK 0;  //正常
#define ACK_NO_SLAVE 1; //没有从设备响应
#define ACK_ADDR_ERROR 2; //器件内部地址无效
#define ACK_WRITE_ERROR 3;  //写数据错误
#define ACK_NO_SLAVE2 4; //重复发起始后没有从设备响应

#define TOUCH_ADJ_OR 0XA0  //校准与否标识符保存地址, 1byte
#define TOUCH_ADJ_ADDR 0XA1  //校准数据保存地址,12bytes


void at24c02_init(void);
u8 at24c02_write_byte(u16 addr,u8 _data);
u8 at24c02_read_byte(u16 addr,u8 *p);
u8 at24c02_read_bytes(u16 addr,u8 num,u8 *p);
u8 at24c02_write_bytes(u16 addr,u8 num,u8 *p);
#endif
