#include "stm32f10x.h" // Device header

int main(void)
{
	// GPIC 的
	RCC->APB2ENR = 0x00000010;
	GPIOC->CRH = 0x00300000;
	GPIOC->ODR = 0x00002000;
}
