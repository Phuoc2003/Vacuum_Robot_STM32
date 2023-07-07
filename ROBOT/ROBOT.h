/*
 * ROBOT.h
 *
 *  Created on: 20 Jun 2023
 *      Author: PC
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "../ECUAL/L298N/L298N.h"
#include "../ECUAL/HCSR04/HCSR04.h"

#define MOTORA 0 //Right motor
#define MOTORB 1 //Left motor

void Robot_Init(TIM_HandleTypeDef *TMR_MOTORA, TIM_HandleTypeDef *TMR_MOTORB, TIM_HandleTypeDef *TMR_HCSR04);
void Move_Forward();
void Move_Backward();
void Move_Right();
void Stop();
void Run(float Distance, uint8_t *state);

#endif /* ROBOT_H_ */
