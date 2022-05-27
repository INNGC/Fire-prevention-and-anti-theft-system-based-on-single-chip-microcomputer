/*
 * @Author: jack----13467346738@163.com
 * @Date: 2022-03-10 23:00:12
 * @LastEditors: jack
 * @LastEditTime: 2022-03-11 20:47:49
 * @FilePath: \USERd:\毕设\code\成功\成功3\HARDWARE\ADC\adc.h
 */
#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"


void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
u16 Adc_Average_To_Ppm(float value);
 
#endif 
