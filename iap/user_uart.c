#include "user_uart.h"

_Uart0_Typedef  	Uart0_Typedef;
//static 				Buffer Buffer_t;
static 				_uart_data_typedef uart_data_typedef;
//_get_command_data 	get_command_data;

void udly1us(uint32_t dlytime) {while(dlytime--);}

void get_packet(void);

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
	
//	memset(&Buffer_t,0,sizeof(Buffer));

}


static void UART1_Init(void)
{
	#if defined( DeBug )
		SYS_EnablePhrClk(AHB_UART1);  
		get_adc_gpio(IOCON_GPIOA,PIN14,PA14_FUNC_TXD1,PULL_UP_EN);
		UART_Open(UART1, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);
	#endif
}



int fputc(int ch, FILE *f) 
{
    while (UART1->STAT.bit.TXF);	
    UART1->DAT.all = ch;
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

#if 0

static uint8_t get_len(uint16_t i)
{

	return (i+1)%BUFFER_LEN ;

}


static uint8_t get_buffer_data(void)
{
	uint8_t i;
	
	i = Buffer_t.buffer[Buffer_t.head];
	Buffer_t.buffer[Buffer_t.head] = 0x00;
	Buffer_t.head = get_len(Buffer_t.head);
	return i;
}







//static bool Xor_verify_data(void)
//{
//	
//	uint16_t 	flag_head;
//	uint8_t		Xor_verify;
//	uint16_t 	i;
//	
//	flag_head = Buffer_t.head;
//	Buffer_t.head += 2; 
//	
//	for(i=0; i<Buffer_t.len; i++)//数据长度 
//	{
//		Xor_verify^=get_buffer_data();  //校验数据
//		if((i+1) == Buffer_t.len)
//		{
//			
//			if(Xor_verify == get_buffer_data())
//			{
//				Buffer_t.head = flag_head;
//				return true;

//			}else{
//				
//				Buffer_t.head = flag_head;
//				return false;
//			}	
//		}
//	}
//	return false;
//}




//void IAP_erase_flash(void)
//{
//	uint8_t i;
//	uint32_t addr_base   = 0x2000;
//	uint32_t sector_size = 0x6000;
//	
//	
//	for(i=0; i< (sector_size/512); i++)
//	{
//		
//		IAP_FlashErase(addr_base);
//		IAP_FlashErase(addr_base+0x200);
//		addr_base += 0x200;
//		
//	}
//}


//void IAP_write_data(void)
//{
//	uint16_t 		i;
//	uint32_t 		data = 0;
//	static uint32_t addr_offesr = 0;
//	uint32_t 		addr_base  = 0x2000;
//	uint16_t 		flag_head;
//	
//	flag_head = Buffer_t.head;
//	Buffer_t.len -= 6;
//	Buffer_t.head = Buffer_t.head+6;
//	
//	for(i=0; i<Buffer_t.len;)
//	{
//		//i = Buffer_t.buffer[Buffer_t.head];
//		//Buffer_t.buffer[Buffer_t.head] = 0x00;
//		//Buffer_t.head = get_len(Buffer_t.head);
//		data = 0;
//		
//		if(i<Buffer_t.len) data = get_buffer_data();
//		i++;
//		if(i<Buffer_t.len) data |= get_buffer_data()<<8;
//		i++;
//		if(i<Buffer_t.len) data |= get_buffer_data()<<16;
//		i++;
//		if(i<Buffer_t.len) data |= get_buffer_data()<<24;
//		i++;
//		IAP_FlashProgram(addr_base+addr_offesr,data);
//		
//		addr_offesr += 4;
//	}
//	if(Buffer_t.packet_num )
//	Buffer_t.head = flag_head;
//}



void get_packet(void)  //确认是否 有效包
{
	uint16_t 	len;
	if(get_command_data.flag)return;
	if(Buffer_t.buffer[Buffer_t.head] != 0xFF  && uart_data_typedef.head != 0xFF )//查找前导码
	{
		//Buffer_t.buffer[Buffer_t.head] = 0;//如果不是前导码错误则丢弃执行轮寻buffer数据
		Buffer_t.head = get_len(Buffer_t.head);//如果不是前导码错误则丢弃执行轮寻buffer数据
		return;
	}
	udly1us(100*1000);	
	//计算有效数据
	if(((Buffer_t.tail+BUFFER_LEN-Buffer_t.head)%BUFFER_LEN) < 6) return;

	uart_data_typedef.head  	 	= get_buffer_data();//移去前导码
	uart_data_typedef.packet_num 	= get_buffer_data();//取出包号
	uart_data_typedef.len	     	= get_buffer_data();//取出数据高8位长度
	uart_data_typedef.command = get_buffer_data();
	if (uart_data_typedef.command == 0xF2)
		uart_data_typedef.low_len	    = get_buffer_data();//取出数据低8位长度 
	
	uart_data_typedef.xor_verify	= uart_data_typedef.len^uart_data_typedef.low_len; //加入校验
	if (uart_data_typedef.command == 0xF2)
		uart_data_typedef.len 			= (uart_data_typedef.len <<8)|uart_data_typedef.low_len; //合成到16位数据
	
	if(uart_data_typedef.len > BUFFER_LEN)  //长度大于BUF有效数据错误
	{
		//memset(Buffer_t.buffer,0,sizeof(&Buffer_t.buffer));//BUF错误清空数据
		memset(&uart_data_typedef,0,sizeof(uart_data_typedef));
		WriteUartBuf(uart_data_typedef.packet_num);
		WriteUartBuf(0x01);
		UART_Send_t(TX_PAG_ACK); //接收失败
		return;
	}

}

uint8_t packet_num =1;

void get_data(void)
{
	uint16_t 	i;
	uint8_t 	b = 1;
	if(get_command_data.flag)	return;
	if( uart_data_typedef.head != 0xFF)return;
	//计算有效数据
	//uart_data_typedef.len 长度为 指令+数据+校验  另须加+帧尾
	if(((Buffer_t.tail+BUFFER_LEN-Buffer_t.head)%BUFFER_LEN) < uart_data_typedef.len+1)return;  //COM-FE长度
	if( Buffer_t.buffer[(uart_data_typedef.len+Buffer_t.head)%BUFFER_LEN] != 0xFE) //指令+数据+校验+1=FE位 读取帧尾 判断是否完整包
	{
		//帧尾错误
		memset(&Buffer_t,0,sizeof(Buffer));
		WriteUartBuf(packet_num);
		WriteUartBuf(0x01);
		UART_Send_t(TX_OTA_DATA_ACK);
		memset(&uart_data_typedef,0,sizeof(uart_data_typedef));
		return;
	}
	get_command_data.data_buffer[0] = uart_data_typedef.command;
	uart_data_typedef.xor_verify ^=get_command_data.data_buffer[0];
	for(i=1; i<uart_data_typedef.len; i++)//取数据校验数据 长度为 = 指令+数据+校验 
	{
		get_command_data.data_buffer[i] = get_buffer_data();//取出数据
		uart_data_typedef.xor_verify ^= get_command_data.data_buffer[i];//校验数据
		if((i+1) == uart_data_typedef.len)//减去 指令+低位长度+帧尾 =校验位
		{
//			memset(&uart_data_typedef,0,sizeof(uart_data_typedef));
			if(uart_data_typedef.xor_verify == get_buffer_data())
			{
				memset(&Buffer_t,0,sizeof(Buffer));
				//校验成功
				if (uart_data_typedef.command == 0xF1)
					UART_Send_t(TX_OTA_ACK);
				else 
					{
						WriteUartBuf(packet_num);
						WriteUartBuf(0x00);
						UART_Send_t(TX_OTA_DATA_ACK);
					}
				memset(&uart_data_typedef,0,sizeof(uart_data_typedef)); //清除状态
				
//				get_command_data.flag = true; //BUF 满标

			}else{
				
				memset(&Buffer_t,0,sizeof(Buffer));
				//校验失败
				if (uart_data_typedef.command == 0xF2)
				{
					WriteUartBuf(packet_num);
					WriteUartBuf(0x01);
					UART_Send_t(TX_OTA_DATA_ACK);
				}
				memset(&uart_data_typedef,0,sizeof(uart_data_typedef));
				memset(&get_command_data,0,sizeof(get_command_data)); //清空BUF
				
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

#endif

#if 1

typedef enum state {
	recv_start,
	packet_num,
	len,
	len_high,
	command,
	len_low,
	frame_number,
	state_xor_verify,
	valid_data,
	recv_end
}_serial_state;

typedef struct recv_data_config {
	uint8_t recv_packet_num;
	uint8_t recv_command;
	uint8_t recv_len;
	uint16_t recv_data_len;
	uint8_t recv_len_low;
	uint8_t total_packet_num;
	uint8_t now_packet_num;
	uint8_t recv_xor_verify;
	uint8_t recv_stop;
	uint8_t recv_update_data[1026];
}_recv_data_config;


uint8_t recv_data = 0;
uint8_t recv_update_flag = 0;
uint8_t receive_complete_flag = 0;
uint16_t recv_ptr = 0;
uint8_t recv_state = recv_start;

uint8_t toal_packet_num = 0;
uint8_t has_receive_packet_num = 0;

_recv_data_config packet_data;


uint8_t XorVerify(void)
{
	uint8_t result;
	result ^= packet_data.recv_command;
	result ^= packet_data.recv_len;
	result ^= packet_data.total_packet_num;
	return result;
}

void reset_config()
{
	memset(&packet_data,0,sizeof(_recv_data_config));
	recv_state = recv_start;
	receive_complete_flag = 0;
	recv_ptr = 0;

}

//	uint32_t RamSource; 
//	
		uint8_t flag = 0;
//	uint8_t buf[40] = {0};
		uint32_t flah_offset = APPLICATION_ADDRESS;   
//	uint32_t flah_offset_erase = (0x2000);//需要擦除的起始地址

void flash_write()
{
	uint32_t write_data;
	uint16_t i;
	uint16_t j;
	#if 0
	static uint32_t addr_offesr = 1020;
	uint32_t addr_base   = 0x7C00;
	uint32_t data        = 0;
	uint32_t sector_size = 1024;
	IAP_FlashProgram(addr_base+addr_offesr,0x55aaaa55);
	data = *((uint32_t *)(addr_base+addr_offesr));
	addr_offesr += 4;
	if(sector_size <= addr_offesr)
		{
			addr_offesr = 0;
			IAP_FlashErase(addr_base);
      IAP_FlashErase(addr_base+512);   
			data = *((uint32_t *)(addr_base+addr_offesr));
		}	
		
	#endif
	
//	if (flag == 0)
//	{
//		if (flah_offset_erase < FLASH_SIZE)
//		 {
//			 IAP_FlashErase(flah_offset_erase);
//			 flah_offset_erase+=512;
//		 }
//		 flag = 1;
//	}
//	RamSource = ( uint32_t)packet_data.recv_update_data;
	if (flag == 0)
	{
		for (i=0; i<FLASH_IMAGE_SIZE; i+=512)
		{
			IAP_FlashErase(ApplicationAddress+i);
		}
		flag = 1;
	}
	if ((packet_data.recv_data_len-2)%4)
	{
		
	}
  for (j=1; (j<packet_data.recv_data_len-2-3) && (flah_offset<FLASH_SIZE); j+=4)
   {

		 write_data = (uint32_t)packet_data.recv_update_data[j];
		 write_data |= (uint32_t)packet_data.recv_update_data[j+1]<<8;
		 write_data |= (uint32_t)packet_data.recv_update_data[j+2]<<16;
		 write_data |= (uint32_t)packet_data.recv_update_data[j+3]<<24;
		 IAP_FlashProgram(flah_offset,write_data);
     flah_offset += 4;
   }


}

void data_handle(uint8_t data)
{
	uint16_t data_length;
	switch (recv_state)
	{
		case recv_start:
			if (data == 0xFF)
			{
				recv_state = frame_number;
			}
			break;
		case frame_number:
			packet_data.recv_packet_num = data;
			recv_state = len;
			break;
		case len:
			if (data <=4)
			{
				packet_data.recv_len = data;
				recv_state = command;
			}
			else
				reset_config();
			break;
		case len_low:
			packet_data.recv_len_low = data;
			packet_data.recv_data_len = (uint16_t)(packet_data.recv_len<<8)|packet_data.recv_len_low ;
			recv_state = valid_data;
			break;
		case command:
			packet_data.recv_command = data;
			if (packet_data.recv_command == 0xF1)
				{
					recv_state = valid_data;
					recv_update_flag = 1;			
				}
			else if (packet_data.recv_command == 0xF2)
				{
					recv_state = len_low;
				}
			break;
//		case packet_num:
//			if (recv_update_flag)
//			{
//				recv_state = xor_verify;
//				packet_data.total_packet_num = data;
//			}
//			else
//			{
//				recv_state = valid_data;
//				packet_data.now_packet_num = data;
//			}
//			break;
		case state_xor_verify:
			packet_data.recv_xor_verify = data;
			recv_state = recv_end;
			break;
		case recv_end:
			packet_data.recv_stop = data;
			if (packet_data.recv_stop == 0xFE)
			{
				receive_complete_flag = 1;
					
			}
			else
				reset_config();
			break;
		case valid_data:
			if (packet_data.recv_command == 0xF1)
				data_length = packet_data.recv_len;
			else if(packet_data.recv_command == 0xF2)
				data_length = packet_data.recv_data_len;
			if (data_length !=0)
			{
				packet_data.recv_update_data[recv_ptr] = data;
				recv_ptr++;

				if (recv_ptr > data_length-2)
				{
					recv_state = state_xor_verify;
	//			recv_ptr = 0;
				}
			}
			else
				reset_config();
			break;
	}
}



void packet_handle()
{
	uint8_t xor_verify = 0;
	uint8_t data_xor_verify = 0;
	uint16_t i = 0;
	if (receive_complete_flag == 1)
	{
		data_xor_verify ^= packet_data.recv_command;
		if (packet_data.recv_command == 0xF1)
			data_xor_verify ^= packet_data.recv_len;
		else
		{
			data_xor_verify ^= packet_data.recv_len;
			data_xor_verify ^= packet_data.recv_len_low;
		}
			
		for (i=0;i<recv_ptr;i++)
		{
			data_xor_verify ^= packet_data.recv_update_data[i];
		}
		if (data_xor_verify == packet_data.recv_xor_verify)
		{
			if (packet_data.recv_command == 0xF1)
			{
				 toal_packet_num= packet_data.recv_update_data[0];
				UART_Send_t(TX_OTA_ACK);
			}
			else if (packet_data.recv_command == 0xF2)
			{
				has_receive_packet_num = packet_data.recv_update_data[0];
				WriteUartBuf(packet_data.recv_update_data[0]);
				WriteUartBuf(0x00);
				UART_Send_t(TX_OTA_DATA_ACK);
				flash_write();
				reset_config();
				if (has_receive_packet_num == toal_packet_num)
				{
					WriteUartBuf(0x00);
					UART_Send_t(TX_OTA_DATA_ACK);
				}
			}
			reset_config();
		}
		else
		{
			if (packet_data.recv_command == 0xF1)
			{
				WriteUartBuf(0x01);
				UART_Send_t(TX_OTA_ACK);
			}
			else if (packet_data.recv_command == 0xF2)
			{
				WriteUartBuf(packet_data.recv_update_data[0]);
				WriteUartBuf(0x01);
				UART_Send_t(TX_OTA_DATA_ACK);
			}
			reset_config();
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
			
				recv_data =  UART0->DAT.bit.DATA;			

				data_handle(recv_data);
		/*clean interrupt */
        UART0->INTSTATUS.bit.RXNEINT = 1;
    }
    
	/*clean interrupt status */
	//UART_ClearIntFlag(UART0);
	return;
}

#endif


