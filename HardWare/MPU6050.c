#include "stm32f10x.h" // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS 0xD0

//* 指定地址写寄存器
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
  MyI2C_Start();
  //* 起始之后，主机必须首先发送一个字节，内容是从机地址 + 读写位，进行寻址，1101 000 0， 1101 000 为 MPU6050 的从机地址，0 代表即将进行写入。
  MyI2C_SendByte(MPU6050_ADDRESS);
  //* 应答
  MyI2C_ReceiveAck();
  //* 指定寄存器地址
  MyI2C_SendByte(RegAddress);
  MyI2C_ReceiveAck();
  //* 指定写入指定寄存器地址下的数据
  MyI2C_SendByte(Data);
  MyI2C_ReceiveAck();
  MyI2C_Stop();
}

//* 指定地址读寄存器
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
  uint8_t Data;

  MyI2C_Start();
  //* 起始之后，主机必须首先发送一个字节，内容是从机地址 + 读写位，进行寻址，1101 000 0， 1101 000 为 MPU6050 的从机地址，0 代表即将进行写入。
  MyI2C_SendByte(MPU6050_ADDRESS);
  //* 应答
  MyI2C_ReceiveAck();
  //* 指定寄存器地址
  MyI2C_SendByte(RegAddress);
  MyI2C_ReceiveAck();

  //? 指定地址，就是设置了 MOU6050 的当前地址指针，设置完地址之后，转入读的时序，必须重新指定读写位，重新指定读写位，就必须重新起始。
  MyI2C_Start();
  //* 0x01 指定接下来要读从机的数据。
  MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
  //* 从机发送一个字节,主机接收一个字节
  MyI2C_ReceiveAck();
  //* 把接收到的数据存起来
  Data = MyI2C_ReceiveByte();
  //* 主机接收一个字节后要给从机发送一个应答
  MyI2C_SendAck(1);
  MyI2C_Stop();

  return Data;
}

void MPU6050_Init(void)
{
  MyI2C_Init();
  MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
  MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
  MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
  MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
  MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
  MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_GetID(void)
{
  return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
  uint8_t DataH, DataL;

  DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
  DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
  *AccX = (DataH << 8) | DataL;

  DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
  DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
  *AccY = (DataH << 8) | DataL;

  DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
  DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
  *AccZ = (DataH << 8) | DataL;

  DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
  DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
  *GyroX = (DataH << 8) | DataL;

  DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
  DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
  *GyroY = (DataH << 8) | DataL;

  DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
  DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
  *GyroZ = (DataH << 8) | DataL;
}
