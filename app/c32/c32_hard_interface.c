#include "config.h"

void adc_value_get(uint8_t *vaild,uint16_t *value)
{
	uint32_t         			adValue_t;

	SYS_EnablePhrClk(AHB_ADC); 

	adValue_t = (*ADC).DR[4].all;
	if(0x80000000 == (adValue_t & 0x80000000)){
		vaild[0] = 1;
		value[0] = adValue_t;
	}

	adValue_t = (*ADC).DR[1].all;
	if(0x80000000 == (adValue_t & 0x80000000)){
		vaild[1] = 1;
		value[1] = adValue_t;
	}

	adValue_t = (*ADC).DR[6].all;
	if(0x80000000 == (adValue_t & 0x80000000)){
		vaild[2] = 1;
		value[2] = adValue_t;
	}
	ADC_IssueSoftTrigger;
}

extern _GetLedComData_t GetLedComData_t;
extern _KAR_STATE kar_state;
extern _KAR_STATE kar_state_t;
void sys_start(void)
{
	uint8_t i =0x1f;
	while(Rtc_Check())
	{
		i--;
		dly1us(100000);
		led_mode_get_tt(LED_MODE_APERTURE_ALL_BLINK,0xff,10);
		if(!i){
			//			kar_off();//进入睡眠
			i = 0xff;
		}
	}//检测RTC

#if defined( DeBug )
	LOG(LOG_DEBUG,"RTC Check Successful.. \r\n");
#endif

	if ((*((uint32_t *)(0x7800))) != 0x55aaaa55)
	{
		if ((1==SYSCON->SYSRESSTAT.bit.SYSRST)||(1==SYSCON->SYSRESSTAT.bit.WDTRST))
		{
			GetLedComData_t.com = LED_MODE_APERTURE_ALL_BLINK;
			kar_state_t =  KAR_RUN;
			kar_state   =  KAR_RUN;
			POWER_ON;
			SYS_ClearResetStatus();
		}
		else
			kar_off();//进入睡眠
	}
	else
	{
		kar_state_t =  KAR_RUN;
		kar_state   =  KAR_RUN;
		POWER_ON;
		GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
		IAP_FlashProgram(0x7800,0);
	}
}

/********************************************************************************************************
**函数信息 ：void uni_wdt_reload(void)           
**功能描述 ：喂狗函数
**输入参数 ：
**输出参数 ：无
********************************************************************************************************/
void uni_wdt_reload(void)
{
	WDT_Feed();
}

static void configpad(uint32_t pinstat)
{
	//IOCON->PIOA_0.all  = pinstat;  //POWERR_KEY
	IOCON->PIOA_1.all  = pinstat;
	IOCON->PIOA_2.all  = pinstat;
	IOCON->PIOA_3.all  = pinstat;
	IOCON->PIOA_4.all  = pinstat;
	//IOCON->PIOA_5.all  = pinstat; //RTC_INT
	IOCON->PIOA_6.all  = pinstat;
	IOCON->PIOA_7.all  = pinstat;
	IOCON->PIOA_8.all  = pinstat;
	IOCON->PIOA_9.all  = pinstat;
	IOCON->PIOA_10.all = pinstat;
	IOCON->PIOA_11.all = pinstat;
	//IOCON->PIOA_12.all = pinstat;
	IOCON->PIOA_13.all = pinstat;
	IOCON->PIOA_14.all = pinstat;
	
	if(GPIO_GetPinState(GPIOA,PIN15))
		IOCON->PIOA_15.all = 1; //MCU_INT
	else 
		IOCON->PIOA_15.all = 0; //MCU_INT
	
	IOCON->PIOB_0.all  = pinstat;
	IOCON->PIOB_1.all  = pinstat;
	IOCON->PIOB_2.all  = pinstat;
	IOCON->PIOB_3.all  = pinstat;
	IOCON->PIOB_4.all  = pinstat;
	IOCON->PIOB_5.all  = pinstat;
	IOCON->PIOB_6.all  = 1;
	IOCON->PIOB_7.all  = 1;

	IOCON->PIOC_0.all  = pinstat;
	IOCON->PIOC_1.all  = pinstat;
	IOCON->PIOC_2.all  = pinstat;
	IOCON->PIOC_3.all  = pinstat;
}

static void DisablePhrClk_t(void)
{
	//IIC
	get_gpio(IOCON_GPIOB,PIN6,PB6_FUNC_GPIO,IO_Output,IO_HIGH,PULL_UP_EN);
	get_gpio(IOCON_GPIOB,PIN7,PB7_FUNC_GPIO,IO_Output,IO_HIGH,PULL_UP_EN);
	//LED
	get_gpio(IOCON_GPIOB,PIN0,PB0_FUNC_GPIO,IO_Output,IO_LOW,PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN7,PA7_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN9,PA9_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN10,PA10_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	//ADC
	get_gpio(IOCON_GPIOA,PIN1,PA1_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN6,PA6_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN4,PA4_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	
	//USB_DET
	get_gpio(IOCON_GPIOA,PIN11,PA11_FUNC_GPIO,IO_Output,IO_LOW,PULL_DOWN_EN);
	
	//uart0
	get_gpio(IOCON_GPIOA,PIN2,PA2_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	get_gpio(IOCON_GPIOA,PIN3,PA3_FUNC_GPIO,IO_Output,IO_LOW, PULL_DOWN_EN);
	
//	NVIC_DisableIRQ(GPIOA_IRQn);
//	SYS_DisablePhrClk(AHB_GPIOA);
	SYS_DisablePhrClk(AHB_GPIOC);
	SYS_DisablePhrClk(AHB_GPIOB);
	SYS_DisablePhrClk(AHB_WDT);
	SYS_DisablePhrClk(AHB_IOCON);
	SYS_DisablePhrClk(AHB_UART0);
	SYS_DisablePhrClk(AHB_ADC);
}

void LowPowerConsumptionConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#ifdef USING_RESET
	//GPIO_InitStructure.bit.FUNC = PC2_FUNC_NRST;
	GPIO_InitStructure.bit.FUNC = PC2_FUNC_GPIO;
#endif

	WDT_Disable;
	aperture_all_off();
	moto_P();
	//configpad(0);
	DisablePhrClk_t();
	SYS_SetDeepSleepWakeupPin(PIN0|PIN5,FALL_EDGE);//设置唤醒引脚	
#if defined( DeBug )
	LOG(LOG_DEBUG," get sleep mode \r\n");
#endif

#if defined( V50_DeBug )
	LOG(LOG_DEBUG," get sleep mode \r\n");
#endif
	SYS_DisablePhrClk(0xfffffff0 & (~(1<<29)));//关闭GPIOA时钟
	IOCON->PIOA_0.all  = PIN0|PIN5;//|PIN5;//设置唤醒引脚上拉
	dly1us(50000);
	SYS_EnterDeepSleep(PD_RTCOSC | PD_BOD, 0);	
	sys_init_t();//重新初始化所有配置

#ifdef USING_RESET
	GPIO_InitStructure.bit.FUNC = PC2_FUNC_NRST;
	//GPIO_InitStructure.bit.FUNC = PC2_FUNC_GPIO;
#endif

	Information_events = get_Alarm_Int_state() ? RTC_INT_EVENTS : POWER_KEY_EVENTS;
# if defined(DeBug)
	LOG(LOG_DEBUG," exit sleep mode...  ->%d \r\n",Information_events);
#endif

# if defined(V50_DeBug)
	LOG(LOG_DEBUG," exit sleep mode...  ->%d \r\n",Information_events);
#endif
}

#define APPLICATION_ADDRESS 0x1800
#define  VECTOR_SIZE (48*4)
void app_iap_init(void)
{
 	memcpy((void*)0x10000000, (void*)APPLICATION_ADDRESS, VECTOR_SIZE); 
	SYS_MemRemaptoSRAM; 

}

void TIMER16_1_IRQHandler(void)
{
	moto_timer_handler();
	CT16B0_ClearIntFlag(TMR1);
}


