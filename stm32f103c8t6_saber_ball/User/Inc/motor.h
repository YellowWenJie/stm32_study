#ifndef MOTOR_H_
#define MOTOR_H_

#include "stm32f1xx_hal.h"
#include "main.h"

typedef enum
{
  Top_Motor,
  Bottom_Motor
} Motor_Single;

void Motor(Motor_Single motor_single, int num);
int myabs(int num);
void Set_Pwm(int pwm);

#endif /* MOTOR_MOTOR_H_ */
