#include "humanBodyInfrar.h"
#include "delay.h"
#include "lcd.h"
//out��PB1

void humanBodyInfrar_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PB.1 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //��ʹ�� AFIO ʱ��
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);//��
//	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStructure); //�ܳ�ʼ�� EXTI �Ĵ���
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //ʹ�ܰ����ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ� 2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //�����ȼ� 2
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure);//�ݳ�ʼ�� NVIC
}

//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);//����
//	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)	 	 //WK_UP����
//	{				 
//		LCD_ShowString(30,40,500,24,24,"ON");
//	}
//	else
//		LCD_ShowString(30,40,500,24,24,"OFF");
//	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
//}
 


