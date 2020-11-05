#include "LD3320.h"
#include "dht11.h"

int main(void)
{

	USART_init(); 
	printf("\r\nUNV-LD3320-Test_Board-V1.0\r\n");	
	LD3320_main();
	for(;;)
	{
  }
}














