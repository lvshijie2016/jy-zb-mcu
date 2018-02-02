#include "config.h"


void uni_sys_init(void)
{
	clock_init();		// ��ʼ��ϵͳʱ��
	uni_wdg_init();		// ��ʼ�����Ź�
	gpio_init();		// ��ʼ����ͨGPIO���ⲿ�ж�
	uni_UART_Init();	// ��ʼ������
	uni_pwm_init();		// ��ʼ������LED ��PWM�ź�
	uni_adc_init();		// ��ʼ��ADC
	uni_timer_init();	// used for motor PWM signal.
	uni_i2c_init();		// ��ʼ��I2C������
	RTC_Start();		// ����RTCоƬ
	moto_P();			// �رյ��

}

#if 0
void sys_init(void)
{
#if defined C32F0	
	SYS_SystemInitial();
	wdt_init_t(2);
	gpio_init_t();
	//get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output, IO_DEFAULT, PULL_UP_EN); //KAR_POWER_ON_OFF  
	UART0_Init();
	UART1_Init();
	pwm_init_t();
	adc_init_t();
	#if defined( DeBug )
		LOG(LOG_DEBUG,"ADC Open  Successful.. \r\n");
	#endif
	timer0_init_t();
	timer1_init_t_PWM();
	IIC_Init(1,400,0xA0);
	
	#if defined( DeBug )
		LOG(LOG_DEBUG,"IIC Open  Successful.. \r\n");
	#endif
	RTC_Start();
	clear_all_event();
	led_mode_get_t(0x06,0xff,30 );
	moto_P();
#elif defined MM32F031K6
	gpio_init_t();
	timer0_init_t();
	pwm_init_t();
	timer1_init_t_PWM();
	exit_irq_init();
	UART2_Init();
	adc_init_t();
	IIC_Init(1,400,0xA0);
	RTC_Start();
	Write_Iwdg_ON(IWDG_Prescaler_32,0x7ff);
	//UART0_Init();
	#if defined MM32F031K8
		UART1_Init();
	#endif
	
#endif
}

void sys_init_t(void)
{
#if defined C32F0
	SYS_SystemInitial();
	wdt_init_t(2);
	gpio_init_t();
	//get_gpio(IOCON_GPIOA,	PIN15,	PA15_FUNC_GPIO,	IO_Output, IO_DEFAULT, PULL_UP_EN); //KAR_POWER_ON_OFF 
	UART0_Init();
	UART1_Init();
	pwm_init_t();
	adc_init_t();
	timer0_init_t();
	timer1_init_t_PWM();
	IIC_Init(1,400,0xA0);
	//RTC_Start();
	//while(Rtc_Check());
	clear_all_event();
	led_mode_get_t(0x06,0xff,30 );
	moto_P();
	#if defined( DeBug )
		LOG(LOG_DEBUG,"mcu wakeup sysinit! .......... \r\n");
	#endif
#endif
}
#endif



