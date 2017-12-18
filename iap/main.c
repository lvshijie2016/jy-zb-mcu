


#include "config.h"
#include "user_uart.h"

extern void packet_handle();
extern uint8_t receive_complete_flag ;
extern uint8_t toal_packet_num ;
extern uint8_t has_receive_packet_num;
extern uint8_t receive_ota_flag ;
extern uint32_t flah_offset ;

typedef  void (*pFunction)(void);
static pFunction Jump_To_Application;
static uint32_t JumpAddress;

int main(void)
{	
	uint32_t time; 
	sys_init();    
	ENABLE_FLASH_CLOCK;
//	while(1)
//	{
//		packet_handle();
//		
//	}
	 while(((*(__IO uint32_t*)ApplicationAddress) == 0xFFFFFFFF) || time < 2000000)
	 {
		 
		 if (receive_complete_flag == 1)
		 {
			 packet_handle();
			 receive_complete_flag = 0;
//			 if (receive_ota_flag)
			 {
				 time = 0;
//				 receive_ota_flag = 0;
			 }
		 }
		 time++;
		 if (time >= 1999998)
		 {
			 if (has_receive_packet_num != toal_packet_num)
			 {
				 time = 0;
				 flah_offset = APPLICATION_ADDRESS;
			 }
		 }
//		 if ((toal_packet_num != 0)&&(has_receive_packet_num != 0))
//		 {
//			 if (has_receive_packet_num == toal_packet_num)
//			 {
//				 //NVIC_DisableIRQ(UART0_IRQn);
//				 time = 1500000;
//			 }
//		 }
	 }
    //进入app，两秒没有进入下载模式则进入到app中
    JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) ApplicationAddress);
    Jump_To_Application();
}



