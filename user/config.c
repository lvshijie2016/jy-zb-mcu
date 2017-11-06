#include "config.h"

/**
  *****************************************************************************
  * @Name   : IO设置
  *
  * @Brief  : none
  *
  * @Input  : Value: gpio  -> IOCON_GPIOA | IOCON_GPIOB |IOCON_GPIOC 
										 pin   -> PIN0 ~ PIN12
										 function -> PA10_FUNC_PWM_OUT4 | PA6_FUNC_GPIO | PB6_FUNC_I2C_SCL|..........
										 I_O ->  IO_Output | IO_Input
										 HL  ->  IO_HIGH   | IO_LOW
  *
  * @Output : none
  *
  * @Return : 
  *******************************************************************************
	**/

void get_gpio(uint8_t gpio, uint16_t pin, uint8_t function,uint8_t I_O, uint8_t HL)
{

	GPIO_TypeDef 		*port;
	IRQn_Type 				IRQ;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SYS_EnablePhrClk(AHB_IOCON);
	GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
	GPIO_InitStructure.bit.PUE    = PULL_UP_ENABLE;
	GPIO_InitStructure.bit.CSE    = SCHMITT_ENABLE;
	GPIO_InitStructure.bit.INV    = INPUT_INVERT_DISABLE;
	GPIO_InitStructure.bit.SRM    = FAST_SLEW_RATE_MODE;
	GPIO_InitStructure.bit.ADM    = DIGIT_ENABLE;
	GPIO_InitStructure.bit.DRV    = LOW_DRIVE;
	GPIO_InitStructure.bit.OD     = OPEN_DRAIN_DISABLE;
	GPIO_InitStructure.bit.S_MODE = INPUT_FILTER_DISABLE;
	//GPIO_InitStructure.bit.IEN    = INPUT_DISABLE;
	
	
	switch(gpio) {
		case IOCON_GPIOA:
				port	 = GPIOA;
				IRQ  	 = GPIOA_IRQn;
				SYS_EnablePhrClk(AHB_GPIOA);
			break;
		case IOCON_GPIOB:
				port	 = GPIOB;
				IRQ 	 = GPIOB_IRQn;
				SYS_EnablePhrClk(AHB_GPIOB);
			break;
		case IOCON_GPIOC:
				port 		= GPIOC;
				IRQ 	 	= GPIOC_IRQn;
				SYS_EnablePhrClk(AHB_GPIOC);
			break;
			default:
		break;
	}
	
	GPIO_InitStructure.bit.FUNC = (function&0x0F);
	if(function == PA4_FUNC_ADC_IN4 || ((function&0xf) == PA13_FUNC_GPIO && pin == PIN13))
	{
			GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
			GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}
	
	
	
	SYS_IOCONInit(gpio, pin, GPIO_InitStructure);
	if(function&0xF0) function = 0;  //特殊功能IO
		
	if(!function)
	{
		if(I_O)
		{
			GPIO_ConfigPinsAsOutput(port, pin);
			if(HL) 
				port->SET.all |= pin;
			else
				port->CLR.all |= pin;
		}
		else
		{
			NVIC_DisableIRQ(IRQ);
			GPIO_ConfigPinsAsInput(port, pin);
			if(HL) 
				GPIO_EnableInt(port, pin, RISE_EDGE);
			else
				GPIO_EnableInt(port, pin, FALL_EDGE);
			NVIC_EnableIRQ(IRQ);
		}
	}
}
	


void gpio_init_t(void)
{
	/***************TOUCH2_DRV****************************/
	get_gpio(IOCON_GPIOC,	PIN1,	PC1_FUNC_GPIO,	IO_Input,		IO_HIGH);  //TOUCH2_INT
	/***************5V_DRV_EN****************************/
	get_gpio(IOCON_GPIOC,	PIN3,	(SPECIAL|PC3_FUNC_GPIO),	IO_Output,	IO_LOW); //5V_DRV_EN
	
	/***************MCU_INT_KAR_PA14****************************/
	get_gpio(IOCON_GPIOA,	PIN14,	(SPECIAL|PA14_FUNC_GPIO),	IO_Output,	IO_LOW);
	
	/***************USB_DET_PA13****************************/
	get_gpio(IOCON_GPIOA,	PIN13,	(SPECIAL|PA13_FUNC_GPIO),	IO_Input,	IO_HIGH);
	
	/***************POWER_KEY_PA0****************************/
	get_gpio(IOCON_GPIOA,	PIN0,	PA0_FUNC_GPIO,	IO_Input,	IO_LOW);  
	
	/***************MCU_POWER_PA15****************************/
	NVIC_SetPriority(GPIOA_IRQn,1);
	get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output,	IO_LOW); //POWER_ON_OFF
	
	/***************RTC_INT_PB5****************************/
	get_gpio(IOCON_GPIOB,	PIN5,	PB5_FUNC_GPIO,	IO_Input,		IO_LOW);  //RTC_INT_PB5
	
	
	get_gpio(IOCON_GPIOB,PIN6,PB6_FUNC_I2C_SCL,0,0);
	get_gpio(IOCON_GPIOB,PIN7,PB7_FUNC_I2C_SDA,0,0);
	
/***************************LED******************************/	
	get_gpio(IOCON_GPIOA,	PIN5,	PA5_FUNC_GPIO,	IO_Output,	IO_LOW); 
	get_gpio(IOCON_GPIOA,	PIN6,	PA6_FUNC_GPIO,	IO_Output,	IO_LOW); 
	get_gpio(IOCON_GPIOA,	PIN11,	PA11_FUNC_GPIO,	IO_Output,	IO_LOW); 
	get_gpio(IOCON_GPIOA,	PIN12,	PA12_FUNC_GPIO,	IO_Output,	IO_LOW); 

	
/***************************MOTO******************************/
	get_gpio(IOCON_GPIOA,	PIN10,	PA10_FUNC_PWM_OUT4,0,0);
	get_gpio(IOCON_GPIOB,	PIN1,	PB1_FUNC_PWM_OUT5,0,0);
	get_gpio(IOCON_GPIOB,	PIN3,	PB3_FUNC_GPIO,	IO_Output,	IO_LOW);  //PB3
	get_gpio(IOCON_GPIOB,	PIN4,	PB4_FUNC_GPIO,	IO_Output,	IO_LOW);  //PB4
	
	PWM_SetDuty(PWM4 | PWM5, 0);

/***************************VBAT_ADC******************************/
	
	get_gpio(IOCON_GPIOA,	PIN4,	PA4_FUNC_ADC_IN4,0,0);
}

static void pwm_init_t(void)
{
	PWM_Init(10, PWMDIV_1, PWM_DEPENDENT, PWM_EDGE, PWM_POSITIVE);
	PWM_SetRiseDeadZone(1);
	PWM_SetFallDeadZone(1);
	PWM_EnableFaultProtect(PWMFAULT, PWM_AUTOMATIC, DISINT);
	PWM_Start();
}

static void adc_init_t(void)
{
	
	SYS_EnablePhrClk(AHB_ADC);  
	ADC_DeInit();
	ADC_Init(100);
	ADC_EnableChannels( ADC_CHN4_ENABLE);
	ADC_SetupChannels(AD4,ADC_DR4);
	ADC_SelectTriggerSource(TRIGGERMODE,ADC_START_BY_SOFTWAER,ADC_TRIGGER_RISE_EDGE);
	ADC_EnableConversionInt(ADC_CHN4_ENABLE);
	ADC_WaitAdcReady();
	NVIC_DisableIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);

}


static void timer0_init_t(void)
{
	CT16B0_Init(TMR0, 100000UL);
	CT16B0_ResetTimerCounter(TMR0);
	
	/* ---------- Set Timer return to zero when it matches MR value ----------*/
	CT16B0_ConfigMatch(TMR0, 100, CT_MATCH_RESET_COUNTER);
	
	CT16B0_EnableInt(TMR0);
	NVIC_EnableIRQ(CT16B0_IRQn);
	CT16B0_START;
	
}


void wdt_init_t(uint8_t timer)
{
	SYS_SystemInitial();
	SYS_EnablePhrClk(AHB_WDT);
	SYS_SelectWDTClkSrc(WDT_CLK);//select watch dog clock 32k
	/* ------------------- Set WachDog Window size ---------------------------*/
    WDT_SetWindowValue(0,0xffffff);
	WDT_SetIntervalinMiliSec(timer*1000);// ~= 5 second		
	WDT_Enable_RESET;
	WDT->MOD.bit.WDLOCKEN = 0;
	WDT_Enable;
	WDT_Feed();	
	
}

void sys_init(void)
{
	SYS_SystemInitial();
	UART0_Init();
	gpio_init_t();
	pwm_init_t();
	adc_init_t();
	timer0_init_t();
	IIC_Init(1,400,0xA0);
	RTC_Start();
	clear_all_event();
	led_mode_get_t(0x06,0xff,30 );
	moto_P();

}






