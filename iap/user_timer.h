#ifndef _USER_TIMER_H
#define _USER_TIMER_H

#include "config.h"


/* Public typedef -------------------------------------------------------*/
typedef enum
{
	TIMER_SYSTEM,
	TIMER_MAX
	
} SOFT_TIMER;



void set_soft_timer(SOFT_TIMER index, uint16_t nms);
uint16_t check_soft_timeout(SOFT_TIMER index );
void timer_delay_ms(uint16_t delay);
uint16_t check_soft_timeout_PWM(SOFT_TIMER index );



#endif




