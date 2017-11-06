/*****************************************************************************
 *   crc.h:  Header file for C32F0
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
#include "c32f0xx_iap.h"
#include "c32f0xx_sys.h"
/*****************************************************************************
Function Name	FlashErase	
Function Definition	FlashErase(uint32_t vrstart);
Function Description	FlashErase
Input Parameters	
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void IAP_FlashErase(uint32_t vrstart)
{
    //initial IAP
	SYS_SetAHBClkDivider(2);
	while((C_FMC->RESERVED[0]&0x100)==0x100);
    C_FMC->RESERVED[2] = vrstart;
    C_FMC->RESERVED[0] = 0x4;
	while((C_FMC->RESERVED[0]&0x100)==0x100);
	SYS_SetAHBClkDivider(1);
}
/*****************************************************************************
Function Name	FlashProgram
Function Definition	FlashProgram(uint32_t vrstart,uint32_t vfrelease);
Function Description	FlashProgram
Input Parameters	
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void IAP_FlashProgram(uint32_t vrstart,uint32_t vfrelease)
{
    //initial IAP
	SYS_SetAHBClkDivider(2);
	while((C_FMC->RESERVED[0]&0x100)==0x100);
    C_FMC->RESERVED[2] = vrstart;
    C_FMC->RESERVED[1] = vfrelease;
    C_FMC->RESERVED[0] = 0x2;
	while((C_FMC->RESERVED[0]&0x100)==0x100);
	SYS_SetAHBClkDivider(1);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
