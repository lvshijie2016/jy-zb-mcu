/*****************************************************************************
 *   IIC:  Head file for MCU IIC(IIC compatible) application
 *
 *   Copyright(C) 2015,
 *   All rights reserved.
 *
 *   History
 *   2015.08.14  ver 1.00
 *
******************************************************************************/
#ifndef __IIC_H
#define __IIC_H

#ifdef __cplusplus
extern "C" {
#endif


#include "c32f0xx.h"
//--IIC Registers BITS Field---------------------------------------------------------------------------------------
struct IIC_CONSET_BITS	
{
    uint32_t TXRX:1;	//Transmit/receive flag
    uint32_t MASL:1;	//Master/slave flag
    uint32_t AA:1;	    //Assert acknowledge flag. 
    uint32_t SI:1;	    //IIC interrupt flag. 
    uint32_t STO:1;	    //STOP flag. 
    uint32_t STA:1;	    //START flag. 
    uint32_t IICEN:1;	//IIC interface enable. 
    uint32_t RSVD:25;	//Reseved 31:7
};
union IIC_CONSET_REG 
{
    uint32_t				    all;
    struct  IIC_CONSET_BITS		bit;
};	
struct IIC_STAT_BITS	
{
    uint32_t STATUS:7;	        //These bits give the actual status information
    uint32_t RSVD1:1;	        //Reseved 7
    uint32_t SLVADDMATCH:1;	    //Slave address matched. 
    uint32_t SLVRXBUFFULL:1;	//Slave receiver buffer is full. 
    uint32_t SLVTXBUFEMPTY:1;	//Slave transmitter buffer is empty. 
    uint32_t RSVD:21;	        //Reseved 31:11
};
union IIC_STAT_REG 
{
    uint32_t				all;
    struct  IIC_STAT_BITS	bit;
};
struct IIC_DAT_BITS	
{
    uint32_t DATA:8;	//This register holds data values that have been received or are to be transmitted.
    uint32_t RSVD:24;	//Reseved 31:8
};
union IIC_DAT_REG 
{
    uint32_t				all;
    struct  IIC_DAT_BITS	bit;
};
struct IIC_ADR_BITS	
{
    uint32_t RSVD0:1;	//Reseved 
    uint32_t ADDR:7;	//The IIC device address for slave mode. 
    uint32_t RSVD1:24;	//Reseved 31:8
};
union IIC_ADR_REG 
{
    uint32_t				all;
    struct  IIC_ADR_BITS	bit;
};
struct IIC_SCLH_BITS	
{
    uint32_t SCLH:16;	//Count for IIC high time period selection 
    uint32_t RSVD:16;	//Reseved 31:16
};
union IIC_SCLH_REG 
{
    uint32_t				all;
    struct  IIC_SCLH_BITS	bit;
};
struct IIC_SCLL_BITS	
{
    uint32_t SCLL:16;	//Count for IIC low time period selection 
    uint32_t RSVD:16;	//Reseved 31:16
};
union IIC_SCLL_REG 
{
    uint32_t				all;
    struct  IIC_SCLL_BITS	bit;
};
struct IIC_CONCLR_BITS	
{
    uint32_t TXRX:1;	//TX/RX select Clear bit. 
    uint32_t MASL:1;	//Master/slave select Clear bit. 
    uint32_t AAC:1;	    //Assert acknowledge Clear bit. 
    uint32_t SIC:1;	    //Interrupt Clear bit. 
    uint32_t RSVD0:1; 
    uint32_t STAC:1;	//START flag Clear bit. 
    uint32_t IICENC:1;	//IIC interface Disable bit. 
    uint32_t RSVD1:25;	//Reseved 31:7
};
union IIC_CONCLR_REG 
{
    uint32_t				    all;
    struct  IIC_CONCLR_BITS		bit;
};
struct IIC_DATA_BITS	
{
    uint32_t DAT:8;	    //This register holds contents of the 8 MSBs of the DAT shift register. 
    uint32_t RSVD:24;	//Reseved 31:8
};
union IIC_DATA_REG 
{
    uint32_t				all;
    struct  IIC_DATA_BITS	bit;
};
struct IIC_MASK_BITS	
{
    uint32_t RSVD0:1;	//Reseved 
    uint32_t MASK:7;	//Mask bits.  
    uint32_t RSVD1:24;	//Reseved 31:8
};
union IIC_MASK_REG 
{
    uint32_t				all;
    struct  IIC_MASK_BITS	bit;
};
/* ================================================================================ */
/* ================                       IIC                      ================ */
/* ================================================================================ */


/**
  * @brief two wires serial communication interface (IIC)
  */

typedef struct                                                    /*!< IIC Structure                                                         */
{                                    
    __IO union IIC_CONSET_REG  CONSET;                            /*!< IIC control set bit register                                          */
    __I  union IIC_STAT_REG    STAT;                              /*!< IIC state register                                                    */
    __IO union IIC_DAT_REG     DAT;                               /*!< IIC data register                                                     */
    __IO union IIC_ADR_REG     ADR0;                              /*!< IIC Slave Address Register 0                                          */
    __IO union IIC_SCLH_REG    SCLH;                              /*!< Duty Cycle Register High Half Word                                    */
    __IO union IIC_SCLL_REG    SCLL;                              /*!< Duty Cycle Register Low Half Word                                     */
    __O  union IIC_CONCLR_REG  CONCLR;                            /*!< IIC Control Clear Register                                            */
    __I  uint32_t              RSVD;
    __IO union IIC_ADR_REG     ADR1;                              /*!< IIC Slave Address Register 1                                          */
    __IO union IIC_ADR_REG     ADR2;                              /*!< IIC slave address register2                                           */
    __IO union IIC_ADR_REG     ADR3;                              /*!< IIC slave address register3                                           */
    __I  union IIC_DATA_REG    DATA_BUFFER;                       /*!< IIC data buffer register                                              */
    __IO union IIC_MASK_REG    MASK0;                             /*!< IIC address mask register0                                            */
    __IO union IIC_MASK_REG    MASK1;                             /*!< IIC address mask register1                                            */
    __IO union IIC_MASK_REG    MASK2;                             /*!< IIC address mask register2                                            */
    __IO union IIC_MASK_REG    MASK3;                             /*!< IIC address mask register3                                            */
} IIC_TypeDef;

#define IIC    ((IIC_TypeDef    *) IIC_BASE)
//IIC mode

//IIC status	
#define IIC_IDLE				            0
#define IIC_STARTED				            1
#define IIC_RESTARTED			            2
#define IIC_REPEATED_START			        3
#define DATA_ACK				            4
#define DATA_NACK				            5
#define IIC_BUSY				            6
#define IIC_NO_DATA				            7
#define IIC_NACK_ON_ADDRESS			        8
#define IIC_NACK_ON_DATA		 	        9
#define IIC_ARBITRATION_LOST  		        10
#define IIC_TIME_OUT			            11
#define IIC_OK					            12

#define IIC_CONSET_TXRX			            ((uint8_t)(0x01))	/*!< Transmit/Receive flag */
#define IIC_CONSET_MASL			            ((uint8_t)(0x02))	/*!< Master/Slave flag */
#define IIC_CONSET_AA			            ((uint8_t)(0x04))	/*!< Assert acknowledge flag */
#define IIC_CONSET_SI			            ((uint8_t)(0x08)) 	/*!< IIC interrupt flag */
#define IIC_CONSET_STO			            ((uint8_t)(0x10)) 	/*!< STOP flag */
#define IIC_CONSET_STA			            ((uint8_t)(0x20)) 	/*!< START flag */
#define IIC_CONSET_I2EN			            ((uint8_t)(0x40)) 	/*!< IIC interface enable */
#define PARAM_IIC_CONSET(IIC_CONSET) 		((((IIC_CONSET) & (uint8_t)0x83) == 0x00) && ((IIC_CONSET) != 0x00))


#define IIC_CONCLR_TXRXC			        ((uint8_t)(0x01))  	/*!< Transmit/Receive Clear bit */
#define IIC_CONCLR_MASLC			        ((uint8_t)(0x02))	/*!< Master/Slave Clear bit */
#define IIC_CONCLR_AAC			            ((uint8_t)(0x04))  	/*!< Assert acknowledge Clear bit */
#define IIC_CONCLR_SIC			            ((uint8_t)(0x08))	/*!< IIC interrupt Clear bit */
#define IIC_CONCLR_STAC			            ((uint8_t)(0x20))	/*!< START flag Clear bit */
#define IIC_CONCLR_I2ENC			        ((uint8_t)(0x40))	/*!< IIC interface Disable bit */
#define PARAM_IIC_CONCLR(IIC_CONCLR) 		((((IIC_CONCLR) & (uint8_t)0x93) == 0x00) && ((IIC_CONCLR) != 0x00))
						

#define IIC_M_TX_START				        0x01		/*!< A start condition has been transmitted */
#define IIC_M_TX_RESTART			        0x10	 	/*!< A repeat start condition has been transmitted */
#define IIC_M_TX_SLAW_ACK			        0x0B	 	/*!< SLA+W has been transmitted, ACK has been received */
#define IIC_M_TX_SLAW_NACK		            0x4B	    /*!< SLA+W has been transmitted, NACK has been received */
#define IIC_M_TX_DAT_ACK			        0x14		/*!< Data has been transmitted, ACK has been received */
#define IIC_M_TX_ARB_LOST			        0x54	 	/*!< Arbitration lost in SLA+R/W or Data bytes */
 

#define IIC_M_RX_START				        0x01		/*!< A start condition has been transmitted */
#define IIC_M_RX_RESTART			        0x22		/*!< A repeat start condition has been transmitted */
#define IIC_M_RX_SLAR_ACK			        0x0B		/*!< SLA+R has been transmitted, ACK has been received */
#define IIC_M_RX_SLAR_NACK		            0x4B 		/*!< SLA+R has been transmitted, NACK has been received */
#define IIC_M_RX_DAT_ACK			        0x1D	  	/*!< Data has been received, ACK has been returned */
#define IIC_M_RX_DAT_NACK			        0x5D	 	/*!< Data has been received, NACK has been return */  

uint16_t IIC_GetIICStatus( void );
uint8_t IIC_ReadFlag( uint8_t IIC_CONSET );
void IIC_SetFlag( uint8_t IIC_CONSET );
void IIC_ClearFlag( uint8_t CONCLR );
void IIC_SendByte( uint8_t DataByte );
uint8_t IIC_GetByte( void );
void IIC_Init(uint8_t Mode, uint32_t ClockRate, uint32_t SlaveAddress);
//void IICMasterTransmitReceiver(uint8_t *MasterBuffer, uint8_t WriteLength,uint8_t ReadLength);
uint8_t IIC_ReadByte(uint8_t SlaveAddress, uint8_t SubAddr);
void IIC_ReadArray(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t Lenth, uint8_t *Buffer);
void IIC_WriteByte(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t Value);
void IIC_WriteArray(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t *Buff, uint8_t Length);
void IIC_ResetDataBuff(void);
void IIC_MasterWriteData(uint8_t SlaveAddress, uint8_t Data);
uint8_t IIC_MasterReadByte(uint8_t SlaveAddress);


#ifdef __cplusplus
}
#endif

#endif 

/* --------------------------------- End Of File ------------------------------ */

