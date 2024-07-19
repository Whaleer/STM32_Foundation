#include "stm32f10x.h"                  // Device header

extern uint16_t Num;

void Timer_Init(void)
{
	// 1. RCC开启时钟
	// TIM2 是APB1总线的外设
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 2. 选择时基单元的时钟源
	TIM_InternalClockConfig(TIM2);
	
	// 3. 配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 指定时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 我们选择向上计数 
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // 周期，就是ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器的值
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	// 4. 配置输出中断控制，允许更新中断输出到NVIC
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// 5. 配置NVIC，在NVIC中打开定时器中断的通道，并分配一个优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	// 6. 运行控制，使能寄存器
	TIM_Cmd(TIM2, ENABLE);
	
}





