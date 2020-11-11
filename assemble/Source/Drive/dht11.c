#include "dht11.h"


GPIO_InitTypeDef GPIO_InitStructure;	

static void GPIO_SETOUT(void);
static void GPIO_SETIN(void);
static u8 DHT11_Check(void);


/**********************************************/
static void DHT11_Rst(void)
{                 
		GPIO_SETOUT();											//output
    GPIO_ResetBits(DHT11_IO,DHT11_PIN);
    Delay_ms(20);    										
    GPIO_SetBits(DHT11_IO,DHT11_PIN); 	
	  Delay_us(30);     									
		GPIO_ResetBits(DHT11_IO,DHT11_PIN);
}


/**********************************************/
u8 DHT11_Init(void){
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DHT11_IO,&GPIO_InitStructure);
	
	DHT11_Rst();
	
	return DHT11_Check();
}


/**********************************************/
static void GPIO_SETOUT(void)
{
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(DHT11_IO,&GPIO_InitStructure);
	
}


/**********************************************/
static void GPIO_SETIN(void)
{
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(DHT11_IO,&GPIO_InitStructure);
}


/**********************************************/
static u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	GPIO_SETIN();		
	
  while (!GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN) && retry<100)
	{
		retry++;
		Delay_us(1);
	}
	if(retry >= 100)
		return 0;
	else 
		retry = 0;
  while (GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN) && retry<100)
	{
		retry++;
		Delay_us(1);
	}
	if(retry>=100)		
		return 0;
	return 1;				
}



/***********************************************/
static u8 DHT11_Read_Bit(void)
{
 	u8 retry = 0;
	
	while(GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN) && retry<100)
	{
		retry++;
		Delay_us(1);
	}
	retry = 0;
	while(!GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN) && retry<100)
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(30);
	if(GPIO_ReadInputDataBit(DHT11_IO,DHT11_PIN)) return 1;
	else return 0;		   
}


/***********************************************************************

************************************************************************/
static u8 DHT11_Read_Byte(void)    
{        
  u8 i,dat;
  dat=0;
	
	for (i=0;i<8;i++) 
	{
   	dat<<=1; 
	  dat|=DHT11_Read_Bit();
  }	
	
  return dat;
}


/**************************************************************************/

u8 DHT11_Read_Data(u8 *temp,u8 *humi,u8 *temp_,u8 *humi_)
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==1)	
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*humi_=buf[1];
			*temp=buf[2];
			*temp_=buf[3];
		}
	}else return 0;		
	return 1;					
}
