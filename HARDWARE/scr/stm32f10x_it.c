/*
 * @Author: jack----13467346738@163.com
 * @Date: 2022-02-04 18:05:46
 * @LastEditors: jack
 * @LastEditTime: 2022-04-21 01:29:15
 * @FilePath: \USERd:\����\code\�ɹ�\�ռ���SIM\HARDWARE\scr\stm32f10x_it.c
 */
/*-------------------------------------------------*/
/*                                                 */
/*            	   �жϷ�����          	   	   */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"	  //������Ҫ��ͷ�ļ�
#include "stm32f10x_it.h" //������Ҫ��ͷ�ļ�
#include "usart1.h"		  //������Ҫ��ͷ�ļ�
#include "usart2.h"		  //������Ҫ��ͷ�ļ�
#include "timer3.h"		  //������Ҫ��ͷ�ļ�
#include "mqtt.h"		  //������Ҫ��ͷ�ļ�
#include "dht11.h"		  //������Ҫ��ͷ�ļ�
#include "lcd.h"
#include "adc.h"
#include "delay.h"
#include "beep.h"
#include "led.h"
#include "key.h"
#include "control.h"
static int flag=1;//�Ƿ��������ƺͷ���������
/*-------------------------------------------------*/
/*������������3�����жϺ�����������ȼ�������������ݣ�*/ ///������ͨ������3���յ�����ESP8266������
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
	{
		if (connectFlag == 0) //���connectFlag����0����ǰ��û�����ӷ�����������ָ������״̬
		{
			if (USART3->DR)
			{												  //����ָ������״̬ʱ������ֵ�ű��浽������
				Usart2_RxBuff[Usart2_RxCounter] = USART3->DR; //���浽������
				Usart2_RxCounter++;							  //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1
			}
		}
		else
		{												  //��֮connectFlag����1�������Ϸ�������
			Usart2_RxBuff[Usart2_RxCounter] = USART3->DR; //�ѽ��յ������ݱ��浽Usart2_RxBuff��
			if (Usart2_RxCounter == 0)
			{ //���Usart2_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧
				TIM_Cmd(TIM4, ENABLE);
			}
			else // else��֧����ʾ��Usart2_RxCounter������0�����ǽ��յĵ�һ������
			{
				TIM_SetCounter(TIM4, 0);
			}
			Usart2_RxCounter++; //ÿ����1���ֽڵ����ݣ�Usart2_RxCounter��1����ʾ���յ���������+1
		}
	}
}
/*-------------------------------------------------*/
/*����������ʱ��4�жϷ�����������MQTT����          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���TIM_IT_Update��λ����ʾTIM4����жϣ�����if
	{
		memcpy(&MQTT_RxDataInPtr[2], Usart2_RxBuff, Usart2_RxCounter); //�������ݵ����ջ�����
		MQTT_RxDataInPtr[0] = Usart2_RxCounter / 256;				   //��¼���ݳ��ȸ��ֽ�
		MQTT_RxDataInPtr[1] = Usart2_RxCounter % 256;				   //��¼���ݳ��ȵ��ֽ�
		MQTT_RxDataInPtr += RBUFF_UNIT;								   //ָ������
		if (MQTT_RxDataInPtr == MQTT_RxDataEndPtr)					   //���ָ�뵽������β����
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];					   //ָ���λ����������ͷ
		Usart2_RxCounter = 0;										   //����2������������������
		TIM_SetCounter(TIM3, 0);									   //���㶨ʱ��3�����������¼�ʱping������ʱ��
		TIM_Cmd(TIM4, DISABLE);										   //�ر�TIM4��ʱ��
		TIM_SetCounter(TIM4, 0);									   //���㶨ʱ��4������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);					   //���TIM4����жϱ�־
	}
}
/*-------------------------------------------------*/
/*����������ʱ��3�жϷ�����  ��ʱ����ping����                    */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���TIM_IT_Update��λ����ʾTIM3����жϣ�����if
	{
		switch (pingFlag) //�ж�pingFlag��״̬
		{
		case 0:				//���pingFlag����0����ʾ����״̬������Ping����
			MQTT_PingREQ(); //���Ping���ĵ����ͻ�����
			break;
		case 1:				  //���pingFlag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
			TIM3_ENABLE_2S(); //���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
			MQTT_PingREQ();	  //���Ping���ĵ����ͻ�����
			break;
		case 2:				//���pingFlag����2��˵����û���յ��������ظ�
		case 3:				//���pingFlag����3��˵����û���յ��������ظ�
		case 4:				//���pingFlag����4��˵����û���յ��������ظ�
			MQTT_PingREQ(); //���Ping���ĵ����ͻ�����
			break;
		case 5:						//���pingFlag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
			connectFlag = 0;		//����״̬��0����ʾ�Ͽ���û���Ϸ�����
			TIM_Cmd(TIM3, DISABLE); //��TIM3
			break;
		}
		pingFlag++;									// pingFlag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //���TIM3����жϱ�־
	}
}

/*-------------------------------------------------*/
/*����������ʱ��2�жϷ�����   ��ʱ����������������                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{
	/// DHT11
	u8 humidity;	//����һ������������ʪ��ֵ
	u8 temperature; //����һ�������������¶�ֵ
	char head1[3];
	char temp[50];	   //����һ����ʱ������1,��������ͷ
	char tempAll[100]; //����һ����ʱ������2��������������
	int dataLen = 0;   //���ĳ���
	///

	/// MQ4
	u16 adcx;
	float a;		  //����һ�������������ѹֵ
	u16 Gas_Strength; //����һ����������������Ũ��
	float temp_;
	char head2[3];
	char temp2[50];		//����һ����ʱ������1,��������ͷ
	char tempAll2[100]; //����һ����ʱ������2��������������
	int dataLen2 = 0;	//���ĳ���
	///
	vu8 key=0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		/////////DHT11
		DHT11_Read_Data(&temperature, &humidity); //��ȡ��ʪ��ֵ
		memset(temp, 0, 50);					  //��ջ�����1
		memset(tempAll, 0, 100);				  //��ջ�����2
		memset(head1, 0, 3);					  //���MQTTͷ
		LCD_ShowString(30, 180, 200, 16, 16, "DHT11 OK");
		POINT_COLOR = BLUE; //��������Ϊ��ɫ
		LCD_ShowString(30, 200, 200, 16, 16, "Temp:  C");
		LCD_ShowString(30, 220, 200, 16, 16, "Humi:  %");
		LCD_ShowNum(30 + 40, 200, temperature, 2, 16); //��ʾ�¶�
		LCD_ShowNum(30 + 40, 220, humidity, 2, 16);	   //��ʾʪ��
		sprintf(temp, "{\"temperature\":\"%d%d\",\"humidity\":\"%d%d\"}",
				temperature / 10, temperature % 10, humidity / 10, humidity % 10); //��������
		head1[0] = 0x03;														   //�̶���ͷ
		head1[1] = 0x00;														   //�̶���ͷ
		head1[2] = strlen(temp);												   //ʣ�೤��
		sprintf(tempAll, "%c%c%c%s", head1[0], head1[1], head1[2], temp);
		u1_printf("\r\n"); //������ʾ�������
		u1_printf("%s\r\n", tempAll + 3);
		dataLen = strlen(temp) + 3;
		MQTT_PublishQs0(Data_TOPIC_NAME, tempAll, dataLen); //������ݣ�������������
		///////////
		DelayMs(50);
		/////ADC
		adcx = Get_Adc_Average(ADC_Channel_1, 10);
		LCD_ShowxNum(156, 130, adcx, 4, 16, 0); //��ʾADC��ֵ
		temp_ = (float)adcx * (3.3 / 4096);
		adcx = temp_;
		LCD_ShowxNum(156, 150, adcx, 1, 16, 0); //��ʾ��ѹֵ
		temp_ -= adcx;
		temp_ *= 1000;
		LCD_ShowxNum(172, 150, temp_, 3, 16, 0X80);
		a = (float)(int)adcx + temp_ / 1000; //�����ѹֵ
		Gas_Strength = Adc_Average_To_Ppm(a); //����ѹת��Ϊ����Ũ��
		u1_printf("Gas_Strength:%d\r\n", Gas_Strength);//������ʾ�������
		memset(temp2, 0, 50);									   //��ջ�����1
		memset(tempAll2, 0, 100);								   //��ջ�����2
		memset(head2, 0, 3);									   //���MQTTͷ
		sprintf(temp2, "{\"Gas_Strength\":\"%d\"}", Gas_Strength); //��������
		head2[0] = 0x03;										   //�̶���ͷ
		head2[1] = 0x01;										   //�̶���ͷ
		head2[2] = strlen(temp2);								   //ʣ�೤��
		sprintf(tempAll2,"%c%c%c%s", head2[0], head2[1], head2[2], temp2);//��������
		u1_printf("\r\n"); //������ʾ�������
		u1_printf("%s\r\n", tempAll2 + 3);
		dataLen2=strlen(temp2) + 3;
		MQTT_PublishQs0(Data_TOPIC_NAME, tempAll2, dataLen2); //������ݣ�������������
		///////

		//�����¶ȣ�����Ũ�Ƚ����ж��ж�
		key=KEY_Scan(1);//����ɨ��
		if(key==KEY1_PRES)
		{
			flag=!flag;
		}
		if((temperature>60||Gas_Strength>9000||humidity>100)&&flag)
		{
			LED1=1;
			BEEP=1;
			//Send_Text_Sms();//����һ������Ϣ
		}
		else
		{
			LED1=0;
			BEEP=0;
		}
		LED1=!LED1;//LED��˸
		Send_Data();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//����жϱ�־
	}
}

/*-------------------------------------------------*/
/*�����������������жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������Ӳ������������жϴ�����             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*���������ڴ�����жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/

void MemManage_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������Ԥȡָʧ�ܣ��洢������ʧ���жϴ�����   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������δ�����ָ���Ƿ�״̬������           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*�����������жϣ�SWI ָ����õĴ�����           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
}

/*-------------------------------------------------*/
/*�����������Լ����������                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
}

/*-------------------------------------------------*/
/*���������ɹ����ϵͳ��������                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������SysTicϵͳ��શ�ʱ��������             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
}
