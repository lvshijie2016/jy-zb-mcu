#include "user_uart.h"


_Uart0_Typedef  Uart0_Typedef;


static Buffer Buffer_t;
void udly1us(uint32_t dlytime) {while(dlytime--);}


void uni_UART_Init(void)
{
	_uart_init();
	memset(&Buffer_t,0,sizeof(Buffer));
}

int fputc(int ch, FILE *f) 
{
#if defined C32F0
	while (UART1->STAT.bit.TXF);	
	UART1->DAT.all = ch;
	return ch;
#elif defined MM32F031K6
	while((UART2->CSR&UART_IT_TXIEN)==0);
	UART2->TDR = (ch & (uint16_t)0x00FF);      
	return ch;
#endif
}

#if defined MM32F031K6 

void UART2_Send_BUFF( uint8_t *Str,uint8_t len)
{
	uint8_t index;
	for(index=0;index<len;index++)
	{
		
		UART_SendData(UART2, Str[index]);
		while(UART_GetFlagStatus(UART2, UART_FLAG_TXEMPTY) == RESET)   // µÈ´ý·¢ËÍ»º³åÎª¿Õ
		{
		}
		
	}	
}

#endif


void WriteUartBuf(uint8_t data)
{
	Uart0_Typedef.tx_buf[4+(Uart0_Typedef.tx_buf_len++)] = data;
}


void UART_Send_t(uint8_t Com)
{
	unsigned char Xor_verify = 0;
	unsigned char i = 0;
	Uart0_Typedef.tx_buf[0] = TX_FH;//帧头
	Uart0_Typedef.tx_buf[1] = Uart0_Typedef.tx_sequence_pag++;//包号
	Uart0_Typedef.tx_buf[2] = Uart0_Typedef.tx_buf_len+2;//长度
	Uart0_Typedef.tx_buf[3] = Com;
	
	for(i=2; i<(4+Uart0_Typedef.tx_buf_len); i++)
	{
		Xor_verify ^= Uart0_Typedef.tx_buf[i];
		
	}
	Uart0_Typedef.tx_buf[4+(Uart0_Typedef.tx_buf_len++)] = Xor_verify;//校验
	Uart0_Typedef.tx_buf[4+(Uart0_Typedef.tx_buf_len++)] = TX_FT;//帧尾
	
	#if defined C32F0
	
	for(i=0; i<(4+Uart0_Typedef.tx_buf_len); i++)
	{
		while (UART0->STAT.bit.TXF);
		UART0->DAT.bit.DATA = Uart0_Typedef.tx_buf[i];
		
	}

	Uart0_Typedef.tx_buf_len = 0;
	if(Uart0_Typedef.tx_sequence_pag >= 0xFF) Uart0_Typedef.tx_sequence_pag = 0;//清楚包号重新累加
	
	#elif defined MM32F031K6

	UART2_Send_BUFF(Uart0_Typedef.tx_buf,4+Uart0_Typedef.tx_buf_len);

	Uart0_Typedef.tx_buf_len = 0;
	if(Uart0_Typedef.tx_sequence_pag >= 0xFF) Uart0_Typedef.tx_sequence_pag = 0;//清楚包号重新累加
	
	#endif

}



static uint8_t get_len(uint8_t i)
{

	return (i+1) == BUFFER_LEN ? 0 : i+1;

}

static uint8_t get_head(uint8_t len)
{
	uint8_t data_len;
	if(Buffer_t.buffer[Buffer_t.head] == 0xFF)//前导码
	{
		udly1us(2000);
		data_len = Buffer_t.buffer[(Buffer_t.head+2)%BUFFER_LEN];
	
		if(Buffer_t.buffer[((Buffer_t.head+2+data_len +1)%BUFFER_LEN)] == 0xfe) //多判断一个包尾
		{
				data_len = Buffer_t.buffer[(Buffer_t.head+2)%BUFFER_LEN]+3;//取出数据长度 bit->3为数据长度
				if(len>data_len) return data_len; //buffer数据不够则返false 等待下一轮数据读取

		}	
		else{
		//Buffer_t.buffer[Buffer_t.head] = 0;//如果不是前导码错误则丢弃执行轮寻buffer数据
			Buffer_t.head = get_len(Buffer_t.head);//如果不是前导码错误则丢弃执行轮寻buffer数据
		}
	}else{
		//Buffer_t.buffer[Buffer_t.head] = 0;//如果不是前导码错误则丢弃执行轮寻buffer数据
		Buffer_t.head = get_len(Buffer_t.head);//如果不是前导码错误则丢弃执行轮寻buffer数据
	}
	return 0;
}



static uint8_t get_buffer_data(void)
{
	uint8_t i;
	
	i = Buffer_t.buffer[Buffer_t.head];
	Buffer_t.buffer[Buffer_t.head] = 0x00;
	Buffer_t.head = get_len(Buffer_t.head);
	return i;
}


void uart0_get_cmd(uint8_t *g_com)
{
	uint8_t valid_data;
	uint8_t packet_num;
	uint8_t Xor_verify;
	uint8_t i;
	
	valid_data = ((Buffer_t.tail+BUFFER_LEN-Buffer_t.head)%BUFFER_LEN);//计算有效数据
	if(valid_data > 5)
	{
		valid_data = get_head(valid_data);
		if(valid_data>3&&valid_data<14)
		{
			valid_data -= 3;
			Xor_verify = valid_data;
			get_buffer_data();
			packet_num = get_buffer_data();
			get_buffer_data();
			
			for(i=0; i<valid_data; i++) 
			{
				g_com[i] = get_buffer_data();
				#if defined( DeBug )
					LOG(LOG_DEBUG,"g_com[%d] = 0x%X\r\n",i,g_com[i]);
				#endif

				if((i+1) == valid_data)
				{
					if(g_com[i] == Xor_verify && get_buffer_data() == 0xFE)
					{
						#if defined( DeBug )
							LOG(LOG_DEBUG,"Xor_verifyte TRUE  packet_num = %d\r\n",packet_num);
						#endif
							WriteUartBuf(packet_num);
							WriteUartBuf(0x00);
							UART_Send_t(TX_PAG_ACK);
					}else{
						#if defined(DeBug)
							if(g_com[i] == Xor_verify) LOG(LOG_DEBUG,"-> tail失败\r\n");
							else	LOG(LOG_DEBUG,"Xor_verifyte FALSE  packet_num = %d\r\n",packet_num);	
						#endif
						
								WriteUartBuf(packet_num);
								WriteUartBuf(0x01);
								UART_Send_t(TX_PAG_ACK);
						
						memset(g_com,0,sizeof(&g_com));//清空数据	
					}
				}else{
					
					Xor_verify ^= g_com[i]; 
					
				}					
			}
			
		}
	}
}



//com rx from c600
#if defined C32F0
void UART0_IRQHandler(void)
{
	
	if( 1 == UART0->INTSTATUS.bit.TXEINT)
    {
		/*clean interrupt */
		UART0->INTSTATUS.bit.TXEINT = 1;
    }
   
   if( 1 == UART0->INTSTATUS.bit.RXNEINT)
    {
			
		Buffer_t.buffer[Buffer_t.tail]	=  UART0->DAT.bit.DATA;
		Buffer_t.tail = get_len(Buffer_t.tail);					

		/*clean interrupt */
        UART0->INTSTATUS.bit.RXNEINT = 1;
    }
    
	/*clean interrupt status */
	//UART_ClearIntFlag(UART0);
	return;
}
#elif defined MM32F031K6
void UART2_IRQHandler(void)                	//
{

  if(UART_GetITStatus(UART2, UART_IT_RXIEN))       
	{
     UART_ClearITPendingBit(UART2,UART_IT_RXIEN);
     Buffer_t.buffer[Buffer_t.tail] = UART_ReceiveData(UART2);
		 Buffer_t.tail = get_len(Buffer_t.tail);
				
  } 
//  else if(UART_GetITStatus(UART2, UART_IT_TXIEN))  //·¢ËÍÖÐ¶Ï
//	{
//		
//		UART_ClearITPendingBit(UART2,UART_IT_TXIEN);
//		Uart2_Tx_Read++;
//		
//		if(Uart2_Tx_Write != Uart2_Tx_Read)//ÊÇ·ñ·¢ËÍÍê³É						
//		{
//			UART_SendData(UART2, UART2_TxBuff[Uart2_Tx_Read]);  
//		}
//		else   //·¢ËÍÍê³É
//		{
//			
//			UART_ITConfig(UART2, UART_IT_TXIEN, DISABLE);			//
//			Uart2_Tx_Read = 0x00;
//			Uart2_Tx_Write = 0x00;
//		}
//		
//		
//	}

		
} 
#endif
