/*
 * @Author: jack----13467346738@163.com
 * @Date: 2022-03-26 18:07:27
 * @LastEditors: jack
 * @LastEditTime: 2022-04-05 16:11:33
 * @FilePath: \USERd:\毕设\code\成功\终极版\HARDWARE\LED\led.c
 */
#include "led.h"



//初始化PB5和PE5为输出口.并使能这两个口的时钟
// LED IO初始化
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
}

/*-------------------------------------------------*/
/*函数名：LED开启                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_On(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5); // PD2 输出低
}

/*-------------------------------------------------*/
/*函数名：LED关闭                                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5); // PD2 输出高
}
