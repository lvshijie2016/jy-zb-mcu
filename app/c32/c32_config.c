#include "config.h"

/**
  *****************************************************************************
  * @Name   : IOÉèÖÃ
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

static void get_gpio(uint8_t gpio, uint16_t pin, uint8_t function,uint8_t I_O, uint8_t HL ,uint8_t edge)
{
	GPIO_TypeDef 		*port;
	IRQn_Type 			IRQ;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	SYS_EnablePhrClk(AHB_IOCON);
	
	if(edge == PULL_UP_EN)
	{
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_ENABLE;
	}else if(PULL_DOWN_EN == edge){
		
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_ENABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}else{
		
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}
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
	
	SYS_IOCONInit(gpio, pin, GPIO_InitStructure);
	
	
	if(function&0xF0) function = 0;  //ÌØÊâ¹¦ÄÜIO
	if(!function)
	{
		if(I_O == IO_Output)
		{
			GPIO_ConfigPinsAsOutput(port, pin);
			
			if(HL == IO_HIGH)
				port->SET.all |= pin;
				
			else if(HL == IO_LOW)
				port->CLR.all |= pin;	
			
		}
		else
		{
			NVIC_DisableIRQ(IRQ);
			GPIO_ConfigPinsAsInput(port, pin);
			
			if(HL == IO_HIGH) 
				GPIO_EnableInt(port, pin, RISE_EDGE);
			else
				GPIO_EnableInt(port, pin, FALL_EDGE);
			
			NVIC_EnableIRQ(IRQ);
		}
	}
}

static void get_adc_gpio(uint8_t gpio, uint16_t pin, uint8_t function, uint8_t edge)	
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	switch(gpio) {
		case IOCON_GPIOA:
				SYS_EnablePhrClk(AHB_GPIOA);
			break;
		case IOCON_GPIOB:
				SYS_EnablePhrClk(AHB_GPIOB);
			break;
		case IOCON_GPIOC:
				SYS_EnablePhrClk(AHB_GPIOC);
			break;
			default:
		break;
	}
	
	
	
	
	if(edge == PULL_UP_EN)
	{
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_ENABLE;
	}else if(PULL_DOWN_EN == edge){
		
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_ENABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}else{
		
		GPIO_InitStructure.bit.PDE    = PULL_DOWN_DISABLE;
		GPIO_InitStructure.bit.PUE    = PULL_UP_DISABLE;
	}
	
	GPIO_InitStructure.bit.CSE    = SCHMITT_ENABLE;
	GPIO_InitStructure.bit.INV    = INPUT_INVERT_DISABLE;
	GPIO_InitStructure.bit.SRM    = FAST_SLEW_RATE_MODE;
	GPIO_InitStructure.bit.ADM    = DIGIT_ENABLE;
	GPIO_InitStructure.bit.DRV    = LOW_DRIVE;
	GPIO_InitStructure.bit.OD     = OPEN_DRAIN_DISABLE;
	GPIO_InitStructure.bit.S_MODE = INPUT_FILTER_DISABLE;
	GPIO_InitStructure.bit.FUNC  = function;
	SYS_IOCONInit(gpio, pin, GPIO_InitStructure);
	
}	

static void c32_gpio_init_t(void)
{
	NVIC_SetPriority(GPIOA_IRQn,1);
	/***************TOUCH2_DRV****************************/
	get_gpio(IOCON_GPIOA,	PIN12,	PA12_FUNC_GPIO,	IO_Input,	IO_HIGH, PULL_UP_EN);  //->MOTO ADC 1

	/***************5V_DRV_EN****************************/
	get_gpio(IOCON_GPIOC, PIN3, (SPECIAL|PC3_FUNC_GPIO), IO_Output,	IO_LOW, PULL_UP_EN); //5V_DRV_EN

	/***************MCU_INT_KAR_PA14****************************/
	get_gpio(IOCON_GPIOA,	PIN14, (SPECIAL|PA14_FUNC_GPIO), IO_Output, IO_LOW, PULL_UP_EN);

	/**************POWER_KEY_PA0****************************/
	get_gpio(IOCON_GPIOA,	PIN0,	PA0_FUNC_GPIO,	IO_Input, IO_LOW, PULL_UP_EN);  

	/**************MCU_POWER_PA15****************************/
	get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output, IO_DEFAULT, PULL_UP_EN); //KAR_POWER_ON_OFF  

	/***************RTC_INT_PA5****************************/
	get_gpio(IOCON_GPIOA,	PIN5,	PA5_FUNC_GPIO,	IO_Input, IO_LOW, PULL_UP_EN);  //RTC_INT_PB5
	//RTC _ IIC IO
	get_adc_gpio(IOCON_GPIOB,PIN6,PB6_FUNC_I2C_SCL,PULL_UP_EN);
	get_adc_gpio(IOCON_GPIOB,PIN7,PB7_FUNC_I2C_SDA,PULL_UP_EN);


	/***************************LED******************************/	
	//GPIO1
	get_gpio(IOCON_GPIOB, PIN5, PB5_FUNC_GPIO,	IO_Output,	IO_LOW, PULL_UP_EN);
	//GPIO2
	get_gpio(IOCON_GPIOC, PIN0, PC0_FUNC_GPIO,	IO_Output,	IO_LOW, PULL_UP_EN); 

	//GPIO3
	get_gpio(IOCON_GPIOC, PIN1, PC1_FUNC_GPIO,	IO_Output,	IO_LOW, PULL_UP_EN);
	//GPIO4
	get_gpio(IOCON_GPIOB, PIN4, PB0_FUNC_GPIO,	IO_Output,	IO_LOW, PULL_UP_EN);

	/***************************USB_DET GPIO******************************/
	get_gpio(IOCON_GPIOA, PIN11, PA11_FUNC_GPIO, IO_Input,	IO_HIGH, DISABLE_ALL_PULL);	

	/***************************MOTO******************************/
	//PWM
	get_gpio(IOCON_GPIOA, PIN10, PA10_FUNC_GPIO, IO_Output, IO_LOW, PULL_UP_EN);
	get_gpio(IOCON_GPIOB, PIN1, PB1_FUNC_GPIO, IO_Output, IO_LOW, PULL_UP_EN);

	//PH PL
	get_gpio(IOCON_GPIOA, PIN13, (SPECIAL|PA13_FUNC_GPIO), IO_Output, IO_LOW, PULL_UP_EN);
	get_gpio(IOCON_GPIOB, PIN3,	PB3_FUNC_GPIO, IO_Output, IO_LOW, PULL_UP_EN);

	//MOTO_ADC
	get_adc_gpio(IOCON_GPIOA, PIN1, PA1_FUNC_ADC_IN1,DISABLE_ALL_PULL);
	get_adc_gpio(IOCON_GPIOA, PIN6, PA6_FUNC_ADC_IN6,PULL_DOWN_EN);

	/***************************VBAT_ADC******************************/
	get_adc_gpio(IOCON_GPIOA,	PIN4,	PA4_FUNC_ADC_IN4, DISABLE_ALL_PULL);
}

void gpio_init(void)
{
	c32_gpio_init_t();
}


static void pwm_init_t(void)
{
	PWM_Init(10, PWMDIV_1, PWM_DEPENDENT, PWM_EDGE, PWM_POSITIVE);
	PWM_SetRiseDeadZone(1);
	PWM_SetFallDeadZone(1);
	PWM_EnableFaultProtect(PWMFAULT, PWM_AUTOMATIC, DISINT);
	PWM_Start();
}

void uni_pwm_init(void)
{
	pwm_init_t();
}

static void adc_init_t(void)
{
	SYS_EnablePhrClk(AHB_ADC);
	ADC_DeInit();
	ADC_Init(10000UL);
	ADC_EnableChannels( ADC_CHN1_ENABLE);
	ADC_EnableChannels( ADC_CHN4_ENABLE);
	ADC_EnableChannels( ADC_CHN6_ENABLE);
	ADC_SelectTriggerSource(TRIGGERMODE,ADC_START_BY_SOFTWAER,ADC_TRIGGER_RISE_EDGE);
	ADC_SetupChannels(AD1, ADC_DR1);
	ADC_SetupChannels(AD6, ADC_DR6);
	ADC_SetupChannels(AD4, ADC_DR4);
//	ADC_EnableConversionInt(ADC_CHN1_ENABLE);
//	ADC_EnableConversionInt(ADC_CHN4_ENABLE);
//	ADC_EnableConversionInt(ADC_CHN6_ENABLE);
	ADC_WaitAdcReady();
	ADC_IssueSoftTrigger;
}


static void UART1_Init(void)
{
	#if defined( DeBug )
		SYS_EnablePhrClk(AHB_UART1);  
		get_adc_gpio(IOCON_GPIOA,PIN14,PA14_FUNC_TXD1,PULL_UP_EN);
		UART_Open(UART1, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);
	#endif
	
	#if defined( V50_DeBug )
		SYS_EnablePhrClk(AHB_UART1);  
		get_adc_gpio(IOCON_GPIOA,PIN14,PA14_FUNC_TXD1,PULL_UP_EN);
		UART_Open(UART1, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);
	#endif
}

void UART0_Init(void)
{
	GPIO_InitTypeDef uart0_gpio;
	SYS_EnablePhrClk(AHB_IOCON);
	SYS_EnablePhrClk(AHB_GPIOA);    
	SYS_EnablePhrClk(AHB_UART0);  

	uart0_gpio.bit.PDE    = PULL_DOWN_DISABLE;
	uart0_gpio.bit.PUE    = PULL_UP_ENABLE;
	
	uart0_gpio.bit.CSE    = SCHMITT_ENABLE;
	uart0_gpio.bit.INV    = INPUT_INVERT_DISABLE;
	uart0_gpio.bit.SRM    = FAST_SLEW_RATE_MODE;
	uart0_gpio.bit.ADM    = DIGIT_ENABLE;
	uart0_gpio.bit.DRV    = LOW_DRIVE;
	uart0_gpio.bit.OD     = OPEN_DRAIN_DISABLE;
	uart0_gpio.bit.S_MODE = INPUT_FILTER_DISABLE;
	uart0_gpio.bit.IEN    = INPUT_DISABLE;
	
	uart0_gpio.bit.FUNC   = PA2_FUNC_TXD0;
	SYS_IOCONInit(IOCON_GPIOA, PIN2, uart0_gpio);
	
	uart0_gpio.bit.FUNC   = PA3_FUNC_RXD0;
	SYS_IOCONInit(IOCON_GPIOA,PIN3,uart0_gpio);
	
	UART_Open(UART0, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);  
	NVIC_SetPriority(UART0_IRQn,0);
	NVIC_EnableIRQ(UART0_IRQn); 
}

void _uart_init(void)
{
	UART1_Init();
	UART0_Init();
}


void clock_init(void)
{
	SYS_SystemInitial();
}

static void wdt_init_t(uint8_t timer)
{
	SYS_EnablePhrClk(AHB_WDT);
	SYS_SelectWDTClkSrc(WDT_CLK);//select watch dog clock 32k
	/* ------------------- Set WachDog Window size ---------------------------*/
	WDT_SetWindowValue(0,0xffffff);
	WDT_SetIntervalinMiliSec(timer*1000);// ~= 5 second		
	WDT_Enable_RESET;
	WDT->MOD.bit.WDLOCKEN = 0;
	WDT_Enable;
	WDT->MOD.bit.WDEN  = 1;
	WDT_Feed();	
}

void uni_wdg_init(void)
{
	wdt_init_t(2);
}

static void timer0_init_t(void)
{
	CT16B0_Init(TMR0, 100000UL);
	CT16B0_ResetTimerCounter(TMR0);
	/* ---------- Set Timer return to zero when it matches MR value ----------*/
	CT16B0_ConfigMatch(TMR0, 100, CT_MATCH_RESET_COUNTER);
	CT16B0_EnableInt(TMR0);
	NVIC_SetPriority(CT16B0_IRQn,2);
	NVIC_EnableIRQ(CT16B0_IRQn);
	CT16B0_START;
}

static void timer1_init_t_PWM(void)
{
	CT16B0_Init(TMR1, 1000000UL);
	CT16B0_ResetTimerCounter(TMR1);
	/* ---------- Set Timer return to zero when it matches MR value ----------*/
	CT16B0_ConfigMatch(TMR1, 100, CT_MATCH_RESET_COUNTER);
	CT16B0_EnableInt(TMR1);
	NVIC_SetPriority(CT16B1_IRQn,3);
	NVIC_EnableIRQ(CT16B1_IRQn);
	CT16B1_STOP;
}

void uni_timer_init(void)
{
	timer0_init_t();
	timer1_init_t_PWM();
}


