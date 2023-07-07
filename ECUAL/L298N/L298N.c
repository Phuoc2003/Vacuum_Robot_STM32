#include "../ECUAL/L298N/L298N.h"
#include "../Core/Inc/main.h"

const L298N_CfgType L298N_Param[L298N_UNITS] = { {
GPIOB,
GPIOB,
GPIO_PIN_10,
GPIO_PIN_11,
TIM2,
TIM_CHANNEL_1, 16, 0, 255 }, {
GPIOB,
GPIOB,
GPIO_PIN_12,
GPIO_PIN_13,
TIM2,
TIM_CHANNEL_2, 16, 0, 255 } };

void L298N_Init(uint8_t MOTOR_Instance, TIM_HandleTypeDef *htim) {
	GPIO_InitTypeDef IN_GPIO_InitStruct = { 0 };

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	if ((L298N_Param[MOTOR_Instance].IN1_GPIO == GPIOA)||(L298N_Param[MOTOR_Instance].IN2_GPIO == GPIOA))
		__HAL_RCC_GPIOA_CLK_ENABLE();
	else if ((L298N_Param[MOTOR_Instance].IN1_GPIO == GPIOB)||(L298N_Param[MOTOR_Instance].IN2_GPIO == GPIOB))
		__HAL_RCC_GPIOB_CLK_ENABLE();
	else if ((L298N_Param[MOTOR_Instance].IN1_GPIO == GPIOC)||(L298N_Param[MOTOR_Instance].IN2_GPIO == GPIOC))
		__HAL_RCC_GPIOC_CLK_ENABLE();
	else if ((L298N_Param[MOTOR_Instance].IN1_GPIO == GPIOD)||(L298N_Param[MOTOR_Instance].IN2_GPIO == GPIOD))
		__HAL_RCC_GPIOD_CLK_ENABLE();
	else if ((L298N_Param[MOTOR_Instance].IN1_GPIO == GPIOE)||(L298N_Param[MOTOR_Instance].IN2_GPIO == GPIOE))
		__HAL_RCC_GPIOE_CLK_ENABLE();

	IN_GPIO_InitStruct.Pin = L298N_Param[MOTOR_Instance].IN1_PIN;
	IN_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	IN_GPIO_InitStruct.Pull = GPIO_NOPULL;
	IN_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(L298N_Param[MOTOR_Instance].IN1_GPIO, &IN_GPIO_InitStruct);

	IN_GPIO_InitStruct.Pin = L298N_Param[MOTOR_Instance].IN2_PIN;
	IN_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	IN_GPIO_InitStruct.Pull = GPIO_NOPULL;
	IN_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(L298N_Param[MOTOR_Instance].IN2_GPIO, &IN_GPIO_InitStruct);

	HAL_GPIO_WritePin(L298N_Param[MOTOR_Instance].IN1_GPIO, L298N_Param[MOTOR_Instance].IN1_PIN, 0);
	HAL_GPIO_WritePin(L298N_Param[MOTOR_Instance].IN2_GPIO, L298N_Param[MOTOR_Instance].IN2_PIN, 0);

	htim->Instance = L298N_Param[MOTOR_Instance].TIM_Instance;
	htim->Init.Prescaler = L298N_Param[MOTOR_Instance].TMR_PSC;
	htim->Init.CounterMode = TIM_COUNTERMODE_UP;
	htim->Init.Period = L298N_Param[MOTOR_Instance].TMR_ARR;
	htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(htim);
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig);
	HAL_TIM_PWM_Init(htim);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, L298N_Param[MOTOR_Instance].PWM_TIM_CH);
	HAL_TIM_MspPostInit(htim);

	/*--------[ Start The PWM Channel ]-------*/

	HAL_TIM_PWM_Start(htim, L298N_Param[MOTOR_Instance].PWM_TIM_CH);
}

void L298N_SetSpeed(uint8_t MOTOR_Instance, uint16_t Speed)
{
	if (Speed > MAX_SPEED)
		Speed = MAX_SPEED;
	if (L298N_Param[MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_1)
		L298N_Param[MOTOR_Instance].TIM_Instance->CCR1 = Speed;
	else if (L298N_Param[MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_2)
		L298N_Param[MOTOR_Instance].TIM_Instance->CCR2 = Speed;
	else if (L298N_Param[MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_3)
		L298N_Param[MOTOR_Instance].TIM_Instance->CCR3 = Speed;
	else if (L298N_Param[MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_4)
		L298N_Param[MOTOR_Instance].TIM_Instance->CCR4 = Speed;
}

void L298N_SetDir(uint8_t MOTOR_Instance, uint8_t DIR) {
	if (DIR == FORWARD)
	{
		HAL_GPIO_WritePin(L298N_Param[MOTOR_Instance].IN1_GPIO, L298N_Param[MOTOR_Instance].IN1_PIN, 1);
		HAL_GPIO_WritePin(L298N_Param[MOTOR_Instance].IN2_GPIO, L298N_Param[MOTOR_Instance].IN2_PIN, 0);
	}
	else if (DIR == BACKWARD)
	{
		HAL_GPIO_WritePin(L298N_Param[MOTOR_Instance].IN1_GPIO, L298N_Param[MOTOR_Instance].IN1_PIN, 0);
		HAL_GPIO_WritePin(L298N_Param[MOTOR_Instance].IN2_GPIO, L298N_Param[MOTOR_Instance].IN2_PIN, 1);
	}
}

void L298N_Stop(uint8_t MOTOR_Instance)
{
	HAL_GPIO_WritePin(L298N_Param[MOTOR_Instance].IN1_GPIO, L298N_Param[MOTOR_Instance].IN1_PIN, 0);
	HAL_GPIO_WritePin(L298N_Param[MOTOR_Instance].IN2_GPIO, L298N_Param[MOTOR_Instance].IN2_PIN, 0);
	L298N_SetSpeed(MOTOR_Instance, 0);
}







