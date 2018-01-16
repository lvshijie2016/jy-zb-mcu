//#include "delay.h"
//#include "sys.h"
#include "led.h"
#include "config.h"

//#include "uart_loop.h"

extern _GetLedComData_t GetLedComData_t;
/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，ARMLED闪动
**输入参数 ：
**输出参数 ：
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
	
	
//	GetLedComData_t.com = LED_MODE_APERTURE_ALL_BLINK;
	//GPIO_SetBits(GPIOA,GPIO_Pin_15);   //power on 

//	led_mode_get_t(LED_MODE_APERTURE_ALL_ON,0xFE,30 );	
//	
//	led_mode_get_t(LED_MODE_APERTURE_ALL_BLINK,0Xff,30 );
	
//	led_mode_get_t(LED_MODE_APERTURE_ALL_BREATHE,0Xff,20 );

	
//	led_mode_get_t(LED_MODE_APERTURE_DOUBLE_RUN,0xff,20 );
////	led_run_task();
////	delay(500000);
////	
////	
	led_mode_get_t(LED_MODE_APERTURE_CLOCKWISE_RUN,3,30 );
////	led_run_task();
////	delay(500000);
////	
////	
//		led_mode_get_t(LED_MODE_APERTURE_ALL_OFF,3,30 );
////	led_run_task();
////	delay(500000);
	
	while(1)              //无限循环
	{



	led_run_task();
//	delay(150000);

	}
}



