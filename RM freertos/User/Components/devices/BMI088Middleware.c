#include "BMI088Middleware.h"
#include "stm32f4xx.h"
#include "mydelay.h"
#include "spi.h"


void BMI088_GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

    //初始化CS1_ACCEL
    GPIO_InitStructure.GPIO_Pin = CS1_ACCEL_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CS1_ACCEL_GPIO_Port,&GPIO_InitStructure);

    //初始化CS1_GYRO
    GPIO_InitStructure.GPIO_Pin = CS1_GYRO_Pin;
    GPIO_Init(CS1_GYRO_GPIO_Port,&GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能 SYSCFG 时钟
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource4);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource5);

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //初始化INT1_ACCEL
    GPIO_InitStructure.GPIO_Pin = INT1_ACCEL_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(INT1_ACCEL_GPIO_Port,&GPIO_InitStructure);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BMI088_NVIC;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);

    //初始化INT1_GRYO
    GPIO_InitStructure.GPIO_Pin = INT1_GRYO_Pin;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
    GPIO_Init(INT1_GRYO_GPIO_Port,&GPIO_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_Init(&NVIC_InitStructure);
}

void BMI088_com_init(void)
{
    SPI1_Init();
}

void BMI088_delay_ms(uint16_t ms)
{
    while(ms--)
    {
        BMI088_delay_us(1000);
    }
}

void BMI088_delay_us(uint16_t us)
{
    mydelay_us(us);
    // uint32_t ticks = 0;
    // uint32_t told = 0;
    // uint32_t tnow = 0;
    // uint32_t tcnt = 0;
    // uint32_t reload = 0;
    // reload = SysTick->LOAD;
    // ticks = us * 168;
    // told = SysTick->VAL;
    // while (1)
    // {
    //     tnow = SysTick->VAL;
    //     if (tnow != told)
    //     {
    //         if (tnow < told)
    //         {
    //             tcnt += told - tnow;
    //         }
    //         else
    //         {
    //             tcnt += reload - tnow + told;
    //         }
    //         told = tnow;
    //         if (tcnt >= ticks)
    //         {
    //             break;
    //         }
    //     }
    // }
}




void BMI088_ACCEL_NS_L(void)
{
    //HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
    GPIO_WriteBit(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, Bit_RESET);
}
void BMI088_ACCEL_NS_H(void)
{
    //HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
    GPIO_WriteBit(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, Bit_SET);
}

void BMI088_GYRO_NS_L(void)
{
    //HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
    GPIO_WriteBit(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, Bit_RESET);
}
void BMI088_GYRO_NS_H(void)
{
    //HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
    GPIO_WriteBit(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, Bit_SET);
}

uint8_t BMI088_read_write_byte(uint8_t txdata)
{
    uint8_t rx_data;
    //HAL_SPI_TransmitReceive(&hspi1, &txdata, &rx_data, 1, 1000);
    rx_data = SPI1_ReadWriteByte(txdata);
    return rx_data;
}

