#ifndef HCSR04_H_
#define HCSR04_H_


#define HAL_TIM_MODULE_ENABLED

#include "stm32f1xx_hal.h"

typedef struct{
	GPIO_TypeDef *TRIG_GPIO;
	uint16_t TRIG_PIN;
	TIM_TypeDef *TIM_Instance;
	uint32_t IC_TIM_CH;
	uint32_t TIM_CLK_MHz;
	uint32_t TMR_PSC;
	uint32_t TMR_ARR;
}HCSR04_CfgType;

typedef struct {
	uint32_t IC_Val1;
	uint32_t IC_Val2;
	uint32_t Difference;
	uint8_t Is_First_Captured;  // is the first value captured ?
	float Distance;
	HAL_TIM_ActiveChannel ACTIV_CH;
}HCSR04_Info;

extern const HCSR04_CfgType HCSR04_Param;

void Delay_us(int time);
void HCSR04_Init(TIM_HandleTypeDef *TMR_Handle);
void HCSR04_TMR_IC_ISR(TIM_HandleTypeDef *htim);
float HCSR04_Read(void);
void HCSR04_Trigger();

#endif
