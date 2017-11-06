#include "user_it.h"


uint16_t 						Information_events = 0;
_all_event_flag 						all_event_flag;
static bool                             ad_falg = true;
static uint32_t                         adValue = 0;


void clear_all_event(void)
{
	Information_events	 &= 	(~DRV_EVENTS);
	Information_events	 &= 	(~MOTO_DET2_EVENTS);
	Information_events	 &= 	(~MOTO_DET1_EVENTS);
	Information_events	 &= 	(~RTC_INT_EVENTS);

}


void GPIOC_IRQHandler(void)
{
	
	switch(GPIO_Read_INTState(GPIOC)) {
		
			case PIN0:
				Information_events |=  MOTO_DET2_EVENTS;
			break;
		case PIN1:
				Information_events |=  DRV_EVENTS;
			break;

		default:break;
		
	}
	

	GPIO_Clear_INT(GPIOC,GPIO_Read_INTState(GPIOC));
	
}




void GPIOA_IRQHandler(void)
{
	
	switch(GPIO_Read_INTState(GPIOA)) {
		
		case PIN0:
		
			Information_events |=  POWER_KEY_EVENTS;
			
			break;
		case PIN13:
			Information_events |=  USB_DET_EVENTS;
			break;
		default:break;
	}
	

	GPIO_Clear_INT(GPIOA,GPIO_Read_INTState(GPIOA));
}




void GPIOB_IRQHandler(void)
{
	
	switch(GPIO_Read_INTState(GPIOB)) {
		
		case PIN0:
		
			break;
		case PIN5:
			Information_events |=  RTC_INT_EVENTS;
			break;
		default:break;
	}
	

	GPIO_Clear_INT(GPIOB,GPIO_Read_INTState(GPIOB));
}



uint8_t get_adc_value(uint8_t offset)
{
	uint8_t adc_buf[20] = {0};
	uint8_t i,j;
	uint8_t dat = 0;
	float adc_dat;
	static uint8_t	bat_value;
	
		SYS_EnablePhrClk(AHB_ADC); 
		NVIC_EnableIRQ(ADC_IRQn);
		
	for(i=0; i<sizeof(adc_buf);i++ )
	{
		ADC_IssueSoftTrigger;
		while(ad_falg);
		
		
		adc_dat =(uint16_t)((adValue*1000000*3.3/4095.0/1000-offset)*2);
		
		if(adc_dat > 3200) adc_buf[i]  = ((adc_dat-3200)/1000*100);
		else return 0;
		
		
		
		adc_buf[i] > BAT_VALUE_HIGH ? i-- : adc_buf[i]; //大于最大值为无效值
		ad_falg = true;
	}
	
		SYS_DisablePhrClk(AHB_ADC);
		NVIC_DisableIRQ(ADC_IRQn);
	
	for(j=0 ; j<sizeof(adc_buf); j++)  //取均值
	{
		for(i=0; i<sizeof(adc_buf)-1;i++ )
		{
			if(adc_buf[i] > adc_buf[i+1])
			{
				dat 			= adc_buf[i];
				adc_buf[i]	 	= adc_buf[i+1];
				adc_buf[i+1] 	= dat;
			}
		}
	}
	dat = adc_buf[(sizeof(adc_buf)/2)]; //取均值返回	
	
	if(bat_value > dat) bat_value= bat_value-((bat_value-dat)/2);
	else if(bat_value < dat ) bat_value = dat -((dat - bat_value)/2);
	//bat_value = adc_buf[(sizeof(adc_buf)/2)]; //取均值返回	
	return bat_value;
	
}


void ADC_IRQHandler(void)
{   
	volatile uint8_t stat;
	stat = ADC->STAT.all;
	switch(stat) {
		
		case 1<<ADC_DR0:
			adValue = ADC_GetConversionData(ADC_DR0);
			break;
		case 1<<ADC_DR1:
			adValue = ADC_GetConversionData(ADC_DR1);
			break;
		case 1<<ADC_DR2:
			adValue = ADC_GetConversionData(ADC_DR2);
			break;
		case 1<<ADC_DR3:
			adValue = ADC_GetConversionData(ADC_DR3);
			break;
		case 1<<ADC_DR4:
			adValue = ADC_GetConversionData(ADC_DR4);
			break;
		case 1<<ADC_DR5:
			adValue = ADC_GetConversionData(ADC_DR5);
			break;
		case 1<<ADC_DR6:
			adValue = ADC_GetConversionData(ADC_DR6);
			break;
		case 1<<ADC_DR7:
			adValue = ADC_GetConversionData(ADC_DR7);
			break;
		default:break;
	}
	
	ad_falg = false;
}

