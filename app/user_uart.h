#ifndef _USER_UART_H
#define _USER_UART_H
#include "config.h"


#define LOG_DEBUG "DEBUG"  
#define LOG_TRACE "TRACE"  
#define LOG_ERROR "ERROR"  
#define LOG_INFO  "INFOR"  
#define LOG_CRIT  "CRTCL"  
 
#define LOG(level, format, ...)  \
														do {fprintf(stderr, "[%s|%s@%s,%d] " \
														format "\n",  level, __func__, __FILE__, __LINE__, \
														##__VA_ARGS__ );} \
														while (0)
#if false
	#define  DeBug 
#endif 





#define TX_FH 																	(unsigned char)	0xFF
#define TX_FT 																	(unsigned char)	0xFE
#define TX_PAG_ACK 																(unsigned char)	0x0E
#define TX_CLOCK_STATE															(unsigned char)	0x24
#define TX_MOTO_STATE															(unsigned char)	0x4F
#define BUFF_VALID_SIZE  														(unsigned char)	6
#define BUFFER_LEN  															(unsigned char)	255
#define BUFF_SIZE   															(unsigned char)	20


#define HANDSHAKE_COMMAND														(unsigned char)	0x01				
#define KAR_RUN_STATE															(unsigned char)	0x02
#define HEARTBEAT_COMMAND														(unsigned char)	0x0F
#define LIGHT_COMMAND															(unsigned char)	0x03
#define TIMER_DATA_COMMAND														(unsigned char)	0x04
#define ALARM_COMMAND															(unsigned char)	0x05
#define MOTO_COMMAND															(unsigned char)	0x06
#define GET_TIMER_COMMAND														(unsigned char)	0x07
#define BAT_COMMAN																(unsigned char)	0x25
#define USB_OUT_COMMAN															(unsigned char)	0x26
#define SLEEP_OFF_TIMER_SEY_COMMAN									(unsigned char)	0x27
#define SYSTEM_UPDATE_COMMAN												(unsigned char)	0xF0

#define KAR_POWER_OFF_COMMAND													(unsigned char)	0x20
#define KAR_POWER__OFF															(unsigned char)	0x00








typedef struct
{
	unsigned char sequence_pag;
	unsigned char tx_sequence_pag;
	unsigned char verify;
	unsigned char tx_buf[BUFF_SIZE];
	unsigned char tx_buf_len;
}_Uart0_Typedef;



typedef struct Buffer_t{
	
	uint16_t head;
	uint16_t tail;
	uint8_t len;
	uint8_t buffer[BUFFER_LEN];
	
}Buffer;



extern _Uart0_Typedef  Uart0_Typedef;



void UART0_Init(void);
void UART2_Init(void);
void UART_Send_t(unsigned char Com);
void WriteUartBuf(unsigned char data);
void uart0_get_cmd(uint8_t *g_com);







#endif

