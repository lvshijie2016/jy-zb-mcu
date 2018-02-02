#include "config.h"


static void ADC1_SoftStart(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
}

void adc_value_get(uint8_t *vaild,uint16_t *value)
{
	uint32_t         			adValue_t;

	ADC1_SoftStart();
	adValue_t = ADC1->ADDR4;
	if(adValue_t & (1<<21)){
		vaild[0] = 1;
		value[0] = adValue_t & 0xfff;
	}
	
	adValue_t = ADC1->ADDR1;
	if(adValue_t & (1<<21)){
		vaild[1] = 1;
		value[1] = adValue_t & 0xfff;
	}

	adValue_t = ADC1->ADDR6;
	if(adValue_t & (1<<21)){
		vaild[2] = 1;
		value[2] = adValue_t & 0xfff;
	}
}

static void app_MAL_Erase()      
{
	uint32_t Address;
	uint8_t index,temp;
	//   UART_Send_Buffer[0] = UART_Receive_Buffer[0] | 0xC0;
	temp = 2;  //100 K
	Address = IAP_UPDATE_FLAG_ADDR;         			
	FLASH_Unlock();  
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	for(index=0;index<temp;index++ )   //???????APP????
	{   	
		FLASH_ErasePage(Address);
		Address = Address + 0x400;   
	}
	FLASH_Lock();
	//   UART2_Send_BUFF(UART_Send_Buffer,LEN);
}

extern _GetLedComData_t GetLedComData_t;
extern _KAR_STATE kar_state;
extern _KAR_STATE kar_state_t;
void sys_start(void)
{
	if ((*((uint32_t *)(0x8007800))) != 0x55aaaa55)
	{
		if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) == SET||RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
		{
			kar_state_t =  KAR_RUN;
			kar_state   =  KAR_RUN;
			POWER_ON;
			GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
			RCC_ClearFlag();
		}
		else
		{
			kar_off();//进入睡眠
		}
	}
	else
	{
		kar_state_t =  KAR_RUN;
		kar_state   =  KAR_RUN;
		POWER_ON;
		GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
		
		app_MAL_Erase();
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		FLASH_ProgramWord(0x8007800,0);
		FLASH_Lock();   //
	}
	
	Rtc_Check();
}



/********************************************************************************************************
**函数信息 ：PVU_CheckStatus(void)                       
**功能描述 ：检查独立看门狗预分频位状态
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void PVU_CheckStatus(void)
{
    while(1)
    {
        /*检查预分频位状态,为RESET才可改变预分频值*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET)                                                       
        {
            break;
        }
    }
}
/********************************************************************************************************
**函数信息 ：RVU_CheckStatus(void)                  
**功能描述 ：检查独立看门狗重载标志
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void RVU_CheckStatus(void)
{
    while(1)
    {
        /*检查重载标志状态*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET)  
        {
            break;
        }
    }
}



/********************************************************************************************************
**函数信息 ：Write_Iwdg_PR(void)             
**功能描述 ：启动独立看门狗
**输入参数 ：IWDG_Prescaler 可选IWDG_Prescaler_X, X为4,8,16,32,64,128,256,对应分频值与X取值相同
Reload<=0xfff,为计数器重载值
**输出参数 ：无
**    备注 ：复位时常计算已LSI 40KHz为参考
Tiwdg=(X/LSI)*Reload
********************************************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler,unsigned short int Reload)
{
    /*启动内部低速时钟,等待时钟就绪*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    
    /*设置时钟预分频*/	
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);
    
    /*设置重载寄存器值*/	
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload&0xfff);	
    
    /*装载并使能计数器*/	
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/********************************************************************************************************
**函数信息 ：void uni_wdt_reload(void)           
**功能描述 ：喂狗函数
**输入参数 ：
**输出参数 ：无
********************************************************************************************************/

void uni_wdt_reload(void)
{
    IWDG_ReloadCounter();
}

static void Sys_Stop(void)
{  
	// 设置UART口为高阻输入，减少休眠状态电流。
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1,DISABLE);	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;	
    //GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR外设时钟
		//RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, DISABLE);	
		//UART_ITConfig(UART2, UART_IT_RXIEN, DISABLE);
  //  PWR_WakeUpPinCmd(ENABLE);  //使能唤醒管脚功能
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI); 

    PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);	  //进入停机（STOP）模式 
}

void LowPowerConsumptionConfig(void)
{
    RCC_LSICmd(DISABLE);
	Sys_Stop();    //进入停机模式
		 
	SystemInit();  //唤醒后重新初始化一下时钟
	Write_Iwdg_ON(IWDG_Prescaler_32,0x7ff);
	uni_UART_Init();
}

#define ApplicationAddress 0x8001800
#define  VECTOR_SIZE (48*4)
void app_iap_init(void)
{
	memcpy((void*)0x20000000, (void*)ApplicationAddress, VECTOR_SIZE);  //把中断向量表映射到RAM里面跑
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
//SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	SYSCFG->CFGR |= 0x03;

}


void TIM2_IRQHandler(void)
{
	moto_timer_handler();
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}



