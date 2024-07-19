#include "stm32f10x.h"                  // Device header

int16_t Encoder_Count;

void Encoder_Init(void)
{
	// 1. 开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 2. 配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 初始化GPIOB外设
	
	// 3. 配置AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	// 4. 配置EXTI
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1; // 指定要配置的中断线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 指定选择的中断线的新状态
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 指定外部中断线的模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 指定触发信号的有效边沿
	EXTI_Init(&EXTI_InitStructure);
	
	// 5. 配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 选择第2种分组方式
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; // 指定中断通道开启 or 关闭
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 指定中断通道是使能还是失能 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; // 指定中断通道开启 or 关闭
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 指定中断通道是使能还是失能 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // 
	NVIC_Init(&NVIC_InitStructure);
}


int16_t Encoder_Get(void)
{
	int16_t temp;
	temp = Encoder_Count;
	Encoder_Count = 0;
	return temp;
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
		{
			Encoder_Count--;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			Encoder_Count++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
