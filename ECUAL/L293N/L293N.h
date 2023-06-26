#ifndef L293N_H_
#define L293N_H_

#define HAL_TIM_MODULE_ENABLED

#include "stm32f1xx_hal.h"

#define FORWARD 1
#define BACKWARD 0
#define MAX_SPEED 255

#define L293N_UNITS 2

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
}L293N_CfgType;

extern const L293N_CfgType L293N_Param[L293N_UNITS];

void L293N_Init(uint8_t MOTOR_Instance, TIM_HandleTypeDef *htim);
void L293N_SetSpeed(uint8_t MOTOR_Instance, uint16_t Speed);
void L293N_SetDir(uint8_t MOTOR_Instance, uint8_t DIR);
void L293N_Stop(uint8_t MOTOR_Instance);

#endif /* L293N_L293N_H_ */
