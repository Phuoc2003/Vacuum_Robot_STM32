#include "../HCSR04/HCSR04.h"

const HCSR04_CfgType HCSR04_Param={
		GPIOB,
		GPIO_PIN_1,
		TIM1,
		TIM_CHANNEL_1,
		16,
		15,
		65535
};

static HCSR04_Info gs_HCSR04_info = { 0 };

void Delay_us(int time)
{
	HCSR04_Param.TIM_Instance->CNT = 0;
	while (HCSR04_Param.TIM_Instance->CNT < time);
}

void HCSR04_Init(TIM_HandleTypeDef *TMR_Handle)
{
	GPIO_InitTypeDef TRIG_GPIO_InitStruct = { 0 };

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_IC_InitTypeDef sConfigIC = { 0 };

	if (HCSR04_Param.TRIG_GPIO == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	else if (HCSR04_Param.TRIG_GPIO == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	else if (HCSR04_Param.TRIG_GPIO == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	else if (HCSR04_Param.TRIG_GPIO == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	else if (HCSR04_Param.TRIG_GPIO == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();

	TRIG_GPIO_InitStruct.Pin = HCSR04_Param.TRIG_PIN;
	TRIG_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	TRIG_GPIO_InitStruct.Pull = GPIO_NOPULL;
	TRIG_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(HCSR04_Param.TRIG_GPIO, &TRIG_GPIO_InitStruct);

	if(HCSR04_Param.IC_TIM_CH == TIM_CHANNEL_1)
		gs_HCSR04_info.ACTIV_CH = HAL_TIM_ACTIVE_CHANNEL_1;
	else if(HCSR04_Param.IC_TIM_CH == TIM_CHANNEL_2)
		gs_HCSR04_info.ACTIV_CH = HAL_TIM_ACTIVE_CHANNEL_2;
	else if(HCSR04_Param.IC_TIM_CH == TIM_CHANNEL_3)
		gs_HCSR04_info.ACTIV_CH = HAL_TIM_ACTIVE_CHANNEL_3;
	else if(HCSR04_Param.IC_TIM_CH == TIM_CHANNEL_4)
		gs_HCSR04_info.ACTIV_CH = HAL_TIM_ACTIVE_CHANNEL_4;

	TMR_Handle->Instance = HCSR04_Param.TIM_Instance;
	TMR_Handle->Init.Prescaler = HCSR04_Param.TMR_PSC;
	TMR_Handle->Init.CounterMode = TIM_COUNTERMODE_UP;
	TMR_Handle->Init.Period = HCSR04_Param.TMR_ARR;
	TMR_Handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TMR_Handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(TMR_Handle);
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(TMR_Handle, &sClockSourceConfig);
	HAL_TIM_IC_Init(TMR_Handle);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(TMR_Handle, &sMasterConfig);
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(TMR_Handle, &sConfigIC, HCSR04_Param.IC_TIM_CH);

	HAL_TIM_Base_Start_IT(TMR_Handle);
	HAL_TIM_IC_Start_IT(TMR_Handle, HCSR04_Param.IC_TIM_CH);

	gs_HCSR04_info.Is_First_Captured = 0;
}

void HCSR04_TMR_IC_ISR(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == gs_HCSR04_info.ACTIV_CH) // if the interrupt source is channel1
		{
		if (gs_HCSR04_info.Is_First_Captured == 0) // if the first value is not captured
		{
			gs_HCSR04_info.IC_Val1 = HAL_TIM_ReadCapturedValue(htim, HCSR04_Param.IC_TIM_CH); // read the first value
			gs_HCSR04_info.Is_First_Captured = 1;  // set the first captured as true
			// Now change the polarity to falling edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, HCSR04_Param.IC_TIM_CH, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if (gs_HCSR04_info.Is_First_Captured == 1)   // if the first is already captured
		{
			gs_HCSR04_info.IC_Val2 = HAL_TIM_ReadCapturedValue(htim, HCSR04_Param.IC_TIM_CH); // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter
			gs_HCSR04_info.Is_First_Captured = 0; // set it back to false

			// set polarity to rising edge
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, HCSR04_Param.IC_TIM_CH, TIM_INPUTCHANNELPOLARITY_RISING);
		}
	}
}

float HCSR04_Read(void)
{
	if (gs_HCSR04_info.IC_Val2 > gs_HCSR04_info.IC_Val1)
		gs_HCSR04_info.Difference = gs_HCSR04_info.IC_Val2 - gs_HCSR04_info.IC_Val1;
	else if (gs_HCSR04_info.IC_Val1 > gs_HCSR04_info.IC_Val2)
		gs_HCSR04_info.Difference = (HCSR04_Param.TMR_PSC - gs_HCSR04_info.IC_Val1) + gs_HCSR04_info.IC_Val2;

	gs_HCSR04_info.Distance = gs_HCSR04_info.Difference * .034 / 2;

	return gs_HCSR04_info.Distance;
}

void HCSR04_Trigger()
{
	HAL_GPIO_WritePin(HCSR04_Param.TRIG_GPIO, HCSR04_Param.TRIG_PIN, 1);
	Delay_us(2);  // wait for 10 us
	HAL_GPIO_WritePin(HCSR04_Param.TRIG_GPIO, HCSR04_Param.TRIG_PIN, 0);
}


















