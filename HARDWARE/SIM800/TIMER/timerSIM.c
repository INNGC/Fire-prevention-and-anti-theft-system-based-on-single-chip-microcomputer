#include "timerSIM.h"

extern vu8 Timer0_start;
extern u8 shijian;
extern u8 Times;
/*******************************************************************************
* ������  : Timer5_Init_Config
* ����    : Timer5��ʼ������
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : 1s��ʱ
*******************************************************************************/
void Timer5_Init_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);		//ʹ��Timer5ʱ��
	
	TIM_TimeBaseStructure.TIM_Period = 9999;					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ(������10000Ϊ1s)
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ(10KHz�ļ���Ƶ��)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����ʱ�ӷָ�:TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	/*�ж����ȼ�NVIC����*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;				//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ��IRQͨ��
	NVIC_Init(&NVIC_InitStructure); 							//��ʼ��NVIC�Ĵ���
	 
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE ); 				//ʹ��TIM5ָ�����ж�
	
	TIM_Cmd(TIM5, ENABLE);  									//ʹ��TIMx����
}	 


