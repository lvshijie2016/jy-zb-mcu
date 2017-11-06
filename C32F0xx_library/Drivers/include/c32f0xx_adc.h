/*****************************************************************************
 *   c32f0xx_adc.h:  Header file for c32f0xx Family Microprocessors
 *
 *   Copyright(C) 2015, tech. Ltd.
 *   All rights reserved.
 *
 *   History
 *   2015  ver 2.00    Prelimnary version, first Release
 *
******************************************************************************/

#ifndef __ADC_H 
#define __ADC_H

#include "c32f0xx.h"

//--ADC Registers BITS Field---------------------------------------------------------------------------------------
struct ADC_CR_BITS	
{
    uint32_t CNVEN:8;	//		Enable ADC conversion value to DR0~DR7
    uint32_t CLKDIV:8;	// 		The APB clock (PCLK) divider 
    uint32_t BURST:1;	// 		Burst mode control. 
    uint32_t RSVD0:7;	// 
    uint32_t START:4;	// 		Conversion starts control.
    uint32_t EDGE:1;	// 		Edge control. This bit is significant only when the START field contains 010-111. 
    uint32_t SCMODE:1;	//		ADC converter sample clock selection, must be 1
    uint32_t RSVD1:2;	// 
};
union ADC_CR_REG 
{
    uint32_t				all;
    struct  ADC_CR_BITS		bit;
};

struct ADC_GDR_BITS	
{
    uint32_t RESULT:12;	//		Last conversion results
    uint32_t CHN:3;	    // 		Last conversion ADC channel 
    uint32_t OVERRUN:1;	// 		Results override flag 
    uint32_t DONE:1;	// 		Conversion complete flag
    uint32_t RSVD:15;	// 
};
union ADC_GDR_REG 
{
    uint32_t				all;
    struct  ADC_GDR_BITS	bit;
};

struct ADC_CHSEL_BITS	
{
    uint32_t DR0CHSEL:3;	//		Select AD channel to be converted in DR0
    uint32_t RSVD0:1;
    uint32_t DR1CHSEL:3;	//		Select AD channel to be converted in DR1 
    uint32_t RSVD1:1;
    uint32_t DR2CHSEL:3;	//		Select AD channel to be converted DR2 
    uint32_t RSVD2:1;
    uint32_t DR3CHSEL:3;	//		Select AD channel to be converted in DR3 
    uint32_t RSVD3:1;
    uint32_t DR4CHSEL:3;	//		Select AD channel to be converted in DR4 
    uint32_t RSVD4:1;
    uint32_t DR5CHSEL:3;	//		Select AD channel to be converted in DR5 
    uint32_t RSVD5:1;
    uint32_t DR6CHSEL:3;	//		Select AD channel to be converted in DR6
    uint32_t RSVD6:1;
    uint32_t DR7CHSEL:3;	//		Select AD channel to be converted in DR7 
    uint32_t RSVD7:1;
};
union ADC_CHSEL_REG 
{
    uint32_t				all;
    struct  ADC_CHSEL_BITS	bit;
};

struct ADC_INTEN_BITS	
{
    uint32_t INTEN:8;	//		Eenable interrupt when Selected DRn receive conversin result
    uint32_t GINTEN:1;	//		Eenable interrupt when every conversin complete 
    uint32_t RSVD:23;
};

union ADC_INTEN_REG 
{
    uint32_t				all;
    struct  ADC_INTEN_BITS	bit;
};

struct ADC_DR_BITS	
{
    uint32_t RESULT:12;	//		Conversin result
    uint32_t RSVD:18;
    uint32_t OVERRUN:1;	//		Override flag 
    uint32_t DONE:1;	//		Conversion complete flag 
};
union ADC_DR_REG 
{
    uint32_t				all;
    struct  ADC_DR_BITS		bit;
};

struct ADC_STAT_BITS	
{
    uint32_t DONE:8;	    //These bits mirror the DONE status flags that appear in the result register for each A/D channel. 
    uint32_t OVERRUN:8;	    //These bits mirror the OVERRRUN status flags that appear in the result register for each A/D channel. 
    uint32_t ADINT:1;	    //This bit is the A/D interrupt flag.  
    uint32_t HILMTFLAG0:1;	//High limit 0 status. 
    uint32_t HILMTFLAG1:1;	//High limit 1 status. 
    uint32_t LOLMTFLAG0:1;	//Low limit 0 status.
    uint32_t LOLMTFLAG1:1;	//Low limit 1 status. 
    uint32_t ADCRDY:1;	    //The bit value 1 indicates ADC_ converter is ready to use after ADC is enable.
    uint32_t RSVD:10; 
};
union ADC_STAT_REG 
{
    uint32_t				all;
    struct  ADC_STAT_BITS	bit;
};

struct ADC_HILMT_BITS	
{
    uint32_t HILMT0:12;	// High Limit value0
    uint32_t CHNSEL0:3;	// channel to be compared with the high limit value0
    uint32_t INTEN0:1;	// Enable High Limit0 interrupt 
    uint32_t HILMT1:12;	// High Limit value1
    uint32_t CHNSEL1:3;	// channel to be compared with the high limit value1
    uint32_t INTEN1:1;	// Enable High Limit1 interrupt
};
union ADC_HILMT_REG 
{
    uint32_t				all;
    struct  ADC_HILMT_BITS	bit;
};

struct ADC_LOLMT_BITS	
{
    uint32_t LOLMT0:12;	// Low Limit value0
    uint32_t CHNSEL0:3;	// channel to be compared with the low limit value0
    uint32_t INTEN0:1;	// Enable low Limit0 interrupt 
    uint32_t LOLMT1:12;	// low Limit value1
    uint32_t CHNSEL1:3;	// channel to be compared with the low limit value1
    uint32_t INTEN1:1;	// Enable low Limit1 interrupt
};
union ADC_LOLMT_REG 
{
    uint32_t				all;
    struct  ADC_LOLMT_BITS	bit;
};

struct ADC_SSCR_BITS	
{
    uint32_t ADC_TRIG:1;	// Set this bit to trigger ADC_ to do one time conversion.
    uint32_t RSVD:31;
};
union ADC_SSCR_REG 
{
    uint32_t				all;
    struct  ADC_SSCR_BITS	bit;
};

/* ================================================================================ */
/* ================                       ADC                      ================ */
/* ================================================================================ */


/**
  * @brief Analog-to-Digital Converter (ADC)
  */

typedef struct                                      /*!< ADC Structure                                                         */
{                                    
    __IO union  ADC_CR_REG      CR;                 /*!< ADC_ control register                                                  */
    __IO union  ADC_GDR_REG     GDR;                /*!< ADC Global Data Register                                              */
    __IO union  ADC_CHSEL_REG   CHSEL;              /*!< Channel select control register                                       */
    __IO union  ADC_INTEN_REG   INTEN;              /*!< ADC Interrupt Enable Register                                         */
    __IO union  ADC_DR_REG      DR[8];              /*!< A/D Channel 0 Data Register                                           */
    __IO  union ADC_STAT_REG    STAT;               /*!< ADC Status Register                                                   */
    __IO union  ADC_HILMT_REG   HILMT;              /*!< ADC High Limit Control Register                                       */
    __IO union  ADC_LOLMT_REG   LOLMT;              /*!< ADC Low Limit Control Register                                        */
    __I  uint32_t               RESERVED0;
    __IO union  ADC_SSCR_REG    SSCR;               /*!< ADC software trigger convert register                                 */
} ADC_TypeDef;

#define		ADC	((ADC_TypeDef	*) ADC_BASE)

//AD channel sel
#define AD0 	                    0x00
#define AD1 	                    0x01
#define AD2 	                    0x02
#define AD3 	                    0x03
#define AD4 	                    0x04
#define AD5 	                    0x05
#define AD6 	                    0x06
#define AD7 	                    0x07

//DR select
#define ADC_DR0SEL 	                0
#define ADC_DR1SEL 	                4
#define ADC_DR2SEL 	                8
#define ADC_DR3SEL 	                12
#define ADC_DR4SEL 	                16
#define ADC_DR5SEL 	                20
#define ADC_DR6SEL 	                24
#define ADC_DR7SEL 	                28

//DR int
#define ADC_DR0INT 	                0x1
#define ADC_DR1INT 	                0x2
#define ADC_DR2INT 	                0x4
#define ADC_DR3INT 	                0x8
#define ADC_DR4INT              	0x10
#define ADC_DR5INT 	                0x20
#define ADC_DR6INT 	                0x40
#define ADC_DR7INT 	                0x80
#define ADC_GINT 		            0x100

//DR index
#define ADC_DR0 	                0x0
#define ADC_DR1 	                0x1
#define ADC_DR2 	                0x2
#define ADC_DR3 	                0x3
#define ADC_DR4 	                0x4
#define ADC_DR5 	                0x5
#define ADC_DR6 	                0x6
#define ADC_DR7 	                0x7

//ADC mode
#define TRIGGERMODE 	            0x0
#define BURSTMODE 		            0x1

//ADC trigger option
#define ADC_NONE_START              0x0 		//No start (this value should be used when clearing PDN to 0).
#define ADC_START_BY_SOFTWAER       0x1 		//Start conversion now.
#define ADC_START_BY_CT16B2_CAP0    0x2 		//Start conversion when the edge selected by bit 28 occurs on CT16B2_CAP0.
#define ADC_START_BY_CT16B2_CAP1    0x3 		//Start conversion when the edge selected by bit 28 occurs on CT16B2_CAP1.
#define ADC_START_BY_CT16B2_MAT0    0x4 		//Start conversion when the edge selected by bit 28 occurs on CT16B2_MAT0.
#define ADC_START_BY_CT16B2_MAT1    0x5 		//Start conversion when the edge selected by bit 28 occurs on CT16B2_MAT1.
#define ADC_START_BY_CT16B3_MAT0    0x6 		//Start conversion when the edge selected by bit 28 occurs on CT16B3_MAT0.
#define ADC_START_BY_CT16B3_MAT1    0x7 		//Start conversion when the edge selected by bit 28 occurs on CT16B3_MAT1.
#define ADC_START_BY_PWM            0x8 		//Start conversion when PWM1 cycle Reload,

//ADC trigger signal edge
#define ADC_TRIGGER_RISE_EDGE       0x0
#define ADC_TRIGGER_FALL_EDGE       0x1

#define   ADC_IssueSoftTrigger 		(ADC->SSCR.bit.ADC_TRIG = 1)
#define   ADC_EnableHighCmp0Int 	(ADC->HILMT.bit.INTEN0 = 1)
#define   ADC_EnableHighCmp1Int 	(ADC->HILMT.bit.INTEN1 = 1)
#define   ADC_EnableLowCmp0Int 		(ADC->LOLMT.bit.INTEN0 = 1)
#define   ADC_EnableLowCmp1Int 		(ADC->LOLMT.bit.INTEN1 = 1)
#define   ADC_DisableHighCmp0Int 	(ADC->HILMT.bit.INTEN0 = 0)
#define   ADC_DisableHighCmp1Int 	(ADC->HILMT.bit.INTEN1 = 0)
#define   ADC_DisableLowCmp0Int 	(ADC->LOLMT.bit.INTEN0 = 0)
#define   ADC_DisableLowCmp1Int 	(ADC->LOLMT.bit.INTEN1 = 0)

#define ADC_CHN0_ENABLE             0x01
#define ADC_CHN1_ENABLE             0x02
#define ADC_CHN2_ENABLE             0x04
#define ADC_CHN3_ENABLE             0x08
#define ADC_CHN4_ENABLE             0x10
#define ADC_CHN5_ENABLE             0x20
#define ADC_CHN6_ENABLE             0x40
#define ADC_CHN7_ENABLE             0x80

void ADC_Init(uint32_t conversionrate);
void ADC_DeInit(void);
void ADC_SetupChannels (uint8_t channel, uint8_t data_reg);
void ADC_SelectTriggerSource(uint8_t triggermode,uint8_t triggersrc, uint8_t edge);
void ADC_SetHighCmp0(uint32_t val, uint8_t selchannel);
void ADC_SetHighCmp1(uint32_t val, uint8_t selchannel);
void ADC_SetLowCmp0 (uint32_t val, uint8_t selchannel);
void ADC_SetLowCmp1 (uint32_t val, uint8_t selchannel);
void ADC_EnableConversionInt(uint32_t inttype);
void ADC_EnableChannels(uint8_t channel);
void ADC_DisableChannels(uint8_t channel);
void ADC_WaitAdcReady(void);
uint32_t ADC_GetConversionData(uint8_t dr);






#endif /* end __ADC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
