#include "humanBodyInfrar.h"
#include "delay.h"
#include "lcd.h"
//out连PB1

void humanBodyInfrar_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PB.1 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //②使能 AFIO 时钟
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);//③
//	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStructure); //④初始化 EXTI 寄存器
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //使能按键外部中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级 2，
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //子优先级 2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
//	NVIC_Init(&NVIC_InitStructure);//⑤初始化 NVIC
}

//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//消抖
//	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)	 	 //WK_UP按键
//	{				 
//		LCD_ShowString(30,40,500,24,24,"ON");
//	}
//	else
//		LCD_ShowString(30,40,500,24,24,"OFF");
//	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
//}
 


