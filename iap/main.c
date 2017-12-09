


#include "config.h"
#include "user_uart.h"

extern uint8_t recv_update_hand_flag;


int main(void)
{	

	sys_init();    

	ENABLE_FLASH_CLOCK;
	while(1)
	{
		get_packet();
		get_data();
		
//		if (recv_update_hand_flag == 1)
//			{
//				WriteUartBuf(0);
//				WriteUartBuf(0x00);
//				UART_Send_t(TX_PAG_ACK);
//				recv_update_hand_flag = 0;
//			}
		
	}
	
    //进入app，两秒没有进入下载模式则进入到app中
    JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) ApplicationAddress);
    Jump_To_Application();
}



