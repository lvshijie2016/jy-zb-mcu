<<<<<<< HEAD



#include "config.h"
#include "user_uart.h"

extern void packet_handle();


int main(void)
{	

	sys_init();    

	ENABLE_FLASH_CLOCK;
	while(1)
	{
//	get_packet();
//	get_data();
		packet_handle();
		
	}
	
    //����app������û�н�������ģʽ����뵽app��
    JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) ApplicationAddress);
    Jump_To_Application();
}



=======
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
	
    //����app������û�н�������ģʽ����뵽app��
    JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) ApplicationAddress);
    Jump_To_Application();
}
>>>>>>> 92867606d15fd17a2514ea9e7883b4cd0d005f2b
