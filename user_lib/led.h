#ifndef _LED_H
#define _LED_H
#include "config.h"


#define  GET_LED_NUM   																			   0x01
#define  GET_LED_DUTY  																			   0x02
#define  GET_LED_CONUT 																			   0x03

/* Public typedef -------------------------------------------------------*/
typedef struct
{
	unsigned int frequency;
	unsigned char count;
	unsigned char com;
	
} _GetLedComData_t;




typedef enum
{
	LED_MODE_APERTURE_ALL_ON,
	LED_MODE_APERTURE_ALL_BLINK,  
	LED_MODE_APERTURE_ALL_BREATHE,
	LED_MODE_APERTURE_DOUBLE_RUN,
	LED_MODE_APERTURE_CLOCKWISE_RUN,
	LED_MODE_APERTURE_ANTICLOCKWISE_RUN,
	LED_MODE_APERTURE_ALL_OFF,
	LED_MODE_MAX
} led_mode_t;

/* Public variable ------------------------------------------------------*/






/* Public function declaration ------------------------------------------*/
//void led_run_ctrl(led_mode_t mode);
void led_run_task(void);
void led_mode_get(uint8_t *Com);
void led_mode_get_t(uint8_t com, uint8_t count, uint8_t frequency);
void aperture_all_off(void);
void led_mode_get_tt(uint8_t com, uint8_t count, uint8_t frequency);



#endif

