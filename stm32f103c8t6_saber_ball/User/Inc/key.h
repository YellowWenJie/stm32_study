#ifndef KEY_H_
#define KEY_H_

#include "stm32f1xx_hal.h"
#include "main.h"

typedef enum
{
  Top_Up,
  Top_Mid,
  Top_Dowm,
  Bottom_Left,
  Bottom_Mid,
  Bottom_Right,
} Key_Single;

uint16_t Get_Key_Status(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif