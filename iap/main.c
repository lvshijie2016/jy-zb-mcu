#include "config.h"
#include "user_uart.h"


int main(void)
{	
   
	sys_init();
	get_packet();
	get_data();
	ENABLE_FLASH_CLOCK;
	
	while(1)
	{
		get_packet();
		get_data();
	}
	
    //进入app，两秒没有进入下载模式则进入到app中
    JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) ApplicationAddress);
    Jump_To_Application();
}