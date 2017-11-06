/*****************************************************************************
 *   spi.h:  Header file for  C32F0 Family Microprocessors
 *
 *   Copyright(C) 2015, 
 *   All rights reserved.
 *
 *   History
 *   2015  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __SPI_H 
#define __SPI_H

#include "c32f0xx.h"

//--SPI Registers BITS Field---------------------------------------------------------------------------------------
struct SPI_CR0_BITS	
{
    uint32_t DSS:4;			//Data Size Select. 
    uint32_t FRF:2;			//Frame Format
    uint32_t CPOL:1;		//Clock Out Polarity. This bit is only used in SPI mode. 
    uint32_t CPHA:1;		//Clock Out Phase. This bit is only used in SPI mode. 
    uint32_t RSVD:24;	    // 
};
union SPI_CR0_REG 
{
    uint32_t				all;
    struct  SPI_CR0_BITS	bit;
};
struct SPI_CR1_BITS	
{
    uint32_t LBM:1;			//Loop Back Mode. 
    uint32_t SSE:1;			//SPI Enable. 
    uint32_t MS:1;			//Master/Slave Mode. 
    uint32_t SOD:1;			//Slave Output Disable. T
    uint32_t CSFL:1;		//SSEL control
    uint32_t RSFR:1;		//Clear receive FIFO
    uint32_t FILTEN:1;		//Enable SPI data line filter
    uint32_t RSVD:25;	    // 
};
union SPI_CR1_REG 
{
    uint32_t				all;
    struct  SPI_CR1_BITS	bit;
};
struct SPI_DR_BITS	
{
    uint32_t DATA:16;	//SPI Data Register 
    uint32_t RSVD:16;	// 
};
union SPI_DR_REG 
{
    uint32_t				all;
    struct  SPI_DR_BITS		bit;
};
struct SPI_SR_BITS	
{
    uint32_t TFE:1;			//Transmit FIFO Empty. This bit is 1 is the Transmit FIFO is empty, 0 if not. 
    uint32_t TNF:1;			//Transmit FIFO Not Full. This bit is 0 if the Tx FIFO is full, 1 if not.
    uint32_t RNE:1;			//Receive FIFO Not Empty. This bit is 0 if Receive FIFO is empty, 1 if not. 
    uint32_t RFF:1;			//Receive FIFO Full. This bit is 1 if Receive FIFO is full, 0 if not. 
    uint32_t BSY:1;			//Busy. This bit is 0 if the SPI controller is idle, or 1 if it is currently sending/receiving a frame and/or the Tx FIFO is not empty. 
    uint32_t RSVD:27;	    // 
};
union SPI_SR_REG 
{
    uint32_t				all;
    struct  SPI_SR_BITS		bit;
};
struct SPI_CPSR_BITS	
{
    uint32_t CPSDVSR:8;		//SPI Clock Prescale Register 
    uint32_t RSVD:24;	    // 
};
union SPI_CPSR_REG 
{
    uint32_t				all;
    struct  SPI_CPSR_BITS	bit;
};
struct SPI_IMSC_BITS	
{
    uint32_t RORIM:1;			//Software should set this bit to enable interrupt when a Receive Overrun occurs, that is, when the Rx FIFO is full and another frame is completely received. The ARM spec implies that the preceding frame data is overwritten by the new frame data when this occurs.
    uint32_t RTIM:1;			//Software should set this bit to enable interrupt when a Receive Time-out condition occurs. A Receive Time-out occurs when the Rx FIFO is not empty, and no has not been read for a “time-out period? The time-out period is the same for master and slave modes and is determined by the SPI bit rate: 32 bits at PCLK / (CPSDVSR [SCR+1]).
    uint32_t RXIM:1;			//Software should set this bit to enable interrupt when the Rx FIFO is at least half full. 
    uint32_t TXIM:1;			//Software should set this bit to enable interrupt when the Tx FIFO is at least half empty. 
    uint32_t RSVD:28;	        // 
};
union SPI_IMSC_REG 
{
    uint32_t				all;
    struct  SPI_IMSC_BITS	bit;
};
struct SPI_RIS_BITS	
{
    uint32_t RORRIS:1;			//This bit is 1 if another frame was completely received while the Rx FIFO was full. The ARM spec implies that the preceding frame data is overwritten by the new frame data when this occurs. 
    uint32_t RTRIS:1;			//This bit is 1 if the Rx FIFO is not empty, and has not been read for a “time-out period? The time-out period is the same for master and slave modes and is determined by the SPI bit rate: 32 bits at PCLK / (CPSDVSR ?[SCR+1]). 
    uint32_t RXRIS:1;			//This bit is 1 if the Rx FIFO is at least half full. 
    uint32_t TXRIS:1;			//This bit is 1 if the Tx FIFO is at least half empty. 
    uint32_t RSVD:28;	        // 
};
union SPI_RIS_REG 
{
    uint32_t				all;
    struct  SPI_RIS_BITS	bit;
};
struct SPI_MIS_BITS	
{
    uint32_t RORMIS:1;			//This bit is 1 if another frame was completely received while the RxFIFO was full, and this interrupt is enabled. 
    uint32_t RTMIS:1;			//This bit is 1 if the Rx FIFO is not empty, has not been read for a “time-out period? and this interrupt is enabled. The time-out period is the same for master and slave modes and is determined by the SPI bit rate: 32 bits at PCLK / (CPSDVSR ?[SCR+1]). 
    uint32_t RXMIS:1;			//This bit is 1 if the Rx FIFO is at least half full, and this interrupt is enabled. 
    uint32_t TXMIS:1;			//This bit is 1 if the Tx FIFO is at least half empty, and this interrupt is enabled. 
    uint32_t RSVD:28;	        // 
};
union SPI_MIS_REG 
{
    uint32_t				all;
    struct  SPI_MIS_BITS	bit;
};
struct SPI_ICR_BITS	
{
    uint32_t RORIC:1;			//Writing a 1 to this bit clears the “frame was received when Rx FIFO was full?interrupt. 
    uint32_t RTIC:1;			//Writing a 1 to this bit clears the “Rx FIFO was not empty and has not been read for a time-out period?interrupt. 
    uint32_t RSVD:30;	        // 
};
union SPI_ICR_REG 
{
    uint32_t				all;
    struct  SPI_ICR_BITS	bit;
};

/* ================================================================================ */
/* ================                       SPI                      ================ */
/* ================================================================================ */


/**
  * @brief Serial peripheral interface (SPI)
  */

typedef struct                                                 /*!< SPI Structure                                                         */
{                                    
    __IO union SPI_CR0_REG   CR0;                              /*!< SPI control register0                                                 */
    __IO union SPI_CR1_REG   CR1;                              /*!< SPI control register1                                                 */
    __IO union SPI_DR_REG    DR;                               /*!< SPI data register                                                     */
    __I  union SPI_SR_REG    SR;                               /*!< SPI status register                                                   */
    __IO union SPI_CPSR_REG  CPSR;                             /*!< SPI Clock Prescale Register                                           */
    __IO union SPI_IMSC_REG  IMSC;                             /*!< Interrupt Mask Set and Clear Register                                 */
    __I  union SPI_RIS_REG   RIS;                              /*!< Raw Interrupt Status Register                                         */
    __I  union SPI_MIS_REG   MIS;                              /*!< Masked Interrupt Status Register                                      */
    __O  union SPI_ICR_REG   ICR;                              /*!< SPI Interrupt Clear Register                                          */
} SPI_TypeDef;

#define SPI                      ((SPI_TypeDef *) SPI_BASE)

#define SPI_FRAME	             0
#define SSI_FRAME	             1

#define SPI_RX_FIFO_FULL	     1
#define SPI_RX_FIFO_NOT_EMPTY	 2
#define SPI_RX_FIFO_HALF_FULL	 4
#define SPI_TX_FIFO_HALF_EMPTY	 8

#define SPI_MODE0	             0
#define SPI_MODE1	             1
#define SPI_MODE2	             2
#define SPI_MODE3	             3
#define SPI_MASTER	             0
#define SPI_SLAVE	             1

//#define RORIC		1
//#define RTIC		2

#define SPI_Set_SSEL_High        (SPI->CR1.bit.CSFL = 1)
#define SPI_Reset_SSEL_Low       (SPI->CR1.bit.CSFL = 0)
#define SPI_Write_FIFO(data)     (SPI->DR.bit.DATA=data)
#define SPI_TX_FIFO_Not_Empty    ((SPI->SR.all&0x11)!=1)
#define SPI_Read_FIFO(data)      (data=SPI->DR.bit.DATA)
//#define SPI_CLR_FIFO (SPI->CR1.bit.RSFR =1; SPI->CR1.bit.RSFR =0)

#define SPI_FRAME_4BITS  (0x03)
#define SPI_FRAME_5BITS  (0x04)
#define SPI_FRAME_6BITS  (0x05)
#define SPI_FRAME_7BITS  (0x06)
#define SPI_FRAME_8BITS  (0x07)
#define SPI_FRAME_9BITS  (0x08)
#define SPI_FRAME_10BITS (0x09)
#define SPI_FRAME_11BITS (0x0A)
#define SPI_FRAME_12BITS (0x0B)
#define SPI_FRAME_13BITS (0x0C)
#define SPI_FRAME_14BITS (0x0D)
#define SPI_FRAME_15BITS (0x0E)
#define SPI_FRAME_16BITS (0x0F)


void SPI_Open(uint8_t framemode, uint8_t bitwidth, uint8_t master, uint8_t spionlymode);
void SPI_Close(void);
void SPI_SetClk(uint32_t freq);
void SPI_ClearFIFO(void);
void SPI_SingleWriteRead(uint16_t *data);
uint8_t SPI_WritetoFIFO (uint16_t *src, uint8_t noofdata);
uint8_t SPI_ReadFIFOData (uint16_t *dst);
void SPI_ClrInt(uint8_t ric);
void SPI_EnableInt(uint8_t inttype);


#endif /* end __SPI_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/

