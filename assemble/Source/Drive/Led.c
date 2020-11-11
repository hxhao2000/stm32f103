
//##############################################头文件
#include "Timer4.h"

//##############################################头文件

//##############################################全局变量声明




//##############################################全局变量声明


/******************************************************************************
函数原型：	void LED_Init(void)
功    能：	LED初始化
*******************************************************************************/ 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Structure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	//改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	//改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	GPIO_Structure.GPIO_Pin =  LED2_Pin;	//LED2：电源旁的指示灯
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED2_GPIO, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Pin =  LED3_Pin;	//LED3：机身周围的夜间灯	
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(LED3_GPIO, &GPIO_Structure);
	
	
}

/******************************************************************************
函数原型：	void LED_ON_OFF(void)
功    能：	LED开机闪烁
*******************************************************************************/ 

uint8_t  ii;

void LED_ON_OFF(void)
{
//	uint8_t i ;
//	for(i=0;i<3;i++)
//	{
//		LED2_ON;LED3_OFF;
//		//Delay_led(200);
//		LED3_ON;LED2_OFF;
//		//Delay_led(200);	
//	}
//	for(i=0;i<3;i++)
//	{
//		LED2_ON;LED3_OFF;
//	//	Delay_led(100);
//		LED3_ON;LED2_OFF;
//		//Delay_led(100);	
//	}	
//	LED2_OFF;
//	LED3_OFF;
////	Delay_led(100);

//LED3_OFF;	

//    if   (ii)
//		{
//			ii++;
//			switch(ii)
//			{
//				case 0:	

//				break;
//				case 1:	
//					LED2_ON;
//				break;
//				case 2:	
//					LED3_ON;
//				break;
//				case 3:	
//   ii=0;
//				break;
//			}
//			
//				__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop(); 
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
//		}

}

