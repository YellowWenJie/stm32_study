#include "stm32f10x.h" // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

// 串口1中断服务程序
// 注意,读取USARTx->SR能避免莫名其妙的错误
// u8 USART1_RX_BUF[USART1_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.
// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
// u16 USART1_RX_STA = 0; // 接收状态标记

//* 蓝牙波特率：38400，平常波特率：9600
void Serial_Init()
{

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  //* 复用推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  //* USART1_TX/TIM1_CH2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //* 上拉输入
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  //* USART1_RX/TIM1_CH3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //* USART1 初始化设置
  //* 波特率
  USART_InitStructure.USART_BaudRate = 38400;
  //* 无硬件数据流控制
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  //* 串口模式 发送：USART_Mode_Tx  接收：USART_Mode_Rx
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  //* 无奇偶校验位
  USART_InitStructure.USART_Parity = USART_Parity_No;
  //* 一个停止位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  //* 字长为8位数据格式
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  //* 初始化串口1
  USART_Init(USART1, &USART_InitStructure);
  //* RXNE 一旦制 1 了，就会向 NVIC 申请中断。开启相关中断。开启串口中断
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //* 清除相对应的标志位
  // USART_ClearFlag(USART1, USART_FLAG_TC);

  //* 对优先级分组
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //* 串口1中断通道
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  //* IRQ通道使能
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //* 抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  //* 子优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  //* 根据指定的参数初始化VIC寄存器、
  NVIC_Init(&NVIC_InitStructure);

  //* 使能串口1
  USART_Cmd(USART1, ENABLE);
}

//* 发送数据
void Serial_SendByte(uint8_t Byte)
{
  USART_SendData(USART1, Byte);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    ;
}

//* 接收数据
uint8_t Serial_GetRxData(void)
{
  return Serial_RxData;
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

//* fputc 是 printf 函数的底层，printf 在打印的时候，就是不断的调用 fputc 函数一个个打印。
//* 把 fputc 函数重定向到串口，那 printf 自然就输出到串口了
int fputc(int ch, FILE *f)
{
  Serial_SendByte(ch);
  return ch;
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
// uint8_t Serial_Collect(void)
// {
//   if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
//   {
//     return USART_ReceiveData(USART1);
//   }
//   else
//     return 0;
// }

//* USART1中断函数
// void USART1_IRQHandler(void) // 串口1中断服务程序
// {
//   //* 接收中断
//   if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//   {
//     //* (USART1->DR);	//读取接收到的数据.返回 USARTx 外设最近接收到的数据。
//     Serial_RxData = USART_ReceiveData(USART1);
//     //* 接收未完成
//     if ((USART1_RX_STA & 0x8000) == 0)
//     {
//       //* 接收到了0x0d
//       if (USART1_RX_STA & 0x4000)
//       {
//         if (Serial_RxData != 0x0a)
//           //* 接收错误,重新开始
//           USART1_RX_STA = 0;
//         else
//           //* 接收完成了
//           USART1_RX_STA |= 0x8000;
//       }
//       //* 还没收到0X0D
//       else
//       {
//         if (Serial_RxData == 0x0d)
//           USART1_RX_STA |= 0x4000;
//         else
//         {
//           USART1_RX_BUF[USART1_RX_STA & 0X3FFF] = Serial_RxData;
//           USART1_RX_STA++;
//           if (USART1_RX_STA > (USART1_REC_LEN - 1))
//             //* 接收数据错误,重新开始接收
//             USART1_RX_STA = 0;
//         }
//       }
//     }
//   }
// }