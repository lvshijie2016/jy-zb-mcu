#ifndef __I2C_H
#define __I2C_H	 
#include "HAL_device.h"


void I2CMasterTest(void);				    
void IIC_Init(uint8_t Mode, uint32_t ClockRate, uint32_t SlaveAddress);
void IIC_WriteByte(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t Value);
void IIC_WriteArray(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t *Buff, uint8_t Length);
uint8_t IIC_ReadByte(uint8_t SlaveAddress, uint8_t SubAddr);
void IIC_ReadArray(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t Lenth, uint8_t *Buffer);
void RTC_Write_Byte(unsigned char REG_ADD, unsigned char dat);
 unsigned char RTC_Read_Byte(unsigned char REG_ADD);
#endif
