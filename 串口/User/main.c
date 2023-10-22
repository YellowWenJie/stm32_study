#include <stdio.h>
#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "HCSR04.h"
#include "Delay.h"

uint8_t RxData;
uint16_t T;

int main(void)
{
  // OLED_Init();
  // OLED_ShowString(1, 1, "RxData:");
  // Serial_Init();

  // while (1)
  // {
  //   if (Serial_GetRxFlag() == 1)
  //   {
  //     RxData = Serial_GetRxData();

  //     Serial_SendByte(RxData);
  //     OLED_ShowHexNum(1, 8, RxData, 4);
  //   }
  // }

  OLED_Init();
  HCSR04_Init();
  Serial_Init();

  OLED_ShowString(1, 1, "distance:");
  OLED_ShowString(1, 13, "cm");
  OLED_ShowString(2, 1, "RxData:");

  while (1)
  {
    T = HCSR04_GetValue();
    OLED_ShowNum(1, 10, T, 3);
    Delay_ms(100);
    if (Serial_GetRxFlag() == 1)
    {
      RxData = Serial_GetRxData();

      Serial_SendByte(RxData);
      OLED_ShowHexNum(2, 8, RxData, 4);
    }
  }
}
