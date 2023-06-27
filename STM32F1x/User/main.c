#include "stm32f10x.h"
#include "main.h"

#include "led.h"
#include "Delay.h"


void BSP_Init(void);

int main(void)
{
	BSP_Init();
	
	while(1)
	{
		led0_toggle();
		Delay_ms(500);
	}
}

void BSP_Init(void)
{
	//led ≥ı ºªØ
	led_Init();
	
}
