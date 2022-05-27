
/*-------------------------------------------------*/
/*                                                 */
/*          	       ����2                  	   */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h" //������Ҫ��ͷ�ļ�
#include "usart2.h"	   //������Ҫ��ͷ�ļ�
#include "delay.h"

char Usart2_RxCompleted = 0;			//����һ������ 0����ʾ����δ��� 1����ʾ�������
unsigned int Usart2_RxCounter = 0;		//����һ����������¼����2�ܹ������˶����ֽڵ�����
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //����һ�����飬���ڱ��洮��2���յ�������

/*-------------------------------------------------*/
/*����������ʼ������3���͹���                        *///���Ǵ���3������������
/*��  ����bound��������                             */
/*����ֵ����                                        */
/*-------------------------------------------------*/
void Usart2_Init(unsigned int bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;   //����һ������GPIO���ܵı���
	USART_InitTypeDef USART_InitStructure; //����һ�����ô��ڹ��ܵı���
	NVIC_InitTypeDef NVIC_InitStructure;   //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //����3ʱ��ʹ��
	USART_DeInit(USART3);								   //����2�Ĵ�����������ΪĬ��ֵ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			   //׼������PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   // IO����50M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		   //����������������ڴ���2�ķ���
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   //��ʼ��PB10

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);				  //��ʼ��PB11

	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						// 8������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							// 1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;					//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������	3

	USART_Cmd(USART3, ENABLE); //ʹ�ܴ���

	USART_ClearFlag(USART3, USART_FLAG_RXNE);				  //������ձ�־λ
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			  //���������ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		  //���ô���2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //���ô���2�ж�
}

/*-------------------------------------------------*/
/*������������2 printf����                          */
/*��  ����char* fmt,...  ��ʽ������ַ����Ͳ���      */
/*����ֵ����                                        */
/*-------------------------------------------------*/

__align(8) char USART2_TxBuff[USART2_TXBUFF_SIZE];

void u2_printf(char *fmt, ...)
{
	unsigned int i, length;

	va_list ap;
	va_start(ap, fmt);
	vsprintf(USART2_TxBuff, fmt, ap);
	va_end(ap);

	length = strlen((const char *)USART2_TxBuff);
	while ((USART3->SR & 0X40) == 0)
		;
	for (i = 0; i < length; i++)
	{
		USART3->DR = USART2_TxBuff[i];
		while ((USART3->SR & 0X40) == 0)
			;
	}
}

/*-------------------------------------------------*/
/*������������2���ͻ������е�����                    */
/*��  ����data������                                */
/*����ֵ����                                        */
/*-------------------------------------------------*/
void u2_TxData(unsigned char *data)
{
	int i;
	while ((USART3->SR & 0X40) == 0)
		;
	for (i = 1; i <= (data[0] * 256 + data[1]); i++)
	{
		USART3->DR = data[i + 1];
		while ((USART3->SR & 0X40) == 0)
			;
	}
	DelayMs(500);
}
