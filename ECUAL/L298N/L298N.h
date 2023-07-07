#ifndef L298N_H_
#define L298N_H_

#define HAL_TIM_MODULE_ENABLED

#include "stm32f1xx_hal.h"

#define FORWARD 1
#define BACKWARD 0
#define MAX_SPEED 255

#define L298N_UNITS 2

typedef struct
{
	GPIO_TypeDef *IN1_GPIO;
	GPIO_TypeDef *IN2_GPIO;
	uint16_t IN1_PIN;
	uint16_t IN2_PIN;
	TIM_TypeDef *TIM_Instance;
	uint32_t PWM_TIM_CH;
	uint32_t TIM_CLK_MHz;
	uint32_t TMR_PSC;
	uint32_t TMR_ARR;
}L298N_CfgType;

extern const L298N_CfgType L298N_Param[L298N_UNITS];

void L298N_Init(uint8_t MOTOR_Instance, TIM_HandleTypeDef *htim);
void L298N_SetSpeed(uint8_t MOTOR_Instance, uint16_t Speed);
void L298N_SetDir(uint8_t MOTOR_Instance, uint8_t DIR);
void L298N_Stop(uint8_t MOTOR_Instance);

#endif /* L298N_L298N_H_ */
