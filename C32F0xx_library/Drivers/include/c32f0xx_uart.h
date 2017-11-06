/*****************************************************************************
 *   c32f0xx_uart.h:  Header file for C32F0
 *
 *   Copyright(C) 2015,
 *   All rights reserved.
 *
 *   History
 *   2015.01.21  ver 1.0    Prelimnary version
 *
******************************************************************************/
//#define __UART0

#ifndef __UART_H 
#define __UART_H
#include "c32f0xx.h"
//stauts and int bit

//--UART Registers BITS Field---------------------------------------------------------------------------------------
struct UARTDAT_BITS	
{
    uint32_t DATA:8;						// 7:0 UART receiver buffer register
    uint32_t RSVD:24;						// 31:8 reserved
};
union UARTDAT_REG 
{
    uint32_t					all;
    struct  UARTDAT_BITS		bit;
};

struct UARTSTATE_BITS 
{
    uint32_t TXE:1; 		//Transmit FIFO  empty flag
    uint32_t RXNE:1;		//Receiver FIFO not empty flag
    uint32_t TXF:1; 		//Transmit FIFO full flag
    uint32_t RXF:1;		    //Receiver FIFO full flag
    uint32_t TXHLF:1;		//Transmit FIFO half full flag
    uint32_t RXHLF:1;		//Receiver FIFO half full flag
    uint32_t PARIERR:1;		//Receiver parity error flag
    uint32_t OVERRUN:1;		//Receiver overrun error flag
    uint32_t RSVD3:24;		// 31:8 reserved
};
union UARTSTATE_REG 
{
    uint32_t					all;
    struct UARTSTATE_BITS		bit;
};

struct UARTCTRL_BITS 
{
    uint32_t TXEIE:1;		// 		TX FIFO empty interrupt enable
    uint32_t RXNEIE:1;		//		RX FIFO not empty interrupt enable
    uint32_t TXFIE:1;		//		TX FIFO full interrupt enable
    uint32_t RXFIE:1;		//		RX FIFO full interrupt enable
    uint32_t TXHLFIE:1;		// 		TX FIFO half full interrupt enable
    uint32_t RXHLFIE:1;		//		RX FIFO half full interrupt enable
    uint32_t PARIERRIE:1;	//		Parity error interrupt enable
    uint32_t OVERRUNIE:1;	//		Overrun interrupt enable
    uint32_t PARISEL:1;		//		Parity selection
    uint32_t PARIEN:1;		//		Enable parity
    uint32_t IRDEN:1;		//		Enable IrDN
    uint32_t RSVD0:11;		
    uint32_t RXEN:1;		//		Enable UART RX
    uint32_t TXEN:1;		//		Enable UART TX
    uint32_t RSVD1:8;
};
union UARTCTRL_REG 
{
    uint32_t					all;
    struct UARTCTRL_BITS		bit;
};

struct UARTINTSTATUS_BITS 
{
    uint32_t TXEINT:1;		    //		Interrupt status. The interrupt occurs when data transmit completed. 
    uint32_t RXNEINT:1;		    //		Interrupt status. The interrupt occurs when received data. 
    uint32_t TXFINT:1;		    //		Interrupt status. The interrupt occurs when TX FIFO is full. 
    uint32_t RXFINT:1;		    //		Interrupt status. The interrupt occurs when RX FIFO is full. 
    uint32_t TXHLFINT:1;		//		Interrupt status. The interrupt occurs when TX FIFO is half full. 
    uint32_t RXHLFINT:1;		//		Interrupt status. The interrupt occurs when RX FIFO is half full.
    uint32_t PARIERRINT:1;		//		Interrupt status. The interrupt occurs when RX parity check error is detected
    uint32_t OVERRUNINT:1;		//		Interrupt status. The interrupt occurs when RX overrun error is detected
    uint32_t RSVD:24;	
};
union UARTINTSTATUS_REG 
{
    uint32_t					all; 
    struct UARTINTSTATUS_BITS	bit;
};

struct UARTBAUDDIV_BITS 
{
    uint32_t  BAUDDIV:20;					// 19:0 baud-rate generation pre-scalar divisor value  0x10
    uint32_t  RSVD1:12;						// 31:20  reserved
};
union UARTBAUDDIV_REG 
{
    uint32_t					all; 
    struct  UARTBAUDDIV_BITS	bit;
};

struct UARTFIFOCLR_BITS 
{
    uint32_t  TXFIFOCLR:1;					// Write 1 to clear TX FIFO
    uint32_t  RXFIFOCLR:1;					// Write 1 to clear RX FIFO
    uint32_t  RSVD:30;						// 31:20  reserved
};
union UARTFIFOCLR_REG 
{
    uint32_t					all; 
    struct  UARTFIFOCLR_BITS	bit;
};

typedef struct
{
    __IO   union UARTDAT_REG		DAT;				/*!< Offset: 0x000 Data Register    (R/W) */
    __IO   union UARTSTATE_REG		STAT;				/*!< Offset: 0x004 Status Register  (R/W) */
    __IO   union UARTCTRL_REG		CTRL;				/*!< Offset: 0x008 Control Register (R/W) */
    __IO   union UARTINTSTATUS_REG	INTSTATUS;			/*!< Offset: 0x00C Interrupt Status Register (R/W ) */
    __IO   union UARTBAUDDIV_REG	BAUDDIV;			/*!< Offset: 0x010 Baudrate Divider Register (R/W) */
    __IO   union UARTFIFOCLR_REG	FIFOCLR;			/*!< Offset: 0x014 Clear TX and RX FIFO */
} UART_TypeDef;

#define UART0                           ((UART_TypeDef    *) UART0_BASE)
#define UART1                           ((UART_TypeDef    *) UART1_BASE)

#define UART_NO_PARITY                  0x0 
#define UART_EVEN_PARITY                0x1
#define UART_ODD_PARITY                 0x2

#define UART_RX_NO_INT                  0x0
#define UART_TX_EMPTY                   0x1
#define UART_RX_NOT_EMPTY               0x2
#define UART_TX_FULL                    0x4 
#define UART_RX_FULL                    0x8
#define UART_TX_HALF_FULL               0x10 
#define UART_RX_HALF_FULL               0x20
#define UART_PARIERRINT                 0x40 
#define UART_OVERRUNINT                 0x80

#define TX_FULL                         1
#define TX_NOT_FULL                     0



void    UART_Open(UART_TypeDef *uart, uint32_t baudrate, uint8_t parityoption, uint8_t rxinttriggerlevel);
void    UART_Close(UART_TypeDef *uart);
uint8_t UART_ByteRead(UART_TypeDef *uart, uint8_t *data);
void    UART_Read(UART_TypeDef *uart, uint8_t * rxbuf, uint8_t *readBytes);
uint8_t UART_ByteWrite(UART_TypeDef *uart, uint8_t data);
uint8_t UART_Write(UART_TypeDef *uart, uint8_t * txbuf, uint32_t writebytes);
void    UART_Send(UART_TypeDef *uart, uint8_t * txbuf, uint32_t sendbytes);
void    UART_PutString (UART_TypeDef *uart, uint8_t * str);
void    UART_PutHex (UART_TypeDef *uart, uint32_t data);
void    UART_EnableInt(UART_TypeDef *uart, uint32_t intcon);
void    UART_DisableInt(UART_TypeDef *uart);
void    UART_ClearIntFlag(UART_TypeDef *uart);
uint8_t UART_GetStatus(UART_TypeDef*uart);
void    UART_ClearFIFO(UART_TypeDef *uart);


#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
