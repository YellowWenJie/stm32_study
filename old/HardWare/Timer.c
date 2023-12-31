#include "stm32f10x.h"

extern int16_t Num;

void Timer_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  // * 使用 APB1 开启时钟函数，因为 TMI2 是 APB1 总线的外设
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  // * GPIO 时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // * 内部时钟，TIM2 的时基单元由内部时钟来驱动
  // TIM_InternalClockConfig(TIM2);

  // * 外部时钟
  TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);

  // * 配置时基单元
  /**
   * * 计数器溢出频率
   * * CK_CNT_OV = CK_CNT / (ARR + 1) = CK_PSC / (PSC + 1) / (ARR + 1)
   * * 定时频率 = 72M  / (PSC + 1) / (ARR + 1)
   */
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  // * ARR 自动重装器的值
  TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;
  // * PSC 预分频器的值
  TIM_TimeBaseInitStructure.TIM_Prescaler = 2 - 1;
  // * 重复计数器的值
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
  TIM_ClearFlag(TIM2, TIM_IT_Update);

  // * 使能中断
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

  // * 配置 NVIC
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);

  // * 启动定时器
  TIM_Cmd(TIM2, ENABLE);
}

uint16_t Timer_GetCounter(void)
{
  return TIM_GetCounter(TIM2);
}

// * 中断函数
// void TIM2_IRQHandler(void)
// {
//   if (TIM_GetITStatus(TIM2, TIM_IT_Update))
//   {
//     // * 清除标志位
//     Num++;
//     TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//   }
// }
