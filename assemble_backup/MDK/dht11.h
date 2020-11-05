#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f10x_gpio.h"
#include "delay.h"
#include "stdint.h"
#include "string.h"
#include "stm32f10x_rcc.h"


#define DHT11_IO 		GPIOB
#define DHT11_PIN		GPIO_Pin_11


u8 DHT11_Init(void);

u8 DHT11_Read_Data(u8 *temp,u8 *humi,u8 *temp_,u8 *humi_);

#endif

