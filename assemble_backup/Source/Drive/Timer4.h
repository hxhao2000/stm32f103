#ifndef _TIMER4_H_
#define _TIMER4_H_
#include "stm32f10x.h"
#include "stdio.h"
#include <stdarg.h>
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "misc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_can.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_it.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


//以上是系统自带库点H

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//自己定义
//#include "Wendu.h"
#include "Timer4.h"
//#include "Uart.h"
//#include "Led.h"

//#include "OLED_I2C.h"
//#include "delay.h"
//#include "codetab.h"
#include "usart.h"
#include "usart_config.h"
#include "LD3320_config.h"
#include "LD3320.h"
#include "stm32f10x_conf.h"
#include "LDchip.h"
#include "Reg_RW.h"
//#include "LD3320_config.h"
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



/******************************************************************************
							全局变量声明
*******************************************************************************/ 
extern   uint32_t Timer4_Count;
extern   uint16_t Timer4_Frequency;

extern  uint8_t Count_1ms,Count_2ms,Count_5ms,Count_250ms;

/******************************************************************************
							全局函数声明
*******************************************************************************/ 
void Timer4_Init(uint16_t Handler_Frequency);
void Nvic_Init(void);
#endif
