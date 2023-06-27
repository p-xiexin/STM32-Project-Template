#include "spi.h"
#include "stm32f4xx.h"


/**SPI1 GPIO Configuration    
PB4     ------> SPI1_MISO
PB3     ------> SPI1_SCK
PA7     ------> SPI1_MOSI
*/
void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//同步时钟空闲状态
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//片选nss信号由软件决定
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE);
}

void SPI1_SetSpeedAndDataSize(uint16_t Speed, uint16_t DataSize)
{
    SPI1->CR1 &= 0xF7C7;
    SPI1->CR1 |= Speed;
    SPI1->CR1 |= DataSize;
    SPI1->CR1 |= 1 << 6;
}

uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{
    uint8_t retry=0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //等待发送区空
    {
        retry++;
        if(retry>200) return 0;
    }
    SPI_I2S_SendData(SPI1, TxData); //通过外设 SPIx 发送一个数据

    retry=0;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //等待接收完一个 byte
    { 
        retry++;
        if(retry>200) return 0;
    }
    return SPI_I2S_ReceiveData(SPI1); //返回通过 SPIx 最近接收的数据
}


uint8_t SPI1_RW_Byte(uint8_t TxData)
{
	uint8_t retry=0;
	while((SPI1->SR & SPI_SR_TXE) == 0)		//等待发送区空			  
	{
		retry++;
		if(retry>=200) return 0;
	}
    SPI1->DR = TxData;
	// *(uint8_t*)&(SPI1->DR) = TxData;

	retry = 0;
	while ((SPI1->SR & SPI_SR_RXNE) == 0) 		//等待接收完一个byte  
	{
		retry++;
		if(retry>=200) return 0;
	}
	return SPI1->DR;
    // return *(uint8_t*)&(SPI1->DR);
}
