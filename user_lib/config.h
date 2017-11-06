#ifndef _CONFIG_H
#define _CONFIG_H

/* sys header files include --------------------------------------------*/

#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"

/*==========================================================
	gpio 
===========================================================*/
#if true

#include "c32f0xx_gpio.h"

#endif
/*==========================================================
	IIC 
===========================================================*/
#if true

#include "c32f0xx_iic.h"

#endif
/*==========================================================
	PWM
===========================================================*/
#if true

#include "c32f0xx_pwm.h"

#endif
/*==========================================================
	UART
===========================================================*/
#if true

#include "c32f0xx_uart.h"

#endif
/*==========================================================
	IAP
===========================================================*/

#if true

#include "c32f0xx_iap.h"

#endif
/*==========================================================
	 timer
===========================================================*/
#if true

#include "c32f0xx_timer.h"

#endif
/*==========================================================
	 adc
===========================================================*/
#if true

#include "c32f0xx_adc.h"

#endif
/*==========================================================
	 sys
===========================================================*/
#if true

#include "c32f0xx_sys.h"

#endif

#if true

#include "c32f0xx_wdt.h"

#endif

/* user header files include --------------------------------------------*/
#include "user_timer.h"
#include "user_it.h"
#include "user_uart.h"
#include "led.h"
#include "rtc.h"
#include "moto.h" 
#include  "Abnormal_Monitoring.h"




/* Public typedef -------------------------------------------------------*/

#define IO_Output       												1
#define IO_Input                                0

#define IO_HIGH       													1
#define IO_LOW                              	  0


#define ORDINARY																0x00
#define SPECIAL																	0x10

/*==========================================================================
5V_DRV_EN
============================================================================*/
#define DRV_Enable   												GPIO_SetPin(GPIOC,PIN3)
#define DRV_Disable    				  								GPIO_ResetPin(GPIOC,PIN3)
/*==========================================================================
POWER_ON _OFF
============================================================================*/
#define POWER_ON   													GPIO_SetPin(GPIOA,PIN15)
#define POWER_OFF   				  								GPIO_ResetPin(GPIOA,PIN15)

#define KAR_DORMANCY_Enable   										GPIO_SetPin(GPIOA,PIN14)
#define KAR_DORMANCY_Disable   				  						GPIO_ResetPin(GPIOA,PIN14)


/*===============================================================================================
MOTO GPIO
==============================================================================================*/


#define MOTO1_HIGH_PH 												GPIO_SetPin(GPIOB,PIN3)
#define MOTO1_LOW_PH  												GPIO_ResetPin(GPIOB,PIN3)

#define MOTO2_HIGH_PH 												GPIO_SetPin(GPIOB,PIN4)
#define MOTO2_LOW_PH  				 								GPIO_ResetPin(GPIOB,PIN4)

#define BAT_VALUE_LOW												(unsigned char)5
#define BAT_VALUE_HIGH												(unsigned char)100

/* version -------------------------------------------------------*/

#define  FIRMWARE_VERSION 								(unsigned char)(10+4)

#define  POISON_VERSION									(unsigned char)(10+2)


typedef enum
{
	LONG_PRESS,
	SHORT_PRESS,
	RESET_PRESS,
	KEYS_DORMANCY_STATE,
	KEYS_OFF_STATE,
	KEYS_ON_STATE,
	MAX_KEYS_EVENT
}_KEY_EVENT;


typedef enum
{
	KAR_RUN,
	KAR_DORMANCY,
	KAR_STOP,
	KAR_RESET,
	MAX_KAR_STATE
}_KAR_STATE;





typedef struct
{
	uint8_t 	timer;
	uint8_t		heartbeat_flag;
	bool    	Open_close;
	
}_handling_timer;

/* Public variable ------------------------------------------------------*/


extern _KEY_EVENT  KEY_EVENT;
/* Public function declaration ------------------------------------------*/
void get_gpio(uint8_t gpio, uint16_t pin, uint8_t function,uint8_t I_O, uint8_t HL);
void sys_init(void);
void wdt_init_t(uint8_t timer);
	













#endif






