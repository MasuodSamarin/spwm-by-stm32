#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
//ALIENTEK Mini STM32�����巶������8
//PWM���ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾

u16 freq = 12800, Period = 0;                       //PWM????????;??????
__IO float Period_percent; 
 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
    
   Period_percent = ((SystemCoreClock / freq) - 1) / 65530.0;
	TIM1_PWM_Init(12800,0);//����Ƶ��PWMƵ��=72000/(899+1)=80Khz 
   TIM2_Int_Init(12800,0);
    
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    
    
   while(1)
	{
         ;
	} 
}


