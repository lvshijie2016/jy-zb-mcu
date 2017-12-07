/*****************************************************************************
 *   c32f0xx_crc.h:  Header file for C32F0
 *
 *   Copyright(C) 2015, 
 *   All rights reserved.
 *
 *   History
 *   2015.07.21  ver 1.01    Prelimnary version
 *
******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "c32f0xx.h"
#include "c32f0xx_crc.h"
#include "c32f0xx_sys.h"

/*****************************************************************************
Function Name	CRC_CCITT
Function Definition	Int16_t CRC_CCITT(uint8_t * str, uint16_t strlen,uint32_t crcseed);
Function Description	CRC CCITT Calculate
Input Parameters	*str: pointer to string
strlen: length of string
crcseed: seed of CRC
Return Value	CRC calculation result
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t CRC_CCITT(uint8_t * str, uint16_t strlen,uint32_t crcseed)
{
	uint16_t crcvalue;
	//initial CRC
	SYSCON->SYSAHBCLKCTRL.bit.CRCCLK = 1;
	CRC->MODE.bit.CRC_POLY=0;
	CRC->SEED.all = crcseed; 
	CRC->MODE.bit.SEED_SET=1;

	//calculate CRC
	while ( strlen != 0)
	{
		 CRC->DATA.all = *str++;
		 strlen --;
	}
	crcvalue=(uint16_t) CRC->DATA.all;
	
	SYSCON->SYSAHBCLKCTRL.bit.CRCCLK = 0;
	
	return crcvalue;
}
/*****************************************************************************
Function Name	CRC_16
Function Definition	uint16_t CRC_16(uint8_t * str, uint16_t strlen,uint32_t crcseed);
Function Description	CRC 16 Calculate
Input Parameters	*str: pointer to string
strlen: length of string
crcseed: seed of CRC
Return Value	CRC calculation result
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint16_t CRC_16(uint8_t * str, uint16_t strlen,uint32_t crcseed)
{
	uint16_t crcvalue;
	//initial CRC
	SYSCON->SYSAHBCLKCTRL.bit.CRCCLK = 1;
	CRC->MODE.bit.CRC_POLY=1;
	CRC->SEED.all = crcseed; 
	CRC->MODE.bit.SEED_SET=1;

	//calculate CRC
	while ( strlen != 0)
	{
		 CRC->DATA.all = *str++;
		 strlen --;
	}
	crcvalue=(uint16_t) CRC->DATA.all;
	
	SYSCON->SYSAHBCLKCTRL.bit.CRCCLK = 0;
	
	return crcvalue;
}
/*****************************************************************************
Function Name	CRC_32
Function Definition	uint32_t CRC_32(uint8_t * str, uint16_t strlen,uint32_t crcseed);
Function Description	CRC 32 Calculate
Input Parameters	*str: pointer to string
strlen: length of string
crcseed: seed of CRC
Return Value	CRC calculation result
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint32_t CRC_32(uint8_t * str, uint16_t strlen,uint32_t crcseed)
{
	uint32_t crcvalue;
	//initial CRC
	SYSCON->SYSAHBCLKCTRL.bit.CRCCLK = 1;
	CRC->MODE.bit.CRC_POLY=2;
	CRC->SEED.all = crcseed; 
	CRC->MODE.bit.SEED_SET=1;

	//calculate CRC
	while ( strlen != 0)
	{
		 CRC->DATA.all = *str++;
		 strlen --;
	}
	crcvalue=CRC->DATA.all;
	
	SYSCON->SYSAHBCLKCTRL.bit.CRCCLK = 0;
	
	return crcvalue;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
