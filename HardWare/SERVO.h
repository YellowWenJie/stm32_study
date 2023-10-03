#include "stm32f10x.h"

#ifndef __SERVO_H
#define __SERVO_H

void PWM_Servos_Init(uint16_t ServosModel);
void Servo_SetAngle(float Angle);

#endif
