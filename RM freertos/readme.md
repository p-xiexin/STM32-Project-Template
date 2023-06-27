pxx的工作：
（1）遥控器数据的接受解码：DBUS使用USART1、DMA1_stream1 chanel4 串口空闲中断
（2）串口发送数据到电脑显示便于调试，printf函数重定向 uart1 
（3）LED 使用GPIOH Pin10、Pin11、Pin12 
（4）底盘can总线驱动电机 can1、can2接收中断
    can1连接底盘四个电机
    can2连接云台 yaw pitch trigger电机（未测试）
（5）IST8310 磁力计 模拟I2C
（6）Key按键 PA0 外部中断
（7）IMU088 硬件spi1(+DMA)
（8）蜂鸣器 PD14 TIM4_CH3
（9）基于CM4内核的数学库移植，可以基于内核级别的数学运算优化
（10）摩擦轮使用pwm控制  PE9 TIM1_CH1、PE11 TIM1_CH2
 待完成.....................
 	姿态解算融合，四元数


需要完成的任务
FreeRTOS的移植
（1）底盘控制任务
（2）云台控制任务
（3）发射机构控制任务
（4）相关设备数据校准的任务
（5）遥控器数据解码             √
（6）can总线数据收发            √
（7）PID控制器				    √
（8）机器人姿态获取

电机和电调设备型号
（1）底盘电机		M3508		电调C620		can   	 	4
（2）yaw电机 	   GM6020		  内置			can			1
（3）pitch电机	   GM6020		  内置			can			1
（4）发射电机    snail 2305		电调C620	  can+pwm		2
（5）换弹电机       M2006		电调C610		can			1

目前存在的问题
（1）ist8310 DRDY中断服务函数无法进入                  √
（2）bmi088  初始化不稳定，需要重复多次运行直到成功
    while(BMI088_init())
	{
		led_green_off();
		led_red_toggle();
		delay_ms(500);
	}
	led_red_off();
	led_green_on();
（3）1、3号电机堵转
