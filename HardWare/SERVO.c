#include "stm32f10x.h"

uint16_t ARR = 20000 - 1;
uint16_t PSC = 72 - 1;
uint16_t Servos_Model = 180;

uint16_t GetServosModel()
{
  return Servos_Model;
}

// * 舵机
void PWM_Servos_Init(uint16_t ServosModel)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  Servos_Model = ServosModel;
  if (ServosModel == 360)
  {
    ARR = 1999;
    PSC = 719;
  };

  // * 使用 APB1 开启时钟函数，因为 TMI2 是 APB1 总线的外设
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  // * AFIO 时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // * GPIO 时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
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
  TIM_TimeBaseInitStructure.TIM_Period = ARR;
  // * PSC 预分频器的值
  TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;
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
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  // * 启动定时器
  TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare2(uint16_t Compare)
{
  TIM_SetCompare2(TIM2, Compare);
}

void Servo_SetAngle(float Angle)
{
  uint16_t pulse;

  if (GetServosModel() == 360)
  {
    // * 360度 舵机

    if (Angle == 0)
    {
      pulse = 152;
    }
    else if (Angle < 0)
    {
      pulse = (-Angle) * 10 + 150;
    }
    else if (Angle > 0)
    {
      pulse = 150 - Angle * 10;
    };
    PWM_SetCompare2(pulse);
  }
  else
  { // * 180度 舵机
    // * 0 500
    // * 180 500
    PWM_SetCompare2(Angle / 180 * 2000 + 500);
  }
}
