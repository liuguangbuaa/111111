//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#include "Timer.h"

void SysTickConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  if (SysTick_Config(SystemCoreClock /100))   
  {	
    while (1);
  }
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}


void TIM2_Init(void)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_DeInit(TIM2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    /*-------ת�ټ��� ��ʱ������---------*/ 
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;   //����ֵ65535
	TIM_TimeBaseStructure.TIM_Prescaler = 71;    //47 ��Ƶϵ��48��Ƶ��1us����һ�Σ����ɼ�65.535ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM2->CNT = 0;//����ֵ����
	TIM_Cmd(TIM2, ENABLE); 
    
}

void TIM3_Init(void)
{
 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;
    TIM_DeInit(TIM3);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /*-------���ڻ��� ��ʱ������---------*/
	TIM_TimeBaseStructure.TIM_Period = 0;         //��ʼ������ֵ��Ϊ0 			
	TIM_TimeBaseStructure.TIM_Prescaler = 71;     //��Ƶϵ��48��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
    /*-------���ڻ��� ��ʱ���ж�����---------*/ 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//���ȼ�����
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3,DISABLE);                                   //��ʧ�ܶ�ʱ��
}


void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//����жϱ�־
        
        
        
        
    }
}
