//############################################################
// Created on: 2019��5��18��
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//��Ȩ���У�����ؾ�
//STM32������ƿ�����
//���ĿƼ�
//��ַ: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//������ƴ������䣺JXKJ_2007@163.com
//############################################################
#include "VvvfControl.h"
#include "State_Machine.h"
#include "Tim1_PWM.h"
#include "Hallless.h"
#include "Drive parameters.h"
#ifdef     HALLLESS
extern    Hallless        Hallless_Three;
extern    State           StateContr;
extern    VvvF_start      VvvF_startPare;

void Delay_US(u32 nCount_temp)
{
	u32 nCount=nCount_temp*8;
	while(nCount--);
}

void Delay_MS(u32 nCount_temp)
{
	u32 nCount=nCount_temp*8000;
	while(nCount--);
}

u8 fix_position=1;
u8 ucDrection=1;   //1:����AB-->AC  0:����AC-->AB
u16 Open_count=0;
u16 Time_count=600;   //62.5*1100=68.75ms
u16 Duty_count=(4500-500);
void Open_loop(void)
{
    Time_count--;
    if(Time_count<=100)
    {
       Time_count=100;
    }
    
    Duty_count++;
    if(Duty_count>=1000)
    {
       Duty_count=1000;
    } 
    StateContr.Duty = Duty_count;
    Open_count++;
    
    
    if(fix_position==1)
    {  
//      MOS_Q16PWM() ;                  //AC  �̶�һ�࣬����������ῴ����ʱ����ת
        MOS_Q41PWM();                   //AB  �̶�һ��,����������ῴ��˳ʱ����ת
        if(Open_count==Time_count)      //62.5*200=12.5ms
        {
            Open_count=0;
            fix_position=2;
            
        }
    
    } 
    else if(fix_position==2)
    {
//        MOS_Q41PWM();                  //AB  �̶�һ��
        MOS_Q16PWM() ;                   //AC  �̶�һ��
        if(Open_count==Time_count)       //62.5*200=12.5ms
        {
            Open_count=0;
            fix_position=3;
            
        }
            
    }
    else if(fix_position==3)
    {
//        MOS_Q54PWM();           //CB  �̶�һ��
        MOS_Q63PWM();           //BC  �̶�һ��
        if(Open_count==Time_count)     //62.5*200=12.5ms
        {
            Open_count=0;
            fix_position=4;
            
        }

    }
    else if(fix_position==4)
    {
//        MOS_Q25PWM();           //CA  �̶�һ��
        MOS_Q32PWM();           //BA  �̶�һ��
        if(Open_count==Time_count)     //62.5*200=12.5ms
        {
            Open_count=0;
            fix_position=5;
            
        }

    }
    else if(fix_position==5)
    {
//        MOS_Q32PWM();           //BA  �̶�һ��
        MOS_Q25PWM();           //CA  �̶�һ��
        if(Open_count==Time_count)     //62.5*200=12.5ms
        {
            Open_count=0;
            fix_position=6;       
        }

    }
    else if(fix_position==6)
    {
//        MOS_Q63PWM();           //BC  �̶�һ��
         MOS_Q54PWM();           //CB  �̶�һ��
        if(Open_count==Time_count)     //62.5*200=12.5ms
        {
            Open_count=0;
            fix_position=1;       
        }

    }  

}

const u8 ucReverseHall[8]={0,5,1,3,2,6,4,0};//��������
const u8 ucForwardHall[8]={0,4,6,2,3,1,5,0};//��������462315.AB AC BC BA CA CB
u8 ucCounter_temp=0;

void Open_loop1(void)
{
    u8 uctemp;
    
    Time_count--;
    if(Time_count<=100)
    {
       Time_count=100;
    }
    
    Duty_count--;
    if(Duty_count<=(4500-1000))
    {
       Duty_count=3500;
    } 
    StateContr.Duty = Duty_count;
    Open_count++;
    if(Duty_count==3500&&Time_count==100)
    {
       ucCounter_temp++;
        if(ucCounter_temp>100)
        {
//          
            StateContr.Start_order = 2;	
        
        }
    
    
    }
    
    
    if(Open_count==Time_count)      //62.5*200=12.5ms
    {
            Open_count=0;
            fix_position=fix_position+1;
            if(fix_position==7)
            {
                fix_position=1;
            
            }
            if(ucDrection==0)
            {
             uctemp=ucReverseHall[fix_position];
            }
            else if(ucDrection==1)
            {
                uctemp=ucForwardHall[fix_position];
            
            }
        OpenLoop_SW(uctemp) ;
      
    }

}





u16 fix_position1=1;
u16 Open_count1=0;
u16 Time_count1=600;   //62.5*1100=68.75ms
u16 Duty_count1=0;
void Open_12step_loop(void)
{
     Time_count1--;
    if(Time_count1<=100)
    {
       Time_count1=100;
    }
    
    Duty_count=Duty_count1+2;
    if(Duty_count1>=1000)
    {
       Duty_count1=1000;
    }
    
    StateContr.Duty = Duty_count1;
    Open_count1++;
    if(fix_position1==1)
    {  
        MOS_Q41PWM();                  //AB  �̶�һ��,����������ῴ��˳ʱ����ת
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=2;
            
        }
    
    } 
    else if(fix_position1==2)
    {
 
        MOS_AbcPWM();
       
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=3;
            
        }
            
    }
    else if(fix_position1==3)
    {

         MOS_Q16PWM() ;         //AC  �̶�һ��
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=4;
            
        }

    }
    else if(fix_position1==4)
    {
        MOS_ABcPWM();

        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=5;
            
        }

    }
    else if(fix_position1==5)
    {

        MOS_Q63PWM();           //BC  �̶�һ��

        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=6;       
        }

    }
    else if(fix_position1==6)
    {
            
        MOS_BacPWM();
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=7;       
        }

    }  
    else if(fix_position1==7)
    {

         MOS_Q32PWM();                  //BA  �̶�һ��
        
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=8;       
        }

    } 
    else if(fix_position1==8)
    {

        
        MOS_BCaPWM();
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=9;       
        }

    } 
    else if(fix_position1==9)
    {

        MOS_Q25PWM();                   //CA  �̶�һ��
        
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=10;       
        }

    } 
    else if(fix_position1==10)
    {

        
        MOS_CabPWM();
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=11;       
        }

    } 
    else if(fix_position1==11)
    {

        MOS_Q54PWM();                 //CB  �̶�һ��
        
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=12;       
        }

    } 
    else if(fix_position1==12)
    {

        
         MOS_CAbPWM();
        if(Open_count1==Time_count1)     //62.5*200=12.5ms
        {
            Open_count1=0;
            fix_position1=1;       
        }

    } 

}




void OpenLoop_SW(u8 ucHallTemp) 
{
	switch(ucHallTemp)
	{
		case  0x4:     
		{
		MOS_Q41PWM();     //AB
		}
		break;
		case  0x6:    
		{
		MOS_Q16PWM();    //AC
		}
		break;
		case  0x2:       
		{
		MOS_Q63PWM();    //BC
		}
		break;
		case 0x3:  
		{
		MOS_Q32PWM();    //BA
		}
		break;
		case 0x1:   
		{
		MOS_Q25PWM();    //CA
		}
		break;
		case 0x5:
		{
		MOS_Q54PWM();    //CB
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



#endif
