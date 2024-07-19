#include "stm32f10x.h"                  // Device header

uint16_t MyDMA_Size;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;
	
	// 1. RCC开启DMA的时钟
	// DMA是AHB总线的设备
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// 2. 调用DMA_Init，初始化各个参数
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 指定外设站点是DST还是SRC
	DMA_InitStructure.DMA_BufferSize = Size; // 传输计数器
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 指定传输计数器是否自动重装
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	// DMA工作的三个条件
	// 1. 传输计数器 > 0 已经满足
	// 2. 触发源有触发信号。这里是软件触发，一直都有触发信号，满足
	// 3. DMA使能
	
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
}


void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET); // 等待转运完成
	DMA_ClearFlag(DMA1_FLAG_TC1);
}	




