#include "user_uart.h"

_Uart0_Typedef  Uart0_Typedef;
static Buffer Buffer_t;
void UART0_Init(void)
{
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

}

int fputc(int ch, FILE *f) 
{
    while (UART0->STAT.bit.TXF);	
    UART0->DAT.all = ch;
    return ch;
}


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
	
	
	for(i=0; i<(4+Uart0_Typedef.tx_buf_len); i++)
	{
		while (UART0->STAT.bit.TXF);
		UART0->DAT.bit.DATA = Uart0_Typedef.tx_buf[i];
		
	}

	Uart0_Typedef.tx_buf_len = 0;
	if(Uart0_Typedef.tx_sequence_pag >= 0xFF) Uart0_Typedef.tx_sequence_pag = 0;//清楚包号重新累加

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
		data_len = Buffer_t.buffer[Buffer_t.head+2]+3;//取出数据长度 bit->3为数据长度
		if(len>data_len) return data_len; //buffer数据不够则返false 等待下一轮数据读取
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
	if(valid_data > 6)
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
					//LOG(LOG_DEBUG,"g_com[%d] = 0x%X\r\n",i,g_com[i]);
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









