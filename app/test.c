//#include "delay.h"
//#include "sys.h"
#include "led.h"
#include "config.h"

//#include "uart_loop.h"

/********************************************************************************************************
**������Ϣ ��int main (void)                          
**�������� ��������ARMLED����
**������� ��
**������� ��
********************************************************************************************************/

void delay(uint32_t nTime)
{	 		  	  
      
    while(nTime--)   
    {      
             
    }	  	    
} 



int main(void)
{
	
	uint8_t loop_index =0;
  sys_init();
	
	
	
	//GPIO_SetBits(GPIOA,GPIO_Pin_15);   //power on 

//	led_mode_get_t(LED_MODE_APERTURE_ALL_ON,3,30 );	
//	
//	led_mode_get_t(LED_MODE_APERTURE_ALL_BLINK,2,30 );
	
	led_mode_get_t(LED_MODE_APERTURE_ALL_BREATHE,5,30 );

	
//	led_mode_get_t(LED_MODE_APERTURE_DOUBLE_RUN,3,30 );
////	led_run_task();
////	delay(500000);
////	
////	
//	led_mode_get_t(LED_MODE_APERTURE_CLOCKWISE_RUN,3,30 );
////	led_run_task();
////	delay(500000);
////	
////	
//		led_mode_get_t(LED_MODE_APERTURE_ALL_OFF,3,30 );
////	led_run_task();
////	delay(500000);
	
	while(1)              //����ѭ��
	{



	led_run_task();

	}
}



