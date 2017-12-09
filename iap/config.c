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

void get_gpio(uint8_t gpio, uint16_t pin, uint8_t function,uint8_t I_O, uint8_t HL ,uint8_t edge)
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
	
	
	if(function&0xF0) function = 0;  //特殊功能IO
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
			else if(HL == IO_LOW)
				GPIO_EnableInt(port, pin, FALL_EDGE);
			
			NVIC_EnableIRQ(IRQ);
		}
	}
}
	

void get_adc_gpio(uint8_t gpio, uint16_t pin, uint8_t function, uint8_t edge)	
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
	

void gpio_init_t(void)
{
	
	/**************MCU_POWER_PA15****************************/
	get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output, IO_DEFAULT, PULL_UP_EN); //KAR_POWER_ON_OFF  

}



void wdt_init_t(uint8_t timer)
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




static void UART1_Init(void)
{
	#if defined( DeBug )
		SYS_EnablePhrClk(AHB_UART1);  
		//get_adc_gpio(IOCON_GPIOA,PIN14,PA14_FUNC_TXD1,PULL_UP_EN);
		UART_Open(UART1, 115200, UART_NO_PARITY, UART_RX_NOT_EMPTY);
	#endif
}





void sys_init(void)
{
	
	SYS_SystemInitial();
//	wdt_init_t(1);
	gpio_init_t();
	UART1_Init();
	UART0_Init();
	
	
	get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output, IO_DEFAULT, PULL_UP_EN); //KAR_POWER_ON_OF
	
	GPIO_SetPin(GPIOA,PIN15);
}



