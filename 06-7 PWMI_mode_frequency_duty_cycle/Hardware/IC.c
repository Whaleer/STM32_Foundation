#include "stm32f10x.h"                  // Device header


void IC_Init(void)
{
	// 1. RCC 开启时钟，把GPIO和TIM的时钟打开
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 2. GPIO初始化，把GPIO配置成输入模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3. 配置时基单元，让CNT计数器在内部时钟的驱动下自增运行
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	// 4. 配置输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 计划使用TIM3的通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF; // 用来配置输入捕获的寄存器
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 极性，我们选择上升沿触发
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 分频器，我们设置每次触发都有效
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 配置数据选择器 
	
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// 5. 选择从模式的触发源，触发源选择TI1FP1
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	
	// 6. 选择触发之后执行的操作，Reset操作
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	// 启动定时器
	TIM_Cmd(TIM3, ENABLE);
}



uint32_t IC_GetFreq(void)
{
	// fx = fc/N 
	// fc = 72M / (PSC+1) 
	// fc = 72M / 72 = 1MHz
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}

