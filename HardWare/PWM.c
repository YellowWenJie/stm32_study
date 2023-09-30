#include "stm32f10x.h"

void PWM_Init(void)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  // * 使用 APB1 开启时钟函数，因为 TMI2 是 APB1 总线的外设
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  // * AFIO 时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // * GPIO 引脚重映射 将定时器的通道 1，从 PA0 移到 PA15
  // * GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
  // *GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

  // * GPIO 时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // * 内部时钟，TIM2 的时基单元由内部时钟来驱动
  TIM_InternalClockConfig(TIM2);

  // * 外部时钟
  // TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);

  // * 配置时基单元
  /**
   * * 计数器溢出频率
   * * CK_CNT_OV = CK_CNT / (ARR + 1) = CK_PSC / (PSC + 1) / (ARR + 1)
   * * 定时频率 = 72M  / (PSC + 1) / (ARR + 1)
   */
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  // * ARR 自动重装器的值
  TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
  // * PSC 预分频器的值
  TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;
  // * 重复计数器的值
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

  // * 输出比较单元
  // * 赋默认初始值
  TIM_OCStructInit(&TIM_OCInitStructure);
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  // * CCR
  TIM_OCInitStructure.TIM_Pulse = 0;

  // TIM_OCInitStructure.TIM_OutputNState = ;
  // TIM_OCInitStructure.TIM_OCNPolarity = ;
  // TIM_OCInitStructure.TIM_OCIdleState = ;
  // TIM_OCInitStructure.TIM_OCNIdleState = ;
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  // * 启动定时器
  TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
  TIM_SetCompare1(TIM2, Compare);
}
