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
#if true
	#define  DeBug 
#endif 




#define TX_FH 																	(unsigned char)	0xFF
#define TX_FT 																	(unsigned char)	0xFE
#define TX_PAG_ACK 																(unsigned char)	0x0E
#define TX_CLOCK_STATE															(unsigned char)	0x24
#define TX_MOTO_STATE															(unsigned char)	0x4F
#define BUFF_VALID_SIZE  														(unsigned char)	6
#define BUFFER_LEN  															(unsigned int)	1088
#define BUFF_SIZE   															(unsigned char)	20

#define TX_OTA_ACK 																(unsigned char)	0xF1

#define TX_OTA_DATA_ACK 													(unsigned char)	0xF2


#define HANDSHAKE_COMMAND														(unsigned char)	0x01				
							

void get_data(void);






typedef struct
{
	unsigned char sequence_pag;
	unsigned char tx_sequence_pag;
	unsigned char verify;
	unsigned char tx_buf[BUFF_SIZE];
	unsigned char tx_buf_len;
}_Uart0_Typedef;


//队列环形BUF
typedef struct Buffer_t{ 
	
	uint16_t 	head;
	uint16_t 	tail;
	uint8_t 	buffer[BUFFER_LEN];

}Buffer;



typedef struct{
	
	uint8_t 	packet_num;
	uint8_t 	command;
	uint8_t 	head;
	uint16_t 	len;
	uint8_t 	low_len;
	uint8_t		xor_verify;
	
}_uart_data_typedef;

typedef struct{
	
	uint8_t 	data_buffer[BUFFER_LEN];
	bool		flag;
	
}_get_command_data;


extern _Uart0_Typedef  Uart0_Typedef;
extern uint8_t 	data_buffer[BUFFER_LEN];


void UART0_Init(void);
void UART_Send_t(unsigned char Com);
void WriteUartBuf(unsigned char data);
void uart0_get_cmd(uint8_t *g_com);
void get_packet(void);
void flash_write(void);





#endif

