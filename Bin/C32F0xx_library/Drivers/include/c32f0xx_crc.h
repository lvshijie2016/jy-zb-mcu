/*****************************************************************************
 *   c32f0xx_crc.h:  Header file for C32F0xx Family Microprocessors
 *
 *   Copyright(C) 2011, Ltd.
 *   All rights reserved.
 *
 *   History
 *   2015 8 21  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
/*****************************************************************************
CRC-CCITT set-up
		Polynomial = x16 + x12 + x5 + 1
		Seed Value = 0xFFFF
		Bit order reverse for data input: NO
		1's complement for data input: NO
		Bit order reverse for CRC sum: NO
		1's complement for CRC sum: NO
		CRC_MODE = 0x0000 0000
		CRC_SEED = 0x0000 FFFF
CRC-16 set-up
		Polynomial = x16 + x15 + x2 + 1
		Seed Value = 0x0000
		Bit order reverse for data input: YES
		1's complement for data input: NO
		Bit order reverse for CRC sum: YES
		1's complement for CRC sum: NO
		CRC_MODE = 0x0000 0015
		CRC_SEED = 0x0000 0000
CRC-32 set-up
		Polynomial = x32+ x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
		Seed Value = 0xFFFF FFFF
		Bit order reverse for data input: YES
		1's complement for data input: NO
		Bit order reverse for CRC sum: YES
		1's complement for CRC sum: YES
		CRC_MODE = 0x0000 0036
		CRC_SEED = 0xFFFF FFFF
*****************************************************************************/
#ifndef __CRC_H 
#define __CRC_H
#include "c32f0xx.h"
#include "c32f0xx_sys.h"

//--CRC Registers BITS Field---------------------------------------------------------------------------------------
struct CRC_MODE_BITS                      // bit description
{									
    uint32_t CRC_POLY:2;                    // 1:0 CRC polynomial type
    uint32_t BIT_RVS_WR:1;                  // 2 data bit order
    uint32_t CMPL_WR:1;                     // 3 data complement
    uint32_t BIT_RVS_WUM:1;                 // 4 CRC sum bit order
    uint32_t CMPL_SUM:1;                    // 5 CRC sum complement
    uint32_t SEED_OP:1;                     // 6 CRC seed option set
    uint32_t SEED_SET:1;                    // 7 write 1 to load seed to CRC generator
    uint32_t RSVD0:24;                      // 31:8 reserved
};
union CRC_MODE_REG 
{
    uint32_t				all;
    struct CRC_MODE_BITS	bit;
};

union CRC_SEED_REG 
{
    uint32_t				all;
};

union CRC_SUM_REG 
{
    uint32_t				all;
};

union CRC_DATA_REG 
{
    uint32_t				all;
};

/*------------- CRC Engine (CRC) ----------------------------*/
/** @addtogroup CRC CRC Engine 
  @{
*/
typedef struct
{
    __IO	union CRC_MODE_REG		MODE;
    __IO	union CRC_SEED_REG		SEED;
    //	union {
    //		__I	union CRC_SUM_REG		SUM; 
    __IO union CRC_DATA_REG	        DATA;
    //	};
} CRC_TypeDef;
/*@}*/ /* end of group CRC */


#define CRC          ((CRC_TypeDef    *) CRC_BASE)
#define ENABLECRCCLK (SYSCON->SYSAHBCLKCTRL.bit.CRCCLK = 1)

uint16_t CRC_CCITT(uint8_t * str, uint16_t strlen,uint32_t crcseed);
uint16_t CRC_16(uint8_t * str, uint16_t strlen,uint32_t crcseed);
uint32_t CRC_32(uint8_t * str, uint16_t strlen,uint32_t crcseed);

#endif /* end __CRC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
