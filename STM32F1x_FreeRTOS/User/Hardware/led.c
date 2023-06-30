#include "stm32f10x.h"                  // Device header
#include "led.h"

void led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	led0_off();
	led1_off();
}

void led0_on(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

void led0_off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void led0_toggle(void)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	}
}

void led1_on(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
}

void led1_off(void)
{
	GPIO_SetBits(GPIOE, GPIO_Pin_5);
}

void led1_toggle(void)
{
	if (GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5) == 0)
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	}
}
