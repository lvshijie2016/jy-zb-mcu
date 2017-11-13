/*****************************************************************************
 *   IIC:  Sourse file for  MCU IIC(I2C compatible) application
 *
 *   Copyright(C) 2012, 
 *   All rights reserved.
 *
 *   History
 *   2012.08.14  ver 1.00
 *
******************************************************************************/

#include "c32f0xx.h"
#include "c32f0xx_sys.h"
#include "c32f0xx_iic.h"
//#include "stdio.h"
//#include "fifo.h"

/* Private variables ---------------------------------------------------------*/
static uint8_t  IICMasterBuffer[8];
static uint8_t  IICSlaveBuffer[8];
static uint8_t  IICMasterState = IIC_IDLE;
static uint32_t IICReadLength, IICWriteLength;
static uint32_t RdIndex = 0;
static uint32_t WrIndex = 0;
static uint32_t Restart = 0;
static uint8_t  Re_Start;
static uint32_t SelAddr = 0;

extern uint32_t SystemClock;
/*****************************************************************************
** Function name:		IIC initial
**
** Descriptions:		IIC initial function
**
** parameters:      Mode: 1---Master 0---Slave
**                        ClockRate: unit in KHz
**                        SlaveAddress: Slave address in slave mode
**                        IICInterrupt: 1---Enable IIC Interrupt 0---Disable IIC Interrupt
**
** Returned value:	None
** 
*****************************************************************************/
void IIC_Init(uint8_t Mode, uint32_t ClockRate, uint32_t SlaveAddress)
{	
    //reset iic buff data to 0
    IIC_ResetDataBuff();	

    /*---  Enable IIC clock and de-assert reset  ---*/
    SYSCON->PRESETCTRL.bit.IIC_RST_N |= 0;
    SYSCON->PRESETCTRL.bit.IIC_RST_N |= 1;
    SYSCON->SYSAHBCLKCTRL.bit.IICCLK |= 1;

    /*---  IIC I/O config  ---*/    
    //    IOCON->PIO1_10 = 0x01 + (1<<10);  // IIC SCL I/O config, PIO1_10-->SCL
    //    IOCON->PIO1_11 = 0x01 + (1<<10);  // IIC SDA I/O config, PIO1_11-->SDA

    /*--- Clear flags ---*/
    IIC->CONCLR.all = 0xFF;   

    /*--- Set Clock rate ---*/
    IIC->SCLH.bit.SCLH = SystemCoreClock/(ClockRate*2000);
    IIC->SCLL.bit.SCLL = SystemCoreClock/(ClockRate*2000);

    if (!Mode)
    {
        IIC->ADR0.all = SlaveAddress; // Slave address
    }

    if (Mode)
    {
        IIC->CONSET.all = 0x42; // Master mode
    } 
    else
    {
        IIC->CONSET.all = 0x40; // Slave mode
    }

    /* Enable the IIC Interrupt */
    NVIC_EnableIRQ(IIC_IRQn);
}


/*****************************************************************************
** Function name:		IIC_GetIICStatus
**
** Descriptions:		Get IIC Status
**
** parameters:      None
**
** Returned value:	None
** 
*****************************************************************************/
uint16_t IIC_GetIICStatus( void )
{
    uint16_t tmp = IIC->STAT.all;
    return tmp;
}

/*****************************************************************************
** Function name:		IIC_ReadFlag
**
** Descriptions:		Read IIC Flag
**
** parameters:      Read bit
**
** Returned value:	None
** 
*****************************************************************************/
uint8_t IIC_ReadFlag( uint8_t IIC_CONSET )
{
	return(IIC->CONSET.all & IIC_CONSET);      /* retuen flag */
}

/*****************************************************************************
** Function name:		IIC_SetFlag
**
** Descriptions:		Set IIC Flag
**
** parameters:      Set bit
**
** Returned value:	None
** 
*****************************************************************************/
void IIC_SetFlag( uint8_t IIC_CONSET )
{
	IIC->CONSET.all = IIC_CONSET;      /* Set flag */
}

/*****************************************************************************
** Function name:		IIC_ClearFlag
**
** Descriptions:		Clear IIC Flag
**
** parameters:      Clear bit
**
** Returned value:	None
** 
*****************************************************************************/
void IIC_ClearFlag( uint8_t IIC_CONCLR )
{
	IIC->CONCLR.all = IIC_CONCLR;      /* Clear flag */
}

/*****************************************************************************
** Function name:		IIC_SendByte
**
** Descriptions:		Send Byte
**
** parameters:      Send data
**
** Returned value:	None
** 
*****************************************************************************/
void IIC_SendByte( uint8_t DataByte )
{
	IIC->DAT.all = DataByte; 
}


/*****************************************************************************
** Function name:		IIC_GetByte
**
** Descriptions:		Get IIC Byte
**
** parameters:      None
**
** Returned value:	IIC Data
** 
*****************************************************************************/

uint8_t IIC_GetByte( void )
{
	return(IIC->DAT.all);
}

/*****************************************************************************
* Function name:    IIC_IRQHandler
*
* Descriptions:     Use status to control process
*
* parameters:       None
* Returned value:   None
* 
*****************************************************************************/
void TWS_IRQHandler(void) 
{
	uint16_t StatValue;
	
	StatValue = IIC_GetIICStatus();
	// 0x4B--SLA+W transmitted but no ACK 
	// 0x54--Data byte in DAT transmitted;no ACK received.
	if 	((StatValue == 0x4B) || (StatValue == 0x54))
	{
			IIC_SetFlag(IIC_CONSET_STO);				// Set Stop flag
			IICMasterState = IIC_OK;
			IIC_ClearFlag(IIC_CONCLR_SIC| IIC_CONCLR_STAC |IIC_CONCLR_TXRXC );
			return;	
	}
	if(IIC->CONSET.all & IIC_CONSET_MASL)// MASTER
	{
			StatValue &= ~(0x740);
		
			switch ( StatValue & 0xFF)
			{
				case IIC_M_TX_START:				/* 0x01: A Start condition is issued. */
					if (Restart == 1)
					{
						//RX SLAD IIC_M_TX_RESTART:
						Restart = 0;
						RdIndex = 0;
						IIC_SendByte(IICMasterBuffer[0] + 1); 			/* Send SLA with R bit set */
						WrIndex++;
						IIC_ClearFlag(IIC_CONCLR_SIC | IIC_CONCLR_STAC |IIC_CONCLR_TXRXC);
					}
					else
					{
						// Start master transmit process
						WrIndex = 0;
						RdIndex = 0;
						IIC_SetFlag(IIC_CONSET_TXRX); 		 // Set tx flag
						IIC_SendByte(IICMasterBuffer[0]);  // Transmit address first 
						WrIndex++;
						IIC_ClearFlag(IIC_CONCLR_SIC | IIC_CONCLR_STAC);
					}
					break;
				
				case IIC_M_TX_RESTART:			/* 0x10: A repeated started is issued */
					RdIndex = 0;
					IIC_SendByte(IICMasterBuffer[WrIndex++]); 			/* Send SLA with R bit set */
					IIC_ClearFlag(IIC_CONCLR_SIC | IIC_CONCLR_STAC);
					break;
				
				case IIC_M_TX_SLAW_ACK:			/* 0x0B: Regardless, it's a ACK */
					if ((IIC->CONSET.all & 0x1)==0x1) // Transmit
					{
						if (IICWriteLength == 1)//only send slave addr
						{
							IIC_SetFlag(IIC_CONSET_STO);      			// Set Stop flag
							IICMasterState = IIC_NO_DATA;
						}
						else
						{
							IIC_SendByte(IICMasterBuffer[WrIndex++]); // Transmit
						}
						IIC_ClearFlag(IIC_CONCLR_SIC);
					}
					else
					{
						if ( (RdIndex + 1) < IICReadLength ) // Read form slave device
						{
							/* Will go to State 0x50 */
							IIC_SetFlag(IIC_CONSET_AA);					/* assert ACK after data is received */
						}
						else
						{
							/* Last byte will no ack, Will go to State 0x58 */
							IIC_ClearFlag(IIC_CONCLR_AAC);				/* assert NACK after data is received */
						}
						IIC_ClearFlag(IIC_CONCLR_SIC | IIC_CONCLR_STAC);
					}
					break;	
				
				case IIC_M_TX_DAT_ACK:			/* 0x14: Data byte has been transmitted, regardless ACK or NACK */
					if ( WrIndex < IICWriteLength )
					{   
						IIC_SendByte(IICMasterBuffer[WrIndex++]);	
					}
					else		/* this should be the last one */
					{
						if ( IICReadLength != 0 )	  
						{
							Restart = 0;
							RdIndex = 0;
							if(Re_Start)
							{
								IIC_SendByte(IICMasterBuffer[0] + 1); 			/* Send SLA with R bit set */
								IIC_SetFlag(IIC_CONSET_STA);				/* Set Repeated-start flag */
							}

							else
							{
								IIC_SendByte(IICMasterBuffer[0] + 1); 			/* Send SLA with R bit set */
							}

							IIC_ClearFlag(IIC_CONCLR_SIC  |IIC_CONCLR_TXRXC);
							break;
						}
						else
						{
							IIC_SetFlag(IIC_CONSET_STO);				/* Set Stop flag */
							IICMasterState = IIC_OK;
						}
					}
					IIC_ClearFlag(IIC_CONCLR_SIC);
					break;			
				
				case IIC_M_RX_DAT_ACK:			/* 0x1D, 0x5D: Data byte has been received, regardless following ACK or NACK */
					IICSlaveBuffer[RdIndex++] = IIC_GetByte();
					if ( (RdIndex + 1) < IICReadLength )
					{   
						IIC_SetFlag(IIC_CONSET_AA);					/* assert ACK after data is received */
						IIC_ClearFlag(IIC_CONCLR_SIC);
					}
					else
					{
						if ((IIC->CONSET.all &0x04)==0x04)
							IIC_ClearFlag(IIC_CONCLR_AAC | IIC_CONCLR_SIC);
						else 
						 {
							 IICMasterState = IIC_OK;
							 IIC_SetFlag(IIC_CONSET_STO);	/* assert NACK on last byte */
							 IIC_ClearFlag(IIC_CONCLR_SIC);
						 }
					}
					break;
					
				default:
					IICMasterState = IIC_ARBITRATION_LOST;
					IIC_ClearFlag(IIC_CONCLR_SIC);	
					break;
			}					
	}
	else// SLAVE
	{
		uint32_t temp = 0;
		if(StatValue & 0x100)
		{
			WrIndex=0;
			RdIndex=0;
			temp = IIC_GetByte();

			if(temp == (IIC->ADR0.all))
			{
				SelAddr=0;
			}
			else if(temp == (IIC->ADR1.all))
			{
				SelAddr=1;
			}
			else if(temp == (IIC->ADR2.all))
			{
				SelAddr=2;
			}
			else if(temp == (IIC->ADR3.all))
			{
				SelAddr=3;
			}				
			IIC_SetFlag(IIC_CONSET_AA);				
			IIC->CONCLR.all=0x100;		//clear addr match interrupt
		}
		else if(StatValue & 0x200)
		{
			IICMasterBuffer[WrIndex++] = IIC->DAT.all;
			//printf("%x \r\n",IICMasterBuffer[WrIndex - 1]);
			//FIFO_Push(IICMasterBuffer[WrIndex - 1]);
			IIC_SetFlag(IIC_CONSET_AA);
		}
		else if(StatValue & 0x400)
		{
#if 0 				
			IIC->DAT.all = IICSlaveBuffer[RdIndex++];
			if(RdIndex >= IICReadLength)
			{
			RdIndex = 0;
			}	
#else
			static uint8_t data_tmp = 0;
			IIC->DAT.all = data_tmp++;			
#endif				
			IIC_SetFlag(IIC_CONSET_AA);
		}		

		IIC->CONCLR.all = IIC_CONCLR_SIC+IIC_CONCLR_STAC;			
	}
	
	return;
}
/*****************************************************************************
* Function name:    IIC_WriteByte
*
* Descriptions:     Write byte to slave chip
*
* parameters:       SlaveAddress: Slave chip address
					SubAddr:  Slave chip register address
					Value :   Value write to slave chip register address
* Returned value:   None
* 
*****************************************************************************/

void IIC_WriteByte(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t Value)
{
    uint8_t i;
    IICMasterBuffer[0] = SlaveAddress; // Slave chip address
    IICMasterBuffer[1] = SubAddr;	   // Chip register address
    IICMasterBuffer[2] = Value;

    Re_Start = 0;

    IICReadLength = 0; // Read length
    IICWriteLength = 3;	// Write length

    IIC_SetFlag(IIC_CONSET_STA); //Set start signal

    IICMasterState = IIC_IDLE;
    while( IICMasterState != IIC_OK ); // Wait for finished

    i = 0xFF;
    while(i--);
}


/*****************************************************************************
* Function name:    IIC_WriteArray
*
* Descriptions:     Write array value to slave chip
*
* parameters:       SlaveAddress: Slave chip address
					SubAddr:  Slave chip register address
					Buff: 	  Write value pointer
					Length:   Length to write
* Returned value:   None
* 
*****************************************************************************/
void IIC_WriteArray(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t *Buff, uint8_t Length)
{
    uint8_t i;
    IICMasterBuffer[0] = SlaveAddress; // Slave chip address
    IICMasterBuffer[1] = SubAddr;

    for( i = 0; i < Length; i++ )
    {
     	IICMasterBuffer[i+2] = *( Buff + i ); // Write value to slave chip register  		
    }

    Re_Start = 0;
    IICReadLength = 0;
    IICWriteLength = Length+2; // Write Length
    IIC_SetFlag(IIC_CONSET_STA); // Start

    IICMasterState = IIC_IDLE;
    while( IICMasterState != IIC_OK ); // Wait for finished

    i = 0xFF;
    while(i--);
}

/*****************************************************************************
* Function name:    IIC_ReadByte
*
* Descriptions:     Read byte from slave chip
*
* parameters:       SlaveAddress: Slave chip address
					SubAddr:  Slave chip register address
* Returned value:   return read value
* 
*****************************************************************************/
uint8_t IIC_ReadByte(uint8_t SlaveAddress, uint8_t SubAddr)
{
    uint8_t i;
    IICMasterBuffer[0] = SlaveAddress; // Slave chip address
    IICMasterBuffer[1] = SubAddr;

    IICWriteLength = 2;
    IICReadLength = 1;

    Re_Start = 1;
    IIC_SetFlag(IIC_CONSET_STA); // Start

    IICMasterState = IIC_IDLE;
    while( IICMasterState != IIC_OK ); // Wait for finished

    i = 0xFF;
    while(i--);

    return(IICSlaveBuffer[0]);
}

/*****************************************************************************
* Function name:    IIC_ReadArray
*
* Descriptions:     Read array value from slave chip
*
* parameters:       SlaveAddress: Slave chip address
					SubAddr:  Slave chip register address
					Length:   Length for reading
					Buffer:   Read value pointer
* Returned value:   None
* 
*****************************************************************************/
void IIC_ReadArray(uint8_t SlaveAddress, uint8_t SubAddr, uint8_t Lenth, uint8_t *Buffer)
{
    uint8_t i;
    IICMasterBuffer[0] = SlaveAddress; // Slave chip address
    IICMasterBuffer[1] = SubAddr;

    IICWriteLength = 2;
    IICReadLength = Lenth;

    Re_Start = 1;
    IIC_SetFlag(IIC_CONSET_STA); // Start

    IICMasterState = IIC_IDLE;
    while( IICMasterState != IIC_OK ); // Wait for finished

    for(i = 0; i < Lenth; i++)
    {
    	*(Buffer+i) = IICSlaveBuffer[i]; // Get pointer value
    }

    i = 0xFF;
    while(i--);
}


/*****************************************************************************
* Function name:    IIC_ResetDataBuff
*
* Descriptions:        innit i2c stuct data to 0
*
* parameters:        None
* Returned value:   None
* 
*****************************************************************************/
void IIC_ResetDataBuff(void)
{
    uint8_t i = 0;
    for(i=0;i<8;i++)
    {
        IICMasterBuffer[i] = 0;
        IICSlaveBuffer[i] = 0;
    }

    IICMasterState = IIC_IDLE;
    IICReadLength  = 0;
    IICWriteLength = 0;
    RdIndex        = 0;
    WrIndex        = 0;
    Restart        = 0;
    Re_Start       = 0;
    SelAddr        = 0;
}

/*****************************************************************************
* Function name:    IIC_MasterWriteData
*
* Descriptions:        write one data to slave
*
* parameters:         SlaveAddress: slave address
                             Data:                             
* Returned value:   None
* 
*****************************************************************************/
void IIC_MasterWriteData(uint8_t SlaveAddress, uint8_t Data)
{
    uint8_t i;
    IICMasterBuffer[0] = SlaveAddress; // Slave chip address
    IICMasterBuffer[1] = Data;	   // Chip register address

    Re_Start = 0;
    IICReadLength  = 0; // Read length
    IICWriteLength = 2;	// Write length

    IIC_SetFlag(IIC_CONSET_STA); //Set start signal

    IICMasterState = IIC_IDLE;
    while( IICMasterState != IIC_OK ); // Wait for finished

    i = 0xFF;
    while(i--);
}

/*****************************************************************************
* Function name:    IIC_MasterReadByte
*
* Descriptions:       read one data from slave
*
* parameters:         SlaveAddress: slave address                       
* Returned value:    data read from slave
* 
*****************************************************************************/

uint8_t IIC_MasterReadByte(uint8_t SlaveAddress)
{
    uint8_t i;
    IICMasterBuffer[0] = SlaveAddress; // Slave chip address

    IICWriteLength = 1;
    IICReadLength = 1;
    Re_Start = 1;
    Restart = 1;

    IIC_SetFlag(IIC_CONSET_STA); // Start

    IICMasterState = IIC_IDLE;
    while( IICMasterState != IIC_OK ); // Wait for finished

    i = 0xFF;
    while(i--);

    return(IICSlaveBuffer[0]);
}


/* --------------------------------- End Of File ------------------------------ */



