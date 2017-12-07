/*****************************************************************************
 *   iap.h:  Header file for C32F0x Family Microprocessors
 *
 *   Copyright(C) 2015, Ltd.
 *   All rights reserved.
 *
 *   History
 *   2015  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "c32f0xx.h"

#ifndef __IAP_H 
#define __IAP_H

#define	SECTORSIZE          0x200             //512 bytes sector size
#define	IAPENTRYVECTOR		0x1FFF00FC

//命令码定义
#define	SECTORERASE			0x11		      //擦除命令
#define	PROGFLASH			0x15		      //RAM写到Flash
#define BSLVER			    0x31		      //读系统信息


//IAP RETURN CODE
#define CMD_SUCC		    0x00
#define INVALID_ADDR	    0x01
#define ERASE_FAILED	    0x02
#define PROG_FAILED		    0x03
#define NOT_BLANK		    0x04
#define INVALID_CMD		    0x05
#define INVALID_PWD		    0x06
#define IRC_NOT_POWERED		0x07

typedef void (*IAP)(uint32_t [],uint32_t[]);
void IAP_FlashErase(uint32_t vrstart);
void IAP_FlashProgram(uint32_t vrstart,uint32_t vfrelease);

#endif /* end __IAP_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
