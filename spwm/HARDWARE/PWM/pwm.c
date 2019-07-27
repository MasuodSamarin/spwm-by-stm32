#include "pwm.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
const u32 spwm[256] = {
 0x7FFF,0x8323,0x8647,0x896A,0x8C8B,0x8FAA,0x92C7,0x95E1,0x98F8,0x9C0A,0x9F19,0xA223,0xA527,0xA826,0xAB1E,0xAE10
,0xB0FB,0xB3DE,0xB6B9,0xB98C,0xBC55,0xBF16,0xC1CD,0xC47A,0xC71C,0xC9B3,0xCC3F,0xCEBF,0xD132,0xD39A,0xD5F4,0xD842
,0xDA81,0xDCB3,0xDED6,0xE0EB,0xE2F1,0xE4E7,0xE6CE,0xE8A5,0xEA6C,0xEC23,0xEDC9,0xEF5E,0xF0E1,0xF254,0xF3B4,0xF503
,0xF640,0xF76B,0xF883,0xF989,0xFA7C,0xFB5C,0xFC28,0xFCE2,0xFD89,0xFE1C,0xFE9C,0xFF08,0xFF61,0xFFA6,0xFFD7,0xFFF5
,0xFFFE,0xFFF5,0xFFD7,0xFFA6,0xFF61,0xFF08,0xFE9C,0xFE1C,0xFD89,0xFCE2,0xFC29,0xFB5C,0xFA7C,0xF989,0xF883,0xF76B
,0xF640,0xF503,0xF3B4,0xF254,0xF0E1,0xEF5E,0xEDC9,0xEC23,0xEA6C,0xE8A5,0xE6CE,0xE4E7,0xE2F1,0xE0EB,0xDED6,0xDCB3
,0xDA81,0xD842,0xD5F4,0xD39A,0xD132,0xCEBF,0xCC3F,0xC9B3,0xC71C,0xC47A,0xC1CD,0xBF16,0xBC55,0xB98C,0xB6B9,0xB3DE
,0xB0FB,0xAE10,0xAB1E,0xA826,0xA527,0xA222,0x9F19,0x9C0A,0x98F8,0x95E1,0x92C7,0x8FAA,0x8C8B,0x8969,0x8647,0x8323
,0x7FFF,0x7CDB,0x79B7,0x7694,0x7373,0x7054,0x6D37,0x6A1D,0x6706,0x63F4,0x60E5,0x5DDB,0x5AD7,0x57D8,0x54E0,0x51EE
,0x4F03,0x4C20,0x4945,0x4672,0x43A8,0x40E8,0x3E31,0x3B84,0x38E2,0x364B,0x33BF,0x313F,0x2ECB,0x2C64,0x2A0A,0x27BC
,0x257D,0x234B,0x2128,0x1F13,0x1D0D,0x1B17,0x1930,0x1759,0x1592,0x13DB,0x1235,0x10A0,0x0F1D,0x0DAA,0x0C49,0x0AFB
,0x09BE,0x0893,0x077B,0x0675,0x0582,0x04A2,0x03D5,0x031C,0x0275,0x01E2,0x0162,0x00F6,0x009D,0x0058,0x0027,0x0009
,0x0000,0x0009,0x0027,0x0058,0x009D,0x00F6,0x0162,0x01E2,0x0275,0x031C,0x03D6,0x04A3,0x0583,0x0675,0x077B,0x0893
,0x09BE,0x0AFB,0x0C4A,0x0DAA,0x0F1D,0x10A1,0x1236,0x13DB,0x1592,0x1759,0x1930,0x1B17,0x1D0E,0x1F13,0x2128,0x234B
,0x257D,0x27BD,0x2A0A,0x2C64,0x2ECC,0x3140,0x33C0,0x364B,0x38E3,0x3B85,0x3E31,0x40E8,0x43A9,0x4673,0x4945,0x4C21
,0x4F04,0x51EE,0x54E0,0x57D9,0x5AD7,0x5DDC,0x60E5,0x63F4,0x6707,0x6A1D,0x6D37,0x7054,0x7374,0x7695,0x79B8,0x7CDB
};

u16 TimerPeriod   = 0; 
u16 Channel1Pulse = 0; 
static u16 i = 0;

//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 pfreq ,u16 psc)
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

   
   TimerPeriod = (SystemCoreClock / pfreq) - 1;
   
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	
   Channel1Pulse = (u16)((u32)(128  * (TimerPeriod - 1)) / 100 );   //???50%
   
   
   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
 
   
}

void TIM2_Int_Init(u16 ifreq,u16 psc)                       //ifreq????????????,psc?????
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);         //????

    TimerPeriod = (SystemCoreClock / ifreq) - 1;                             

    TIM_TimeBaseStructure.TIM_Period        = TimerPeriod;              //?????????????
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;                      //??????  10Khz?????  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                        //????????
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;       //TIM??????
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);           

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );                           //?????TIM2??,??????(????????????)

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;   //TIM2??
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;           //????3?
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 4;           //????4?
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      //IRQ?????
    NVIC_Init(&NVIC_InitStructure);                                     //??NVIC_InitStruct???????????NVIC???

    TIM_Cmd(TIM2, ENABLE);                                              //??TIM2                 
}

//???2??????
void TIM2_IRQHandler(void)  
{
    extern float Period_percent;

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)                  //??TIM2????????
    {    
        LED0 = !LED0;
        TIM_SetCompare1(TIM1,(u16)(Period_percent*spwm[i])) ; //??TIM1??1?PWM???,?????/?????1??
        i++;
        if(i == 256)  //?????100??
        {          
            i = 0;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //??TIM2?????? 
    }
}
