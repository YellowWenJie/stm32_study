#include "key.h"

uint16_t Get_Key_Status(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
  {
    HAL_Delay(10); // 延时10ms, 做一个软件的消抖, 防止因抖动而检测到按键按下
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
    {
      return 1;
    }
  }
  return 0;
}
