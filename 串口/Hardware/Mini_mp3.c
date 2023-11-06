#include "stm32f10x.h" // Device header

void MiniMp3_Init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; // 声明一个结构体变量，用来初始化GPIO
  // 使能串口的RCC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能UART3所在GPIOB的时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  // // 串口使用的GPIO口配置
  // //  Configure USART3 Rx (PB.11) as input floating
  // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  // GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure USART3 Tx (PB.10) as alternate function push-pull
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // 配置串口
  // 波特率
  USART_InitStructure.USART_BaudRate = 9600;
  // 字长
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  // 停止位
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  // 校验位
  USART_InitStructure.USART_Parity = USART_Parity_No;
  // 硬件流控制
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  // USART 模式
  USART_InitStructure.USART_Mode = USART_Mode_Tx;

  // Configure USART3
  USART_Init(USART3, &USART_InitStructure); // 配置串口3
  // Enable the USART3
  USART_Cmd(USART3, ENABLE); // 使能串口3
};

void MiniMp3_SendByte(uint8_t Byte)
{
  USART_SendData(USART3, Byte);
  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
    ;
};

void MiniMp3_SendArray(int *Array)
{
  uint16_t i;
  for (i = 0; i < 10; i++)
  {
    MiniMp3_SendByte(Array[i]);
  }
}

// 7E --- 起始命令
// FF --- 版本信息
// 06 --- 数据长度(不包含校验)
// 03 --- 代表命令字节
// 00 --- 是否需要应答[0x01:需要应答，0x00:不需要返回应答]
// 00 --- 曲目的高字节[DH]
// 01 --- 曲目的低字节[DL],这里代表的是第一首歌播放
// FF --- 校验的高字节

// E6 --- 校验的低字节
// EF --- 结束命令

void MiniMp3_Play()
{
  int Arr[10] = {0x7E, 0xFF, 0x06, 0x0D, 0x00, 0x00, 0x00, 0xFE};
  MiniMp3_SendArray(Arr);
}

void MiniMp3_Specify_File_Play()
{
  int Arr[10] = {0x7E, 0xFF, 0x06, 0x0F, 0x00, 0x01, 0x03, 0xFE};
  MiniMp3_SendArray(Arr);
}

void MiniMp3_Stop()
{
  int Arr[10] = {0x7E, 0xFF, 0x06, 0x0E, 0x00, 0x00, 0x00, 0xEF};
  MiniMp3_SendArray(Arr);
}
