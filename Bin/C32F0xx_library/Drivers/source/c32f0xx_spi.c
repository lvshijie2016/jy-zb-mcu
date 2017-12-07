/*   Copyright(C) 20012,
 *   All rights reserved.
 *
 *   History
 *   2012.07.21  ver 1.01    Prelimnary version
 *
******************************************************************************/
/*----------- This driver is for general SPI usage ----------------*/
/* Includes ------------------------------------------------------------------*/
#include "c32f0xx.h"
#include "c32f0xx_sys.h"
#include "c32f0xx_spi.h"

extern uint32_t SystemCoreClock;
/*****************************************************************************
Function Name	SPI_Open
Function Definition	void SPI_Open(uint8_t framemode, uint8_t bitwidth, uint8_t master)
Function Description	Initial SPI infterface 
Input Parameters	Framemode: SPI_FRAME/SSI_FRAME, current support SPI_FRAME only
Bitwidth: data size, from 4-bit to 16-bit
Master: selection of master or slave, SPI_MASTER/SPI_SLAVE
spionlymode: SPI_MODE0~SPI_MODE3
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SPI_Open(uint8_t framemode, uint8_t bitwidth, uint8_t master, uint8_t spionlymode)
{
    //enable SPI clock
    SYSCON->SYSAHBCLKCTRL.bit.SPICLK=1;
    //set spi frame SPI/SSI selcetion
    SPI->CR0.bit.FRF=framemode;
    //select bit width
    SPI->CR0.bit.DSS=bitwidth;
    //Set master/slave
    if (master==SPI_MASTER)
        SPI->CR1.bit.MS=0;
    else
        SPI->CR1.bit.MS=1;
    //set SPI mode
    SPI->CR0.bit.CPOL=spionlymode;
    SPI->CR0.bit.CPHA=spionlymode>>1;
    //enable SPIcontroller
    SPI->CR1.bit.SSE=1;

    return;
}
/*****************************************************************************
Function Name	SPI_Close
Function Definition	void SPI_Close(void)
Function Description	De-Initial SPI interface 
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SPI_Close(void)
{
    //Disable SPI clock
    SYSCON->SYSAHBCLKCTRL.bit.SPICLK=0;
    return;
}
/*****************************************************************************
Function Name	SPI_SetClk
Function Definition	void SPI_SetClk(uint32_t freq)
Function Description	Setup SPI clock. In Master mode, SPI frequency set no more than system clock divide by 7; 
												in Slave mode, the frequency set no more than system clock divide by 12.
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SPI_SetClk(uint32_t freq)
{
    uint32_t div;

    div=SystemCoreClock/freq;

    if (SPI->CR1.bit.MS)
    {
        //slave mode
        if (div<12)
            div=12;
    }
    else
    {
        //master mode
        if (div<7)
            div=7;
    }
    //set up divider
    SPI->CPSR.bit.CPSDVSR=div;
    return;
}
/*****************************************************************************
Function Name	SPI_ClearFIFO
Function Definition	void SPI_ClearFIFO(void)
Function Description	Clear SPI TX and RX FIFO.
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SPI_ClearFIFO(void)
{
    //wait untile TX FIFO empty
    while(!SPI->SR.bit.TFE);
    //clear RX FIFO
    //SPI->ICR.bit.RTIC=1;
    //SPI->ICR.bit.RTIC=0;
	SPI->CR1.bit.RSFR = 1;
	SPI->CR1.bit.RSFR = 0;
    return;
}
/*****************************************************************************
Function Name	SPI_SingleWriteRead
Function Definition	void SPI_SingleWriteRead(uint16_t *data)
Function Description	Write data to external SPI device and same time read same size data back from SPI device.
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SPI_SingleWriteRead(uint16_t *data)
{
    //wait untile TX FIFO empty
    while(!SPI->SR.bit.TFE);
    //clear RX FIFO
    SPI->ICR.bit.RTIC=1;
    SPI->ICR.bit.RTIC=0;
    //Write data to FIFO
    SPI->DR.bit.DATA=*data;
    //wait untile TX FIFO empty
    while(!SPI->SR.bit.TFE);
    //Read data from FIFO	
    *data=SPI->DR.bit.DATA;
    return;
}
/*****************************************************************************
Function Name	SPI_WritetoFIFO
Function Definition	void SPI_WritetoFIFO (uint16_t *src, uint8_t *noofdata)
Function Description	Write data to SPI FIFO, if FIFO full, stop with rest number of data and current data pointer
Input Parameters	*src: pointer to data
*noofdata: number of data to send
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint8_t SPI_WritetoFIFO (uint16_t *src, uint8_t noofdata)
{
	uint8_t num = 0;
    while((SPI->SR.bit.TNF)&&(noofdata!=0))
    {
        //Write data to FIFO
        SPI->DR.bit.DATA=*src++;	
        noofdata--;
		num++;
    }
    return num;
}
/*****************************************************************************
Function Name	SPI_ReadFIFOData
Function Definition	void SPI_ReadFIFOData (uint16_t *dst, uint8_t *noofdata)
Function Description	Read data from SPI FIFO.
Input Parameters	*dst: pointer to destination
*noofdata: number of data read
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
uint8_t SPI_ReadFIFOData (uint16_t *dst)
{
	uint8_t num = 0;
    while(SPI->SR.bit.RNE)
    {
        *dst++=SPI->DR.bit.DATA;
        num++;
    }
    return num;
}
/*****************************************************************************
Function Name	SPI_ClrInt(may not need)
Function Definition	void SPI_ClrInt(void)
Function Description	Clear SPI interrupt
Input Parameters	No
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SPI_ClrInt(uint8_t ric)
{
	SPI->ICR.all = ric;
}
/*****************************************************************************
Function Name	SPI_EnableInt
Function Definition	void SPI_EnableInt(uint8_t inttype)
Function Description	Setup SPI interrupt
Input Parameters	Inttype: combination of 
					SPI_RX_FIFO_FULL	1
					SPI_RX_FIFO_NOT_EMPTY	2
					SPI_RX_FIFO_HALF_FULL	4
					SPI_TX_FIFO_HALF_EMPTY	8
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SPI_EnableInt(uint8_t inttype)
{
	SPI->IMSC.all |= inttype;
	return;
}
/*****************************************************************************
Function Name	SPI_DisableInt
Function Definition	void SPI_DisableInt(uint8_t inttype)
Function Description	Setup SPI interrupt
Input Parameters	Inttype: combination of 
					SPI_RX_FIFO_FULL	1
					SPI_RX_FIFO_NOT_EMPTY	2
					SPI_RX_FIFO_HALF_FULL	4
					SPI_TX_FIFO_HALF_EMPTY	8
Return Value	No
Condition	No
Function called	-

Last Chang Date: 2015/09/12			
*****************************************************************************/
void SPI_DisableInt(uint8_t inttype)
{
	SPI->IMSC.all &= ~inttype;
	
	return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
