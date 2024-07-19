#include "stm32f10x.h"                  // Device header



void AD_Init(void)
{
	// 1. 开启RCC时钟，包括ADC和GPIO的时钟
	// ADCCLK的分频器也需要配置一下
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADCCLK = 72MHz / 6 = 12MHz
	
	// 2. 配置GPIO，把需要用的GPIO配置成模拟输入的模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);			
	
	// 3. 配置这里的多路开关，把通道接入到右边的规则组表里
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	// 4. 配置ADC转换器了
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // ADC工作在独立模式，还是双ADC模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ADC数据是左对其还是右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 触发控制的触发源
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 选择连续转换还是单次转换
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 扫描还是非扫描
	ADC_InitStructure.ADC_NbrOfChannel = 1; // 在扫描模式下，总共用多少个通道
	
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 开关控制，开启ADC
	ADC_Cmd(ADC1, ENABLE);
	
	
	// 校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // 转换完成后，自动跳出while循环
	return ADC_GetConversionValue(ADC1);
}
