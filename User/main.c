#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"
int main(void)
{

  // // * 寄存器方式
  // // * GPIC 的时钟
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

  // 流水灯
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

  // 蜂鸣器
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
  OLED_Init();
  CountSensor_Init();

  OLED_ShowString(1, 1, "Count:");

  while (1)
  {
    OLED_ShowNum(1, 7, CountSensor_Get(), 5);
  }
}
