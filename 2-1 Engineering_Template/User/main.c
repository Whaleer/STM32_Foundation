#include "stm32f10x.h"                  // Device header


int main()
{
	// 1. 首先开启使能时钟
	// 这样GPIOC的外设始终就配置好了
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	// 2. 配置端口模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 通用推挽输出
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 3. 设置端口的高低电平，开始电灯
	GPIO_SetBits(GPIOC, GPIO_Pin_13); // 将PC13号口置为高电平
	// GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 将PC13号口置为低电平
	while(1)
	{
		
	
	}
}
