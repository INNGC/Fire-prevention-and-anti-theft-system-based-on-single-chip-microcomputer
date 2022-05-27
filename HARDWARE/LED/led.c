/*
 * @Author: jack----13467346738@163.com
 * @Date: 2022-03-26 18:07:27
 * @LastEditors: jack
 * @LastEditTime: 2022-04-05 16:11:33
 * @FilePath: \USERd:\����\code\�ɹ�\�ռ���\HARDWARE\LED\led.c
 */
#include "led.h"



//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��
// LED IO��ʼ��
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 
}

/*-------------------------------------------------*/
/*��������LED����                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_On(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5); // PD2 �����
}

/*-------------------------------------------------*/
/*��������LED�ر�                                  */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5); // PD2 �����
}
