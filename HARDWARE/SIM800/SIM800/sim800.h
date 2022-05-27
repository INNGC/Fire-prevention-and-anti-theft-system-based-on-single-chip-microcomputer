#ifndef __SIM800_H
#define __SIM800_H
#include "sys.h"
#include "string.h"
#include "usartSIM.h"
#include "delay.h"







void Wait_CREG(void);
void Send_Text_Sms(void);
void Set_Text_Mode(void);
void Second_AT_Command(char *b,char *a,u8 wait_time);
u8 Find(char *a);
void CLR_Buf2(void);
void TIM5_IRQHandler(void);   //TIM3ÖÐ¶Ï
void USART2_IRQHandler(void);

#endif



