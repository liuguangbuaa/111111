//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#ifndef _Tim1_PWM_H
#define _Tim1_PWM_H 
#include "stm32f10x.h"

#define PWM_GPIO  GPIOB 
#define PWM1L GPIO_Pin_13
#define PWM2L GPIO_Pin_14
#define PWM3L GPIO_Pin_15

void Tim1_PWM_Init(void);
void TIM1_Gpio(void);
void Start_Motor(void);
void Stop_Motor(void);
void MOS_Q41PWM(void) ;    // �л�Q14MOS����
void MOS_Q16PWM(void) ;    // �л�Q16MOS����
void MOS_Q63PWM(void) ;    // �л�Q36MOS����
void MOS_Q32PWM(void) ;    // �л�Q32MOS����
void MOS_Q25PWM(void) ;    // �л�Q52MOS����
void MOS_Q54PWM(void) ;    // �л�Q54MOS����


void MOS_AbcPWM(void);
void MOS_ABcPWM(void);
void MOS_BacPWM(void);
void MOS_BCaPWM(void);
void MOS_CabPWM(void);
void MOS_CAbPWM(void);
#endif
