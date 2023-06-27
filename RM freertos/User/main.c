#include "main.h"
#include "stm32f4xx.h"
#include "stdio.h"

#include "mydelay.h"
#include "led.h"
#include "buzzer.h"
#include "key.h"
#include "rc.h"
#include "uart1.h"
#include "can.h"
#include "trigger.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "remote_control.h"
#include "CAN_receive.h"

#include "start_task.h"

#include "ist8310driver.h"
#include "BMI088driver.h"



const RC_ctrl_t* local_rc_ctrl;
const motor_measure_t* local_motor_chassis[4];
fp32 local_mag[3] = {0};
fp32 gyro[3], accel[3], temp;


void BSP_Init(void);

int main(void)
{
	BSP_Init();
	
	while(ist8310_init())
	{
		led_green_off();
		led_red_on();
	}

	while(BMI088_init())
	{
		led_green_off();
		led_red_toggle();
		mydelay_ms(500);
	}
	led_red_off();
	led_green_on();

	trigger_PWM_Init();
	
	local_rc_ctrl = get_remote_control_point();
	local_motor_chassis[0] = get_Chassis_Motor_Measure_Point(0);
	local_motor_chassis[1] = get_Chassis_Motor_Measure_Point(1);
	local_motor_chassis[2] = get_Chassis_Motor_Measure_Point(2);
	local_motor_chassis[3] = get_Chassis_Motor_Measure_Point(3);

	startTast();
	vTaskStartScheduler();
	
	while(1)
	{
		// BMI088_read(gyro, accel, &temp);
		// ist8310_read_mag(local_mag);
		// printf("%d\r\n", (int)temp);
		// printf("mag:\r\n%d\r\n%d\r\n%d\r\n",(int)local_mag[0],(int)local_mag[1],(int)local_mag[2]);
		// mydelay_ms(200);

		// CAN_CMD_CHASSIS(5*local_rc_ctrl->rc.ch[0],5*local_rc_ctrl->rc.ch[1],5*local_rc_ctrl->rc.ch[2],5*local_rc_ctrl->rc.ch[3]);
		// printf("*****remote******\r\n");
		// printf("ch[0]:%d\r\n", local_rc_ctrl->rc.ch[0]);
		// printf("ch[1]:%d\r\n", local_rc_ctrl->rc.ch[1]);
		// printf("ch[2]:%d\r\n", local_rc_ctrl->rc.ch[2]);
		// printf("ch[3]:%d\r\n", local_rc_ctrl->rc.ch[3]);
		// printf("ch[4]:%d\r\n", local_rc_ctrl->rc.ch[4]);
		// printf("s[0]: %d\r\n", local_rc_ctrl->rc.s[0]);
		// printf("s[1]: %d\r\n", local_rc_ctrl->rc.s[1]);

		// printf("******Motor1******\r\n");
		// printf("speed_rpm:%d\r\n", local_motor_chassis[0]->speed_rpm);
		// printf("given_current:%d\r\n", local_motor_chassis[0]->given_current);
		// printf("temperate:%d\r\n", local_motor_chassis[0]->temperate);
		// printf("******Motor2******\r\n");
		// printf("speed_rpm:%d\r\n", local_motor_chassis[1]->speed_rpm);
		// printf("given_current:%d\r\n", local_motor_chassis[1]->given_current);
		// printf("temperate:%d\r\n", local_motor_chassis[1]->temperate);
		// printf("******Motor3******\r\n");
		// printf("speed_rpm:%d\r\n", local_motor_chassis[2]->speed_rpm);
		// printf("given_current:%d\r\n", local_motor_chassis[2]->given_current);
		// printf("temperate:%d\r\n", local_motor_chassis[2]->temperate);
		// printf("******Motor4******\r\n");
		// printf("speed_rpm:%d\r\n", local_motor_chassis[3]->speed_rpm);
		// printf("given_current:%d\r\n", local_motor_chassis[3]->given_current);
		// printf("temperate:%d\r\n", local_motor_chassis[3]->temperate);
		// printf("*****************\r\n\r\n");
		// mydelay_ms(200);
	}
}

void BSP_Init(void)
{
	//中断组4
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    //LED灯初始化
	led_Init();

	//蜂鸣器初始化
	// buzzer_init(100, 7200);

	//自定义按键初始化
	key_Init();

	//遥控器初始化
	remote_control_init();

	//串口1初始化
	UART1_Init();

	//CAN通信初始化
	CAN1_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
    CAN2_mode_init(CAN_SJW_1tq, CAN_BS2_2tq, CAN_BS1_6tq, 5, CAN_Mode_Normal);
}

/*----------------陀螺仪数据准备 中断服务函数--------------*/

void EXTI3_IRQHandler(void)//ist8310
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		//ist8310_read_mag(local_mag);
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI4_IRQHandler(void)//accel
{
	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
		//BMI088_read(gyro, accel, &temp);
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void)//gyro
{
	if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{
		//BMI088_read(gyro, accel, &temp);
	}
	EXTI_ClearITPendingBit(EXTI_Line5);
}
/*-----------------------------------------------------*/
