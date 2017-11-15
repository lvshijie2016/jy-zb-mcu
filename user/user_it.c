#include "user_it.h"


uint16_t 						Information_events = 0;
_all_event_flag 						all_event_flag;
static uint32_t                         adValue = 0;
static uint32_t                         adValue_1 = 0;
static uint32_t                         adValue_6 = 0;


void clear_all_event(void)
{
	Information_events	 &= 	(~DRV_EVENTS);
	Information_events	 &= 	(~MOTO_DET2_EVENTS);
	Information_events	 &= 	(~MOTO_DET1_EVENTS);
	Information_events	 &= 	(~RTC_INT_EVENTS);
	
	
}





/**********************清楚GPIO中断*****************************/
void GPIO_Clear_INT(GPIO_TypeDef *port, uint16_t pin)
{
		port->IC.all |= pin;
}

/**********************读取GPIO中断状态*****************************/
uint16_t GPIO_Read_INTState(GPIO_TypeDef *port)
{
	return 	(uint16_t)port->MIS.all;
}

void GPIOA_IRQHandler(void)
{
	switch(GPIOA->MIS.all) {
		
		case PIN0:
		
			Information_events |=  POWER_KEY_EVENTS;
			GPIOA->IC.all |= PIN0;
			
			break;
		case PIN5:
		
			Information_events |=  RTC_INT_EVENTS;
			GPIOA->IC.all |= PIN5;
			break;
		case PIN11:
		
			Information_events |=  DRV_EVENTS;
			GPIOA->IC.all |= PIN11;
			break;
		case USB_DET:
		
			Information_events |=  USB_DET_EVENTS;
			GPIOA->IC.all |= USB_DET;
			break;
		default:break;
	}
}


uint8_t get_adc_value(void)
{
	static uint8_t i =0;
	static float adc_dat;
	static float j = 100;
	float dat;
	SYS_DisablePhrClk(AHB_ADC);
	NVIC_DisableIRQ(ADC_IRQn);
	if(adValue)
	{
		dat =(adValue*1000000*3.3/4095.0/1000);
		if(1750>dat) adc_dat = adc_dat + 0;
		else adc_dat = adc_dat+((dat-1750)/350*100);
		set_soft_timer(TIMER_BAT,ENERGY_SAMPLING_TIMER);  
		adValue = 0;
		i++;
		if(i == 20)
		{
			adc_dat = adc_dat/i;
			j = adc_dat;
			i=0;
			if(j>100) j = 100;
			
			#if defined( DeBug )
				//LOG(LOG_DEBUG,"adc_dat_t->=%d\r\n",(uint8_t)j);
			#endif
			
			 
		}	
	}

	
	return (uint8_t)j;
}

uint32_t get_adc_moto(void)
{
	uint32_t i = 0;
	if(adValue_6)
	{
		i = ((adValue_6*1000000*3.3/4095.0/1000));
		adValue_6 = 0;
		
	}
	
	return i;
}
void ADC_IRQHandler(void)
{   
	volatile uint8_t stat;
	
	stat = ADC->STAT.all;
	
	
	if(stat == (1<<ADC_DR0)) adValue = ADC_GetConversionData(ADC_DR0);
	
	
	if(stat == (1<<ADC_DR1)){
		adValue_1 = ADC_GetConversionData(ADC_DR1);
	}
	
	
	if(stat == (1<<ADC_DR2)) adValue = ADC_GetConversionData(ADC_DR2);
	
	
	if(stat == (1<<ADC_DR3)) adValue = ADC_GetConversionData(ADC_DR3);
	
	
	if(stat == (1<<ADC_DR4)){
		adValue = ADC_GetConversionData(ADC_DR4);
		Information_events |= ADC_BAT_EVENTS;
	}
	
	
	if(stat == (1<<ADC_DR5)) adValue = ADC_GetConversionData(ADC_DR5);
	
	
	if(stat == (1<<ADC_DR6)) {
		adValue_6 = ADC_GetConversionData(ADC_DR6);
	}
	
	
	if(stat == (1<<ADC_DR7)) adValue = ADC_GetConversionData(ADC_DR7);
	
}


void WAKEUP_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(WAKEUP_IRQn);
	SYS_ResetDeepSleepWakeupPin();
	
}























