


#include "config.h"
#include "user_uart.h"
#include "HAL_conf.h"



extern void Boot_MAL_Erase_app();

typedef  void (*pFunction)(void);
static pFunction Jump_To_Application;
static uint32_t JumpAddress;

unsigned char update_down_flag = 0;

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

//GPIO_SetBits(GPIOA,GPIO_Pin_15);
	if((*(uint32_t*)(0x8007800)) == 0xaa5555aa || (*(__IO uint32_t*)ApplicationAddress) == 0xFFFFFFFF){
	//if(1){
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
		while(time < 2000000)
		{
				time++;
				packet_hd();
				
				if(update_down_flag == 2){
					break;
				}else if(update_down_flag == 1){
					update_down_flag = 0;
					time = 0;
				}
				time = 0;
		}
	}else{
		//GPIO_ResetBits(GPIOA,GPIO_Pin_15);
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



