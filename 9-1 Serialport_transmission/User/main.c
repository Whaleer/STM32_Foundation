#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main()
{
	OLED_Init();
	
	Serial_Init();
	

	Serial_Printf("Num = %d\r\n", 666);
	
	while(1){}
}
