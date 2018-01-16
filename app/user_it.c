#include "user_it.h"


uint16_t 						Information_events = 0;
_all_event_flag 						all_event_flag;

void clear_all_event(void)
{
	Information_events	 &= 	(~DRV_EVENTS);
	Information_events	 &= 	(~MOTO_DET2_EVENTS);
	Information_events	 &= 	(~MOTO_DET1_EVENTS);
	Information_events	 &= 	(~RTC_INT_EVENTS);
}



#if defined C32F0

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
		case PIN12:
		
			Information_events |=  DRV_EVENTS;
			GPIOA->IC.all |= PIN12;
			break;
		case USB_DET:
		
			Information_events |=  USB_DET_EVENTS;
			GPIOA->IC.all |= USB_DET;
			break;
		default:break;
	}
}




void WAKEUP_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(WAKEUP_IRQn);
	SYS_ResetDeepSleepWakeupPin();
}


#elif defined MM32F031K6

/********************************************************************************************************

********************************************************************************************************/
void EXTI0_1_IRQHandler(void)
{ 	
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		Information_events |=  POWER_KEY_EVENTS;
	}
   EXTI_ClearITPendingBit(EXTI_Line0); 
     	
} 

/********************************************************************************************************

********************************************************************************************************/


void EXTI4_15_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
	{
		Information_events |=  RTC_INT_EVENTS;
	}
	EXTI_ClearITPendingBit(EXTI_Line5); 	
	
}

#endif




















