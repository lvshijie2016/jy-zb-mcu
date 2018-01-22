


#include "config.h"
#include "user_uart.h"
#include "HAL_conf.h"

extern void packet_handle();
extern uint8_t receive_complete_flag ;
extern uint8_t toal_packet_num ;
extern uint8_t has_receive_packet_num;
extern uint8_t receive_ota_flag ;
extern uint32_t flah_offset ;

extern void Boot_MAL_Erase_app();

typedef  void (*pFunction)(void);
static pFunction Jump_To_Application;
static uint32_t JumpAddress;

uint8_t *MAL_Read (uint32_t SectorAddress, uint32_t DataLength)	//??Flash??
{
	return (uint8_t*)SectorAddress;
}


int main(void)
{	
	 uint32_t  data; 
	#if 1
	uint32_t time = 0; 
	sys_init();    
//	ENABLE_FLASH_CLOCK;
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
	 
	 
	 #endif
	 

	
  //???????????
	 
	 
//	Boot_MAL_Erase_app();
//  	
//  FLASH_Unlock();
//  FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);  //??flash
//  
//  FLASH_ProgramWord(0x8004800, 0x55aaaa55);    //program
//  
//  FLASH_Lock();   //????flash  
//  
//  data  = *((uint32_t *)(0x4800));
	 
	 
	 
    //进入app，两秒没有进入下载模式则进入到app中
				/* Jump to user application */ 
				JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
				Jump_To_Application = (pFunction) JumpAddress;

				/* Initialize user application's Stack Pointer */ 
				__set_MSP(*(__IO uint32_t*) ApplicationAddress);
				
				//Jump to APP
				Jump_To_Application();	
}



