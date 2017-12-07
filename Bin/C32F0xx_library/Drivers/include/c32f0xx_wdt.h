/*****************************************************************************
 *   wdt.h:  Header file for C32F0xx
 *
 *   Copyright(C) 2015,
 *   All rights reserved.
 *
 *   History
 *   2015.01.21  ver 1.0    Prelimnary version
 *
******************************************************************************/

#ifndef __WDT_H 
#define __WDT_H
#include "c32f0xx.h"
//--WDT Registers BITS Field---------------------------------------------------------------------------------------
struct WDT_MOD_MODE_BITS 
{								// bit description
    uint32_t WDEN:1;			//Watchdog enable bit. 
    uint32_t WDRESET:1;			//Watchdog reset enable bit. 
    uint32_t WDTOF:1;			//Watchdog time-out flag. 
    uint32_t WDINT:1;			//The Watchdog interrupt flag 
    uint32_t WDPROTECT:1;		//Watchdog update mode. 
    uint32_t WDLOCKCLK:1;		//Watchdog clock lock bit. 
    uint32_t WDLOCKDP:1;		//Power-down disable bit. 
    uint32_t WDLOCKEN:1;		//Watchdog enables and reset lockout bit. 
    uint32_t RSVD:24;           //31:8 reserved
};

union WDT_MOD_REG 
{
    uint32_t				    all;
    struct WDT_MOD_MODE_BITS	bit;
};
struct WDT_TC_MODE_BITS       // bit description
{                                   
    uint32_t WDTC:24;			//Watchdog time-out interval.  
    uint32_t RSVD:8;            // 31:24 reserved
};

union WDT_TC_REG 
{
    uint32_t				all;
    struct WDT_TC_MODE_BITS	bit;
};
struct WDT_FEED_MODE_BITS    // bit description
{                                   
    uint32_t WDFEED:8;			//Feed value should be 0xAA followed by 0x55.   
    uint32_t RSVD:24;           // 31:8 reserved
};

union WDT_FEED_REG 
{
    uint32_t				    all;
    struct WDT_FEED_MODE_BITS	bit;
};
struct WDT_TV_MODE_BITS       // bit description
{                                   
    uint32_t WDTV:24;			//Counter timer value.   
    uint32_t RSVD:8;            // 31:24 reserved
};

union WDT_TV_REG 
{
    uint32_t				all;
    struct WDT_TV_MODE_BITS	bit;
};
struct WDT_WARNINT_MODE_BITS    // bit description
{                                   
    uint32_t WARNINT:10;		   //Watchdog warning interrupt compare value.    
    uint32_t RSVD:22;              // 31:10 reserved
};

union WDT_WARNINT_REG 
{
    uint32_t				        all;
    struct WDT_WARNINT_MODE_BITS	bit;
};
struct WDT_WINDOW_MODE_BITS      // bit description
{                                   
    uint32_t WDWINDOW:24;			//Watchdog window value.     
    uint32_t RSVD:8;                // 31:24 reserved
};

union WDT_WINDOW_REG 
{
    uint32_t				    all;
    struct WDT_WINDOW_MODE_BITS	bit;
};
/* ================================================================================ */
/* ================                       WDT                      ================ */
/* ================================================================================ */


/**
  * @brief Watch dog (WDT)
  */

typedef struct                                      /*!< WDT Structure                                                         */
{                                    
    __IO union WDT_MOD_REG      MOD;                /*!< WDT mode register                                                   */
    __IO union WDT_TC_REG       TC;                 /*!< Watchdog timer constant register                               */
    __O  union WDT_FEED_REG     FEED;               /*!< Watchdog feed sequence register                               */
    __I  union WDT_TV_REG       TV;                 /*!< Watchdog timer value register                                   */
    __I  uint32_t               RSVD;
    __IO union WDT_WARNINT_REG  WARNINT;            /*!< Watchdog Warning Interrupt compares value              */
    __IO union WDT_WINDOW_REG   WINDOW;             /*!< Watchdog Window compares value                             */
} WDT_TypeDef;

#define WDT               ((WDT_TypeDef *) WDT_BASE)

#define WDT_Enable        {uint16_t count = 500;(WDT->MOD.bit.WDEN  = 1);while(count--);}      //open wdt
#define WDT_Enable_RESET  (WDT->MOD.bit.WDRESET = 0)      //enable wdt reset
#define WDT_Disable_RESET  (WDT->MOD.bit.WDRESET = 1)
#define WDT_Disable       (WDT->MOD.bit.WDEN    = 0)      //close wdt

void WDT_Init (void);
void WDT_Deinit(void);
void WDT_SetWindowValue (uint32_t intticks,uint32_t windowticks);
void WDT_Feed (void);
void WDT_SetIntervalinTick(uint32_t ticks);
void WDT_SetIntervalinMiliSec(uint32_t ms);
void WDT_EnableResetSystem(void);
void WDT_ClearIntFlag(void);
void WDT_GetIntStatus (void);
void WDT_EnableLock (void);


#endif /* end __TOUCH_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
