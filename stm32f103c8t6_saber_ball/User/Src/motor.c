#include "motor.h"
#include "tim.h"

void Motor(Motor_Single motor_single, int num)
{
  switch (motor_single)
  {
  case Top_Motor:
    if (num == 1)
    {
      HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET); // 0
      HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);   // 1
    }
    else if (num == -1)
    {
      HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);   // 1
      HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET); // 0
    }
    else
    {
      HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET); // 0
      HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET); // 0
    }
    break;
  case Bottom_Motor:
    if (num == 1)
    {
      HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET); // 0
      HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);   // 1
    }
    else if (num == -1)
    {
      HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);   // 1
      HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET); // 0
    }
    else
    {
      HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET); // 0
      HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET); // 0
    }
    break;
  }
}

int myabs(int num)
{
  int temp;
  if (num < 0)
    temp = -num;
  else
    temp = num;
  return temp;
}

void Set_Pwm(int pwm)
{
  int pwmb_abs;
  pwmb_abs = myabs(pwm);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwmb_abs);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwmb_abs);
}