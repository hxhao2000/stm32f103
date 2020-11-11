#include "delay.h"

u32 SysTic_count = 100;

 void Delay_ms(u16 ms)
 {
	 SysTic_count = ms;
   
	 SysTick_Config(72000);		
	 while( SysTic_count!= 0);
	 SysTick->CTRL = 0;  
 }
 

 void Delay_us(u32 us)
 {
	 SysTic_count = us;
   
	 SysTick_Config(72);		
	 while( SysTic_count!= 0);
	 SysTick->CTRL = 0;  
 }


 void SysTick_Handler(void)
{
	 if(SysTic_count != 0) SysTic_count--;
}
