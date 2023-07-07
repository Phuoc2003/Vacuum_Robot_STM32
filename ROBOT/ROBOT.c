#include "ROBOT.h"

void Robot_Init(TIM_HandleTypeDef *TMR_MOTORA, TIM_HandleTypeDef *TMR_MOTORB, TIM_HandleTypeDef *TMR_HCSR04)
{
	L293N_Init(MOTORA, TMR_MOTORA);
	L293N_Init(MOTORB, TMR_MOTORB);
	HCSR04_Init(TMR_HCSR04);
}

void Move_Forward()
{
	L293N_SetDir(MOTORA, FORWARD);
	L293N_SetDir(MOTORB, FORWARD);
	L293N_SetSpeed(MOTORA, 150);
	L293N_SetSpeed(MOTORB, 150);
}

void Move_Backward()
{
	L293N_SetDir(MOTORA, BACKWARD);
	L293N_SetDir(MOTORB, BACKWARD);
	L293N_SetSpeed(MOTORA, 180);
	L293N_SetSpeed(MOTORB, 180);
}

void Move_Right()
{
	L293N_SetDir(MOTORB, FORWARD);
	L293N_SetSpeed(MOTORB, 250);
	L293N_Stop(MOTORA);
}

void Stop()
{
	L293N_Stop(MOTORA);
	L293N_Stop(MOTORB);
}

void Run(float Distance, uint8_t *state)
{
		if (*state==1&&Distance > 10.0)
		{
			Move_Forward();
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
		}
		else if(*state==0 || Distance < 10.0)
		{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
			Stop();
			HAL_Delay(1000);
			Move_Backward();
			HAL_Delay(1000);
			Move_Right();
			HAL_Delay(1000);
			*state = 1;
		}
}
