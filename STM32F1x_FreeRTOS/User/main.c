#include "stm32f10x.h"
#include "main.h"

#include "led.h"
#include "delay.h"

#include "start_task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"


void BSP_Init(void);

int main(void)
{
	BSP_Init();

	startTast();
	vTaskStartScheduler();

	
	while(1)
	{
		led0_toggle();
		led1_toggle();
		delay_ms(1000);
	}
}

void BSP_Init(void)
{
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//	
//	//��ʼ���δ�ʱ��
   	delay_init(configTICK_RATE_HZ);
	
	//led ��ʼ��
	led_Init();
	led1_on();
}
