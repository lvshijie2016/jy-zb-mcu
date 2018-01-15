#include "user_uart.h"


_Uart0_Typedef  Uart0_Typedef;


static Buffer Buffer_t;
void udly1us(uint32_t dlytime) {while(dlytime--);}


//com for c600
#if defined C32F0
void UART0_Init(void)
#elif defined MM32F031K6
void UART1_Init(void)
#endif
{
#if defined C32F0
	
	GPIO_InitTypeDef uart0_gpio;
	SYS_EnablePhrClk(AHB_IOCON);
	SYS_EnablePhrClk(AHB_GPIOA);    
	SYS_EnablePhrClk(AHB_UART0);  

	uart0_gpio.bit.PDE    = PULL_DOWN_DISABLE;
	uart0_gpio.bit.PUE    = PULL_UP_ENABLE;
	
	uart0_gpio.bit.CSE    = SCHMITT_ENABLE;
	uart0_gpio.bit.INV    = INPUT_INVERT_DISABLE;
	uart0_gpio.bit.SRM    = FAST_SLEW_RATE_MODE;
	uart0_gpio.bit.ADM    = DIGIT_ENABLE;
	uart0_gpio.bit.DRV    = LOW_DRIVE;
	uart0_gpio.bit.OD     = OPEN_DRAIN_DISABLE;
	uart0_gpio.bit.S_MODE = INPUT_FILTER_DISABLE;
	uart0_gpio.bit.IEN    = INPUT_DISABLE;
	
	uart0_gpio.bit.FUNC   = PA2_FUNC_TXD0;
	SYS_IOCONInit(IOCON_GPIOA, PIN2, uart0_gpio);
	
	uart0_gpio.bit.FUNC   = PA3_FUNC_RXD0;
	SYS_IOCONInit(IOCON_GPIOA,PIN3,uart0_gpio);
	
	UART_Open(UART0, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);  
	NVIC_SetPriority(UART0_IRQn,0);
	NVIC_EnableIRQ(UART0_IRQn); 
	
#elif MM32F031K6
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	
#ifdef Debug	
    
    
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
    
    
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
    NVIC_Init(&NVIC_InitStructure);	
    
    
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
    
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	
    
    UART_Init(UART1, &UART_InitStructure);      
    UART_ITConfig(UART1, UART_IT_RXIEN|UART_IT_TXIEN, ENABLE);
    UART_Cmd(UART1, ENABLE);                     
    
    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //UART1_RX	  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
#endif


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
    

    NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;		
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
    NVIC_Init(&NVIC_InitStructure);	
    
  
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_1);
    
    UART_InitStructure.UART_BaudRate = 115200;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	
    
    UART_Init(UART2, &UART_InitStructure); 
    UART_ITConfig(UART2, UART_IT_RXIEN|UART_IT_TXIEN, ENABLE);
    UART_Cmd(UART2, ENABLE);                    
    
    //UART2_TX   GPIOA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

#endif
	
	memset(&Buffer_t,0,sizeof(Buffer));
}

int fputc(int ch, FILE *f) 
{
#if defined C32F0
    while (UART1->STAT.bit.TXF);	
    UART1->DAT.all = ch;
    return ch;
#elif defined MM32F031K6
	
	
	
#endif
}

#if defined MM32F031K6 

void UART_PutChar(UART_TypeDef* UARTx, uint8_t Data)  
{  
    UART_SendData(UART2, Data);  
    while(UART_GetFlagStatus(UART1, UART_FLAG_TXEMPTY) == RESET){}  
}  
void UART_PutStr (UART_TypeDef* UARTx, uint8_t *str)    
{    
    while (0 != *str)    
    {    
        UART_PutChar(UART2, *str);    
        str++;    
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

	UART_PutStr(UART1,Uart0_Typedef.tx_buf);	

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
		if(valid_data)
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