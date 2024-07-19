#include "stm32f10x.h"                  // Device header



void PWM_Init(void)
{
	// 1. RCC开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	
	
	// 配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);			
	
	
	// 2. 配置时基单元
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 指定时钟分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 我们选择向上计数 
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // 周期，就是ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 重复计数器的值
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	
	// 3. 配置输出比较单元
	// 我们使用的是PA0口，对应的就是第一个输出比较通道
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); // 给结构体赋默认值，不然会导致某些成员未定义而产生问题
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // 设置输出比较的模式, 选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 设置输出比较的极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 
	TIM_OCInitStructure.TIM_Pulse = 0; // 设置CCR的值
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	
	// 运行控制，使能寄存器
	TIM_Cmd(TIM2, ENABLE);

}


void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

// 设置一个单独调节 PSC 的函数
void PWM_SetPrescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate); // 第三个参数，也就是影子寄存器，是立刻生效，还是在更新事件生效
}






