
/****************************************************************************************************//**
 * @file     C32F0xx.h
 *
 * @brief    CMSIS Cortex-M0 Peripheral Access Layer Header File for
 *           C32F0xx from <unknown Vendor>.
 *
 * @version  V1.0
 * @date     16. October 2015
 *
 * @note     Generated with SVDConv V2.75 
 *           from CMSIS SVD File 'C32F0xx.svd' Version 1.0,
 *******************************************************************************************************/



/** @addtogroup (null)
  * @{
  */

/** @addtogroup C32F0xx
  * @{
  */

#ifndef C32F0xx_H
#define C32F0xx_H

#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum 
{
/* -------------------  Cortex-M0 Processor Exceptions Numbers  -------------------                                                                 */
    Reset_IRQn                    = -15,              /*!<   1  Reset Vector, invoked on Power up and warm reset                  */
    NonMaskableInt_IRQn           = -14,              /*!<   2  Non maskable Interrupt, cannot be stopped or preempted         */
    HardFault_IRQn                = -13,              /*!<   3  Hard Fault, all classes of Fault                                                 */
    SVCall_IRQn                   =  -5,              /*!<  11  System Service Call via SVC instruction                                  */
    DebugMonitor_IRQn             =  -4,              /*!<  12  Debug Monitor                                                                      */
    PendSV_IRQn                   =  -2,              /*!<  14  Pendable request for system service                                       */
    SysTick_IRQn                  =  -1,              /*!<  15  System Tick Timer                                                                */
/* ---------------------  C32F0xx Specific Interrupt Numbers  --------------------                                                                     */
    WAKEUP_IRQn                   =   0,              /*!<   0  WAKEUP interrupt                                                                  */
    PMWFAULT_IRQn                 =   1,              /*!<   1  PWM fault interrupt                                                                 */
    IIC_IRQn                      =   2,              /*!<   2  IIC interrupt                                                                          */
    CT16B0_IRQn                   =   3,              /*!<   3  CT16B0 interrupt                                                                    */
    CT16B1_IRQn                   =   4,              /*!<   4  CT16B1 interrupt                                                                    */
    CT16B2_IRQn                   =   5,              /*!<   5  CT16B2 interrupt                                                                    */
    CT16B3_IRQn                   =   6,              /*!<   6  CT16B3 interrupt                                                                    */
    UART0_IRQn                    =   7,              /*!<   7  UART0 interrupt                                                                      */
    UART1_IRQn                    =   8,              /*!<   8  UART1 interrupt                                                                      */
    ADC_IRQn                      =   9,              /*!<   9  ADC interrupt                                                                         */
    WDT_IRQn                      =  10,              /*!<  10  WDT interrupt                                                                        */
    BOD_IRQn                      =  11,              /*!<  11  BOD interrupt                                                                        */
    GPIOA_IRQn                    =  12,              /*!<  12  GPIOA interrupt                                                                     */
    GPIOB_IRQn                    =  13,              /*!<  13  GPIOB interrupt                                                                     */
    GPIOC_IRQn                    =  14,              /*!<  14  GPIOC interrupt                                                                     */
    RTC_IRQn                      =  15,              /*!<  15  RTC interrupt                                                                         */
    SPI_IRQn                      =  16,              /*!<  16  SPI interrupt                                                                          */
    PWMEVENT_IRQn                 =  17,              /*!<  17  PWM Event interrupt                                                               */
} IRQn_Type;


/** @addtogroup Configuration_of_CMSIS
  * @{
  */


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* ----------------Configuration of the Cortex-M0 Processor and Core Peripherals---------------- */
#define __CM0_REV                 0x0000            /*!< Cortex-M0 Core Revision                                               */
#define __MPU_PRESENT                  0            /*!< MPU present or not                                                    */
#define __NVIC_PRIO_BITS               2            /*!< Number of Bits used for Priority Levels                               */
#define __Vendor_SysTickConfig         0            /*!< Set to 1 if different SysTick Config is used                          */
/** @} */ /* End of group Configuration_of_CMSIS */

#include <core_cm0.h>                               /*!< Cortex-M0 processor and core peripherals                              */
#include "system_C32F0xx.h"                         /*!< C32F0xx System                                                                   */


/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */


/** @addtogroup Device_Peripheral_Registers
  * @{
  */


/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined(__CC_ARM)
    #pragma push
    #pragma anon_unions
#elif defined(__ICCARM__)
    #pragma language=extended
#elif defined(__GNUC__)
    /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
    /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
    #pragma warning 586
#else
    #warning Not supported compiler type
#endif



/* ================================================================================ */
/* ================                       DIA                      ================ */
/* ================================================================================ */


/**
  * @brief Configure information block (CIB)
  */

typedef struct 
{                                    /*!< CIB Structure                                                         */
    __I  uint32_t  IRCTRIM;                           /*!< Internal IRC trim bits                                                */
    __I  uint32_t  UNIQUEID1;                         /*!< Unique device serial no- high 32-bit                                  */
    __I  uint32_t  UNIQUEID0;                         /*!< Unique device serial no- low 32-bit                                   */
    __I  uint32_t  VERID;                             /*!< Hardware version                                                      */
    __I  uint32_t  DID;                               /*!< Device ID                                                             */
    __I  uint32_t  RESERVED1;
    __I  uint32_t  FUNCDESC;                          /*!< Device function description                                           */
    __I  uint32_t  RESERVED0;
} CIB_Type;


/* ================================================================================ */
/* ================                       IIC                      ================ */
/* ================================================================================ */


/**
  * @brief two wires serial communication interface (IIC)
  */
typedef struct                                        /*!< IIC Structure                                                              */
{                                    
    __IO uint32_t  CONSET;                            /*!< IIC control set bit register                                             */
    __I  uint32_t  STAT;                              /*!< IIC state register                                                         */
    __IO uint32_t  DAT;                               /*!< IIC data register                                                          */
    __IO uint32_t  ADR0;                              /*!< IIC Slave Address Register 0                                         */
    __IO uint32_t  SCLH;                              /*!< Duty Cycle Register High Half Word                                */
    __IO uint32_t  SCLL;                              /*!< Duty Cycle Register Low Half Word                                 */
    __O  uint32_t  CONCLR;                            /*!< IIC Control Clear Register                                             */
    __I  uint32_t  RESERVED0;
    __IO uint32_t  ADR1;                              /*!< IIC Slave Address Register 1                                          */
    __IO uint32_t  ADR2;                              /*!< IIC slave address register2                                            */
    __IO uint32_t  ADR3;                              /*!< IIC slave address register3                                            */
    __I  uint32_t  DATA_BUFFER;                       /*!< IIC data buffer register                                                  */
    __IO uint32_t  MASK0;                             /*!< IIC address mask register0                                            */
    __IO uint32_t  MASK1;                             /*!< IIC address mask register1                                            */
    __IO uint32_t  MASK2;                             /*!< IIC address mask register2                                            */
    __IO uint32_t  MASK3;                             /*!< IIC address mask register3                                            */
} IIC_Type;


/* ================================================================================ */
/* ================                       WDT                      ================ */
/* ================================================================================ */


/**
  * @brief Watch dog (WDT)
  */

typedef struct                                      /*!< WDT Structure                                                                      */
{                                    
    __IO uint32_t  MOD;                               /*!< WDT mode register                                                                */
    __IO uint32_t  TC;                                /*!< Watchdog timer constant register                                            */
    __O  uint32_t  FEED;                              /*!< Watchdog feed sequence register                                            */  
    __I  uint32_t  TV;                                /*!< Watchdog timer value register                                                 */
    __I  uint32_t  RESERVED0;
    __IO uint32_t  WARNINT;                           /*!< Watchdog Warning Interrupt compares value                            */
    __IO uint32_t  WINDOW;                            /*!< Watchdog Window compares value                                           */
} WDT_Type;


/* ================================================================================ */
/* ================                      UART0                     ================ */
/* ================================================================================ */


/**
  * @brief Universal asynchronous receiver/transmitter (UART0)
  */

typedef struct /*!< UART0 Structure                                                       */
{                                    
    union 
    {
        __O  uint32_t  TDR;                           /*!< Transmit Data Register                                                */
        __I  uint32_t  RDR;                           /*!< Receiver Data Register                                                */
    } ;
    __IO uint32_t  STATE;                             /*!< State of current RX and TX FIFO                                   */
    __IO uint32_t  CTRL;                              /*!< Control UART interrupt enable and disable                     */
    __IO uint32_t  INTSTATUS;                         /*!< UART RX/TX interrupt status                                        */
    __IO uint32_t  BAUDDIV;                           /*!< UART Baud rate divider                                                */
    __O  uint32_t  FIFOCLR;                           /*!< Clear TX and RX FIFO                                                  */
} UART0_Type;


/* ================================================================================ */
/* ================                     CT16B0                     ================ */
/* ================================================================================ */


/**
  * @brief 16 bits timer (CT16B0)
  */
typedef struct                                        /*!< CT16B0 Structure                                                   */
{                                    
    __IO uint32_t  IR;                                /*!< Interrupt register                                                    */
    __IO uint32_t  TCR;                               /*!< Timer control register                                              */
    __IO uint32_t  TC;                                /*!< Timer counter register                                             */
    __IO uint32_t  PR;                                /*!< Prescale register                                                     */
    __IO uint32_t  PC;                                /*!< Prescale Counter register                                         */
    __IO uint32_t  MCR;                               /*!< Match Control Register                                             */
    __IO uint32_t  MR0;                               /*!< Match register0                                                       */
} CT16B0_Type;


/* ================================================================================ */
/* ================                     CT16B2                     ================ */
/* ================================================================================ */


/**
  * @brief 16 bits timer (CT16B2)
  */
typedef struct                                        /*!< CT16B2 Structure                                                    */
{                                   
    __IO uint32_t  IR;                                /*!< Interrupt register                                                    */
    __IO uint32_t  TCR;                               /*!< Timer control register                                              */
    __IO uint32_t  TC;                                /*!< Timer counter register                                             */
    __IO uint32_t  PR;                                /*!< Prescale register                                                     */
    __IO uint32_t  PC;                                /*!< Prescale Counter register                                        */
    __IO uint32_t  MCR;                               /*!< Match Control Register                                             */
    __IO uint32_t  MR0;                               /*!< Match register0                                                        */
    __IO uint32_t  MR1;                               /*!< Match register1                                                        */
    __IO uint32_t  MR2;                               /*!< Match register2                                                        */
    __IO uint32_t  MR3;                               /*!< Match register3                                                        */
    __IO uint32_t  CCR;                               /*!< Capture Control Register                                          */
    __I  uint32_t  CR0;                               /*!< Capture register0                                                     */
    __I  uint32_t  CR1;                               /*!< Capture register1                                                     */
    __I  uint32_t  CR2;                               /*!< Capture register2                                                     */
    __I  uint32_t  CR3;                               /*!< Capture register3                                                     */
    __IO uint32_t  EMR;                               /*!< External match register                                             */
    __I  uint32_t  RESERVED0[12];
    __IO uint32_t  CTCR;                              /*!< Counter control register                                             */
} CT16B2_Type;


/* ================================================================================ */
/* ================                       ADC                      ================ */
/* ================================================================================ */


/**
  * @brief Analog-to-Digital Converter (ADC)
  */

typedef struct                                        /*!< ADC Structure                                                              */
{                                    
    __IO uint32_t  CR;                                /*!< ADC control register                                                      */
    __IO uint32_t  GDR;                               /*!< ADC Global Data Register                                               */
    __IO uint32_t  CHSEL;                             /*!< Channel select control register                                         */
    __IO uint32_t  INTEN;                             /*!< ADC Interrupt Enable Register                                         */
    __IO uint32_t  DR0;                               /*!< A/D Channel 0 Data Register                                           */
    __IO uint32_t  DR1;                               /*!< A/D Channel 1 Data Register                                           */
    __IO uint32_t  DR2;                               /*!< A/D Channel 2 Data Register                                           */
    __IO uint32_t  DR3;                               /*!< A/D Channel 3 Data Register                                           */
    __IO uint32_t  DR4;                               /*!< A/D Channel 4 Data Register                                           */
    __IO uint32_t  DR5;                               /*!< A/D Channel 5 Data Register                                           */
    __IO uint32_t  DR6;                               /*!< A/D Channel 6 Data Register                                           */
    __IO uint32_t  DR7;                               /*!< A/D Channel 7 Data Register                                           */
    __IO uint32_t  STAT;                              /*!< ADC Status Register                                                        */
    __IO uint32_t  HILMT;                             /*!< ADC High Limit Control Register                                       */
    __IO uint32_t  LOLMT;                             /*!< ADC Low Limit Control Register                                        */
    __I  uint32_t  RESERVED0;
    __IO uint32_t  SSCR;                              /*!< ADC software trigger convert register                               */
} ADC_Type;


/* ================================================================================ */
/* ================                       PMU                      ================ */
/* ================================================================================ */


/**
  * @brief Power management unit (PMU)
  */

typedef struct                                        /*!< PMU Structure                                                           */
{                                    
    __IO uint32_t  PCON;                              /*!< Power control register                                                */
} PMU_Type;


/* ================================================================================ */
/* ================                       SPI                      ================ */
/* ================================================================================ */


/**
  * @brief Serial peripheral interface (SPI)
  */

typedef struct                                        /*!< SPI Structure                                                          */
{                                    
    __IO uint32_t  CR0;                               /*!< SPI control register0                                                */
    __IO uint32_t  CR1;                               /*!< SPI control register1                                                */
    __IO uint32_t  DR;                                /*!< SPI data register                                                     */
    __I  uint32_t  SR;                                /*!< SPI status register                                                   */
    __IO uint32_t  CPSR;                              /*!< SPI Clock Prescale Register                                      */
    __IO uint32_t  IMSC;                              /*!< Interrupt Mask Set and Clear Register                       */
    __I  uint32_t  RIS;                               /*!< Raw Interrupt Status Register                                   */
    __I  uint32_t  MIS;                               /*!< Masked Interrupt Status Register                               */
    __O  uint32_t  ICR;                               /*!< SPI Interrupt Clear Register                                      */
} SPI_Type;


/* ================================================================================ */
/* ================                      IOCON                     ================ */
/* ================================================================================ */


/**
  * @brief IO config (IOCON)
  */

typedef struct                                        /*!< IOCON Structure                                                     */
{                                    
    __IO uint32_t  PIOA_0;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_1;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_2;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_3;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_4;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_5;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_6;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_7;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_8;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_9;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_10;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_11;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_12;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_13;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_14;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOA_15;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_0;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_1;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_2;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_3;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_4;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_5;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_6;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_7;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_8;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_9;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_10;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_11;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_12;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_13;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_14;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOB_15;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_0;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_1;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_2;                            /*!< Pin config register                                                   */	
    __IO uint32_t  PIOC_3;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_4;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_5;                            /*!< Pin config register                                                   */	
    __IO uint32_t  PIOC_6;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_7;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_8;                            /*!< Pin config register                                                   */	
    __IO uint32_t  PIOC_9;                            /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_10;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_11;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_12;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_13;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_14;                           /*!< Pin config register                                                   */
    __IO uint32_t  PIOC_15;                           /*!< Pin config register                                                   */

} IOCON_Type;


/* ================================================================================ */
/* ================                     SYSCON                     ================ */
/* ================================================================================ */


/**
  * @brief System control register (SYSCON)
  */

typedef struct                                        /*!< SYSCON Structure                                                           */
{                                    
    __IO uint32_t  SYSMEMREMAP;                       /*!< System memory remap register                                        */
    __IO uint32_t  PRESETCTRL;                        /*!< Peripheral reset control register                                         */
    __I  uint32_t  RESERVED0[7];
    __IO uint32_t  WDTOSCCTRL;                        /*!< WDT oscillator control register                                           */
    __IO uint32_t  IRCCTRL;                           /*!< Internal RC oscillator control register                                  */
    __I  uint32_t  RESERVED1;
    __IO uint32_t  SYSRESSTAT;                        /*!< System reset status register                                               */
    __I  uint32_t  RESERVED2[15];
    __IO uint32_t  MAINCLKSEL;                        /*!< Main clock source select register                                         */
    __IO uint32_t  MAINCLKUEN;                        /*!< Main clock source update enable register                             */
    __IO uint32_t  SYSAHBCLKDIV;                      /*!< System AHB clock divider register                                       */
    __I  uint32_t  RESERVED3;
    __IO uint32_t  SYSAHBCLKCTRL;                     /*!< AHB clock control register                                                   */
    __I  uint32_t  RESERVED4[5];
    __IO uint32_t  UART0CLKDIV;                       /*!< UART0 clock divider register                                               */
    __IO uint32_t  UART1CLKDIV;                       /*!< UART1 clock divider register                                               */
    __I  uint32_t  RESERVED5[16];
    __IO uint32_t  CLKOUTCLKSEL;                      /*!< Clockout clock source select register                                   */
    __IO uint32_t  CLKOUTUEN;                         /*!< Clockout clock source update enable register                       */
    __IO uint32_t  CLKOUTDIV;                         /*!< Clockout clock divider register                                            */
    __I  uint32_t  RESERVED6[24];
    __IO uint32_t  IOCONFIGCLKDIV0;                   /*!< IO input filter0 register                                                       */
    __IO uint32_t  BODCTRL;                           /*!< BOD power down control register                                        */
    __I  uint32_t  RESERVED7[8];
    __IO uint32_t  INTNMI;                            /*!< NMI interrupt source configuration control                            */
    __I  uint32_t  RESERVED8[34];
    __IO uint32_t  DSWAKECTL;                         /*!< Deep sleep wake up control register                                    */
    __IO uint32_t  DSWAKEEN;                          /*!< Deep sleep wake up signal enable control register                 */
    __O  uint32_t  DSWAKECLR;                         /*!< Deep sleep wake up signal reset register                              */
    __I  uint32_t  DSWAKE;                            /*!< Deep sleep wake up signal status register                             */
    __I  uint32_t  RESERVED9[8];
    __IO uint32_t  PDSLEEPCFG;                        /*!< Power-down states in Deep-sleep mode                               */
    __IO uint32_t  PDAWAKECFG;                        /*!< Power-down states after wake-up from Deep-sleep mode     */
    __IO uint32_t  PDRUNCFG;                          /*!< Power-down configuration register                                       */
} SYSCON_Type;


/* ================================================================================ */
/* ================                       PWM                      ================ */
/* ================================================================================ */


/**
  * @brief Pulse-Width Modulation (PWM)
  */

typedef struct                                        /*!< PWM Structure                                                        */
{                                    
    __IO uint32_t  CTRL;                              /*!< Control Register                                                      */
    __IO uint32_t  FCTRL;                             /*!< Fault Control Register                                              */
    __IO uint32_t  FLTACK;                            /*!< Fault Status/Acknowledge Register                           */
    __IO uint32_t  OUTCTRL;                           /*!< Output Control Register                                           */
    __I  uint32_t  CNTR;                              /*!< Counter Register                                                     */
    __IO uint32_t  CMOD;                              /*!< Counter Modulo Register                                          */
    __IO uint32_t  VAL0;                              /*!< Value Register 0                                                      */
    __IO uint32_t  VAL1;                              /*!< Value Register 1                                                      */
    __IO uint32_t  VAL2;                              /*!< Value Register 2                                                      */
    __IO uint32_t  VAL3;                              /*!< Value Register 3                                                      */
    __IO uint32_t  VAL4;                              /*!< Value Register 4                                                      */
    __IO uint32_t  VAL5;                              /*!< Value Register 5                                                      */
    __I  uint32_t  RESERVED0[2];
    __IO uint32_t  DTIM0;                             /*!< Deadtime Register 0                                                 */
    __IO uint32_t  DTIM1;                             /*!< Deadtime Register 1                                                 */
    __IO uint32_t  DMAP0;                             /*!< Disable Mapping Register0                                         */
    __IO uint32_t  DMAP1;                             /*!< Disable Mapping Register1                                         */
    __IO uint32_t  CNFG;                              /*!< Configure Register                                                    */
    __IO uint32_t  CCTRL;                             /*!< Channel Control Register                                           */
    __IO uint32_t  PORT;                              /*!< Port Register                                                            */
    __IO uint32_t  ICCTRL;                            /*!< Internal Correction Control Register                            */
    __I  uint32_t  RESERVED1[2];
    __IO uint32_t  PSCR;                              /*!< Polarity Invert control Register                                   */
    __O  uint32_t  CNTRINI;                           /*!< Counter Init Register                                                 */
} PWM_Type;


/* ================================================================================ */
/* ================                       RTC                      ================ */
/* ================================================================================ */


/**
  * @brief Real-time clock (RTC)
  */

typedef struct                                        /*!< RTC Structure                                                          */
{                                    
    __I  uint32_t  DR;                                /*!< RTC data register                                                     */
    __IO uint32_t  MR;                                /*!< RTC match register                                                  */
    __IO uint32_t  LR;                                /*!< RTC load register                                                     */
    __IO uint32_t  CR;                                /*!< RTC control register                                                  */
    __IO uint32_t  ICSC;                              /*!< Interrupt control set/clear register                             */
    __I  uint32_t  RIS;                               /*!< Raw interrupt status register                                      */
    __I  uint32_t  MIS;                               /*!< Masked interrupt status register                                 */
    __O  uint32_t  ICR;                               /*!< RTC interrupt clear register                                        */
} RTC_Type;



/* ================================================================================ */
/* ================                      GPIO                      ================ */
/* ================================================================================ */


/**
  * @brief General-purpose I/Os (GPIO)
  */

typedef struct                                        /*!< GPIO Structure                                                            */
{                                    
    __IO uint32_t  MSK;                               /*!< Pin value mask register                                                */
    __I  uint32_t  PIN;                               /*!< Pin state register                                                          */
    __IO uint32_t  OUT;                               /*!< Pin output value register                                               */
    __O  uint32_t  SET;                               /*!< Pin output value set register                                          */
    __O  uint32_t  CLR;                               /*!< Pin output value clear register                                       */
    __O  uint32_t  NOT;                               /*!< Pin output value invert register                                      */
    __I  uint32_t  RESERVED0[2];
    __IO uint32_t  DIR;                               /*!< Data direction register                                                  */
    __IO uint32_t  IS;                                /*!< interrupt sense register                                                 */
    __IO uint32_t  IBE;                               /*!< Interrupt both edges register                                         */
    __IO uint32_t  IEV;                               /*!< Interrupt event register                                                 */
    __IO uint32_t  IE;                                /*!< Interrupt mask register                                                 */
    __I  uint32_t  RIS;                               /*!< Raw interrupt status register                                          */
    __I  uint32_t  MIS;                               /*!< Masked interrupt status register                                      */
    __O  uint32_t  IC;                                /*!< Interrupt clear register                                                  */
} GPIO_Type;


/* ================================================================================ */
/* ================                       FMC                      ================ */
/* ================================================================================ */


/**
  * @brief Flash control block (FMC)
  */

typedef struct                                        /*!< FMC Structure                                                           */
{                                    
    __IO uint32_t  RESERVED[3];
    __IO uint32_t  FLASH_RDCYC;                       /*!< Flash read cycle register                                             */
} FMC_Type;


/* ================================================================================ */
/* ================                       CRC                      ================ */
/* ================================================================================ */


/**
  * @brief Micro digital signal processor (CRC)
  */

typedef struct                                        /*!< CRC Structure                                                         */
{                                    
    __IO uint32_t  MODE;                              /*!< CRC mode register                                                   */
    __IO uint32_t  SEED;                              /*!< CRC seed register                                                    */
    __IO uint32_t  SUM;                               /*!< CRC checksum and data register                                */
} CRC_Type;


/* --------------------  End of section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
    #pragma pop
#elif defined(__ICCARM__)
    /* leave anonymous unions enabled */
#elif defined(__GNUC__)
    /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
    /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
    #pragma warning restore
#else
    #warning Not supported compiler type
#endif




/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */

#define CIB_BASE                        0x1FFF07e0UL
#define IIC_BASE                        0x40000000UL
#define WDT_BASE                        0x40004000UL
#define UART0_BASE                      0x40008000UL
#define UART1_BASE                      0x4000C000UL
#define CT16B0_BASE                     0x40010000UL
#define CT16B1_BASE                     0x40014000UL
#define CT16B2_BASE                     0x40018000UL
#define CT16B3_BASE                     0x4001C000UL
#define ADC_BASE                        0x40020000UL
#define PMU_BASE                        0x40038000UL
#define SPI_BASE                        0x40040000UL
#define IOCON_BASE                      0x40044000UL
#define SYSCON_BASE                     0x40048000UL
#define PWM_BASE                        0x4004C000UL
#define RTC_BASE                        0x40050000UL
#define GPIOA_BASE                      0x50000000UL
#define GPIOB_BASE                      0x50010000UL
#define GPIOC_BASE                      0x50020000UL
#define FMC_BASE                        0x50060000UL
#define CRC_BASE                        0x50070000UL


/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define C_CIB       ((CIB_Type      *)  CIB_BASE)
#define C_IIC       ((IIC_Type      *)  IIC_BASE)
#define C_WDT       ((WDT_Type      *)  WDT_BASE)
#define C_UART0     ((UART0_Type    *)  UART0_BASE)
#define C_UART1     ((UART0_Type    *)  UART1_BASE)
#define C_CT16B0    ((CT16B0_Type   *)  CT16B0_BASE)
#define C_CT16B1    ((CT16B0_Type   *)  CT16B1_BASE)
#define C_CT16B2    ((CT16B2_Type   *)  CT16B2_BASE)
#define C_CT16B3    ((CT16B2_Type   *)  CT16B3_BASE)
#define C_ADC       ((ADC_Type      *)  ADC_BASE)
#define C_PMU       ((PMU_Type      *)  PMU_BASE)
#define C_SPI       ((SPI_Type      *)  SPI_BASE)
#define C_IOCON     ((IOCON_Type    *)  IOCON_BASE)
#define C_SYSCON    ((SYSCON_Type   *)  SYSCON_BASE)
#define C_PWM       ((PWM_Type      *)  PWM_BASE)
#define C_RTC       ((RTC_Type      *)  RTC_BASE)
#define C_GPIOA     ((GPIO_Type     *)  GPIOA_BASE)
#define C_GPIOB     ((GPIO_Type     *)  GPIOB_BASE)
#define C_GPIOC     ((GPIO_Type     *)  GPIOC_BASE)
#define C_FMC       ((FMC_Type      *)  FMC_BASE)
#define C_CRC       ((CRC_Type      *)  CRC_BASE)



/** @} */ /* End of group Device_Peripheral_Registers */
/** @} */ /* End of group C32F0xx */
/** @} */ /* End of group (null) */

#ifdef __cplusplus
}
#endif


#endif  /* C32F0xx_H */

