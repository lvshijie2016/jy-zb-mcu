#include "user_uart.h"

//_Uart0_Typedef  	Uart0_Typedef;
//static 				Buffer Buffer_t;
//static 				_uart_data_typedef uart_data_typedef;
//_get_command_data 	get_command_data;

void udly1us(uint32_t dlytime) {while(dlytime--);}


void uart_print(char *str,int len);

void UART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	UART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

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
	UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE);
	UART_Cmd(UART2, ENABLE);                    

	//UART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

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


void Boot_MAL_Erase_app()      
{
	uint32_t Address;
	uint8_t index,temp;
	//   UART_Send_Buffer[0] = UART_Receive_Buffer[0] | 0xC0;
	temp = 24;  //100 K
	Address = 0x08001800;         			// APP flash   0x08004000 -- 0x08020000  0x1C000  APP起始地址
	//UART_Cmd(UART2, DISABLE); 
	FLASH_Unlock();  
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	for(index=0;index<temp;index++ )   //把需要使用到的APP空间擦除
	{   	
		FLASH_ErasePage(Address);
		Address = Address + 0x400;   
	}
	FLASH_Lock();
	//UART_Cmd(UART2, ENABLE); 
	//   UART2_Send_BUFF(UART_Send_Buffer,LEN);
}

void flash_write(unsigned char *buf, int len)
{
	uint32_t write_data;
	uint16_t i;
	uint16_t j;
	static unsigned char flag = 0;
	static uint32_t flash_offset = 0x08001800;

	if (flag == 0)
	{
	  Boot_MAL_Erase_app();
	  flag = 1;
  }

	//memset(bufs,0,sizeof(bufs));
  //sprintf(bufs,"write flash len:%d\n",len);
  //uart_print(bufs,strlen(bufs));
  //UART_Cmd(UART2, DISABLE); 

	for (j=0; (j<len && (flash_offset<FLASH_SIZE)); j+=4)
	{

	  write_data = 0;
	  write_data |= (uint32_t)buf[j];
	  write_data |= (uint32_t)buf[j+1]<<8;
	  write_data |= (uint32_t)buf[j+2]<<16;
	  write_data |= (uint32_t)buf[j+3]<<24;

	  FLASH_Unlock();
	  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	  FLASH_ProgramWord(flash_offset,write_data);
	  FLASH_Lock();  //

	  flash_offset += 4;
  }

	//UART_Cmd(UART2, ENABLE); 
  //memset(bufs,0,sizeof(bufs));
  //sprintf(bufs,"**write flash len:%d start: %02x %02x\n",len,buf[0],buf[len-1]);
  //uart_print(bufs,strlen(bufs));

}

#endif



static void UART1_Init(void)
{
#if defined( DeBug )
	SYS_EnablePhrClk(AHB_UART1);  
	get_adc_gpio(IOCON_GPIOA,PIN14,PA14_FUNC_TXD1,PULL_UP_EN);
	UART_Open(UART1, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);
#endif
}



enum uart_rx_status{
	PACKET_START = 0,
	PACKET_NUM,
	PACKET_LEN,
	PACKET_LEN_LOW,
	PACKET_CMD,
	PACKET_DATA,
	PACKET_VERIFY,
	PACKET_END
};



static unsigned char uart_rx_buf[1300];
static unsigned char uart_pkt[1100];

int rx_start = 0;
int rx_stop = 0;

unsigned char total_pkt;
unsigned char current_pkt = 0;

void rx_start_set(len)
{
	if(rx_start+len >= sizeof(uart_rx_buf)){
		rx_start = (rx_start+len)-sizeof(uart_rx_buf);
	}else{
		rx_start += len;
	}
}

int rx_buf_len_get(void)
{
	int len;
	if(rx_stop < rx_start){
		len = sizeof(uart_rx_buf) - rx_start + rx_stop;
	}else{
		len = rx_stop - rx_start;
	}
	return len;
}

void copy_buf(void){
	if(rx_stop < rx_start){
		int len = sizeof(uart_rx_buf) - rx_start;
		memcpy(uart_pkt,&uart_rx_buf[rx_start],len);
		memcpy(uart_pkt+len,&uart_rx_buf[0],rx_stop);
	}else{
		memcpy(uart_pkt,&uart_rx_buf[rx_start],rx_stop-rx_start);
	}
}

struct uart_packet{
	unsigned char start;
	unsigned char packet_count;
	unsigned char len;
	unsigned char cmd;
	unsigned char data;
};

/*包头1   序列号1 长度1      功能1 数据N 校验1 帧尾1 (6+N) (2+N)*/
//异或检验
static char get_checkout_result(char *data,int len)
{
	char result;
	int i;
	if( ( data == NULL ) || (len <= 1 ) )
	{
	  return 0;
  }
	result = data[0] ^ data[1];
	for(i=2;i<len;i++)
	{
	  result ^= data[i]; 
  }
	//printf("checkout:%02x %02x\n",data[0],data[len-1]);
	return result;
}

static unsigned char uart_pkt_count = 0;
void uart_pkt_send_f1()
{
	char buf[32];
	struct uart_packet *pkt;

	pkt = (struct uart_packet*)buf;
	pkt->start = 0xff;
	pkt->packet_count = uart_pkt_count++;
	pkt->len = 0x2;
	pkt->cmd = 0xf1;
	buf[4] = get_checkout_result(&pkt->len,2);
	buf[5] = 0xfe;

	UART2_Send_BUFF(buf,6);
}

void uart_pkt_send_f2(unsigned char packet_num,unsigned char flag)
{
	char buf[32];
	struct uart_packet *pkt;

	pkt = (struct uart_packet*)buf;
	pkt->start = 0xff;
	pkt->packet_count = uart_pkt_count++;
	pkt->len = 0x4;
	pkt->cmd = 0xf2;
	pkt->data = packet_num;
	buf[5] = flag;
	buf[6] = get_checkout_result(&pkt->len,4);
	buf[7] = 0xfe;

	UART2_Send_BUFF(buf,8);
}

void uart_pkt_send_f3(unsigned char flag)
{
	char buf[32];
	struct uart_packet *pkt;

	pkt = (struct uart_packet*)buf;
	pkt->start = 0xff;
	pkt->packet_count = uart_pkt_count++;
	pkt->len = 0x3;
	pkt->cmd = 0xf3;
	pkt->data = flag;
	buf[5] = get_checkout_result(&pkt->len,3);
	buf[6] = 0xfe;

	UART2_Send_BUFF(buf,7);
}

void uart_print(char *str,int len)
{
	char buf[32];
	struct uart_packet *pkt;
	pkt = (struct uart_packet*)buf;
	pkt->start = 0xff;
	pkt->packet_count = uart_pkt_count++;
	pkt->len = len + 2;
	pkt->cmd = 0xf;
	memcpy(&pkt->data,str,len);
	buf[len+4] = get_checkout_result(&pkt->len,len+2);
	buf[len+5] = 0xfe;
	UART2_Send_BUFF(buf,len+6);
}

extern unsigned char update_down_flag;
void packet_hd(void)
{
	struct uart_packet *pkt;
	int buf_len;	
	int pkt_len;
	int i;
	int len;
	unsigned char *data;
	unsigned char xer = 0;
	unsigned char *tmp = uart_pkt;
	char buf[32];


	buf_len = rx_buf_len_get();
	while(buf_len >= 5){
		if(uart_rx_buf[rx_start] == 0xff){
			//printf("receive start, len:%d,rx_start:%d %d\n",buf_len,rx_start,rx_stop);
			copy_buf();
			pkt =(struct uart_packet*) &tmp[0];
			if(pkt->cmd == 0xf2){
				pkt_len = (pkt->len<<8) + pkt->data;
				//printf("f2 len :%d %02x %02x %d\n",pkt_len,pkt->len,pkt->data,(pkt->len<<8));
		// 如果报文长度小于1100
				if(pkt_len > 1100){
					rx_start_set(1);
					buf_len = rx_buf_len_get();
					continue;
				}
			}else{			
				pkt_len = pkt->len;
				if(pkt_len > 100){
					rx_start_set(1);
					buf_len = rx_buf_len_get();
					continue;
				}
			}
			//printf("cmd:%x,len:%d\n",pkt->cmd,pkt_len);
	  // 长度不够包长，退出函数，等待新数据
			if(buf_len < pkt_len+3+3)
				return;

			// 判断结束标记
			if(pkt->cmd == 0xf2){
				xer = (tmp[pkt_len+2+2+1] != 0xfe);
				//printf("f2 end: %02x\n",tmp[pkt_len+2+2+1]);
			}else{
				xer = (tmp[pkt_len+2+2] != 0xfe);
			}
			if(xer){
				rx_start_set(1);
				buf_len = rx_buf_len_get();
				continue;
			}


			// 校验
			if(pkt->cmd == 0xf2){
				xer = get_checkout_result(&tmp[2],pkt_len+2);
				//printf("xet:: %02x %02x\n",xer,tmp[pkt_len+4]);
				xer = (xer != tmp[pkt_len+4]);


			}else{
				xer = get_checkout_result(&tmp[2],pkt_len+1);
				xer = (xer != tmp[pkt_len+3]);
			}
			//printf("xer:%x\n",xer);

			if(xer){
				rx_start_set(1);
				buf_len = rx_buf_len_get();
				//printf("no xer cmd:%x \n",pkt->cmd);
				continue;
			}

			//printf("receive command :%02x \n",pkt->cmd);
			switch(pkt->cmd){
				case 0xf1:
					total_pkt = tmp[4];
					//current_pkt = 0;
					uart_pkt_send_f1();
					break;
				case 0xf2:
					if(current_pkt+1 != tmp[5]){
						if(current_pkt+1 > tmp[5])
							uart_pkt_send_f2(tmp[5],0);
						else{
							uart_pkt_send_f2(current_pkt+1,0x2);
						}
						break;
					}
					current_pkt++;
					flash_write(&tmp[6],pkt_len-2);
					if(current_pkt == total_pkt){
						uart_pkt_send_f2(tmp[5],0);
						uart_pkt_send_f3(0);
						FLASH_Unlock();  
						FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
						FLASH_ErasePage(0x08007800);
						FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
						FLASH_ProgramWord(0x8007800,0x55aaaa55);
						FLASH_Lock(); 

						update_down_flag = 2;
					}else{
						update_down_flag = 1;
						uart_pkt_send_f2(tmp[5],0);
					}
					break;
			}

			rx_start_set(pkt_len+2);

		}else{
			rx_start_set(1);
			buf_len = rx_buf_len_get();
		}
	}

}

void UART2_IRQHandler(void)                	//
{

	if(UART_GetITStatus(UART2, UART_IT_RXIEN))       
	{
	  UART_ClearITPendingBit(UART2,UART_IT_RXIEN);
	  uart_rx_buf[rx_stop++] = UART_ReceiveData(UART2);
	  if(rx_stop >= sizeof(uart_rx_buf)){
		  rx_stop = 0;
	  }

  } 
}

int fputc(int ch, FILE *f) 
{
	while((UART2->CSR&UART_IT_TXIEN)==0);
	UART2->TDR = (ch & (uint16_t)0x00FF);      
	return ch;

}





