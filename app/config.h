#ifndef _CONFIG_H
#define _CONFIG_H

/* sys header files include --------------------------------------------*/

//#include "stdint.h"
//#include "stdbool.h"



#if defined C32F0
#include "stdbool.h"
#endif

#include "string.h"
#include "stdio.h"


#if defined C32F0
	#include "c32/c32_config.h"
#elif MM32F031K6
	#include "mm32/mm32_config.h"
#endif



/* user header files include --------------------------------------------*/

#include "user_timer.h"
#include "user_it.h"
#include "user_uart.h"
#include "led.h"
#include "rtc.h"
#include "moto.h" 
#include "Abnormal_Monitoring.h"


#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define USING_V50

/* Public typedef -------------------------------------------------------*/

#define IO_Output       											1
#define IO_Input                               						0

#define IO_HIGH       												1
#define IO_LOW                              	  					0
#define IO_DEFAULT                             	  					2

#define ORDINARY													0x00
#define SPECIAL														0x10

#define PULL_DOWN_EN												0x00
#define PULL_UP_EN													0x01
#define DISABLE_ALL_PULL											0x03

#define DISABLE_ALL_CHANNEL     9



#define BAT_VALUE_LOW												(unsigned char)5
#define BAT_VALUE_HIGH												(unsigned char)100
	
#define  BAT_MIN_STEP                       (unsigned char)5


#define ENERGY_SAMPLING_TIMER 											50 //ms
	

/* version -------------------------------------------------------*/

//#define  FIRMWARE_VERSION 								(unsigned char)(20+6)

#define  POISON_VERSION									(unsigned char)(10+2)

#define  SLEEP_DEFAULT_OFF_TIMER						(unsigned char)60 //?

#define  BAT_VALUE_BUFFER								(unsigned char) 50
	
#define USING_RESET


typedef struct
{
	unsigned char bat_buffer[BAT_VALUE_BUFFER];
	unsigned char head;
	uint16_t  data;
} _ADC_typedef;

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
	KAR_STARTING,
	KAR_STOPING,
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
void led_gpio_config(void);


void get_gpio(uint8_t gpio, uint16_t pin, uint8_t function,uint8_t I_O, uint8_t HL ,uint8_t edge);
void sys_init(void);
void sys_init_t(void);
void gpio_init_t(void);
void wdt_init_t(uint8_t timer);
void DisablePhrClk_t(void);	
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler,unsigned short int Reload);
#endif


