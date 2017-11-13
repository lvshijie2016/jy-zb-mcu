#include "led.h"


static unsigned char led_x = 0;
static unsigned char led_y = 0;
static unsigned char pwm_duty = 0;
static unsigned char PWM_flag = 0;
static unsigned char led_flag = 0;
_GetLedComData_t GetLedComData_t;


static void led_set_x(unsigned char num)
{
	GPIO_InitTypeDef init_gpio;
	SYS_EnablePhrClk(AHB_IOCON);
	SYS_EnablePhrClk(AHB_GPIOA);
	SYS_EnablePhrClk(AHB_GPIOB);
	init_gpio.bit.PDE    = PULL_DOWN_DISABLE;
	init_gpio.bit.PUE    = PULL_UP_ENABLE;
	init_gpio.bit.CSE    = SCHMITT_ENABLE;
	init_gpio.bit.INV    = INPUT_INVERT_DISABLE;
	init_gpio.bit.SRM    = FAST_SLEW_RATE_MODE;
	init_gpio.bit.ADM    = DIGIT_ENABLE;
	init_gpio.bit.DRV    = HIGH_DRIVE;
	init_gpio.bit.OD     = OPEN_DRAIN_DISABLE;
	init_gpio.bit.S_MODE = INPUT_FILTER_DISABLE;
	//PA7
	init_gpio.bit.FUNC = PA7_FUNC_GPIO;
	SYS_IOCONInit(IOCON_GPIOA, PIN7, init_gpio);
	//PA8
	init_gpio.bit.FUNC = PA8_FUNC_GPIO;
	SYS_IOCONInit(IOCON_GPIOA, PIN8, init_gpio);
	//PA9
	init_gpio.bit.FUNC = PA9_FUNC_GPIO;
	SYS_IOCONInit(IOCON_GPIOA, PIN9, init_gpio);
	//PB0
	init_gpio.bit.FUNC = PB0_FUNC_GPIO;
	SYS_IOCONInit(IOCON_GPIOB, PIN0, init_gpio);
	
	GPIO_ConfigPinsAsOutput(GPIOA,PIN9 | PIN8 | PIN7);
	GPIO_ConfigPinsAsOutput(GPIOB,PIN0);
		
		
	switch(num&0x0f) {
		case 0x01:
			GPIO_SetPin(GPIOA, PIN8);
			GPIO_ResetPin(GPIOA, PIN7 | PIN9);
			GPIO_ResetPin(GPIOB, PIN0);
			break;
		case 0x02:
			GPIO_SetPin(GPIOA, PIN7);
			GPIO_ResetPin(GPIOA, PIN8 | PIN9);
			GPIO_ResetPin(GPIOB, PIN0);
			break;
		case 0x04:
			GPIO_SetPin(GPIOA, PIN9);
			GPIO_ResetPin(GPIOA, PIN8 | PIN7);
			GPIO_ResetPin(GPIOB, PIN0);
			break;
		case 0x08:
			GPIO_ResetPin(GPIOA, PIN8 | PIN7 | PIN9);
			GPIO_SetPin(GPIOB, PIN0);
			break;
		case 0x0f:
			GPIO_SetPin(GPIOA, PIN8 | PIN7 | PIN9);
			GPIO_SetPin(GPIOB, PIN0);
			break;
		case 0x00:
			GPIO_ResetPin(GPIOA, PIN8 | PIN7 | PIN9);
			GPIO_ResetPin(GPIOB, PIN0);
			break;
		default:
			break;
	}
}



static void led_set_x_pwm(unsigned char value)
{
	GPIO_InitTypeDef init_gpio;
	SYS_EnablePhrClk(AHB_IOCON);
	SYS_EnablePhrClk(AHB_GPIOA);
	SYS_EnablePhrClk(AHB_GPIOB);
	init_gpio.bit.PDE    = PULL_DOWN_DISABLE;
	init_gpio.bit.PUE    = PULL_UP_ENABLE;
	init_gpio.bit.CSE    = SCHMITT_ENABLE;
	init_gpio.bit.INV    = INPUT_INVERT_DISABLE;
	init_gpio.bit.SRM    = FAST_SLEW_RATE_MODE;
	init_gpio.bit.ADM    = DIGIT_ENABLE;
	init_gpio.bit.DRV    = LOW_DRIVE;
	init_gpio.bit.OD     = OPEN_DRAIN_DISABLE;
	init_gpio.bit.S_MODE = INPUT_FILTER_DISABLE;
	
	//PWM1
	init_gpio.bit.FUNC = PA7_FUNC_PWM_OUT1;
	SYS_IOCONInit(IOCON_GPIOA, PIN7, init_gpio);
		
	//PWM2
	init_gpio.bit.FUNC = PA9_FUNC_PWM_OUT2;
	SYS_IOCONInit(IOCON_GPIOA, PIN9, init_gpio);
		
		
		//PWM3
	init_gpio.bit.FUNC = PB0_FUNC_PWM_OUT3;
	SYS_IOCONInit(IOCON_GPIOB, PIN0, init_gpio);
		
	//PWM0
	init_gpio.bit.FUNC = PA8_FUNC_PWM_OUT0;
	SYS_IOCONInit(IOCON_GPIOA, PIN8, init_gpio);
	
	
	PWM_SetDuty(PWM1 | PWM2 |PWM3 | PWM0, value);
}




static void led_set_y(unsigned char num)
{
	

	switch(num&0x0f) {

	case 0x01:
		GPIO_1_HIGH;
		GPIO_2_LOW;
		GPIO_3_LOW;
		GPIO_4_LOW;
		break;
	case 0x02:
		GPIO_2_HIGH;
		GPIO_1_LOW;
		GPIO_3_LOW;
		GPIO_4_LOW;
		
		break;
	case 0x04:
		GPIO_3_HIGH;
		GPIO_1_LOW;
		GPIO_2_LOW;
		GPIO_4_LOW;
	
		break;
	case 0x08:
	
		GPIO_4_HIGH;
		GPIO_1_LOW;
		GPIO_2_LOW;
		GPIO_3_LOW;
		break;
	//Ë«µÆ
	case 0x03:
		GPIO_1_HIGH;
		GPIO_2_HIGH;
		GPIO_3_LOW;
		GPIO_4_LOW;
		
		break;
	case 0x06:
		GPIO_2_HIGH;
		GPIO_3_HIGH;
		GPIO_1_LOW;
		GPIO_4_LOW;
		break;
	case 0x0C:
		GPIO_3_HIGH;
		GPIO_4_HIGH;
		GPIO_1_LOW;
		GPIO_2_LOW;
		break;
	//È«ÁÁ
	case 0x0f:
		GPIO_1_HIGH;
		GPIO_2_HIGH;
		GPIO_3_HIGH;
		GPIO_4_HIGH;
		break;
	//È«Ãð
	case 0x00:
		GPIO_1_LOW;
		GPIO_2_LOW;
		GPIO_3_LOW;
		GPIO_4_LOW;
		break;	
	default:
		break;
	}
}

static void aperture_all_breathe(void)
{	
	if (check_soft_timeout(TIMER_LED)) 
	{
		if (PWM_flag) {
			pwm_duty += 1;
			if (pwm_duty >= 99)
			{
				PWM_flag = 0;
				led_flag++;
			}
		} else {
			pwm_duty -= 1;
			if (pwm_duty <= 1) {
				PWM_flag = 1;
			}
		}
		if(led_flag == GetLedComData_t.count) GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
		else if(GetLedComData_t.count == 0xFF) led_flag = 0;
		led_set_y(0x0F);
		led_set_x_pwm(pwm_duty);
		set_soft_timer(TIMER_LED, (GetLedComData_t.frequency/10));
	}
}


static void aperture_all_on(void)
{
	led_set_y(0x0f);
	led_set_x(0x0f);
}



void aperture_all_off(void)
{
	led_set_x(0x00);
	led_set_y(0x00);
}

static void aperture_clockwise_run(void)
{

	
	if (check_soft_timeout(TIMER_LED)) {
		led_y =(led_y >= 0x08 || !led_y) ? 1 :(led_y	<< 1);
		led_set_x(0x0f);
		led_set_y(led_y);
		
		
		
		if((led_flag/4) == GetLedComData_t.count) GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
		else if(GetLedComData_t.count == 0xFF) led_flag = 0;
		(led_y == 0x08) ? led_flag++ : 0;
		set_soft_timer(TIMER_LED, GetLedComData_t.frequency);
		
		
		
	}
	
}



static void aperture_anticlockwise_run(void)
{
	
	if (check_soft_timeout(TIMER_LED)) {
		
		led_y =(led_y <= 0x01 || led_y > 0x08) ? 0x08 :(led_y	>> 1);
		led_set_x(0x0f);
		led_set_y(led_y);
	
		
		if((led_flag/4) == GetLedComData_t.count) GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
		else if(GetLedComData_t.count == 0xFF) led_flag = 0;
		
		(led_y == 0x01) ? led_flag++ : 0;
		set_soft_timer(TIMER_LED, GetLedComData_t.frequency);
	}
	
}

static void aperture_double_run(void)
{
	if (check_soft_timeout(TIMER_LED)) 
	{
	
		
		
		led_x= (!led_x) ? 0x01 : led_x;
		led_y =(led_y >= 0x0C || !led_y) ? 0x03 :(led_y	<< 1);
		led_set_x(led_x);
		led_set_y(led_y);
		if(led_y >= 0x0C)
		{
			if(led_x >= 0x08)
			{
				led_x =  0x01; 
				led_flag++;
			} else led_x <<= 1 ;
		}
		
		
		if(led_flag == GetLedComData_t.count) 
			GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
		else if(GetLedComData_t.count == 0xFF) led_flag = 0;
		set_soft_timer(TIMER_LED, GetLedComData_t.frequency);
	}
	
	
	//GetLedComData_t.com = LED_MODE_APERTURE_ALL_ON;
	
}

static void aperture_all_blink(void)
{
	
	if (check_soft_timeout(TIMER_LED)) {
		
		led_set_y(led_x);
		led_set_x(led_x);
		led_x = (led_x) ? 0x00 : 0x0F;
		if(led_flag == GetLedComData_t.count)  GetLedComData_t.com = LED_MODE_APERTURE_ALL_OFF;
		else if(GetLedComData_t.count == 0xFF)led_flag = 0;
		(led_x) ? led_flag++: 0;
		set_soft_timer(TIMER_LED, GetLedComData_t.frequency);
	}
}


void led_run_task(void)
{
switch(GetLedComData_t.com) {
	//aperture
	case LED_MODE_APERTURE_ALL_ON:
		aperture_all_on();
		break;
	case LED_MODE_APERTURE_ALL_OFF:
		aperture_all_off();
		break;
	case LED_MODE_APERTURE_ALL_BLINK:
		aperture_all_blink();
		break;
	case LED_MODE_APERTURE_ALL_BREATHE:
		aperture_all_breathe();
		break;
	case LED_MODE_APERTURE_CLOCKWISE_RUN:
		aperture_clockwise_run();
		break;
	case LED_MODE_APERTURE_ANTICLOCKWISE_RUN:
		aperture_anticlockwise_run();
		break;
	case LED_MODE_APERTURE_DOUBLE_RUN:
		aperture_double_run();
		break;
	
	default:
		break;
	}
}



void led_mode_get(uint8_t *Com)
{
	
	if (GetLedComData_t.com != Com[GET_LED_NUM] &&  Com[GET_LED_NUM] < LED_MODE_MAX ) {
		PWM_flag = 0;
		led_x = 0;
		led_y = 0;
		led_flag = 0;
		pwm_duty = 0;
		GetLedComData_t.com  				= 	Com[GET_LED_NUM];
		GetLedComData_t.count			 	= 	Com[GET_LED_CONUT];
		GetLedComData_t.frequency 	= 	(Com[GET_LED_DUTY]*10);
	}
}


void led_mode_get_t(uint8_t com, uint8_t count, uint8_t frequency)
{
	
	if (com < LED_MODE_MAX ) {
		PWM_flag = 0;
		led_x = 0;
		led_y = 0;
		led_flag = 0;
		pwm_duty = 0;
		GetLedComData_t.com 				= 	com;
		GetLedComData_t.count			 	= 	count;
		GetLedComData_t.frequency 	= 	(frequency*10);

	}
	
	
}



















