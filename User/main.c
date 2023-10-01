#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "CountSensor.h"
#include "Encoder.h"
#include "Timer.h"
#include "PWM.h"
#include "SERVO.h"
#include "IC.h"
#include "AD.h"
#include "MyDMA.h"

uint16_t Num;
uint8_t i;
uint8_t KeyNum;
float Angle;
uint16_t AD0, AD1, AD2, AD3;

uint8_t DataA[] = {0x01, 0x02, 0x03, 0x04};
uint8_t DataB[] = {0, 0, 0, 0};
int main(void)
{

  // * 寄存器方式
  // * GPIC 的时钟
  // RCC->APB2ENR = 0x00000010;
  // // * PC13口的模式
  // GPIOC->CRH = 0x00300000;
  // // * 端口输出寄存器
  // GPIOC->ODR = 0x00002000;
  // // ! 声明不得出现在代码块中的可执行语句之后
  // GPIO_InitTypeDef GPIO_InitStructure;
  // // * 库函数方式
  // // * GPIC 的时钟
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  // // * PC13口的模式
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // GPIO_Init(GPIOC,&GPIO_InitStructure);
  // // * 把指定端口设置为高电平
  // GPIO_SetBits(GPIOC,GPIO_Pin_13);
  // // * 把指定端口设置为低电平
  // // GPIO_ResetBits(GPIOC,GPIO_Pin_13);

  // * led灯闪烁
  // int sum = 0;
  // GPIO_InitTypeDef GPIO_InitStructure;
  // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  // // * 推挽输出
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  // GPIO_Init(GPIOB, &GPIO_InitStructure);
  // * 设置高低电平
  // GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
  // while (sum < 20)
  // {
  //   GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1);
  //   Delay_ms(500);
  //   GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);
  //   Delay_ms(500);
  //   sum++;
  // }

  // * 流水灯
  // while (sum < 2)
  // {
  //   GPIO_Write(GPIOA, ~0x0001); // 0000 0000 0000 0001
  //   Delay_ms(500);
  //   GPIO_Write(GPIOA, ~0x0002); // 0000 0000 0000 0010
  //   Delay_ms(500);
  //   GPIO_Write(GPIOA, ~0x0004); // 0000 0000 0000 0100
  //   Delay_ms(500);
  //   GPIO_Write(GPIOA, ~0x0008); // 0000 0000 0000 1000
  //   Delay_ms(500);
  //   GPIO_Write(GPIOA, ~0x0010); // 0000 0000 0001 0000
  //   Delay_ms(500);
  //   GPIO_Write(GPIOA, ~0x0020); // 0000 0000 0010 0000
  //   Delay_ms(500);
  //   GPIO_Write(GPIOA, ~0x0040); // 0000 0000 0100 0000
  //   Delay_ms(500);
  //   GPIO_Write(GPIOA, ~0x0080); // 0000 0000 1000 0000
  //   Delay_ms(500);
  //   sum++;
  // }

  // * 蜂鸣器
  // while (sum < 5)
  // {
  //   GPIO_ResetBits(GPIOB, GPIO_Pin_12);
  //   Delay_ms(100);
  // 	GPIO_SetBits(GPIOB, GPIO_Pin_12);
  //   Delay_ms(100);
  // 	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
  //   Delay_ms(200);
  // 	GPIO_SetBits(GPIOB, GPIO_Pin_12);
  //   Delay_ms(200);
  //   sum++;
  // }

  // * OLED
  // OLED_Init();
  // OLED_ShowChar(1, 1, 'A');
  // OLED_ShowString(1,3,"HellowWorld");
  // OLED_ShowNum(2,1,12345,6);
  // while(1){};

  // * 对射式红外传感器
  // OLED_Init();
  // CountSensor_Init();
  // OLED_ShowString(1, 1, "Count:");
  // while (1)
  // {
  //   OLED_ShowNum(1, 7, CountSensor_Get(), 5);
  // }
  // * 旋转编码器
  // int16_t Num;
  // OLED_Init();
  // Encoder_Init();
  // OLED_ShowString(1, 1, "Num:");
  // while (1)
  // {
  //   Num += Encoder_Get();
  //   OLED_ShowSignedNum(1, 5, Num, 5);
  // }

  // * TIM 时钟
  // OLED_Init();
  // Timer_Init();
  // OLED_ShowString(1, 1, "Num:");
  // OLED_ShowString(2, 1, "CNT:");
  // while (1)
  // {
  //   OLED_ShowNum(1, 5, Num, 5);
  //   OLED_ShowNum(2, 5, TIM_GetCounter(TIM2), 5);
  // }
  // * PWM 驱动 LED 呼吸灯
  // PWM_Init();
  // while (1)
  // {
  // 	// LED 逐渐变亮
  //   for (i = 0; i <= 100; i++)
  //   {
  //     PWM_SetCompare1(i);
  // 		Delay_ms(10);
  //   }
  // 	// LED 逐渐变暗
  // 	for (i = 0; i <= 100; i++)
  //   {
  //    PWM_SetCompare1(100-i);
  // 		Delay_ms(10);
  //   }
  // }

  // * 驱动舵机
  // PWM_Servos_Init(360);
  // Key_Init();
  // OLED_Init();
  // OLED_ShowString(1, 1, "Angle:");
  // while (1)
  // {
  //   KeyNum = Key_GetNum();
  //   if (KeyNum == 1)
  //   {
  //     Angle += 1;
  //     if (Angle > 9)
  //     {
  //       Angle = 0;
  //     }
  //   }
  //   else if (KeyNum == 2)
  //   {
  //     Angle = 0;
  //   }
  //   Servo_SetAngle(Angle);
  //   OLED_ShowNum(1, 7, Angle, 3);
  // }

  // * PWM 驱动直流电机

  // * 输入捕获模式测频率
  // OLED_Init();
  // PWM_Init();
  // IC_Init();
  // OLED_ShowString(1, 1, "Freq:00000Hz");
  // OLED_ShowString(2, 1, "Duty:00%");
  // // * 频率 Freq = 72M / (PSC + 1) / 100
  // PWM_SetPrescaler(7200 - 1);
  // // * 占空比 Duty = CCR / 100
  // PWM_SetCompare1(80);
  // while (1)
  // {
  //   OLED_ShowNum(1, 6, IC_GetFreq(), 5);
  //   OLED_ShowNum(2, 6, IC_GetDuty(), 2);
  // }

  // * AD 多通道
  // OLED_Init();
  // AD_Init();
  // OLED_ShowString(1, 1, "AD0:");
  // OLED_ShowString(2, 1, "AD1:");
  // OLED_ShowString(3, 1, "AD2:");
  // OLED_ShowString(4, 1, "AD3:");
  // while (1)
  // {
  //   AD0 = AD_GetValue(ADC_Channel_0);
  //   AD1 = AD_GetValue(ADC_Channel_1);
  //   AD2 = AD_GetValue(ADC_Channel_2);
  //   AD3 = AD_GetValue(ADC_Channel_3);
  //   OLED_ShowNum(1, 5, AD0, 4);
  //   OLED_ShowNum(2, 5, AD1, 4);
  //   OLED_ShowNum(3, 5, AD2, 4);
  //   OLED_ShowNum(4, 5, AD3, 4);
  //   Delay_ms(100);
  // }

  // * DMA 数据转运
  OLED_Init();

  OLED_ShowHexNum(1, 1, DataA[0], 2);
  OLED_ShowHexNum(1, 4, DataA[1], 2);
  OLED_ShowHexNum(1, 7, DataA[2], 2);
  OLED_ShowHexNum(1, 10, DataA[3], 2);

  OLED_ShowHexNum(2, 1, DataB[0], 2);
  OLED_ShowHexNum(2, 4, DataB[1], 2);
  OLED_ShowHexNum(2, 7, DataB[2], 2);
  OLED_ShowHexNum(2, 10, DataB[3], 2);

  MyDMA_Init((uint32_t)DataA, (uint32_t)DataB, 4);

  OLED_ShowHexNum(3, 1, DataA[0], 2);
  OLED_ShowHexNum(3, 4, DataA[1], 2);
  OLED_ShowHexNum(3, 7, DataA[2], 2);
  OLED_ShowHexNum(3, 10, DataA[3], 2);

  OLED_ShowHexNum(4, 1, DataB[0], 2);
  OLED_ShowHexNum(4, 4, DataB[1], 2);
  OLED_ShowHexNum(4, 7, DataB[2], 2);
  OLED_ShowHexNum(4, 10, DataB[3], 2);
}

// * 中断函数
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
  {
    // * 清除标志位
    Num++;
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}
