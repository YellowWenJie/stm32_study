#include "stm32f10x.h" // Device header

#ifndef __MiniMp3_H
void MiniMp3_Init(void);
void MiniMp3_SendByte(uint8_t Byte);
void MiniMp3_SendArray(int *Array);

void MiniMp3_Play();
void MiniMp3_Specify_File_Play();
void MiniMp3_Stop();

#endif