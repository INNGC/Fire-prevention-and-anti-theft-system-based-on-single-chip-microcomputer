/*
 * @Author: jack----13467346738@163.com
 * @Date: 2022-04-21 00:29:36
 * @LastEditors: jack
 * @LastEditTime: 2022-05-27 23:55:19
 * @FilePath: \终极版SIM\HARDWARE\LED\led.h
 */
#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

void LED_Init(void);//��ʼ��
void LED_On(void);
void LED_Off(void);
		 				    
#endif
