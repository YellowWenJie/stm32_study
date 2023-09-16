#include "stm32f10x.h" // Device header

int main(void)
{
  // GPIC 的时钟
  RCC->APB2ENR = 0x00000010;
  // PC13口的模式
  GPIOC->CRH = 0x00300000;
  // 端口输出寄存器
  GPIOC->ODR = 0x00002000;

  while (1)
  {
  }
}
