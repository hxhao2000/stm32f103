#include "LD3320.h"
#include "dht11.h"

int main(void)
{

	USART_init();
	LCD_Init ();	
	LD3320_main();
	for(;;)
	{
  }
}














