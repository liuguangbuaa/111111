//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#ifndef _TIMER_H
#define _TIMER_H 

#include "stm32f10x.h"

void SysTickConfig(void);
void TIM2_Init(void);
void TIM3_Init(void);
void TIM3_IRQHandler(void);

	
#endif
