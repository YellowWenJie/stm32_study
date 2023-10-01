#include "stm32f10x.h"
uint16_t MyDMA_Size;
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
  DMA_InitTypeDef DMA_InitStructure;
  MyDMA_Size = Size;
  /**
   *! DMA 转运有三个条件
   ** 1. 传输计数器大于 0
   ** 2. 触发源有触发信号
   ** 3. DMA 使能
   */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  //* 外设站点
  DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  //* 存储器站点
  DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  //* 传输方向
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  //! 如果传一个大于 0 的 size 的话，第一个条件满足
  DMA_InitStructure.DMA_BufferSize = Size;
  //! 触发源为软件触发，一直都有触发信号，第二个条件满足
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  //! 使能 DMA，第三个条件满足，DMA进行数据转运
  DMA_Cmd(DMA1_Channel1, DISABLE);
}

//* 调用一次再次启动 DMA 转运
void MyDMA_Transfer()
{
  //* 重新给传输计数器赋值，必须要先给 DMA 失能
  DMA_Cmd(DMA1_Channel1, DISABLE);
  DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  while (DMA_GetCurrDataCounter(DMA1_FLAG_TC1) == RESET)
    ;
  //* 清除标志位
  DMA_ClearFlag(DMA1_FLAG_TC1)
}
