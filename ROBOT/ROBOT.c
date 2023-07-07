/*
 * ROBOT.c
 *
 *  Created on: 20 Jun 2023
 *      Author: PC
 */

#include "ROBOT.h"

void Robot_Init(TIM_HandleTypeDef *TMR_MOTORA, TIM_HandleTypeDef *TMR_MOTORB, TIM_HandleTypeDef *TMR_HCSR04)
{
	L298N_Init(MOTORA, TMR_MOTORA);
	L298N_Init(MOTORB, TMR_MOTORB);
	HCSR04_Init(TMR_HCSR04);
}

void Move_Forward()
{
	L298N_SetDir(MOTORA, FORWARD);
	L298N_SetDir(MOTORB, FORWARD);
	L298N_SetSpeed(MOTORA, 150);
	L298N_SetSpeed(MOTORB, 150);
}

void Move_Backward()
{
	L298N_SetDir(MOTORA, BACKWARD);
	L298N_SetDir(MOTORB, BACKWARD);
	L298N_SetSpeed(MOTORA, 180);
	L298N_SetSpeed(MOTORB, 180);
}

void Move_Right()
{
	L298N_SetDir(MOTORB, FORWARD);
	L298N_SetSpeed(MOTORB, 250);
	L298N_Stop(MOTORA);
}

void Stop()
{
	L298N_Stop(MOTORA);
	L298N_Stop(MOTORB);
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






