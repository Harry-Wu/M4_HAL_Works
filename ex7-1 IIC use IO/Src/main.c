/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "iwdg.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
/*****************************************************************************
@File name:  
@Description:
	- 加入模拟方式IIC通讯功能,操作AT24C02 EEPROM, 将校准数据保存到AT24C02
	- 加上触摸屏功能, 按下触摸屏,通过串口打印xy的坐标值
	- 加入RTC实时时钟功能,将时间显示到LCD屏上
	- 控制ILI9341驱动的2.8寸显示屏，在屏幕上显示ascii字符和汉字(数组方式存储). 	
	- spi读取W25Q64的id信息, 通过串口打印出来
	- uart1用中断方式接收数据,再发回给pc
@Hardware:
	TOUCH PANEL:
		T_SCK(PB0)----普通功能推挽输出  
		T_PEN(PB1)----普通输入,无上下拉   
		T_CS(PC13)----普通功能推挽输出
		T_MISO(PB2)----普通输入,无上下拉 
		T_MOSI(PF11)----普通功能推挽输出
	AT24C04 IIC:
		SCL(pb8) SDA(pb9)-----普通功能开漏输出
		
@Author: Harry Wu
@Version: V1.0
@Date: 2016-12-25
@History: 
    V1.0: 参考ex9-2 - touch_adj\工程					
*****************************************************************************/
#include "MyTypeDef.h"
#include "key.h"
#include "led.h"
#include "usart.h"
#include "flash.h"
#include "stdio.h"
#include "string.h"
#include "lcd.h"
#include "delay.h"
#include "touch.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	u8 key;
	u16 times=0;
	u8 *p="hello world!!! Harry\r\n";
	u8 buf[100];
	
	u8 t;
	u8 tbuf[40];
	_TOUCH_COOR_TYPEDEF touch_add;
//	u8 len;  //获取接收到的字节数
//	#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//	u8 rec_data_buf[USART_REC_LEN];
	//u8 buff[10];
	//硬件初始化
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  //MX_IWDG_Init();

  /* USER CODE BEGIN 2 */
	delay_init(21);  //确定systime timer的时钟,单位MHz
	
	__HAL_SPI_ENABLE(&hspi1);
	
  HAL_UART_Receive_IT(&huart1, (u8 *)aRxBuffer, RXBUFFERSIZE);
  
  SPI_Flash_Init();
	lcd_init();
	
	touch_init();
	
  SPI_Flash_Write((u8 *)p,0x100,strlen(p)+1);
  SPI_Flash_Read(buf,0x100,strlen(p)+1);
	
	MX_IWDG_Init();
	
	Draw_Circle(120,160,100);
	LCD_DrawLine(0,0,239,319);
	LCD_DrawRectangle(10,10,60,60);
	LCD_DrawSolidRectangle(10, 10, 230, 60, RED);
	Draw_SolidCircle(120,160,50);
	
	show_ascii(100, 10, 's', 1);
	HAL_Delay(100);
	show_ascii(108, 10, 'o', 1);
	HAL_Delay(100);
	show_ascii(116, 10, 'l', 1);
	HAL_Delay(100);
	show_ascii(124, 10, 'v', 1);
	HAL_Delay(100);
	show_ascii(132, 10, 'e', 1);
	HAL_IWDG_Refresh(&hiwdg);
	show_ascii(100, 30, 'L', 0);
	HAL_Delay(100);
	show_ascii(108, 30, 'O', 0);
	HAL_Delay(100);
	show_ascii(116, 30, 'V', 0);
	HAL_Delay(100);
	show_ascii(124, 30, 'E', 0);
	HAL_Delay(100);
	show_ascii(132, 30, '!', 0);
	HAL_IWDG_Refresh(&hiwdg);
	HAL_Delay(500);
	HAL_IWDG_Refresh(&hiwdg);
	BACK_COLOR = GREEN;
	LCD_ShowString(80,160,"hello, how are you?",0);
	
	LCD_Show_CH(80, 160+16, 0, 0);
	LCD_Show_CH(80+16, 160+16, 1, 0);
	LCD_Show_CH(80+32, 160+16, 2, 0);
	HAL_Delay(500);
	LCD_ShowPic(0, 0, (u8 *)gImage_Betty2014);	
	LCD_DrawSolidRectangle(50, 50,100,100,RED);
	
	printf("0x%x\r\n",SPI_FLASH_TYPE);
	printf("%s",buf);
	//HAL_Delay(500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	key = key_scanf();
	if(key == KEY1_OK) LED5 = !LED5;  
	else if(key == KEY2_OK) LED6 = !LED6;  
	else if(key == KEY3_OK) BUZZER1 = !BUZZER1;
	
	if(t!=RTC_TimeStruct.Seconds)
		{
			t=RTC_TimeStruct.Seconds;
			//RTC_Get_Time(&hour,&min,&sec,&ampm);
			//sprintf((char*)tbuf,"Time:%02d:%02d:%02d",time_date.hour,time_date.min,time_date.sec); 
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.Hours,RTC_TimeStruct.Minutes,RTC_TimeStruct.Seconds);
			LCD_ShowString(30,140,tbuf,0);			
			//RTC_Get_Date(&year,&month,&date,&week);
			//sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",time_date.year,time_date.month,time_date.date);
			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",RTC_DateStruct.Year,RTC_DateStruct.Month,RTC_DateStruct.Date); 
			LCD_ShowString(30,160,tbuf,0);	
			//sprintf((char*)tbuf,"Week:%d",time_date.week); 
			sprintf((char*)tbuf,"Week:%d",RTC_DateStruct.WeekDay); 
			LCD_ShowString(30,180,tbuf,0);
			
			sprintf((char*)tbuf,"AlarmA:%02d:%02d:%02d",RTC_AlarmStruct.AlarmTime.Hours,RTC_AlarmStruct.AlarmTime.Minutes,RTC_AlarmStruct.AlarmTime.Seconds); 
			LCD_ShowString(30,200,tbuf,0);
			sprintf((char*)tbuf,"Week/Day:%d",RTC_AlarmStruct.AlarmDateWeekDay); 
			LCD_ShowString(30,220,tbuf,0);
			
		} 
		HAL_IWDG_Refresh(&hiwdg);
	
    if(USART_RX_STA&(0x01<<15))  //如果接收完成
	{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\n您发送的消息为:\r\n");
//			HAL_UART_Transmit(&huart1,rec_data_buf,len,1000);	//发送接收到的数据
//			while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=SET);		//等待发送结束
//			printf("\r\n\r\n");//插入换行
//			USART_RX_STA=0;
	}
	//如果接收没完成,接收完成了时候需要发送数据会上位机,所以下面的任务在没完成时处理比较好
	else  
	{
		times++;  //time++很慢，后面都基本很长时间才能看到
		if(times%100==0)
		{
			printf("\r\n等的牙都长了\r\n");
			//printf("正点原子@ALIENTEK\r\n\r\n\r\n");
		}
		if(times%4==0)printf("请输入数据,以回车键结束\r\n");  
		if(times%2==0)LED5=!LED5;//闪烁LED,提示系统正在运行.
		HAL_Delay(500);   
	} 
	//delay_ms(500); //1s以内需喂狗,如果加上这个延时whil(1)循环就会超过1s,从而导致喂狗不及时
	
	//触幕屏按键开关指示灯
	touch_scanf(&touch_add, 0);
	if(touch_add.x>50 &&touch_add.x<100 &&touch_add.y>50 &&touch_add.y<100 )
	{
		LED6=!LED6;
		if(LED6)  //如果灯是灭的
		{
			LCD_DrawSolidRectangle(50, 50,100,100,RED);
		}
		else
		{
			LCD_DrawSolidRectangle(50, 50,100,100,GREEN);
		}
	}
	
	//以下为绘图程序
	CNV_touch2lcd(&touch_add);
	if(touch_add.x!=0xffff)
	{
		LCD_DrawPoint(touch_add.x,touch_add.y);
		LCD_DrawPoint(touch_add.x+1,touch_add.y);
		LCD_DrawPoint(touch_add.x,touch_add.y+1);
		LCD_DrawPoint(touch_add.x+1,touch_add.y+1);
		printf("x = %4d y = %4d\r\n", touch_add.x, touch_add.y);
	}
	
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/8000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
