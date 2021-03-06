/*
 * @Author: jack----13467346738@163.com
 * @Date: 2022-02-04 18:05:46
 * @LastEditors: jack
 * @LastEditTime: 2022-04-21 01:29:15
 * @FilePath: \USERd:\毕设\code\成功\终极版SIM\HARDWARE\scr\stm32f10x_it.c
 */
/*-------------------------------------------------*/
/*                                                 */
/*            	   中断服务函数          	   	   */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"	  //包含需要的头文件
#include "stm32f10x_it.h" //包含需要的头文件
#include "usart1.h"		  //包含需要的头文件
#include "usart2.h"		  //包含需要的头文件
#include "timer3.h"		  //包含需要的头文件
#include "mqtt.h"		  //包含需要的头文件
#include "dht11.h"		  //包含需要的头文件
#include "lcd.h"
#include "adc.h"
#include "delay.h"
#include "beep.h"
#include "led.h"
#include "key.h"
#include "control.h"
static int flag=1;//是否开启警报灯和蜂鸣器报警
/*-------------------------------------------------*/
/*函数名：串口3接收中断函数（最高优先级，处理接收数据）*/ ///开发板通过串口3接收到来自ESP8266的数据
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
	{
		if (connectFlag == 0) //如果connectFlag等于0，当前还没有连接服务器，处于指令配置状态
		{
			if (USART3->DR)
			{												  //处于指令配置状态时，非零值才保存到缓冲区
				Usart2_RxBuff[Usart2_RxCounter] = USART3->DR; //保存到缓冲区
				Usart2_RxCounter++;							  //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1
			}
		}
		else
		{												  //反之connectFlag等于1，连接上服务器了
			Usart2_RxBuff[Usart2_RxCounter] = USART3->DR; //把接收到的数据保存到Usart2_RxBuff中
			if (Usart2_RxCounter == 0)
			{ //如果Usart2_RxCounter等于0，表示是接收的第1个数据，进入if分支
				TIM_Cmd(TIM4, ENABLE);
			}
			else // else分支，表示果Usart2_RxCounter不等于0，不是接收的第一个数据
			{
				TIM_SetCounter(TIM4, 0);
			}
			Usart2_RxCounter++; //每接收1个字节的数据，Usart2_RxCounter加1，表示接收的数据总量+1
		}
	}
}
/*-------------------------------------------------*/
/*函数名：定时器4中断服务函数。处理MQTT数据          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //如果TIM_IT_Update置位，表示TIM4溢出中断，进入if
	{
		memcpy(&MQTT_RxDataInPtr[2], Usart2_RxBuff, Usart2_RxCounter); //拷贝数据到接收缓冲区
		MQTT_RxDataInPtr[0] = Usart2_RxCounter / 256;				   //记录数据长度高字节
		MQTT_RxDataInPtr[1] = Usart2_RxCounter % 256;				   //记录数据长度低字节
		MQTT_RxDataInPtr += RBUFF_UNIT;								   //指针下移
		if (MQTT_RxDataInPtr == MQTT_RxDataEndPtr)					   //如果指针到缓冲区尾部了
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];					   //指针归位到缓冲区开头
		Usart2_RxCounter = 0;										   //串口2接收数据量变量清零
		TIM_SetCounter(TIM3, 0);									   //清零定时器3计数器，重新计时ping包发送时间
		TIM_Cmd(TIM4, DISABLE);										   //关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);									   //清零定时器4计数器
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);					   //清除TIM4溢出中断标志
	}
}
/*-------------------------------------------------*/
/*函数名：定时器3中断服务函数  定时发送ping报文                    */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //如果TIM_IT_Update置位，表示TIM3溢出中断，进入if
	{
		switch (pingFlag) //判断pingFlag的状态
		{
		case 0:				//如果pingFlag等于0，表示正常状态，发送Ping报文
			MQTT_PingREQ(); //添加Ping报文到发送缓冲区
			break;
		case 1:				  //如果pingFlag等于1，说明上一次发送到的ping报文，没有收到服务器回复，所以1没有被清除为0，可能是连接异常，我们要启动快速ping模式
			TIM3_ENABLE_2S(); //我们将定时器6设置为2s定时,快速发送Ping报文
			MQTT_PingREQ();	  //添加Ping报文到发送缓冲区
			break;
		case 2:				//如果pingFlag等于2，说明还没有收到服务器回复
		case 3:				//如果pingFlag等于3，说明还没有收到服务器回复
		case 4:				//如果pingFlag等于4，说明还没有收到服务器回复
			MQTT_PingREQ(); //添加Ping报文到发送缓冲区
			break;
		case 5:						//如果pingFlag等于5，说明我们发送了多次ping，均无回复，应该是连接有问题，我们重启连接
			connectFlag = 0;		//连接状态置0，表示断开，没连上服务器
			TIM_Cmd(TIM3, DISABLE); //关TIM3
			break;
		}
		pingFlag++;									// pingFlag自增1，表示又发送了一次ping，期待服务器的回复
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除TIM3溢出中断标志
	}
}

/*-------------------------------------------------*/
/*函数名：定时器2中断服务函数   定时发送数据至服务器                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{
	/// DHT11
	u8 humidity;	//定义一个变量，保存湿度值
	u8 temperature; //定义一个变量，保存温度值
	char head1[3];
	char temp[50];	   //定义一个临时缓冲区1,不包括报头
	char tempAll[100]; //定义一个临时缓冲区2，包括所有数据
	int dataLen = 0;   //报文长度
	///

	/// MQ4
	u16 adcx;
	float a;		  //定义一个变量，保存电压值
	u16 Gas_Strength; //定义一个变量，保存气体浓度
	float temp_;
	char head2[3];
	char temp2[50];		//定义一个临时缓冲区1,不包括报头
	char tempAll2[100]; //定义一个临时缓冲区2，包括所有数据
	int dataLen2 = 0;	//报文长度
	///
	vu8 key=0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		/////////DHT11
		DHT11_Read_Data(&temperature, &humidity); //读取温湿度值
		memset(temp, 0, 50);					  //清空缓冲区1
		memset(tempAll, 0, 100);				  //清空缓冲区2
		memset(head1, 0, 3);					  //清空MQTT头
		LCD_ShowString(30, 180, 200, 16, 16, "DHT11 OK");
		POINT_COLOR = BLUE; //设置字体为蓝色
		LCD_ShowString(30, 200, 200, 16, 16, "Temp:  C");
		LCD_ShowString(30, 220, 200, 16, 16, "Humi:  %");
		LCD_ShowNum(30 + 40, 200, temperature, 2, 16); //显示温度
		LCD_ShowNum(30 + 40, 220, humidity, 2, 16);	   //显示湿度
		sprintf(temp, "{\"temperature\":\"%d%d\",\"humidity\":\"%d%d\"}",
				temperature / 10, temperature % 10, humidity / 10, humidity % 10); //构建报文
		head1[0] = 0x03;														   //固定报头
		head1[1] = 0x00;														   //固定报头
		head1[2] = strlen(temp);												   //剩余长度
		sprintf(tempAll, "%c%c%c%s", head1[0], head1[1], head1[2], temp);
		u1_printf("\r\n"); //串口显示相关数据
		u1_printf("%s\r\n", tempAll + 3);
		dataLen = strlen(temp) + 3;
		MQTT_PublishQs0(Data_TOPIC_NAME, tempAll, dataLen); //添加数据，发布给服务器
		///////////
		DelayMs(50);
		/////ADC
		adcx = Get_Adc_Average(ADC_Channel_1, 10);
		LCD_ShowxNum(156, 130, adcx, 4, 16, 0); //显示ADC的值
		temp_ = (float)adcx * (3.3 / 4096);
		adcx = temp_;
		LCD_ShowxNum(156, 150, adcx, 1, 16, 0); //显示电压值
		temp_ -= adcx;
		temp_ *= 1000;
		LCD_ShowxNum(172, 150, temp_, 3, 16, 0X80);
		a = (float)(int)adcx + temp_ / 1000; //计算电压值
		Gas_Strength = Adc_Average_To_Ppm(a); //将电压转换为气体浓度
		u1_printf("Gas_Strength:%d\r\n", Gas_Strength);//串口显示相关数据
		memset(temp2, 0, 50);									   //清空缓冲区1
		memset(tempAll2, 0, 100);								   //清空缓冲区2
		memset(head2, 0, 3);									   //清空MQTT头
		sprintf(temp2, "{\"Gas_Strength\":\"%d\"}", Gas_Strength); //构建报文
		head2[0] = 0x03;										   //固定报头
		head2[1] = 0x01;										   //固定报头
		head2[2] = strlen(temp2);								   //剩余长度
		sprintf(tempAll2,"%c%c%c%s", head2[0], head2[1], head2[2], temp2);//构建报文
		u1_printf("\r\n"); //串口显示相关数据
		u1_printf("%s\r\n", tempAll2 + 3);
		dataLen2=strlen(temp2) + 3;
		MQTT_PublishQs0(Data_TOPIC_NAME, tempAll2, dataLen2); //添加数据，发布给服务器
		///////

		//根据温度，甲烷浓度进行判断判断
		key=KEY_Scan(1);//按键扫描
		if(key==KEY1_PRES)
		{
			flag=!flag;
		}
		if((temperature>60||Gas_Strength>9000||humidity>100)&&flag)
		{
			LED1=1;
			BEEP=1;
			//Send_Text_Sms();//发送一条短消息
		}
		else
		{
			LED1=0;
			BEEP=0;
		}
		LED1=!LED1;//LED闪烁
		Send_Data();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除中断标志
	}
}

/*-------------------------------------------------*/
/*函数名：不可屏蔽中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：硬件出错后进入的中断处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：内存管理中断处理函数                     */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/

void MemManage_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：预取指失败，存储器访问失败中断处理函数   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：未定义的指令或非法状态处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：软中断，SWI 指令调用的处理函数           */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：调试监控器处理函数                       */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：可挂起的系统服务处理函数                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*函数名：SysTic系统嘀嗒定时器处理函数             */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
}
