//##############################################头文件
#include "Timer4.h"

//##############################################头文件


//##############################################全局变量声明

uint32_t Timer4_Count = 0;  //记录Timer3中断次数
uint16_t Timer4_Frequency;//Timer3中断频率


//##############################################全局变量声明
//******************************************************TIMER4初始化函数
void Timer4_Init(uint16_t Handler_Frequency)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	Timer4_Frequency = Handler_Frequency;
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = 1000*1000/Handler_Frequency ;//装载值
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	//分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //不分割时钟
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);//清除中断标志
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);//使能定时器3
	
}
//******************************************************TIMER4初始化函数


//******************************************************TIMER4中断优先级设定
void Nvic_Init(void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;
	
	
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级分组

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//从占优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

//******************************************************TIMER4中断优先级设定





//******************************************************中断服务函数TIMER4

//##############################################全局变量声明
uint8_t Count_1ms,Count_2ms,Count_5ms,Count_250ms;

//##############################################全局变量声明
void TIM4_IRQHandler(void)//Timer4中断
{	
	if(TIM4->SR & TIM_IT_Update)
	{     
		TIM4->SR = ~TIM_FLAG_Update;//清除中断标志
		
		//if( Bsp_Int_Ok == 0 )	return;//硬件未初始化完成，则返回
		Timer4_Count++;
		Count_1ms++;
		Count_2ms++;
		Count_5ms++;
		Count_250ms++;
	}
}
//******************************************************中断服务函数TIMER4
