/*
 * PB12(SPI2_NSS)  -> MAX7219 CS
 * PB13(SPI2_SCK)  -> MAX7219 CLK
 * PB14(SPI2_MISO) -> N/A
 * PB15(SPI2_MOSI) -> MAX7219 DIN
 */

#include "stm32f10x.h"

#define ADDR_NOOP        0
#define ADDR_DIGIT0      1
#define ADDR_DIGIT1      2
#define ADDR_DIGIT2      3
#define ADDR_DIGIT3      4
#define ADDR_DIGIT4      5
#define ADDR_DIGIT5      6
#define ADDR_DIGIT6      7
#define ADDR_DIGIT7      8
#define ADDR_DECODEMODE  9
#define ADDR_INTENSITY   10
#define ADDR_SCANLIMIT   11
#define ADDR_SHUTDOWN    12
#define ADDR_DISPLAYTEST 15

void delay(int i) {
	while (i) {
		i--;
	}
}

void SPI2_WriteByte(u16 TxData) {
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	SPI_I2S_SendData(SPI2, TxData);
	delay(120); // Keeping CS pin LOW since send over.
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

int main(void) {
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
 	GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; // Only write.
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; // MAX7219's CS pin need to control.
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
 
	SPI_Cmd(SPI2, ENABLE);

	SPI2_WriteByte((ADDR_DECODEMODE<<8)|0xff);
	SPI2_WriteByte((ADDR_INTENSITY<<8)|2);
	SPI2_WriteByte((ADDR_SCANLIMIT<<8)|7);
	SPI2_WriteByte((ADDR_SHUTDOWN<<8)|1);
	SPI2_WriteByte((ADDR_DISPLAYTEST<<8)|0);

	SPI2_WriteByte((ADDR_DIGIT0<<8)|1);
	SPI2_WriteByte((ADDR_DIGIT1<<8)|2);
	SPI2_WriteByte((ADDR_DIGIT2<<8)|3);
	SPI2_WriteByte((ADDR_DIGIT3<<8)|4);
	SPI2_WriteByte((ADDR_DIGIT4<<8)|5);
	SPI2_WriteByte((ADDR_DIGIT5<<8)|6);
	SPI2_WriteByte((ADDR_DIGIT6<<8)|7);
	SPI2_WriteByte((ADDR_DIGIT7<<8)|8);
	
	while (1) {
		
	}
}
