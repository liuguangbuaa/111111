//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#include "Tim1_ISR_MCLoop.h"
#include "ADC_int.h"
#include "Hall.h"
#include "State_Machine.h"
#include "PI_Cale.h"
#include "VvvfControl.h"
#include "Hallless.h"
#include "Drive parameters.h"
#include "Tim1_PWM.h"
#include "Control stage parameters.h"
#ifdef HALL
extern    Hall          Hall_Three;
#endif
#ifdef HALLLESS
extern    Hallless      Hallless_Three;
#endif
extern    PIDREG_T      pi_spd;
extern    PIDREG_T      pi_ICurr;
extern    State         StateContr;
extern    ADCSamp       ADCSampPare;
extern    VvvF_start    VvvF_startPare;

u8 Hall_state=0;
u8 Pre_Hall_state=0;
u8 BEMC_state_flag=0;

u16 Hall_count=0;
u16 Hall_60_time=0;
u16 Hall_30_time=0;
extern int Current10Times;

u16 uiCurrentAD_Last = 0;
u16 uiVoltageAD_Last = 0;
u16 Vbus10Times=0;
u16 Vbus=0;
float uiCurrent = 0;
u16 uiCurrent10 = 0;

//�˴��жϵļ��ʱ��Ϊ1/16k=62.5us
void TIM1_UP_IRQHandler(void)
{
//	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    

    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
       TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    
      Hall_state=(HALLLESS_U_STATUS<<2)+(HALLLESS_V_STATUS<<1)+HALLLESS_W_STATUS;
    
    if( Hall_state==Pre_Hall_state)
    {  
        Hall_count++;
    }
    else if(Hall_state!=Pre_Hall_state)
    {
        
        Hall_60_time=Hall_count;       
        Hall_30_time=Hall_60_time>>1;
        BEMC_state_flag=1;            //������־λ
        Hall_count=0;
    
    }
     Vbus = ADC1->JDR1;                     //����ADֵ

     HALLLESS_ADC_Sample(Pre_Hall_state);   //��������
   
     uiCurrent=(float)(1.65*ADC1->JDR1/4095);
    
     Vbus10Times=Caculate_10TimesVbus(Vbus);
    
     uiCurrent10=(u16)(240*uiCurrent-66);
     uiVoltageAD_Last=Vbus;
    
//     Current10Times=Caculate_10TimesCurrent(100);
    
    
     Pre_Hall_state= Hall_state;

   
	if(StateContr.drive_car == 1)
	{	
#ifdef HALLLESS
	
	
        if(StateContr.Start_order == 1)  //����ʽ��������һ������λ���ڶ�������������������һ���ٶȣ�������;���������㡣������������������20��ʱ�䡣   
        {

           Open_loop1();
//        Open_12step_loop();
       
//		   StateContr.Start_order = 2;	
	    }
	    if(StateContr.Start_order == 2)
	   {	

//		  Hallless_SW();           //��⵽���綯�ƹ���㣬����������ʱ30�Ⱥ���
//        Get_Hall_EMC_state();
         
           if(BEMC_state_flag==1)
           {
               
               Hall_30_time--;
               if(Hall_30_time<=1)
               {
                   BEMC_state_flag=0;
                   //״̬�л� 
                   switch(Hall_state)
                   {
                       case  0x5:         
                       {
                           MOS_Q41PWM();    //AB
                       }   
                       break;
                       case  0x4:     
                       {
                           MOS_Q16PWM();    //AC
                       } 
                       break;
                       case  0x6:   
                       {
		
                           MOS_Q63PWM();    //BC
                       }
                       break;
                       case  0x2:  
                       {
                           MOS_Q32PWM();    //BA
        
                       }
                       break;
                       case  0x3:    
                       {
                           MOS_Q25PWM();      //CA

                       }
                       break;
                       case  0x1:    
                       {
                           MOS_Q54PWM();       //CB
                       }
                       break;
                       default:
                       {
                           Stop_Motor();
                           Hallless_Three.Speed_RPM=0;
                       } 
      
                       break;
	     
   
                   }
 
               }
    
           }

	      //��������
	     if(StateContr.Control_Mode == 1)
	     {
		    StateContr.Duty = 4500-1000; //900 StateContr.Aim_Duty �ٷ�֮20StateContr.Aim_Duty
	     }
       
       
	     //�ٶȻ�
	     if(StateContr.Control_Mode == 2)
	     {
	    	StateContr.Speed_Count++;
	    	if(StateContr.Speed_Count > 2)
	     	{
			    StateContr.Speed_Count = 0;
                pi_spd.Fdb = Hallless_Three.Speed_RPMF ;   
			    PID_CALC(pi_spd);
			    FirstOrder_LPF_Cacl(pi_spd.Out,pi_spd.OutF,0.08379f);
			
			    StateContr.Duty= pi_spd.OutF;
		    }
	     }
	     //������+�ٶȻ�
	     if(StateContr.Control_Mode == 3)
	     {
	    	StateContr.Speed_Count++;
	     	StateContr.Current_Count++;
	    	if(StateContr.Speed_Count > 2)
	     	{
			 pi_spd.Fdb = Hallless_Three.Speed_RPMF ;   
			 PID_CALC(pi_spd);
			 FirstOrder_LPF_Cacl(pi_spd.Out,pi_spd.OutF,0.08379f);
			 StateContr.Speed_Count = 0;
	 	    }
		   if(StateContr.Current_Count > 1)
	       {
			 pi_ICurr.Ref = Torque;                                     //����Ĭ��0.2A����
			 pi_ICurr.Fdb = ADCSampPare.BUS_Curr;   
			 PID_CALC(pi_ICurr);
			 FirstOrder_LPF_Cacl(pi_ICurr.Out,pi_ICurr.OutF,0.08379f);
			 StateContr.Current_Count = 0;
		   }
		  if(pi_ICurr.OutF>pi_spd.OutF)	 
		  {
			 StateContr.Duty= pi_spd.OutF;              
			 pi_ICurr.Ui  = pi_spd.Ui;
		  } 
		  else 
		  {
			 StateContr.Duty= pi_ICurr.OutF;          
			 pi_spd.Ui  = pi_ICurr.Ui;
	 	  }
	   }
	 }
	
#endif	
	
#ifdef HALL
    HALL_ADC_Sample();
	Hall_SW();
	//����
	if(StateContr.Control_Mode == 1)
	{
		StateContr.Duty = StateContr.Aim_Duty;
	}
	//�ٶȻ�
	if(StateContr.Control_Mode == 2)
	{
		StateContr.Speed_Count++;
		if(StateContr.Speed_Count > 2)
		{
			 pi_spd.Fdb = Hall_Three.Speed_RPMF ;   
			 PID_CALC(pi_spd);
			 FirstOrder_LPF_Cacl(pi_spd.Out,pi_spd.OutF,0.08379f);
			 StateContr.Speed_Count = 0;
			 StateContr.Duty= pi_spd.OutF;
		}
	}
	//������+�ٶȻ�
	if(StateContr.Control_Mode == 3)
	{
		StateContr.Speed_Count++;
		StateContr.Current_Count++;
		if(StateContr.Speed_Count > 2)
		{
			 pi_spd.Fdb = Hall_Three.Speed_RPMF ;   
			 PID_CALC(pi_spd);
			 FirstOrder_LPF_Cacl(pi_spd.Out,pi_spd.OutF,0.08379f);
			 StateContr.Speed_Count = 0;
		}
		if(StateContr.Current_Count > 1)
		{
			 pi_ICurr.Ref = Torque;                                     //����Ĭ��0.2A����
			 pi_ICurr.Fdb = ADCSampPare.BUS_Curr ;   
			 PID_CALC(pi_ICurr);
			 FirstOrder_LPF_Cacl(pi_ICurr.Out,pi_ICurr.OutF,0.08379f);
			 StateContr.Current_Count = 0;
		}
		if(pi_ICurr.OutF>pi_spd.OutF)	 
		{
			 StateContr.Duty= pi_spd.OutF;              
			 pi_ICurr.Ui  = pi_spd.Ui;
		} 
		else 
		{
			 StateContr.Duty= pi_ICurr.OutF;          
			 pi_spd.Ui  = pi_ICurr.Ui;
		}
	}
#endif
    }
}
}
