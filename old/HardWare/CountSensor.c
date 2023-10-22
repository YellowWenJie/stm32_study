#include "stm32f10x.h" // Device header

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  // * GPIOB
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  // * AFIO
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  // * 配置 GPIO  IPU为上拉输入，默认高电平
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // * AFIO 外部引脚选择配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

  // * 配置 EXTI
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_Init(&EXTI_InitStructure);

  // * 配置 NVIC
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  // * 10 ~ 15 都合并到了 EXTI15_10_IRQn 这个通道
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}

uint16_t CountSensor_Get(void)
{
  return CountSensor_Count;
}

void EXTI15_10_IRQHandler(void)
{
  if (EXTI_GetITStatus(EXTI_Line14) == SET)
  {
    /*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
    {
      CountSensor_Count++;
    }
    EXTI_ClearITPendingBit(EXTI_Line14);
  }
}
