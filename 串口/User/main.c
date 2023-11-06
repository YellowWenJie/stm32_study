#include <stdio.h>
#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "HCSR04.h"
#include "Delay.h"
#include "Motor.h"
#include "Key.h"
#include "Mini_mp3.h"

uint8_t RxData;
uint16_t T;
int8_t Speed;
uint8_t KeyNum;

int main(void)
{

  OLED_Init();
  Motor_Init();
  Key_Init();
  // HCSR04_Init();
  Serial_Init();
  MiniMp3_Init();
  OLED_ShowString(1, 1, "Speed:");
  OLED_ShowString(2, 1, "RxData:");

  while (1)
  {

    if (Serial_GetRxFlag() == 1)
    {
      RxData = Serial_GetRxData();
      Serial_SendByte(RxData);
      OLED_ShowHexNum(2, 8, RxData, 4);

      if (RxData == 1)
      {
        MiniMp3_Play();
      }
      if (RxData == 2)
      {
        MiniMp3_Stop();
      }
      if (RxData == 3)
      {
        MiniMp3_Specify_File_Play();
      }
    }

    Motor_SetSpeed(Speed);
    OLED_ShowSignedNum(1, 7, Speed, 3);
  }
}

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

// OLED_Init();
// HCSR04_Init();
// Serial_Init();
// Key_Init();

// OLED_ShowString(1, 1, "distance:");
// OLED_ShowString(1, 13, "cm");
// OLED_ShowString(2, 1, "RxData:");
// OLED_ShowString(3, 1, "Speed:");

// while (1)
// {
//   T = HCSR04_GetValue();
//   OLED_ShowNum(1, 10, T, 3);
//   Delay_ms(100);
//   if (Serial_GetRxFlag() == 1)
//   {
//     RxData = Serial_GetRxData();

//     Serial_SendByte(RxData);
//     OLED_ShowHexNum(2, 8, RxData, 4);

//     KeyNum = Key_GetNum();

//     if (KeyNum == 1)
//     {
//       Speed += 20;
//       if (Speed > 100)
//       {
//         Speed = -100;
//       }
//     }
//     TB6612_SetSpeed(Speed);
//     OLED_ShowSignedNum(3, 7, Speed, 3);
//   }
// }
