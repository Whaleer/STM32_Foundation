#include "stm32f10x.h"                  // Device header


uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	// 1. 开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// 开启AFIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 2. 配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	// EXTO 配置浮空，上拉，下拉都是可以的
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 这里我们配置上拉输入，默认为高电平的输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); // 初始化GPIOB外设
	
	// 3. 配置AFIO
	// 当这样配置时，AFIO的第14个数据选择器就拨好了
	// 其中输入端被拨到了 GPIOB 外设上，对应的就是 PB14 号引脚
	// 输出端固定连接的是 EXTI 的第14个中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// 4. 配置EXTI
	// 将 EXTI 的第14个线路配置为中断模式，下降沿触发，开启中断
	// 这样，PB14的电平信号就能够通过 EXTI 通向下一级 NVIC 了
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // 指定要配置的中断线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 指定选择的中断线的新状态
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 指定外部中断线的模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 指定触发信号的有效边沿
	EXTI_Init(&EXTI_InitStructure);
	
	// 5. 配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 选择第2种分组方式
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; // 指定中断通道开启 or 关闭
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 指定中断通道是使能还是失能 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 
	NVIC_Init(&NVIC_InitStructure);
	
}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}
	


void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountSensor_Count++;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
