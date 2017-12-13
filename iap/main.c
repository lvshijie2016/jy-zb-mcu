


#include "config.h"
#include "user_uart.h"

extern void packet_handle();
extern uint8_t receive_complete_flag ;

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
	 while(((*(__IO uint32_t*)ApplicationAddress) == 0xFFFFFFFF) || time < 3000000)
	 {
		 if (receive_complete_flag == 1)
		 {
			 packet_handle();
			 receive_complete_flag = 0;
			 time = 0;
		 }
		 time++;
	 }
    //进入app，两秒没有进入下载模式则进入到app中
    JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) ApplicationAddress);
    Jump_To_Application();
}



