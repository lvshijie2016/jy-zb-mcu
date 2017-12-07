/*****************************************************************************
 *   rtc.h:  Header file for C32F0xx Microprocessors
 *
 *   Copyright(C) 2015, Ltd.
 *   All rights reserved.
 *
 *   History
 *   2015  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/

#ifndef __RTC_H 
#define __RTC_H
#include "c32f0xx.h"
#include "c32f0xx_sys.h"

union RTC_DR_REG 
{
    uint32_t				all;
};
union RTC_MR_REG 
{
    uint32_t				all;
};
union RTC_LR_REG 
{
    uint32_t				all;
};
struct RTC_CR_BITS             // bit description
{                                   
    uint32_t RTCSTART:1; 		//Enables the RTC. 
    uint32_t RSVD:31;           // 31:1 reserved
};
union RTC_CR_REG 
{
    uint32_t			all;
    struct RTC_CR_BITS	bit;
};
struct RTC_ICSC_BITS      // bit description
{                                   
    uint32_t RTCIC:1; 		//Interrupt control register 
    uint32_t RSVD:31;       // 31:1 reserved
};
union RTC_ICSC_REG 
{
    uint32_t				all;
    struct RTC_ICSC_BITS	bit;
};
struct RTC_RIS_BITS       // bit description
{                                   
    uint32_t RTCRIS:1; 		//Raw interrupt event flag register.  
    uint32_t RSVD:31;       // 31:1 reserved
};
union RTC_RIS_REG 
{
    uint32_t				all;
    struct RTC_RIS_BITS	    bit;
};
struct RTC_MIS_BITS       // bit description
{                                   
    uint32_t RTCMIS:1; 		//Masked interrupt register status.  
    uint32_t RSVD:31;       // 31:1 reserved
};
union RTC_MIS_REG 
{
    uint32_t				all;
    struct RTC_MIS_BITS	    bit;
};
struct RTC_ICR_BITS       // bit description
{                                   
    uint32_t RTCICR:1; 		//Raw interrupt event flag clear register.   
    uint32_t RSVD:31;       // 31:1 reserved
};
union RTC_ICR_REG 
{
    uint32_t				all;
    struct RTC_ICR_BITS	    bit;
};

/* ================================================================================ */
/* ================                       RTC                      ================ */
/* ================================================================================ */


/**
  * @brief Real-time clock (RTC)
  */

typedef struct                                                  /*!< RTC Structure                                                         */
{                                    
    __I  union RTC_DR_REG    DR;                                /*!< RTC data register                                                     */
    __IO union RTC_MR_REG    MR;                                /*!< RTC match register                                                    */
    __IO union RTC_LR_REG    LR;                                /*!< RTC load register                                                     */
    __IO union RTC_CR_REG    CR;                                /*!< RTC control register                                                  */
    __IO union RTC_ICSC_REG  ICSC;                              /*!< Interrupt control set/clear register                                  */
    __I  union RTC_RIS_REG   RIS;                               /*!< Raw interrupt status register                                         */
    __I  union RTC_MIS_REG   MIS;                               /*!< Masked interrupt status register                                      */
    __O  union RTC_ICR_REG   ICR;                               /*!< RTC interrupt clear register                                          */
} RTC_TypeDef;

typedef struct 
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t milisecond;
}DateTime_TypeDef;

#define RTC             ((RTC_TypeDef   *) RTC_BASE )

//RTC clk source type
#define HZCLK 		0
#define IRCCLK 		1
#define KHZCLK 		2
#define RTCPCLK 	3

//MACRO
#define RTC_EnableTimerCounter (RTC->CR.bit.RTCSTART=1)


uint32_t RTC_DateTimetoSecond(DateTime_TypeDef *dt);
void RTC_SecondtoDateTime(uint32_t seconds, DateTime_TypeDef *dt);
void RTC_SetDateTime(DateTime_TypeDef *dt);
void RTC_GetDateTime(DateTime_TypeDef *dt);
void RTC_SetAlarmDateTime (DateTime_TypeDef *dt);
void RTC_SetAlarmInTickLater(uint32_t ticks);
void RTC_EnableAlarmInt(void);
void RTC_DisableAlarmInt(void);
void RTC_ClearIntFlag(void);
void RTC_SetLoader(uint32_t val);
uint32_t RTC_GetTimer(void);


#endif /* end __RTC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
