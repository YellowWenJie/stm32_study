#include "key.h"

uint16_t Get_Key_Status(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
  {
    HAL_Delay(10); // ��ʱ10ms, ��һ�����������, ��ֹ�򶶶�����⵽��������
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
    {
      return 1;
    }
  }
  return 0;
}
