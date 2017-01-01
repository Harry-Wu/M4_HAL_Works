#include "stm32f4xx.h"
#include "touch.h"
#include "delay.h"
#include "lcd.h"
#include "at24c02.h"


//端口设置已在GPIO中设置过了, 这里只进行触摸校准
void touch_init(void)
{
	u8 touch_adj_or_not;  //触摸屏是否被校准标识符
	
	//T_SCK(PB0)----普通功能推挽输出  
	//T_PEN(PB1)----普通输入,无上下拉   
	//T_CS(PC13)----普通功能推挽输出
	//T_MISO(PB2)----普通输入,无上下拉 
	//T_MOSI(PF11)----普通功能推挽输出
	//端口时钟使能
//	RCC->AHB1ENR |= 0X01<<1 | 0X01<<2| 0X01<<5;
//	
//	GPIOB->MODER &= ~(0X03<<0 | 0X03<<2 | 0X03<<4);
//	GPIOB->MODER |= 0X01<<0;
//	GPIOB->OTYPER &= ~(0X01<<0);
//	GPIOB->OSPEEDR &= ~(0X03<<0);  //SCK 2M
//	GPIOB->PUPDR  &=~(0X03<<0 | 0X03<<2 |0x03<<4);  //无上下拉
//	T_SCK = 0;
//	
//	GPIOC->MODER &= ~(0X03<<26);
//	GPIOC->MODER |= 0X01<<26;
//	GPIOC->OTYPER &= ~(0X01<<13);  //推挽
//	GPIOC->OSPEEDR &= ~(0X03<<26); //CS 2M
//	GPIOC->PUPDR  &=~(0X03<<26);  //无上下拉
//	T_CS = 1;
//	
//	GPIOF->MODER &= ~(0X03<<22);
//	GPIOF->MODER |= 0X01<<22;
//	GPIOF->OTYPER &= ~(0X01<<11);  //推挽
//	GPIOF->OSPEEDR &= ~(0X03<<22);  //MOSI 2M
//	GPIOF->PUPDR  &=~(0X03<<22);  //无上下拉
//	T_MOSI = 0;
	
	//读取触摸屏是否已经校准过参数
	at24c02_read_byte(TOUCH_ADJ_OR, &touch_adj_or_not);
	
	if(touch_adj_or_not!=0x51)//如果没有校准过,进入校准程序
	{
		//touch_adj();
		if(touch_adj()) LCD_ShowString(20,160,"TOUCH ADJUST PASS!",0);
		delay_ms(500);
		delay_ms(500);
		touch_adj_or_not=0x51;  //标识符置为已校准
		at24c02_write_byte(TOUCH_ADJ_OR, touch_adj_or_not);
		at24c02_write_bytes(TOUCH_ADJ_ADDR, sizeof(_TOUCH_ADJ_TYPEDEF), (u8 *)&touch_adj_parm);
	}
	at24c02_read_bytes(TOUCH_ADJ_ADDR, sizeof(_TOUCH_ADJ_TYPEDEF), (u8 *)&touch_adj_parm);
}


//获取X/Y方向触摸屏坐标转换的结果
//参数：commom=CMD_RDX 获取X方向转换的结果
//		commom=CMD_RDY 获取Y方向转换的结果
//返回值：AD转换结果
u16 get_touch_ad(u8 common)
{
	u8 i;
	u16 _data;
	T_MOSI = 0;
	T_SCK = 0;
	T_CS = 0;
	for(i=0; i<8; i++)  //发送命令字
	{
		T_SCK = 0;
		if(common&0x80) T_MOSI = 1;  //高位在前
		else T_MOSI = 0;
		delay_us(1);
		T_SCK = 1; //拉高后,XPT2046开始采集数据
		delay_us(1);
		common = common<<1;
	}
	T_SCK = 0;
	
	delay_us(150);
	//等待转换完清busy, 通过拉高T_SCK来清
	T_SCK = 1;
	delay_us(1);
	
	//从MISO获取返回的12位坐标值,赋给_data
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
	u16 buf[10],a;  //a用来临时存放较大值和总和
	
	//求x的平均值
	for(i=0; i<10; i++)
	{
		buf[i] = get_touch_ad(CMD_RDX);
	}
	//先从小到大排序, 再取1~8的数组求平均值
	for(i=0; i<9; i++)  
		for(j=i+1; j<10; j++)  //for循环嵌套, 加不加大括号都一样
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
	
	//求y的平均值
	for(i=0; i<10; i++)
	{
		buf[i] = get_touch_ad(CMD_RDY);
	}
	//先从小到大排序, 再取1~8的数组求平均值
	for(i=0; i<9; i++)  
		for(j=i+1; j<10; j++)  //for循环嵌套, 加不加大括号都一样
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

//画十字
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
	LCD_DrawCross(20, 20, 10);  //在LCD上画第一个点
	while(T_PEN);     					 //等待触摸屏被按下  
	get_touch_ad_filled(&touch[0]);	//获取触摸坐标
	while(T_PEN==0);			//等待触摸屏被释放
	
	POINT_COLOR=BACK_COLOR;
	LCD_DrawCross(20,20, 10);  //清除LCD上第一个点
	delay_ms(100);
	
	POINT_COLOR=BLACK;
	LCD_DrawCross(239-20,20, 10);   					 //在LCD上画第2个点
	while(T_PEN);     					 //等待触摸屏被按下  
	get_touch_ad_filled(&touch[1]);	//获取触摸坐标
	while(T_PEN==0);			//等待触摸屏被释放	
	
	POINT_COLOR=BACK_COLOR;
	LCD_DrawCross(239-20,20, 10);   //清除LCD上第2个点
	delay_ms(100);
	
	POINT_COLOR=BLACK;
	LCD_DrawCross(20,319-20, 10);   					 //在LCD上画第3个点
	while(T_PEN);     					 //等待触摸屏被按下  
	get_touch_ad_filled(&touch[2]);	//获取触摸坐标
	while(T_PEN==0);			//等待触摸屏被释放
	
	POINT_COLOR=BACK_COLOR;
	LCD_DrawCross(20,319-20, 10);    //清除LCD上第3个点
	delay_ms(100);
		
	POINT_COLOR=BLACK;
	LCD_DrawCross(239-20,319-20, 10);   					 //在LCD上画第4个点
	while(T_PEN);     					 //等待触摸屏被按下  
	get_touch_ad_filled(&touch[3]);	//获取触摸坐标
	while(T_PEN==0);			//等待触摸屏被释放
	
	POINT_COLOR=BACK_COLOR;
	LCD_DrawCross(239-20,319-20, 10);    //清除LCD上第4个点
	POINT_COLOR=BLACK;
	
	//验证触点是否准确
	//第1 2两点间触摸屏距离的平方
	f1=(touch[1].x - touch[0].x)*(touch[1].x - touch[0].x)
		+(touch[1].y - touch[0].y)*(touch[1].y - touch[0].y);
	
	//第3 4两点间触摸屏距离的平方
	f2=(touch[3].x - touch[2].x)*(touch[3].x - touch[2].x)
		+(touch[3].y - touch[2].y)*(touch[3].y - touch[2].y);
		
	if(f1/f2>1.1f  || f1/f2<0.9f)
	{
		goto READJ;
	}
		
	//第1 3两点间触摸屏距离的平方
	f1=(touch[2].x - touch[0].x)*(touch[2].x - touch[0].x)
		+(touch[2].y - touch[0].y)*(touch[2].y - touch[0].y);
	
	//第2 4两点间触摸屏距离的平方
	f2=(touch[3].x - touch[1].x)*(touch[3].x - touch[1].x)
		+(touch[3].y - touch[1].y)*(touch[3].y - touch[1].y);
		
	if(f1/f2>1.1f  || f1/f2<0.9f)  //精度允许误差10%, 如果误差偏大,则返回重新校准
	{
		goto READJ;
	}
			
	//第1 4两点间触摸屏距离的平方
	f1=(touch[3].x - touch[0].x)*(touch[3].x - touch[0].x)
		+(touch[3].y - touch[0].y)*(touch[3].y - touch[0].y);
	
	//第2 2两点间触摸屏距离的平方
	f2=(touch[2].x - touch[1].x)*(touch[2].x - touch[1].x)
		+(touch[2].y - touch[1].y)*(touch[2].y - touch[1].y);
		
	if(f1/f2>1.1f  || f1/f2<0.9f)  //精度允许误差10%
	{
		goto READJ;
	}
			
	/********************************************************
	//找LCD跟触摸屏对应关系
	//触摸屏坐标跟LCD坐标关系：

	//公式：lcd_x=fx*touch_x+xoffset;
	//lcd_x：		某点对应lcd X坐标
	//touch_x：	某点对应触摸屏 X坐标
	//fx：			比例系数
	//xoffset：		偏移量

	//列方程求比例系数和偏移量
	//20 =fx*touchxy[0].x+xoffset
	//220=fx*touchxy[1].x+xoffset

	//fx=((240-20) -20)/(touchxy[1].x-touchxy[0].x)
	//xoffset=20-fx*touchxy[0].x
	************************************************************/
	//x方向比例系数
	touch_adj_parm.fx = (float)(239-20 - 20)/(touch[1].x - touch[0].x);
	//x方向偏移量
	touch_adj_parm.x_offset = 20 - touch_adj_parm.fx * touch[0].x;
	
	//y方向比例系数
	touch_adj_parm.fy=(float)(319-20 - 20)/(touch[2].y - touch[0].y);
	//y方向偏移量
	touch_adj_parm.y_offset=20 - touch_adj_parm.fy * touch[0].y;
	
	return TOUCH_ADJ_OK;
}

void CNV_touch2lcd(_TOUCH_COOR_TYPEDEF *p)
{
	if(T_PEN==0)   //触摸屏有被按下
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

//触摸按下识别
//mode=0, 非连续识别, mode=1, 连续识别

void touch_scanf(_TOUCH_COOR_TYPEDEF *p, u8 mode)
{
	static u8 touch_sta=1;  //touch_sta=1允许识别
	_TOUCH_COOR_TYPEDEF touch;
	touch.x = 0xffff;
	touch.y = 0xffff;
	
	if(mode)  //连续识别
	{
		touch_sta = 1;  //按下没有释放时, 重复调用touch_scanf函数, 可以再次识别
	}
	if((touch_sta==1)&&(T_PEN==0))  //允许按键识别, 并且触摸屏被按下
	{
		delay_ms(5);  //延时消抖
		if(T_PEN==0)
		{
			get_touch_ad_filled(p);  //得到触摸屏的x,y值
			touch.x=touch_adj_parm.fx * p->x +touch_adj_parm.x_offset;
			touch.y=touch_adj_parm.fy * p->y +touch_adj_parm.y_offset;
			touch_sta=0;    //自锁
		}
	}
	else if(T_PEN == 1)
	{
		touch_sta = 1;
	}
	*p = touch;
}

