#include "stm32f10x.h" // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void Serial_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  //* 复用推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //* 波特率
  USART_InitStructure.USART_BaudRate = 9600;
  //* 硬件流控制
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  //* 串口模式 发送：USART_Mode_Tx  接收：USART_Mode_Rx
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  //* 校验位
  USART_InitStructure.USART_Parity = USART_Parity_No;
  //* 停止位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  //* 字长
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART1, &USART_InitStructure);

  //* RXNE 一旦制 1 了，就会向 NVIC 申请针对
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(USART1, ENABLE);
}

//* 发送数据
void Serial_SendByte(uint8_t Byte)
{
  USART_SendData(USART1, Byte);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    ;
}

//* 发送数组
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
  uint16_t i;
  for (i = 0; i < Length; i++)
  {
    Serial_SendByte(Array[i]);
  }
}

//* 发送字符串
void Serial_SendString(char *String)
{
  uint8_t i;
  for (i = 0; String[i] != '\0'; i++)
  {
    Serial_SendByte(String[i]);
  }
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
  uint32_t Result = 1;
  while (Y--)
  {
    Result *= X;
  }
  return Result;
}

//* 发送一个数字
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
  uint8_t i;
  for (i = 0; i < Length; i++)
  {
    //* 0x30 == '0'
    Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + 0x30);
  }
}

//* fputc 是 printf 函数的底层，printf 在打印的时候，就是不断的调用 fputc 函数一个个打印。
//* 把 fputc 函数重定向到串口，那 printf 自然就输出到串口了
int fputc(int ch, FILE *f)
{
  Serial_SendByte(ch);
  return ch;
}

//* 封装 string
void Serial_Printf(char *format, ...)
{
  char String[100];
  va_list arg;
  va_start(arg, format);
  vsprintf(String, format, arg);
  va_end(arg);
  Serial_SendString(String);
}

//* 收信息
uint8_t Serial_Collect(void)
{
  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
  {
    return USART_ReceiveData(USART1);
  }
  else
    return 0;
}

uint8_t Serial_GetRxFlag(void)
{
  if (Serial_RxFlag == 1)
  {
    Serial_RxFlag = 0;
    return 1;
  }
  return 0;
}

uint8_t Serial_GetRxData(void)
{
  return Serial_RxData;
}

//* 中断函数  RXNE 一旦制 1 了，就会向 NVIC 申请中断
void USART1_IRQHandler(void)
{
  if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
  {
    Serial_RxData = USART_ReceiveData(USART1);
    Serial_RxFlag = 1;
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
}
